/**
 * @file ProjectEvent.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief Custom event for application
 */
#pragma once
#include <Projects/ProjectDescriptor.h>
#include <QEvent>


namespace UI
{
	template<uint32_t event>
	struct VEvent: public QEvent {
		static constexpr const QEvent::Type etype = (QEvent::Type)(QEvent::User + event);
		VEvent() :QEvent(etype) {}
	};

	struct ProjectEvent : public VEvent<1> {
		ProjectDescriptor pd;
		ProjectEvent(ProjectDescriptor pd):pd(pd) {}
	};
	struct NameChangedEvent : public VEvent<3> {
		QString name;
		NameChangedEvent(QString new_name) : name(new_name) {}
	};
}


