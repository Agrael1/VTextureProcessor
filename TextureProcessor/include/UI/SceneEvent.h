#pragma once
#include <QEvent>

struct SceneEvent : public QEvent
{
	SceneEvent(std::filesystem::path&& projPath, QPoint size)
		:QEvent((QEvent::Type)(QEvent::User + 2)) {}
};