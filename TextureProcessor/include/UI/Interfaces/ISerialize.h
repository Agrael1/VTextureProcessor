#pragma once
#include <Utils/Macros.h>
#include <QJsonObject>

namespace UI
{
	struct NOVTABLE ISerialize
	{
		virtual ~ISerialize() = default;
		virtual QJsonObject Serialize() = 0;
		virtual void Deserialize(QJsonObject) = 0;
	};
}