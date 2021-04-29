#include <UI/Connection.h>
#include <UI/FlowView.h>

using namespace UI;

Connection::Connection(Node& node, Port ty, size_t portidx)
{
	switch (ty)
	{
	case Port::Sink:
		out = node.GetPortPos(ty, portidx);
		connector.second = &node;
		break;
	case Port::Source:
		in = node.GetPortPos(ty, portidx);
		connector.first = &node;
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