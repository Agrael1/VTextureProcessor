#include <UI/FlowView.h>

FlowView::FlowView(UI::FlowScene& scene)
	:scene(scene), QGraphicsView(&scene, nullptr), menu(nullptr), view_menu("View"),
	delet("Delete selected"), clrselect("Clear Selection")
{
	setRenderHints(QPainter::Antialiasing |
		QPainter::TextAntialiasing |
		QPainter::HighQualityAntialiasing |
		QPainter::SmoothPixmapTransform);

	setDragMode(QGraphicsView::ScrollHandDrag);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	setCacheMode(QGraphicsView::CacheBackground);

	auto& m = scene.GetGroupList();
	for (const auto& x : m)
	{
		menu.AppendGroup(x.first);
		for (const auto& y : x.second)
			menu.AppendItem(y);
	}
	menu.Finish();
	menu.SetItemClickCallback([this](QTreeWidgetItem* item, int) {OnItemSelected(item, 0); });

	clrselect.setShortcut(Qt::Key::Key_Escape);
	delet.setShortcut(Qt::Key::Key_Delete);
	connect(&clrselect, &QAction::triggered, &scene, &UI::FlowScene::clearSelection);
	connect(&delet, &QAction::triggered, &scene, &UI::FlowScene::DeleteSelected);
	view_menu.addAction(&delet);
	view_menu.addAction(&clrselect);
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
	last_event = event->pos();
	if (auto* x = itemAt(event->pos()); x && x->isSelected())
	{
		QGraphicsSceneContextMenuEvent e{ QGraphicsSceneContextMenuEvent::GraphicsSceneContextMenu };
		e.setPos(event->pos());
		e.setScreenPos(event->globalPos());
		scene.sendEvent(x, &e);
		return;
	}

	menu.Execute(event->globalPos());
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
	constexpr double factor = 1 / 1.2;
	QTransform t = transform();
	if (t.m11() < 0.2)
		return;
	scale(factor, factor);
}

void FlowView::OnItemSelected(QTreeWidgetItem* item, int)
{
	auto modelName = item->data(0, Qt::UserRole).toString().toStdString();

	if (modelName == skipper)return;

	auto& type = scene.CreateNode(modelName);
	{
		QPointF posView = mapToScene(last_event);
		type.setPos(posView);
	}

	menu.close();
}