#include <UI/FlowView.h>
#pragma optimize ("", off)
FlowView::FlowView(FlowScene& scene)
	:scene(scene), QGraphicsView(&scene, nullptr), menu(nullptr)
{
	setRenderHints(QPainter::Antialiasing |
		QPainter::TextAntialiasing |
		QPainter::HighQualityAntialiasing |
		QPainter::SmoothPixmapTransform);

	setDragMode(QGraphicsView::ScrollHandDrag);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

	auto& m = scene.GetGroupList();
	for (const auto& x : m)
	{
		menu.AppendGroup(x.first);
		for (const auto& y : x.second)
		{
			menu.AppendItem(y);
		}
	}	
	menu.Finish();
}

void FlowView::wheelEvent(QWheelEvent* event)
{
	QPoint delta = event->angleDelta();

	if (delta.y() == 0)
	{
		event->ignore();
		return;
	}

	const float d = float(delta.y()) / std::abs(delta.y());

	if (d > 0.0f)
		scaleUp();
	else
		scaleDown();
}
void FlowView::contextMenuEvent(QContextMenuEvent* event)
{
	menu.Execute(event->globalPos());

	//auto& x = scene.CreateNode("somebody");
	//QPoint pos = event->pos();
	//QPointF posView = mapToScene(pos);
	//x.setPos(posView);
}

void FlowView::scaleUp()
{
	constexpr double factor = 1.2;

	QTransform t = transform();
	if (t.m11() > 2.0)
		return;

	scale(factor, factor);
}
void FlowView::scaleDown()
{
	constexpr double factor = 1/1.2;
	scale(factor, factor);
}