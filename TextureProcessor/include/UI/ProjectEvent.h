/**
 * @file ProjectEvent.h
 * @author David Černý (xcerny74)
 * @brief Custom event for application
 */
#pragma once
#include <filesystem>
#include <QEvent>

struct ProjectEvent : public QEvent {
	std::filesystem::path projPath;

	ProjectEvent(std::filesystem::path&& projPath)
		:projPath(std::move(projPath)), QEvent((QEvent::Type)(QEvent::User + 1)) {}
};
struct NewProjEvent : public QEvent {
	enum class Type { Forward, Back }ty;
	NewProjEvent(Type ty = Type::Forward) :QEvent((QEvent::Type)(QEvent::User + 2)), ty(ty) {}
};

