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
#include <Logic/PortType.h>

namespace ver
{
	class Node;
}

namespace UI
{
	namespace Windows{class XPropertyElement;}
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

	class XConnection;
	struct NOVTABLE IXNode:public QGraphicsWidget, public ISerialize
	{
		virtual ~IXNode() = default;
		virtual std::unique_ptr<IXNode> Clone(std::string&& name)const = 0;
		virtual std::string_view Name()const = 0;
		virtual void UpdateProperties(Windows::XPropertyElement& properties) = 0;
		virtual void Update() = 0;
		virtual void OnConnect(uint8_t sinkN, IXNode& source, uint8_t sourceN) = 0;
		virtual void OnDisconnect(uint8_t sinkN) = 0;
		virtual ver::Node& Model()noexcept = 0;

		virtual std::pair<Port, uint8_t> PortHitScene(QPointF scene_point) = 0;
		virtual void SetConnection(std::unique_ptr<XConnection>&& in, uint8_t portN) = 0;
		virtual QPointF GetPortPos(Port portTy, uint8_t portN) = 0;
	};
}
