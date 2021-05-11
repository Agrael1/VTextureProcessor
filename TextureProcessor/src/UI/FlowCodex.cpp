/**
 * @file FlowCodex.cpp
 * @author Ilya Doroshenko (xdoros01), David Černý (xcerny74)
 * @brief Class for loading Node definitions from files
 */
#include <UI/FlowCodex.h>
#include <fmt/printf.h>
#include <fstream>
#include <streambuf>

constexpr const char* output = R"(
{"Output": {
	"Node": {
		"Class": "Texture",
			"Group" : "Output",
			"Sinks" : [{"Name": "In", "Type" : "Grayscale"}]
	},
		"NodeStyle" : {
			"TitleColor": "black",
				"FontColor" : "white"
		},
			"Value" : ["uniform sampler2D u_Sampler;								\n",
				"in vec2 texcoords;\n",
				"out vec4 color;\n",
				"void main() { color = texture2D(u_Sampler, texcoords);}"]
}})";

using namespace UI;

/**
 * @brief Construct a new Flow Codex:: Flow Codex object,
 *
 */
FlowCodex::FlowCodex()
	:engine(QSize{ 256,256 })
{
	namespace fs = std::filesystem;

	ParseJson(QJsonDocument::fromJson( output ));

	fs::path ndir{ NodesDir };
	// Creates node directory and exits if it doesn't exist
	if (!fs::is_directory(ndir))
	{
		fs::create_directory(ndir);
		return;
	}

	// Scans for all Nodes .json files in the Node directory and loads them
	QJsonParseError e;
	for (auto& p : fs::directory_iterator(ndir))
	{
		if (!p.is_regular_file())
			continue;

		if (p.path().extension() == ".json")
		{
			std::ifstream t( p.path() );
			std::string str;

			t.seekg(0, std::ios::end);
			str.reserve(t.tellg());
			t.seekg(0, std::ios::beg);

			str.assign((std::istreambuf_iterator<char>(t)),
				std::istreambuf_iterator<char>());

			// Deserialize loaded JSON
			auto json = QJsonDocument::fromJson(QByteArray::fromStdString(str), &e);
			if (e.error != QJsonParseError::NoError) { qDebug() << e.errorString(); continue; }
			ParseJson(json);
		}
	}
}

/**
 * @brief Fallback constructor
 *
 * @param nodes
 */
FlowCodex::FlowCodex(QJsonDocument nodes)
	:engine(QSize{ 128,128 })
{
	ParseJson(nodes);
}

/**
 * @brief Loads a Node type from its JSON specification
 *
 * @param json JSON definition of the Node type
 */
void FlowCodex::ParseJson(const QJsonDocument& json)
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
			p = RefCountPair::set<TextureNode>(obj, wkey, engine);

		// Adds new group to the context menu
		auto pair = codex.emplace(wkey, std::move(p));
		auto x = node["Group"].toString().toStdString();
		cats[x].emplace_back(pair.first->second->GetStyleName());
	}
}

/**
 * @brief Copies a Node object (or its subtypes)
 *
 * @param nodety Node type located in the codex
 * @return const pv::polymorphic_value<UI::Node>&
 */
const pv::polymorphic_value<UI::Node>& FlowCodex::GetNode(std::string_view nodety) const
{
	return codex.at(nodety.data());
}
const pv::polymorphic_value<UI::Node>* FlowCodex::TryGetNode(std::string_view nodety)const
{
	if (!codex.contains(nodety.data()))return nullptr;
	return &GetNode(nodety);
}


/**
 * @brief Changes reference cout to target Node type (can only be increased)
 *
 * @param nodety Node type to increase count of
 * @param cnt New count
 */
void FlowCodex::SetMaxRef(std::string_view nodety, size_t cnt)
{
	auto& r = codex.at(nodety.data());
	r.refcount = std::max(r.refcount, cnt);
}

/**
 * @brief Increments refrence count to target Node type
 *
 * @param nodety Target Node type
 * @return size_t Previous reference count
 */
size_t FlowCodex::AddRef(std::string_view nodety)
{
	return codex.at(nodety.data()).refcount++;
}
