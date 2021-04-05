#include <UI/Connection.h>
#include <UI/FlowView.h>

using namespace UI;

Connection::Connection(QPointF pos, Port ty)
{
	switch (ty)
	{
	case Port::Sink:
		out = pos;
		break;
	case Port::Source:
		in = pos;
		break;
	default:
		break;
	}
	Init();
}

void Connection::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{

}

void UI::Connection::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	auto& view = static_cast<FlowView&>(*event->widget());


	auto* node = view.LocateNode(event->pos());

}