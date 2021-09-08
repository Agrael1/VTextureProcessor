#include <UI/Port.h>
#include <UI/Connection.h>
#include <QPainter>
#include <ranges>

UI::Port::Port(INode& parent, uint8_t port_num)
	:parent(parent), port_num(port_num)
{
	setGraphicsItem(this);
}
UI::Port::Port(Port&& in) noexcept
	:parent(in.parent)
	, port_num(in.port_num)
{
	setGraphicsItem(this);
}

QRectF UI::Port::boundingRect() const
{
	return { 0,0,PortStyle::port_bbox,PortStyle::port_bbox };
}

void UI::Port::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->drawEllipse(QPointF{ PortStyle::port_bbox / 2,PortStyle::port_bbox / 2 }, PortStyle::diameter / 2, PortStyle::diameter / 2);
}

QSizeF UI::Port::sizeHint(Qt::SizeHint which, const QSizeF& constraint) const
{
	return { PortStyle::port_bbox,PortStyle::port_bbox };
}

UI::Sink::Sink(INode& parent, uint8_t port_num, ver::Sink& model)
	:Port(parent, port_num), model(model)
{}

void UI::Sink::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	auto style = PortStyle::Grayscale;
	painter->setBrush(connection ? style.brSinkUsed : style.brSink);
	painter->drawEllipse(QPointF{ PortStyle::port_bbox / 2,PortStyle::port_bbox / 2 }, PortStyle::diameter / 2, PortStyle::diameter / 2);
}

void UI::Sink::MoveConnections(QPointF delta)
{
	if (connection)
		connection->Move(delta, PortSide::Sink);
}

void UI::Sink::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	if (connection)
		return ConnectionMap::AttachTemporary(std::move(connection));
	ConnectionMap::MakeTemporary(*this);
}

void UI::Source::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	auto style = PortStyle::Grayscale;
	painter->setBrush(style.brSource);
	painter->drawEllipse(QPointF{ PortStyle::port_bbox / 2,PortStyle::port_bbox / 2 }, PortStyle::diameter / 2, PortStyle::diameter / 2);
}

void UI::Source::MoveConnections(QPointF delta)
{
	for (auto& s : ConnectionMap::Get(Node()) | std::views::transform([](Connection* x)->IConnection& {return Query(x); }))
		s.Move(delta, PortSide::Source);
}

void UI::Source::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	ConnectionMap::MakeTemporary(*this);
}
