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

FlowCodex::FlowCodex()
	:engine(QSize{ 256,256 })
{
	namespace fs = std::filesystem;

	ParseJson(QJsonDocument::fromJson( output ));

	fs::path ndir{ NodesDir };
	if (!fs::is_directory(ndir))
	{
		fs::create_directory(ndir);
		return;
	}
	QJsonParseError e;
	for (auto& p : fs::directory_iterator(ndir))
	{
		if (!p.is_regular_file())continue;
		if (p.path().extension() == ".json")
		{
			std::ifstream t{ p };
			std::string str;

			t.seekg(0, std::ios::end);
			str.reserve(t.tellg());
			t.seekg(0, std::ios::beg);

			str.assign((std::istreambuf_iterator<char>(t)),
				std::istreambuf_iterator<char>());
			
			auto json = QJsonDocument::fromJson(QByteArray::fromStdString(str), &e);
			if (e.error != QJsonParseError::NoError) { qDebug() << e.errorString(); continue; }
			ParseJson(json);
		}
	}
}
FlowCodex::FlowCodex(QJsonDocument nodes)
	:engine(QSize{ 128,128 })
{
	ParseJson(nodes);
}

void FlowCodex::ParseJson(const QJsonDocument& json)
{
	QJsonObject topLevelObject = json.object();

	for (auto key : topLevelObject.keys())
	{
		QJsonObject obj = topLevelObject[key].toObject();
		auto wkey = key.toStdString();
		auto node = obj["Node"].toObject();

		RefCountPair p;

		if (node["Class"].toString() == "Texture")
			p = RefCountPair::set<TextureNode>(obj, wkey, engine);

		auto pair = codex.emplace(wkey, std::move(p));

		auto x = node["Group"].toString().toStdString();
		cats[x].emplace_back(pair.first->second->GetStyleName());
	}
}

const pv::polymorphic_value<UI::Node>& FlowCodex::GetNode(std::string_view nodety)const
{
	return codex.at(nodety.data());
}
void FlowCodex::SetMaxRef(std::string_view nodety, size_t cnt)
{
	auto& r = codex.at(nodety.data());
	r.refcount = std::max(r.refcount, cnt);
}
size_t FlowCodex::AddRef(std::string_view nodety)
{
	return codex.at(nodety.data()).refcount++;
}