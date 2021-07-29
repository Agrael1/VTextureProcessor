/**
 * @file ProjectEvent.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief Custom event for application
 */
#pragma once
#include <filesystem>
#include <QEvent>

namespace UI
{
	struct ProjectEvent : public QEvent {
		std::filesystem::path projPath;
		std::pair<int, int> size;

		ProjectEvent(std::filesystem::path&& projPath, std::pair<int, int> size)
			:projPath(std::move(projPath)), QEvent((QEvent::Type)(QEvent::User + 1)), size(size) {}
	};
	struct NewProjEvent : public QEvent {
		enum class Type { Forward, Back }ty;
		NewProjEvent(Type ty = Type::Forward) :QEvent((QEvent::Type)(QEvent::User + 2)), ty(ty) {}
	};
}


