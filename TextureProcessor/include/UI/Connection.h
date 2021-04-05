#pragma once
#include <PortType.h>

namespace UI
{
	class Connection : public QGraphicsItem
	{
	public:
		Connection(QPointF pos, Port ty);
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
		QPointF in;
		QPointF out;
		constexpr static const qreal linewidth = 2.0;
	};
}

