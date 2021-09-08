#include <UI/FlowCodex.h>
#include <UI/Node.h>
#include <QJsonDocument>
#include <Logic/ShaderNode.h>

using namespace UI;

constexpr const char* output = R"(
{
    "NodeStyle": {
      "TitleColor": "black",
      "FontColor": "white"
    }
})";

FlowCodex::FlowCodex()
{
	namespace fs = std::filesystem;

	fs::path ndir{ NodesDir };
	// Creates node directory and exits if it doesn't exist
	if (!fs::is_directory(ndir))
	{
		fs::create_directory(ndir);
		return;
	}

	// Scans for all Nodes .json files in the Node directory and loads them
	QJsonParseError e;

	auto json = QJsonDocument::fromJson(output, &e).object();
	if (e.error != QJsonParseError::NoError) { qDebug() << e.errorString(); }
	RefCountPair p = RefCountPair::set<XNode<ver::OutputNode>>(json, "Output");

	// Adds new group to the context menu
	auto pair = codex.emplace("Output", std::move(p));
	cats["Output"].emplace_back("Output");


	for (auto& p : fs::directory_iterator(ndir))
	{
		if (!p.is_regular_file())
			continue;

		if (p.path().extension() == ".json")
		{
			std::ifstream t(p.path());
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

std::unique_ptr<UI::INode> FlowCodex::GetNode(std::string_view nodety)const
{
	auto& x = codex.at(nodety.data());
	return x->Clone(std::format("{}_{}", nodety, x.refcount++));
}
std::unique_ptr<UI::INode> FlowCodex::GetNode(std::string_view nodety, size_t ref)const
{
	auto& x = codex.at(nodety.data());
	x.refcount = std::max(x.refcount, ref + 1);
	return x->Clone(std::format("{}_{}", nodety, ref));
}

bool UI::FlowCodex::contains(std::string_view nodety) const
{
	return codex.contains(nodety.data());
}


void UI::FlowCodex::ParseJson(const QJsonDocument& json)
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
			p = RefCountPair::set<XNode<ver::ShaderNode>>(obj, wkey);

		// Adds new group to the context menu
		auto pair = codex.emplace(wkey, std::move(p));
		auto x = node["Group"].toString().toStdString();
		cats[x].emplace_back((*pair.first).first);
	}
}
