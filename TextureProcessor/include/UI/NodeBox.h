#pragma once
#include <QGraphicsScene>

class NodeBox : public QGraphicsScene
{
public:
	NodeBox(QObject* parent);
protected:
	void drawBackground(QPainter* painter, const QRectF& rect) override;
private:
	QColor Cbackground;
	QColor Clight;
	QColor Cdark;

	QPen Plight;
	QPen Pdark;

	QBrush Bbackground;
};