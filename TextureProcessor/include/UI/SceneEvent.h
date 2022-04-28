#pragma once
#include <QContextMenuEvent>

struct SceneEvent : public QContextMenuEvent
{
	QPointF scene_pos;
	SceneEvent(const QContextMenuEvent& e, QPointF scene_pos)
		:QContextMenuEvent(e), scene_pos(scene_pos){}
};
struct SceneMouseEvent : public QMouseEvent
{
	QPointF scene_pos;
	SceneMouseEvent(const QMouseEvent& e, QPointF scene_pos)
		:QMouseEvent(e), scene_pos(scene_pos) {}
};