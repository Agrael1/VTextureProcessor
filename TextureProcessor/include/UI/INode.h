/**
 * @file INode.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief Interfaces for class management
 */
#pragma once
#include <Utils/Macros.h>
#include <QJsonObject>
#include <QGraphicsWidget>
#include <memory>

namespace UI
{
	namespace Windows{class XProperties;}
	class Node;
	struct NOVTABLE INode
	{
		virtual ~INode() = default;
		virtual void OnConnect(uint8_t sinkN, Node& source, uint8_t sourceN) = 0;
		virtual void OnDisconnect(uint8_t sinkN) = 0;
		virtual void Update() = 0;
		virtual std::string Export() = 0;
		virtual void ExportSilent(std::string_view hint) = 0;
	};
	struct NOVTABLE ISerialize
	{
		virtual ~ISerialize() = default;
		virtual QJsonObject Serialize() = 0;
		virtual void Deserialize(QJsonObject) = 0;
	};

	struct NOVTABLE IXNode:public QGraphicsWidget
	{
		virtual ~IXNode() = default;
		virtual std::unique_ptr<IXNode> Clone(std::string&& name)const = 0;
		virtual std::string_view Name()const = 0;
		virtual void UpdateProperties(Windows::XProperties& properties) = 0;
		virtual void Update() = 0;
	};
}
