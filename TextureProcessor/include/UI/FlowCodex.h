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
	UI::Node MakeNode(std::wstring_view in)
	{
		return codex.at(in.data());
	}
private:
	std::unordered_map<std::wstring, UI::Node> codex;
	std::unordered_map<std::wstring, std::vector<std::wstring>> cats;
};