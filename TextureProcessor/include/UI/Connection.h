#pragma once
#include <unordered_map>
#include <span>
#include <PortType.h>

namespace UI
{
	class Node;
	class Connection : public QGraphicsItem
	{
		friend class ConnMapper;
	private:
		Connection(Node& node, Port ty, uint8_t portidx);
	public:
		void paint(QPainter* painter,
			const QStyleOptionGraphicsItem* option,
			QWidget* widget = nullptr) override;
		QRectF boundingRect()const override;
		Node* GetSink()const noexcept { return connector.second; }
		void Remove();
	private:
		void Init();
		void mouseMoveEvent(QGraphicsSceneMouseEvent* event)override;
		void mouseReleaseEvent(QGraphicsSceneMouseEvent* event)override;


		std::pair<QPointF, QPointF> PointsC1C2()const;
		Port Requires()const;
		void MoveEndpoint(Port port, QPointF offset);
		PortType GetType()const noexcept;
		Node* StartNode()const noexcept { return connector.first ? connector.first : (connector.second ? connector.second : nullptr); }
		void FillPair();
	private:
		std::pair<Node*/*source*/, Node*/*sink*/> connector{};
		uint8_t sourceN;
		uint8_t sinkN;
		QPointF source;
		QPointF sink;
		constexpr static const int linewidth = 2;
	};


	class ConnMapper
	{
	public:
		static void MakeTemporary(Node& node, Port port, uint8_t portidx);
		static void ClearTemporary();
		static void ConnectApply();
		static ConnMapper& Instance();
		static void Map(Node* n, Connection* c);
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

