#include <Windows/EditorTab.h>
#include <fstream>
#include <iterator>

using namespace UI::Windows;

void EditorTab::Load()
{
	using namespace std::string_literals;
	QFile t(Path().string().c_str());
	t.open(QIODevice::ReadOnly | QIODevice::Text);

	edit.LoadText(t.readAll());
}