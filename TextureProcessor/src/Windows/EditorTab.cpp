#include <Windows/EditorTab.h>
#include <Logic/Engine.h>
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
	using namespace std::string_literals;
	//QFile t(Path().string().c_str());
	//t.open(QIODevice::ReadOnly | QIODevice::Text);
	//edit.LoadText(t.readAll());
}

void UI::Windows::EditorTab::OnEnter() noexcept
{
	Engine::SwitchScene(&scene.scene);
	tp.show();
}
void UI::Windows::EditorTab::OnLeave() noexcept
{
	tp.hide();
}
