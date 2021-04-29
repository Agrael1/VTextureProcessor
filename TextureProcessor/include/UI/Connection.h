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
	private:
		void Init()
		{
			setFlag(QGraphicsItem::ItemIsMovable, true);
			setFlag(QGraphicsItem::ItemIsFocusable, true);
			setFlag(QGraphicsItem::ItemIsSelectable, true);
			setAcceptHoverEvents(true);
			setZValue(-1.0);
		}
		void mouseMoveEvent(QGraphicsSceneMouseEvent* event)override;
	private:
		std::pair<Node*, Node*> connector{};
		QPointF in;
		QPointF out;
		constexpr static const qreal linewidth = 2.0;
	};
}

