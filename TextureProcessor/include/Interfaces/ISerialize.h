#pragma once
#include <Utils/Macros.h>
#include <QJsonObject>


struct NOVTABLE ISerialize
{
	virtual ~ISerialize() = default;
	virtual void Serialize(QJsonObject& doc) = 0;
	virtual bool Deserialize(QJsonObject doc) = 0;
};
