#pragma once 
#include <Interfaces/ISerialize.h>
#include <Logic/PortType.h>
#include <QPointF>

namespace UI
{
	struct NOVTABLE IConnection : public ISerialize
	{
		virtual ~IConnection() = default;
		virtual void Move(QPointF deltapos, PortSide ty) = 0;
		virtual void UpdatePosition() = 0;
		virtual void RemoveForce()noexcept = 0;
	};
}
