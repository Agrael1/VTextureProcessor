#include <UI/UINode.h>
#include <unordered_map>

class FlowCodex
{
public:
	FlowCodex(QJsonObject nodes);
public:

private:
	std::unordered_map<std::wstring, UI::Node> codex;
	std::vector<std::wstring> cats;
};