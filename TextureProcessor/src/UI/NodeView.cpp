#include <UI/NodeView.h>

NodeSceneView::NodeSceneView(QGraphicsScene* scene)
	:QGraphicsView(scene, nullptr)
{
	setRenderHints(QPainter::Antialiasing |
		QPainter::TextAntialiasing |
		QPainter::HighQualityAntialiasing |
		QPainter::SmoothPixmapTransform);

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setResizeAnchor(NoAnchor);
	setTransformationAnchor(AnchorUnderMouse);
}