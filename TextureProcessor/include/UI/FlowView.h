#pragma once
#include <QGraphicsView>
#include <UI/FlowScene.h>
#include <UI/ContextMenu.h>

class FlowView : public QGraphicsView
{
public:
	FlowView(UI::FlowScene& scene);
public:
	void AppendViewMenu(QMenuBar& mb)noexcept { mb.addMenu(&view_menu); }
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
	QMenu view_menu;
	QAction delet;
	QAction clrselect;
};
