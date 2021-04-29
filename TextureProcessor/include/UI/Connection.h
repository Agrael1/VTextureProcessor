#pragma once
#include <PortType.h>

namespace UI
{
	class Node;
	class Connection : public QGraphicsItem
	{
	public:
		Connection(Node& node, Port ty, size_t portidx);
	public:
		void paint(QPainter* painter,
			const QStyleOptionGraphicsItem* option,
			QWidget* widget = nullptr) override;
		QRectF boundingRect()const override;
	private:
		void Init();
		void mouseMoveEvent(QGraphicsSceneMouseEvent* event)override;
		void mouseReleaseEvent(QGraphicsSceneMouseEvent* event)override;


		std::pair<QPointF, QPointF> PointsC1C2()const;
		Port Requires()const;
		void MoveEndpoint(Port port, QPointF offset);
	private:
		std::pair<Node*, Node*> connector{};
		QPointF source;
		QPointF sink;
		constexpr static const qreal linewidth = 2.0;
	};
}

