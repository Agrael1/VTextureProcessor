#pragma once
#include <QGraphicsScene>

class FlowScene : public QGraphicsScene
{
public:
	FlowScene(QObject* parent);
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