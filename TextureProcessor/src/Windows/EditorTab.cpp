#include <Windows/EditorTab.h>
#include <Windows/Properties.h>
#include <Logic/Engine.h>
#include <Logic/Constants.h>
#include <UI/ProjectEvent.h>
#include <Editor/NodeParser.h>

#include <QJsonDocument>
#include <QSaveFile>
#include <DockWidgetTab.h>

#include <fstream>
#include <filesystem>
#include <iterator>
#include <ranges>


namespace fs = std::filesystem;

using namespace UI::Windows;

constexpr QSize EditSize{ 128,128 };


class DockState
{
private:
	constexpr static auto cfg = ver::config<"edock">();
public:
	static DockState& instance()
	{
		static DockState ds;
		return ds;
	}
	void SaveState(QByteArray st)
	{
		docker_state = std::move(st);
		QSaveFile file(cfg.c_str());
		file.open(QIODevice::WriteOnly);
		file.write(docker_state);
		file.commit();
	}
	auto& State()const
	{
		return docker_state;
	}
private:
	DockState()
	{
		QFile CurrentFile(cfg.c_str());
		if (!CurrentFile.open(QIODevice::ReadOnly)) return;
		docker_state = CurrentFile.readAll();
	}
private:
	QByteArray docker_state;
};



EditorTab::SceneDock::SceneDock(Properties& props)
	:base_class(QStringLiteral("scene")), scene(nullptr, props, EditSize), view(&scene)
{
	scene.setSceneRect(-32000, -32000, 64000, 64000);
	setFeatures(QFlags{ base_class::DockWidgetFeature::DefaultDockWidgetFeatures } &= ~base_class::DockWidgetFeature::DockWidgetClosable);
	setWidget(&view);
	Engine::BindScene(&scene, EditSize);
}
UI::Windows::EditorTab::SceneDock::~SceneDock()
{
	Engine::UnbindScene(&scene);
}

bool EditorTab::Load()
{
	edit.edit.LoadText(tdesc->shader_body);
	return true;
}

void UI::Windows::EditorTab::OnEnter() noexcept
{
	Engine::SwitchScene(&scene.scene);
	tp.show();
	scene.scene.OnSelectionChanged();
}
void UI::Windows::EditorTab::OnLeave() noexcept
{
	tp.hide();
}

void UI::Windows::EditorTab::Save()
{
	std::fstream f;
	f.open(Path(), std::ios::out);
	tdesc->group = tp.GetCategory();
	QJsonDocument doc{ tdesc->Save() };
	f << doc.toJson().constData();
}
void UI::Windows::EditorTab::SaveAs()
{
	fs::path proj_path{ QFileDialog::getSaveFileName(
	nullptr,
	QStringLiteral("Save Node As"),
	"",
	ver::node_filter.c_str()).toStdString() };

	if (proj_path.empty()) return;

	if (!proj_path.has_extension()) {
		proj_path.replace_extension(ver::proj_ext.c_str());
	}

	std::fstream f;
	f.open(proj_path, std::ios::out);
	tdesc->group = tp.GetCategory();
	QJsonDocument doc{ tdesc->Save() };
	f << doc.toJson().constData();
	SetPath(proj_path);
	SetName(QString::fromStdWString(proj_path.filename().wstring()));
}

void UI::Windows::EditorTab::timerEvent(QTimerEvent* event)
{
	if (!text_changed) return;
	auto code = edit.edit.GetText().toStdWString();
	if (code.empty())return;

	NodeParser p(code);
	p.Parse();

	edit.edit.SetInfo(p.GetTypesInfo(), p.GetMacrosInfo(), p.GetFuncsInfo());
	text_changed = false;
}

void UI::Windows::EditorTab::Init(Properties& props) noexcept
{
	docker.setStyleSheet("");
	docker.addDockWidget(ads::LeftDockWidgetArea, &edit);
	docker.addDockWidget(ads::RightDockWidgetArea, &scene);
	docker.addDockWidget(ads::BottomDockWidgetArea, &con);
	((QMainWindow*)props.parentWidget())->addDockWidget(Qt::RightDockWidgetArea, &tp, Qt::Vertical);

	connect(&edit.edit, &Editor::TextChanged, [this]() {
		text_changed = true;
		});
	connect(&tp, &TableProperties::NameChanged, [this](const QString& name) {
		tdesc->style.SetStyleName(name);
		node->UpdateHeader();
		});
	connect(&tp, &TableProperties::HeadColorChanged, [this](const QColor& c) {
		tdesc->style.SetHeaderColor(c);
		node->update();
		});
	connect(&tp, &TableProperties::FontColorChanged, [this](const QColor& c) {
		tdesc->style.SetFontColor(c);
		node->UpdateFontColor();
		});
}

