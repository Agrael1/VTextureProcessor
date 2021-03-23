#include <UI/UINode.h>
#include <unordered_map>


class FlowCodex
{
public:
	FlowCodex(QJsonDocument nodes);
public:
	const auto& CatMap()const noexcept
	{
		return cats;
	}
	UI::Node MakeNode(std::string_view in)
	{
		return codex.at(in.data());
	}
private:
	std::unordered_map<std::string, UI::Node> codex;
	std::unordered_map<std::string, std::vector<std::string_view>> cats;
};