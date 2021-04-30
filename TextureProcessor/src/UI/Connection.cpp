#include <UI/Connection.h>
#include <UI/FlowView.h>

using namespace UI;

Connection::Connection(Node& node, Port ty, uint8_t portidx)
{
	node.scene()->addItem(this);
	sink = source = node.GetPortPos(ty, portidx);
	switch (ty)
	{
	case Port::Sink:
		connector.second = &node;
		sinkN = portidx;
		break;
	case Port::Source:
		connector.first = &node;
		sourceN = portidx;
		break;
	default:
		break;
	}
	Init();
}

void Connection::Init()
{
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);

	setAcceptHoverEvents(true);
	setZValue(1.0);
}

QRectF UI::Connection::boundingRect() const
{
	auto points = PointsC1C2();
	QRectF c1c2Rect = QRectF(points.first, points.second).normalized();
	QRectF commonRect = QRectF(source, sink).normalized().united(c1c2Rect);

	constexpr QPointF cornerOffset(PortStyle::diameter, PortStyle::diameter);

	commonRect.setTopLeft(commonRect.topLeft() - cornerOffset);
	commonRect.setBottomRight(commonRect.bottomRight() + 2 * cornerOffset);
	return commonRect;
}



void Connection::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	QPen p;
	p.setWidth(linewidth);
	p.setColor(Qt::white);
	p.setStyle(Qt::DashLine);

	painter->setPen(p);
	painter->setBrush(Qt::NoBrush);

	auto c1c2 = PointsC1C2();
	QPainterPath cubic(source);
	cubic.cubicTo(c1c2.first, c1c2.second, sink);
	painter->drawPath(cubic);
}

void UI::Connection::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	prepareGeometryChange();

	if (auto x = Requires(); any(x))
		MoveEndpoint(x, event->pos() - event->lastPos());

	update();
	event->accept();
}
void UI::Connection::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	ungrabMouse();
	event->accept();

	auto& xscene = static_cast<UI::FlowScene&>(*scene());
	auto* node = xscene.LocateNode(event->scenePos());
	if (node && node != StartNode())
		if (auto p = node->PortHit(event->pos()); p)
		{

		}


	if (any(Requires()))ConnMapper::ClearTemporary();
}

std::pair<QPointF, QPointF> Connection::PointsC1C2()const
{
	constexpr qreal defaultOffset = 200;
	qreal xDistance = sink.x() - source.x();
	qreal horizontalOffset = qMin(defaultOffset, std::abs(xDistance));
	qreal verticalOffset = 0;
	qreal ratioX = 0.5;

	if (xDistance <= 0)
	{
		qreal yDistance = sink.y() - sink.y() + 20;
		qreal vector = yDistance < 0 ? -1.0 : 1.0;
		verticalOffset = qMin(defaultOffset, std::abs(yDistance)) * vector;
		ratioX = 1.0;
	}
	horizontalOffset *= ratioX;

	return { {source.x() + horizontalOffset, source.y() + verticalOffset},
		{sink.x() - horizontalOffset, sink.y() - verticalOffset } };
}

Port Connection::Requires()const
{
	if (!connector.first)
		return Port::Source;
	if (!connector.second)
		return Port::Sink;
	return Port::None;
}
void Connection::MoveEndpoint(Port port, QPointF offset)
{
	switch (port)
	{
	case Port::Sink:
		sink += offset;
		break;
	case Port::Source:
		source += offset;
		break;
	default:
		break;
	}
}
PortType Connection::GetType()const noexcept
{
	if (connector.first)
		return connector.first->GetSourceType(sourceN);
	if (connector.second)
		return connector.first->GetSinkType(sinkN);
	return PortType::None;
}

void Connection::Remove()
{
	if (connector.first && connector.second)
		connector.second->RemoveConnection(this);
}

/////////////////////////////////////////////////////////////////////////

ConnMapper& UI::ConnMapper::Instance()
{
	static ConnMapper mapper;
	return mapper;
}

void ConnMapper::Map(Node* n, Connection* c)
{
	auto& i = Instance();
	i.map[n].push_back(c);
}

std::span<Connection*> UI::ConnMapper::Get(Node* n)
{
	return Instance().map.at(n);
}

void UI::ConnMapper::MakeTemporary(Node& node, Port port, uint8_t portidx)
{
	Instance().tmp.reset(new Connection{node, port, portidx});
	Instance().tmp->grabMouse();
}
void UI::ConnMapper::ConnectApply()
{
	auto& i = Instance();
	i.map[i.tmp->GetSink()].emplace_back(i.tmp.get());
}

void UI::ConnMapper::ClearTemporary()
{
	Instance().tmp.reset();
}