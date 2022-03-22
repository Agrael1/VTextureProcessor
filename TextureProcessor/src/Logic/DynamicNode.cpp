//#include <Logic/DynamicNode.h>
//#include <Logic/SinksT.h>
//#include <Logic/SourcesT.h>
//
//void ver::DynamicNode::ModifySinks(std::vector<ver::PortDesc> descs)
//{
//	sinks.clear();
//	if (descs.size() != sinks.size())
//	{
//		sinks.reserve(descs.size());
//		inputs.resize(descs.size());
//	}
//	for (size_t i = 0; i < descs.size(); i++)
//	{
//		auto& d = descs[i];
//		if (d.name.isEmpty() || !any(d.type))continue;
//		RegisterSink(DirectTextureSink::Make(d.name.toStdString(), inputs[i], d.type));
//	}
//}
//
//void ver::DynamicNode::ModifySources(std::vector<ver::PortDesc> descs)
//{
//	sources.clear();
//	if (auto a = descs.size() - sources.size(); a > 0)
//	{
//		outputs.reserve(descs.size());
//		for (size_t i = 0; i < a; i++)
//			outputs.emplace_back(std::make_shared<QImage>());
//	}
//	sources.reserve(descs.size());
//	outputs.resize(descs.size());
//	for (size_t i = 0; i < descs.size(); i++)
//	{
//		auto& d = descs[i];
//		if (d.name.isEmpty() || !any(d.type))continue;
//		RegisterSource(DirectTextureSource::Make(d.name.toStdString(), "", outputs[i], d.type));
//	}
//}