void UI::Windows::EditorTab::SetCBufInfo()
{
	std::unordered_set<std::wstring> consts;
	for (auto& i : tdesc->buffer.Get())
		consts.emplace(i.first.begin(), i.first.end());
	edit.edit.SetCBufInfo(std::move(consts));
}

UI::Windows::EditorTab::EditorTab(std::filesystem::path&& p, Properties& props)
	:Tab(std::move(p)), scene(props), tp(this), docker(this)
{
	Engine::SwitchScene(&scene.scene);

	con.console.setPlainText("---------------------Compiling Node Shader----------------------");
	tdesc.emplace(Parse(Path()));
	tdesc->prop_callback = [this]() {SetCBufInfo(); };

	auto rx = std::move(tdesc->last_error);
	con.console.appendPlainText(rx);
	if (rx.isEmpty())
		con.console.appendPlainText("---------------------Compilation Successful-----------------------");
	else
		con.console.appendPlainText("---------------------Compilation Failed---------------------------");

	node.emplace(tdesc->MakeModel());
	SetCBufInfo();

	tdesc->SetParent(&node.value());

	Init(props);
	auto x = tdesc->style.StyleName();

	tp.SetName(x.isEmpty() ? QStringLiteral("Node") : x);
	tp.SetCatList(scene.scene.GetCategories());
	tp.SetCategory(tdesc->group);
	tp.SetTitleColor(tdesc->style.HeaderColor());
	tp.SetFontColor(tdesc->style.FontColor());
	scene.scene.addItem(&*node);

	docker.restoreState(DockState::instance().State());
	tid = startTimer(2000);
}
UI::Windows::EditorTab::~EditorTab()
{
	killTimer(tid);
	DockState::instance().SaveState(docker.saveState());
	docker.removeDockWidget(&scene);
	docker.removeDockWidget(&edit);
	docker.removeDockWidget(&con);
}
QJsonObject UI::Windows::EditorTab::Parse(const std::filesystem::path& p)
{
	if (p.empty())return{};

	std::ifstream t(p);
	if(!t.is_open())return{};

	std::string str;

	t.seekg(0, std::ios::end);
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());
	if(str.empty())return{};

	QJsonParseError e{};
	// Deserialize loaded JSON
	auto json = QJsonDocument::fromJson(QByteArray::fromStdString(str), &e);
	if (e.error != QJsonParseError::NoError) { qDebug() << e.errorString(); return{}; }

	QJsonObject node = json.object();

	// Loads texture Node style
	if (node[u"Class"].toString() == u"Texture")
		return node;
	return {};
}

void UI::Windows::EditorTab::Compile()
{
	con.console.setPlainText("---------------------Compiling Node Shader----------------------");

	tdesc->shader_body = edit.edit.GetText();
	tdesc->Recompile();
	auto x = std::move(tdesc->last_error);
	con.console.appendPlainText(x);
	if (x.isEmpty())
		con.console.appendPlainText("---------------------Compilation Successful-----------------------");
	else
		con.console.appendPlainText("---------------------Compilation Failed---------------------------");

	SetCBufInfo();
	auto pos = node->pos();
	node.emplace(tdesc->MakeModel());
	scene.scene.addItem(&*node);
	node->setPos(pos);

	timerEvent(nullptr);
}
void UI::Windows::EditorTab::Request(UI::Request rq)
{
	switch (rq)
	{
	case UI::Request::Save:if (IsTemporary())return SaveAs(); return Save();
	case UI::Request::SaveAs:return SaveAs();
	case UI::Request::Compile:return Compile();
	case UI::Request::Delete:return scene.scene.DeleteSelected();
	case UI::Request::Clear:return scene.scene.Clear();
	case UI::Request::ClearSel:return scene.scene.clearSelection();
	default:break;
	}
}

UI::Windows::EditorTab::EditorDock::EditorDock() :base_class(QStringLiteral("Editor")) 
{ 
	setFeatures(QFlags{ base_class::DockWidgetFeature::DefaultDockWidgetFeatures } &= ~base_class::DockWidgetFeature::DockWidgetClosable); setWidget(&edit); 
}
UI::Windows::EditorTab::ConsoleDock::ConsoleDock() :base_class("Console")
{
	console.setReadOnly(true);
	QFont a{};
	a.setPixelSize(14);
	console.setFont(a);
	setFeatures(QFlags{ base_class::DockWidgetFeature::DefaultDockWidgetFeatures } &= ~base_class::DockWidgetFeature::DockWidgetClosable);
	setWidget(&console);
}
