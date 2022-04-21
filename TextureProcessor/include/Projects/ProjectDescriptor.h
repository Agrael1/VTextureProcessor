#pragma once
#include <filesystem>
#include <QSize>

struct ProjectDescriptor {
	std::filesystem::path project_path;
	QSize pixel_size;
};
