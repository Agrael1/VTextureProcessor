#include <Logic/DynamicNode.h>
#include <Logic/SinksT.h>
#include <Logic/SourcesT.h>
#include <UI/UINode.h>
#include <UI/PortContainer.h>
#include <Windows/Properties.h>
#include <UI/PropertyGenerator.h>

#include <UI/PropertyContainer.h>


void ver::DynamicNode::GetProperties(UI::Windows::PropertyElement& props)
{
	auto& d = Desc();
	if (!bchange)
	{
		struct WB :public QWidget
		{
			QHBoxLayout hl;
			QToolButton tb;
			WB()
			{
				tb.setIcon(QIcon{ ":/icons8-edit.png" });
				hl.setAlignment(Qt::AlignRight);
				hl.addWidget(&tb);
				hl.setContentsMargins(0, 0, 0, 0);
				setLayout(&hl);
			}
		};
		auto& tb = props.AppendWidget<WB>();
		tb.tb.connect(&tb.tb, &QToolButton::pressed, [this, &props]() {bchange = true; Desc().UpdateProperties(props); });
		UI::PropertyBuffer(props, buf, desc.params);
	}
	else {
		d.pcont->SetDiscardCallback([this, &props]()
			{
				auto& d = Desc();
				bchange = false;
				d.ResetContainer();
				d.UpdateProperties(props);
			});
		d.pcont->SetSaveCallback([this, &props]()
			{
				auto& d = Desc();
				bool x = d.AcceptContainer();
				if (!x)return;
				bchange = false;
				d.GatherPropertyInfo();
				d.prop_callback();
				d.ResetContainer();
				d.UpdateProperties(props);
			});
		props.Attach(d.pcont);
	}
	props.Attach(d.prop);
}

ver::DynamicDescriptor::DynamicDescriptor()
	:prop(std::make_shared<UI::PortsProperty>(sinks, sources))
	, pcont(std::make_shared<UI::PropertyContainer>(buffer, params))
	, group(QStringLiteral("Ungrouped"))
{
}

ver::DynamicDescriptor::DynamicDescriptor(QJsonObject document)
	: TextureDescriptor(document)
	, prop(std::make_shared<UI::PortsProperty>(sinks, sources))
	, pcont(std::make_shared<UI::PropertyContainer>(buffer, params))
	, group(document.contains(u"Group")? document[u"Group"].toString():QStringLiteral("Ungrouped"))
{
}

ver::DynamicDescriptor::~DynamicDescriptor()
{}

std::unique_ptr<ver::Node> ver::DynamicDescriptor::MakeModel()
{
	return std::make_unique<ver::DynamicNode>(*this);
}
void ver::DynamicDescriptor::ModifySinks()
{
	sinks = std::move(prop->GetSinks());
}
void ver::DynamicDescriptor::ModifySources()
{
	sources = std::move(prop->GetSources());
}

void ver::DynamicDescriptor::Recompile()
{
	ModifySinks();
	ModifySources();
	Assemble();
}

QJsonObject ver::DynamicDescriptor::Save()
{
	QJsonObject top;
	QJsonObject xstyle;

	style.Serialize(top);
	top.insert(u"Name", style.StyleName());
	top.insert(u"Value", shader_body);
	top.insert(u"Class", QStringLiteral("Texture"));
	top.insert(u"Group", group.isEmpty()?QStringLiteral("Ungrouped"): group);

	QJsonArray xsinks;
	for (auto& i : sinks)
	{
		QJsonObject a;
		a.insert(u"Name", i.name);
		a.insert(u"Type", to_str(i.type).data());
		xsinks.append(a);
	}
	if (!xsinks.isEmpty()) top.insert(u"Sinks", xsinks);

	QJsonArray xsources;
	for (auto& i : sources)
	{
		QJsonObject a;
		a.insert(u"Name", i.name);
		a.insert(u"Type", to_str(i.type).data());
		xsources.append(a);
	}
	if (!xsources.isEmpty()) top.insert(u"Sources", xsources);
	if (auto x = SaveOptions(); !x.isEmpty())
		top.insert(u"Properties", x);

	return top;
}

QJsonArray ver::DynamicDescriptor::SaveOptions()
{
	QJsonArray out;

	auto it = params.begin();
	for (auto& i : buffer.Get())
	{
		QJsonObject o;
		o.insert(u"CName", i.first.data());
		o.insert(u"Type", i.second.GetSignature().data());
		if (it == params.end())break;
		QJsonObject o2;

		if(it->enable_alias)
			o.insert(u"Tag", it->alias.c_str());
		if (it->enable_def)
			o2.insert(u"default", QJsonValue::fromVariant(it->param.get_def()));
		if (it->enable_min)
			o2.insert(u"min", QJsonValue::fromVariant(it->param.get_min()));
		if (it->enable_max)
			o2.insert(u"max", QJsonValue::fromVariant(it->param.get_max()));

		o.insert(u"Val", o2);
		out.append(o);
		it++;
	}

	return out;
}

void ver::DynamicDescriptor::ResetContainer()
{
	pcont = std::make_shared<UI::PropertyContainer>(buffer, params);
}
bool ver::DynamicDescriptor::AcceptContainer()
{
	return pcont->Accept();
}
void ver::DynamicDescriptor::GatherPropertyInfo()
{
	params = std::move(pcont->GatherOptions());
	buffer = std::move(pcont->GatherLayout());
}

void ver::DynamicDescriptor::UpdateProperties(UI::Windows::PropertyElement& props)
{
	if (!node)return;
	props.Clear();
	node->UpdateProperties(props);
}
