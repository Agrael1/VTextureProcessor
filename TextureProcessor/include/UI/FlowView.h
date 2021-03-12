#pragma once
#include <QGraphicsView>

class FlowView : public QGraphicsView
{
public:
	FlowView(QGraphicsScene* scene);
public:
	void wheelEvent(QWheelEvent* event) override;
private:
	void scaleUp();
	void scaleDown();
};
