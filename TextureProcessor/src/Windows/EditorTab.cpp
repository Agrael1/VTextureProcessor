#include <Windows/EditorTab.h>
#include <Windows/Properties.h>
#include <Logic/Engine.h>
#include <UI/ProjectEvent.h>
#include <Editor/NodeParser.h>

#include <QJsonDocument>
#include <DockWidgetTab.h>

#include <fstream>
#include <iterator>
#include <ranges>

using namespace UI::Windows;

constexpr QSize EditSize{ 128,128 };


EditorTab::SceneDock::SceneDock(Properties& props)
	:base_class("scene"), scene(nullptr, props, EditSize), view(&scene)
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

void EditorTab::Load()
{
	edit.edit.LoadText(tdesc->shader_body);
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
void UI::Windows::EditorTab::Init(Properties& props) noexcept
{
	docker.setStyleSheet("");
	docker.addDockWidget(ads::LeftDockWidgetArea, &edit);
	docker.addDockWidget(ads::RightDockWidgetArea, &scene);
	docker.addDockWidget(ads::BottomDockWidgetArea, &con);
	((QMainWindow*)props.parentWidget())->addDockWidget(Qt::RightDockWidgetArea, &tp, Qt::Vertical);
	tp.hide();
}
bool UI::Windows::EditorTab::event(QEvent* e)
{
	switch (e->type())
	{
	case NameChangedEvent::etype:
		tdesc->style.SetStyleName(((NameChangedEvent*)e)->name);
		node->UpdateHeader();
		return true;
	default:return QMainWindow::event(e);
	}
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
	auto [a, b] = Parse(Path());

	con.console.setPlainText("---------------------Compiling Node Shader----------------------");
	tdesc.emplace(a, b);
	tdesc->prop_callback = [this]() {SetCBufInfo(); };

	auto rx = std::move(tdesc->last_error);
	con.console.appendPlainText(rx);
	if (rx.isEmpty())
		con.console.appendPlainText("---------------------Compilation Successful-----------------------");
	else
		con.console.appendPlainText("---------------------Compilation Failed---------------------------");

	node.emplace(tdesc->MakeModel());
	SetCBufInfo();

	Init(props);
	auto& x = tdesc->style.StyleName();
	tp.SetName(x.isEmpty() ? "Node" : x);
	scene.scene.addItem(&*node);
}
std::pair<QJsonObject, std::string> UI::Windows::EditorTab::Parse(const std::filesystem::path& p)
{
	if (p.empty())return{};

	std::ifstream t(p);
	std::string str;

	t.seekg(0, std::ios::end);
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());
	QJsonParseError e{};
	// Deserialize loaded JSON
	auto json = QJsonDocument::fromJson(QByteArray::fromStdString(str), &e);
	if (e.error != QJsonParseError::NoError) { qDebug() << e.errorString(); return{}; }

	QJsonObject topLevelObject = json.object();

	auto key = topLevelObject.keys()[0];
	{
		QJsonObject obj = topLevelObject[key].toObject();
		// Name of the node style
		auto wkey = key.toStdString();
		auto node = obj["Node"].toObject();

		// Loads texture Node style
		if (node["Class"].toString() == "Texture")
			return { obj, wkey };
		return {};
	}
}

void UI::Windows::EditorTab::Compile()
{
	con.console.setPlainText("---------------------Compiling Node Shader----------------------");

	tdesc->shader_body = edit.edit.GetText();
	tdesc->Recompile();
	auto x = std::move(tdesc->last_error);
	con.console.appendPlainText(x);
	if(x.isEmpty())
		con.console.appendPlainText("---------------------Compilation Successful-----------------------");
	else
		con.console.appendPlainText("---------------------Compilation Failed---------------------------");


	auto pos = node->pos();
	node.emplace(tdesc->MakeModel());
	scene.scene.addItem(&*node);
	node->setPos(pos);

	auto code = edit.edit.GetText().toStdWString();
	if (code.empty())return;

	NodeParser p(code);
	p.Parse();

	edit.edit.SetInfo(p.GetTypesInfo(), p.GetMacrosInfo(), p.GetFuncsInfo());
}
void UI::Windows::EditorTab::Request(UI::Request rq)
{
	switch (rq)
	{
	case UI::Request::Save:return Save();
	case UI::Request::Compile:return Compile();
	case UI::Request::Delete:return scene.scene.DeleteSelected();
	case UI::Request::Clear:return scene.scene.Clear();
	case UI::Request::ClearSel:return scene.scene.clearSelection();
	default:break;
	}
}

UI::Windows::EditorTab::EditorDock::EditorDock() :base_class("Editor") { setFeatures(QFlags{ base_class::DockWidgetFeature::DefaultDockWidgetFeatures } &= ~base_class::DockWidgetFeature::DockWidgetClosable); setWidget(&edit); }

UI::Windows::EditorTab::ConsoleDock::ConsoleDock() :base_class("Console")
{
	console.setReadOnly(true);
	QFont a{};
	a.setPixelSize(14);
	console.setFont(a);
	setFeatures(QFlags{ base_class::DockWidgetFeature::DefaultDockWidgetFeatures } &= ~base_class::DockWidgetFeature::DockWidgetClosable); 
	setWidget(&console);
}
