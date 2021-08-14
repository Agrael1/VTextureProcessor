#include <UI/REFlowCodex.h>
#include <QJsonDocument>
#include <Logic/ShaderNode.h>

using namespace UI::RE;


XFlowCodex::XFlowCodex()
{
	namespace fs = std::filesystem;
	engine.emplace(QSize{ 256, 256 });

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
			p = RefCountPair::set<XNode<ver::ShaderNode>>(obj, wkey, *engine);

		// Adds new group to the context menu
		auto pair = codex.emplace(wkey, std::move(p));
		auto x = node["Group"].toString().toStdString();
		cats[x].emplace_back(wkey);
	}
}