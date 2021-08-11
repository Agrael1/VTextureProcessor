#include <UI/REFlowCodex.h>
#include <QJsonDocument>

using namespace UI::RE;

constexpr const char* output = R"(
{
"Output": {
	"Node": {
		"Class": "Texture",
			"Group" : "Output",
			"Sinks" : [{"Name": "In", "Type" : "Grayscale"}]
		},
		"NodeStyle" : {
			"TitleColor": "black",
			"FontColor" : "white"
		},
		"Layout" : [{"Type" : "sink", "Value":0}],
		"Value" : ["uniform sampler2D u_Sampler;								\n",
			"in vec2 texcoords;\n",
			"out vec4 color;\n",
			"void main() { color = texture2D(u_Sampler, texcoords);}"]
}})";


XFlowCodex::XFlowCodex()
{
	namespace fs = std::filesystem;
	engine.emplace(QSize{ 256, 256 });
	ParseJson(QJsonDocument::fromJson(output));
}

std::unique_ptr<UI::IXNode> XFlowCodex::GetNode(std::string_view nodety)const
{
	auto& x = codex.at(nodety.data());
	return x->Clone(std::format("{}_{}", nodety, x.refcount++));
}

void UI::RE::XFlowCodex::ParseJson(const QJsonDocument& json)
{
	QJsonObject topLevelObject = json.object();

	for (auto key : topLevelObject.keys())
	{
		QJsonObject obj = topLevelObject[key].toObject();
		// Name of the node style
		auto wkey = key.toStdString();
		auto node = obj["Node"].toObject();

		// Loads texture Node style
		RefCountPair p;
		if (node["Class"].toString() == "Texture")
			p = RefCountPair::set<XNode>(obj, wkey, *engine);

		// Adds new group to the context menu
		auto pair = codex.emplace(wkey, std::move(p));
		auto x = node["Group"].toString().toStdString();
		cats[x].emplace_back(wkey);
	}
}
