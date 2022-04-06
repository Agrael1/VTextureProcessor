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
{
}

ver::DynamicDescriptor::DynamicDescriptor(QJsonObject document, std::string_view styleName)
	: TextureDescriptor(document, styleName)
	, prop(std::make_shared<UI::PortsProperty>(sinks, sources))
	, pcont(std::make_shared<UI::PropertyContainer>(buffer, params))
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
