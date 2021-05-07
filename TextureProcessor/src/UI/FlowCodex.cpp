#include <UI/FlowCodex.h>
#include <fmt/printf.h>
#include <fstream>
#include <streambuf>

using namespace UI;

FlowCodex::FlowCodex()
	:engine(QSize{ 128,128 })
{
	namespace fs = std::filesystem;
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

std::pair<const pv::polymorphic_value<UI::Node>&, size_t> FlowCodex::MakeNode(std::string_view in)const
{
	const auto& r = codex.at(in.data());
	return { r, r.refcount++ };
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