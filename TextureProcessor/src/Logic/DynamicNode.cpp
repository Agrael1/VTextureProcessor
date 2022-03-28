#include <Logic/DynamicNode.h>
#include <Logic/SinksT.h>
#include <Logic/SourcesT.h>
#include <UI/UINode.h>
#include <UI/PortContainer.h>
#include <Windows/Properties.h>




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
	base_class::GetProperties(props);
	props.Attach(static_cast<DynamicDescriptor&>(desc).prop);
}

ver::DynamicDescriptor::DynamicDescriptor()
	:prop(std::make_shared<UI::PortsProperty>())
{
}

ver::DynamicDescriptor::DynamicDescriptor(QJsonObject document, std::string_view styleName)
	:prop(std::make_shared<UI::PortsProperty>()), TextureDescriptor(document, styleName)
{
}

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
