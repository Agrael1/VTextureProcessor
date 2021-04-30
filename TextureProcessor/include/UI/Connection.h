#pragma once
#include <unordered_map>
#include <span>
#include <optional>
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
		//void Remove();
	private:
		void Init();
		void mouseMoveEvent(QGraphicsSceneMouseEvent* event)override;
		void mouseReleaseEvent(QGraphicsSceneMouseEvent* event)override;
		void PlaceConnection(std::optional<std::pair<Port, uint8_t>> port, Node* node);


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
		static auto DetachTemporary()
		{
			return std::move(Instance().tmp);
		}
		static void ConnectApply();
		static ConnMapper& Instance();
		static void Map(Node* n, Connection* c);
		static void Unmap(Node* n, Connection* c)
		{
			auto& x = Instance().map.at(n);
			auto it = std::find(x.begin(), x.end(), c);
			if (it != x.end())x.erase(it);
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

