#include <UI/FlowCodex.h>
#include <fmt/printf.h>

using namespace UI;

FlowCodex::FlowCodex(QJsonDocument nodes)
	:engine({128,128})
{
	QJsonObject topLevelObject = nodes.object();

	for (auto key : topLevelObject.keys())
	{
		QJsonObject obj = topLevelObject[key].toObject();
		auto wkey = key.toStdString();
		auto node = obj["Node"].toObject();

		RefCountPair p;
		
		if(node["Class"].toString() == "Texture")
			p = RefCountPair::set<TextureNode>(obj, wkey, engine);

		auto pair = codex.emplace(wkey, std::move(p));

		auto x = node["Group"].toString().toStdString();
		cats[x].emplace_back(pair.first->second->GetStyleName());
	}
}

std::pair<const pv::polymorphic_value<UI::Node>&, size_t> FlowCodex::MakeNode(std::string_view in)const
{
	const auto& r = codex.at(in.data());
	return { r, r.refcount++ };
}