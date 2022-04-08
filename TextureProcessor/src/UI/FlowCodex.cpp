#include <UI/FlowCodex.h>
#include <UI/UINode.h>
#include <QJsonDocument>
#include <Logic/ShaderNode.h>
#include <Logic/OutputNode.h>
#include <Logic/Constants.h>
#include <filesystem>
#include <fstream>

using namespace UI;

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

	// Adds new group to the context menu
	auto pair = codex.emplace("Output", std::make_unique<ver::OutputDescriptor>());
	cats["Output"].emplace_back("Output");


	for (auto& p : fs::directory_iterator(ndir))
	{
		if (!p.is_regular_file())
			continue;

		if (p.path().extension() == ver::node_ext.c_str())
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

UI::FlowCodex::~FlowCodex()
{

}


std::unique_ptr<UI::INode> FlowCodex::GetNode(std::string_view nodety)const
{
	auto it = codex.find(nodety.data());
	return it == codex.end() ? nullptr : std::make_unique<UI::NodeUI>(it->second->MakeModel());
}

bool UI::FlowCodex::contains(std::string_view nodety) const
{
	return codex.contains(nodety.data());
}

void UI::FlowCodex::ClearCounts()
{
	for (auto& i : codex)
		i.second->refcount = 0;
}


void UI::FlowCodex::ParseJson(const QJsonDocument& json)
{
	QJsonObject node = json.object();
	
	// Loads texture Node style
	std::unique_ptr<ver::Descriptor> p;
	if (node[u"Class"].toString() == u"Texture")
		p = std::make_unique<ver::TextureDescriptor>(node);
	if (!p || !p->valid())
		return;

	// Adds new group to the context menu
	auto pair = codex.emplace(((ver::TextureDescriptor*)p.get())->style.StyleName().toString().toStdString(), std::move(p));
	auto x = node[u"Group"].toString().toStdString();
	cats[x].emplace_back((*pair.first).first);
}
