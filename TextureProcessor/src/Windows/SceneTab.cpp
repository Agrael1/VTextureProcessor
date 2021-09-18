#include <windows\SceneTab.h>
#include <fstream>
#include <QJsonDocument>
#include <QFileDialog>
#include <Logic/Constants.h>
#include <Logic/Engine.h>


using namespace UI::Windows;
namespace fs = std::filesystem;

SceneTab::SceneTab(Properties& props, std::filesystem::path&& xproj_path, QSize resolution)
	:Tab(std::move(xproj_path)), scene(nullptr, props, resolution), view(&scene)
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
	QJsonDocument doc{ scene.Serialize() };
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
	QJsonDocument doc{ scene.Serialize() };
	f << doc.toJson().constData();
}

void SceneTab::Clear()
{
	scene.Clear();
}

void SceneTab::Load()
{
	using namespace std::string_literals;
	std::fstream t;
	t.open(Path(), std::ios::in);

	std::string str;
	t.seekg(0, std::ios::end);

	//preallocation
	int x = t.tellg();
	str.reserve(x);
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	if (str.empty())return;

	QJsonParseError e;

	auto json = QJsonDocument::fromJson(QByteArray::fromStdString(str), &e).object();
	if (e.error != QJsonParseError::NoError) { qDebug() << e.errorString(); return; }
	Engine::Instance().BindScene(&scene, scene.Dimensions(json));
	OnEnter();
	scene.Deserialize(json);
}

void UI::Windows::SceneTab::OnEnter() noexcept
{
	Engine::SwitchScene(&scene);
}
