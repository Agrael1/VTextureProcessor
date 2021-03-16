#pragma once
#include <QGraphicsView>
#include <UI/FlowScene.h>
#include <UI/ContextMenu.h>

class FlowView : public QGraphicsView
{
public:
	FlowView(FlowScene& scene);
public:
	void wheelEvent(QWheelEvent* event) override;
	void contextMenuEvent(QContextMenuEvent* event)override;
private:
	void scaleUp();
	void scaleDown();
private:
	FlowScene& scene;
	ContextMenu menu;
};
