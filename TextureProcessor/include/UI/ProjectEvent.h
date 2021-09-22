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
	template<uint32_t event>
	struct VEvent: public QEvent {
		static constexpr const QEvent::Type etype = (QEvent::Type)(QEvent::User + event);
		VEvent() :QEvent(etype) {}
	};

	struct ProjectEvent : public VEvent<1> {
		std::filesystem::path projPath;
		QSize size;

		ProjectEvent(std::filesystem::path&& projPath, QSize size)
			:projPath(std::move(projPath)), size(size) {}
	};
	struct NewProjEvent : public VEvent<2> {
		enum class Type { Forward, Back }ty;
		NewProjEvent(Type ty = Type::Forward) :ty(ty) {}
	};
	struct NameChangedEvent : public VEvent<3> {
		QString name;
		NameChangedEvent(QString new_name) : name(new_name) {}
	};
}


