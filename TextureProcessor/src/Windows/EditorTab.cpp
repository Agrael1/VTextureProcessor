#include <Windows/EditorTab.h>
#include <Windows/Properties.h>
#include <Logic/Engine.h>
#include <UI/ProjectEvent.h>
#include <Editor/NodeParser.h>

#include <QJsonDocument>

#include <fstream>
#include <iterator>

using namespace UI::Windows;

constexpr QSize EditSize{ 128,128 };

EditorTab::SceneDock::SceneDock(Properties& props)
	:scene(nullptr, props, EditSize), view(&scene)
{
	setWidget(&view);
	Engine::BindScene(&scene, EditSize);
}

UI::Windows::EditorTab::SceneDock::~SceneDock()
{
	Engine::UnbindScene(&scene);
}

void EditorTab::Load()
{
	//using namespace std::string_literals;
	//QFile t(Path().string().c_str());
	//t.open(QIODevice::ReadOnly | QIODevice::Text);
	//edit.LoadText(t.readAll());
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
	addDockWidget(Qt::LeftDockWidgetArea, &edit);
	addDockWidget(Qt::RightDockWidgetArea, &scene);
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
UI::Windows::EditorTab::EditorTab(std::filesystem::path&& p, Properties& props)
	:Tab(std::move(p)), scene(props), tp(this)
{
	Engine::SwitchScene(&scene.scene);
	auto[a,b] = Parse(Path());
	tdesc.emplace(a, b);
	node.emplace(tdesc->MakeModel());
	tdesc->SetParent(&*node);

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
	tdesc->Recompile();

	auto code = edit.edit.GetText().toStdWString();
	if (!code.empty())return;

	NodeParser p(code);
	p.Parse();

	edit.edit.SetInfo(p.GetTypesInfo(), p.GetMacrosInfo(), p.GetFuncsInfo());
}
void UI::Windows::EditorTab::Request(UI::Request rq)
{
	if (rq == Request::Compile)
		return Compile();
}
