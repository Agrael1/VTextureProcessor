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
	void DrawNodeRect(QPainter* painter);
	void DrawConnectionPoints(QPainter* painter);
private:
	size_t sinks, sources;
	QSizeF size;


	QPen Sources{ "#FF000000" };
	QPen Sinks{ "#FF000000" };

	struct Boundary
	{
		QColor Selected{ "#FFFF36A7" };
		QColor Normal{ "#7F000000" };
	}bnd;
	

	QBrush Title{ "#E3212121" };
	QBrush Background{ "#E31a1a1a" };
	QBrush BrushSources{ "#FFFF7700" };
	QBrush BrushSinks{ "#FF0077FF" };
	constexpr static const qreal diameter = 10.0;
	constexpr static const qreal offset = 5;
	constexpr static const qreal title_height = 20.0;
};