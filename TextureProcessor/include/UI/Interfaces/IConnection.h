#pragma once 
#include <UI/Interfaces/ISerialize.h>
#include <Logic/PortType.h>
#include <QPointF>

namespace UI
{
	struct NOVTABLE IXConnection : public ISerialize
	{
		virtual ~IXConnection() = default;
		virtual void Move(QPointF deltapos, Port ty) = 0;
		virtual void RemoveForce()noexcept = 0;
	};
}
