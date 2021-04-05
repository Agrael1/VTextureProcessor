#pragma once
#include <QGraphicsView>
#include <UI/FlowScene.h>
#include <UI/ContextMenu.h>

class FlowView : public QGraphicsView
{
public:
	FlowView(UI::FlowScene& scene);
public:
	UI::Node* LocateNode(QPointF pos)noexcept;

private:
	void wheelEvent(QWheelEvent* event) override;
	void contextMenuEvent(QContextMenuEvent* event)override;
	void scaleUp();
	void scaleDown();
	void OnItemSelected(QTreeWidgetItem* item, int);
private:
	UI::FlowScene& scene;
	ContextMenu menu;

	QPoint last_event;
};
