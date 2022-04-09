#include <windows\SceneTab.h>
#include <fstream>
#include <QJsonDocument>
#include <QFileDialog>
#include <Logic/Constants.h>
#include <Logic/Engine.h>


using namespace UI::Windows;
namespace fs = std::filesystem;

SceneTab::SceneTab(std::filesystem::path&& p, Properties& props, QSize resolution)
	:Tab(std::move(p)), scene(nullptr, props, resolution), view(&scene)
{
	scene.setSceneRect(-32000, -32000, 64000, 64000);
	if (resolution != QSize{})Save();
}

UI::Windows::SceneTab::~SceneTab()
{
	Engine::Instance().UnbindScene(&scene);
}

void SceneTab::Save()
{
	std::fstream f;
	f.open(Path(), std::ios::out);
	QJsonObject o;
	scene.Serialize(o);
	QJsonDocument doc{ o };
	f << doc.toJson().constData();
}
void SceneTab::SaveAs()
{
	fs::path proj_path{ QFileDialog::getSaveFileName(
		nullptr,
		"Create new project",
		"",
		ver::proj_filter.c_str()
	).toStdString() };

	if (proj_path.empty()) return;

	if (!proj_path.has_extension()) {
		proj_path.replace_extension(ver::proj_ext.c_str());
	}

	std::fstream f;
	f.open(proj_path, std::ios::out);
	if (!f.is_open()) return;
	QJsonObject o;
	scene.Serialize(o);
	QJsonDocument doc{ o };
	f << doc.toJson().constData();
}

void SceneTab::Clear()
{
	scene.Clear();
}

bool SceneTab::Load()
{
	using namespace std::string_literals;
	std::ifstream t;
	t.open(Path());
	if (!t.is_open())return false;

	std::string str;
	t.seekg(0, std::ios::end);

	//preallocation
	int x = t.tellg();
	str.reserve(x);
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	if (str.empty())return false;

	QJsonParseError e;

	auto json = QJsonDocument::fromJson(QByteArray::fromStdString(str), &e).object();
	if (e.error != QJsonParseError::NoError) { qDebug() << e.errorString(); return false; }
	Engine::Instance().BindScene(&scene, scene.Dimensions(json));
	Engine::SwitchScene(&scene);
	return scene.Deserialize(json);
}

void UI::Windows::SceneTab::OnEnter() noexcept
{
	Engine::SwitchScene(&scene);
	scene.OnSelectionChanged();
}

void UI::Windows::SceneTab::Request(UI::Request rq)
{
	switch (rq)
	{
	case UI::Request::Save:if (IsTemporary())return SaveAs(); return Save();
	case UI::Request::SaveAs:return Save();
	case UI::Request::Delete: return scene.DeleteSelected();
	case UI::Request::Clear: return scene.Clear();
	case UI::Request::ClearSel: return scene.clearSelection();
	case UI::Request::Compile:
	case UI::Request::Export: return scene.ExportAll();
	default:break;
	}
}
