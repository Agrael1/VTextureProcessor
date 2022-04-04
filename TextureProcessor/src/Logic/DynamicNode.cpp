#include <Logic/DynamicNode.h>
#include <Logic/SinksT.h>
#include <Logic/SourcesT.h>
#include <UI/UINode.h>
#include <UI/PortContainer.h>
#include <Windows/Properties.h>
#include <UI/PropertyGenerator.h>

#include <UI/PropertyContainer.h>




ver::DynamicNode::DynamicNode(DynamicDescriptor& td)
	:base_class(td)
{
}

void ver::DynamicNode::UpdateSinks()
{
	auto& descs = desc.sinks;
	sinks.clear();
	if (descs.size() != sinks.size())
	{
		sinks.reserve(descs.size());
		inputs.resize(descs.size());
	}
	for (size_t i = 0; i < descs.size(); i++)
	{
		auto& d = descs[i];
		if (d.name.isEmpty() || !any(d.type))continue;
		RegisterSink(DirectTextureSink::Make(d.name.toStdString(), inputs[i], d.type));
	}
}

void ver::DynamicNode::UpdateSources()
{
	auto& descs = desc.sources;
	sources.clear();
	if (auto a = descs.size() - sources.size(); a > 0)
	{
		outputs.reserve(descs.size());
		for (size_t i = 0; i < a; i++)
			outputs.emplace_back(std::make_shared<QImage>());
	}
	sources.reserve(descs.size());
	outputs.resize(descs.size());
	for (size_t i = 0; i < descs.size(); i++)
	{
		auto& d = descs[i];
		if (d.name.isEmpty() || !any(d.type))continue;
		RegisterSource(DirectTextureSource::Make(d.name.toStdString(), outputs[i], d.type));
	}
}

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
				d.UpdateProperties(props);
			});
		props.Attach(d.pcont);
	}
	props.Attach(d.prop);
}

ver::DynamicDescriptor::DynamicDescriptor()
	:prop(std::make_shared<UI::PortsProperty>())
	, pcont(std::make_shared<UI::PropertyContainer>(buffer, params))
{
}

ver::DynamicDescriptor::DynamicDescriptor(QJsonObject document, std::string_view styleName)
	: TextureDescriptor(document, styleName)
	, prop(std::make_shared<UI::PortsProperty>())
	, pcont(std::make_shared<UI::PropertyContainer>(buffer, params))
{
}

ver::DynamicDescriptor::~DynamicDescriptor()
{}

std::unique_ptr<ver::Node> ver::DynamicDescriptor::MakeModel()
{
	auto p = std::make_unique<ver::DynamicNode>(*this);
	prop->LoadPorts(*p);
	return p;
}

void ver::DynamicDescriptor::ModifySinks()
{
	sinks = std::move(prop->GetSinks());
	auto& dn = static_cast<ver::DynamicNode&>(node->GetModel());
	dn.UpdateSinks();
	node->MakeSinks();
	node->UpdateLayouts();
	node->Update();
}

void ver::DynamicDescriptor::ModifySources()
{
	sources = std::move(prop->GetSources());
	auto& dn = static_cast<ver::DynamicNode&>(node->GetModel());
	dn.UpdateSources();
	node->MakeSources();
	node->UpdateLayouts();
	node->Update();
}

void ver::DynamicDescriptor::Recompile()
{
	sinks = std::move(prop->GetSinks());
	sources = std::move(prop->GetSources());
	auto& dn = static_cast<ver::DynamicNode&>(node->GetModel());
	dn.UpdateSources();
	dn.UpdateSinks();
	Assemble();

	node->MakeSinks();
	node->MakeSources();
	node->UpdateLayouts();
	node->Update();
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
