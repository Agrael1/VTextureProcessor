#include <Windows/EditorTab.h>
#include <Windows/Properties.h>
#include <Logic/Engine.h>
#include <UI/ProjectEvent.h>
#include <Editor/NodeParser.h>

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
		edited.Rename(((NameChangedEvent*)e)->name);
		return true;
	default:return QMainWindow::event(e);
	}
}


void UI::Windows::EditorTab::Compile()
{
	auto code = edit.edit.GetText().toStdWString();
	if (code.empty())return;

	NodeParser p;
	p.Parse(code);

	edit.edit.SetTypeInfo(p.GetTypesInfo());
	edit.edit.SetFuncInfo(p.GetFuncsInfo());
}
void UI::Windows::EditorTab::Request(UI::Request rq)
{
	if (rq == Request::Compile)
		return Compile();
}
