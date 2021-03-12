#pragma once
#include <QGraphicsItem>
#include <QBrush>
#include <QPen>

class UINode : public QGraphicsItem
{
public:
	UINode();
public:
	QRectF boundingRect() const override;
	void paint(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		QWidget* widget = nullptr) override;
private:
	QSizeF size{ 150, 120 };

	QPen Default{ "#7F000000" };
	QPen Selected{ "#FFFF36A7" };
	QPen Sources{ "#FF000000" };
	QPen Sinks{ "#FF000000" };

	QBrush Title{ "#E3212121" };
	QBrush Background{ "#E31a1a1a" };
	QBrush BrushSources{ "#FFFF7700" };
	QBrush BrushSinks{ "#FF0077FF" };
};