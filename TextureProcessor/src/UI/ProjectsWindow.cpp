#include <UI/ProjectsWindow.h>

using namespace UI;
namespace fs = std::filesystem;

void ProjectsWindow::OnCreateClicked(bool checked) 
{
	auto projName = QFileDialog::getSaveFileName(
		nullptr,
		"Create new project",
		"",
		"(*.vtproj);;"
	).toStdString();

	if (projName.empty()) return;

	std::filesystem::path projPath{ projName };

	if (!pdata.InCache(projPath)) {
		pdata.AppendCache(projPath);
	}

	if (!fs::exists(projPath))
		std::ofstream{ projPath };

	OpenApp(projPath);
}