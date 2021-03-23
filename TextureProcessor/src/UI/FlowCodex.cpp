#include <UI/FlowCodex.h>
#include <fmt/printf.h>

using namespace UI;

FlowCodex::FlowCodex(QJsonDocument nodes)
{
	QJsonObject topLevelObject = nodes.object();

	for (auto key : topLevelObject.keys())
	{
		QJsonObject obj = topLevelObject[key].toObject();
		auto wkey = key.toStdString();
		auto pair = codex.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(wkey),
			std::forward_as_tuple(obj, wkey)
		);

		auto x = obj["Node"].toObject()["Group"].toString().toStdString();
		cats[x].emplace_back(pair.first->second.GetStyleName());
	}
}

std::pair<const UI::Node&, size_t> FlowCodex::MakeNode(std::string_view in)const
{
	const auto& r = codex.at(in.data());
	return { r, r.refcount++ };
}