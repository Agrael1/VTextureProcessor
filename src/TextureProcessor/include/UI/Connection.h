﻿/**
 * @file Connection.h
 * @author Ilya Doroshenko (xdoros01), David Černý (xcerny74)
 * @brief Class for managing connections between Nodes
 */
#pragma once
#include <unordered_map>
#include <span>
#include <optional>
#include <Logic/PortType.h>
#include <UI/INode.h>

namespace UI
{
	class Node;
	class Connection : public QGraphicsItem, public ISerialize
	{
		friend class ConnMapper;
	private:
		Connection(Node& node, Port ty, uint8_t portidx);
	public:
		~Connection();
	public:
		void paint(QPainter* painter,
			const QStyleOptionGraphicsItem* option,
			QWidget* widget = nullptr) override;
		QRectF boundingRect()const override;
		Node* GetSink()const noexcept { return connector.second; }
		void Move(QPointF deltapos, Port ty);
		void ResetSink()
		{
			connector.second = nullptr;
			bFinished = false;
		}
		void RemoveForce()noexcept;
		void Update();
		void UpdateDisconnect();

		virtual QJsonObject Serialize()override;
		virtual void Deserialize(QJsonObject)override {}
	private:
		void Init();
		void mouseMoveEvent(QGraphicsSceneMouseEvent* event)override;
		void mouseReleaseEvent(QGraphicsSceneMouseEvent* event)override;
		void PlaceConnection(std::optional<std::pair<Port, uint8_t>> port, Node* node);

		std::pair<QPointF, QPointF> PointsC1C2()const;
		Port Requires()const;
		PortType GetType()const noexcept;
		Node* StartNode()const noexcept { return connector.first ? connector.first : (connector.second ? connector.second : nullptr); }
	private:
		std::pair<Node*/*source*/, Node*/*sink*/> connector{};
		uint8_t sourceN;
		uint8_t sinkN;
		bool bFinished = false;
		QPointF source;
		QPointF sink;
	};


	class ConnMapper
	{
	public:
		static void MakeTemporary(Node& node, Port port, uint8_t portidx);
		static void ConnectTemporary(Node& node, Port port, uint8_t portidx)
		{
			Instance().tmp->PlaceConnection({ {port, portidx} }, &node);
		}
		static void AttachTemporary(std::unique_ptr<Connection>&& in);
		static void ClearTemporary();
		static auto DetachTemporary()
		{
			return std::move(Instance().tmp);
		}
		static void ConnectApply();
		static void UpdateGraph(Node* from)
		{
			for (auto* c : Get(from))
				c->Update();
		}
		static ConnMapper& Instance();
		static void Map(Node* n, Connection* c);
		static void Unmap(Node* n, Connection* c)
		{
			auto& x = Instance().map.at(n);
			auto it = std::find(x.begin(), x.end(), c);
			if (it != x.end())x.erase(it);
		}
		static void Trim(Node* n)
		{
			auto& x = Instance().map[n];
			for (auto* i : x)
				i->RemoveForce();
			Instance().map.erase(n);
		}
		static std::span<Connection*> Get(Node* n);
	private:
		ConnMapper() = default;
		ConnMapper(ConnMapper&) = delete;
		ConnMapper(ConnMapper&&) = delete;
		void operator=(ConnMapper&) = delete;
		void operator=(ConnMapper&&) = delete;
	private:
		std::unordered_map<Node*, std::vector<Connection*>> map;
		std::unique_ptr<Connection> tmp;
	};
}

