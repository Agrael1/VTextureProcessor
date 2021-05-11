/**
 * @file UINode.cpp
 * @author Ilya Doroshenko (xdoros01), David Černý (xcerny74)
 * @brief Class specifying Node behavior
 */

#include <UI/UINode.h>
#include <UI/Connection.h>
#include <fstream>

/**
 * @brief Statically casts a unique pointer
 *
 * @tparam TO Type of the returned pointer
 * @tparam FROM Type of the input pointer
 * @param old Input pointer
 * @return std::unique_ptr<TO> Returned pointer
 */
template<typename TO, typename FROM>
std::unique_ptr<TO> static_unique_pointer_cast(std::unique_ptr<FROM>&& old) {
	return std::unique_ptr<TO>{static_cast<TO*>(old.release())};
}

using namespace UI;

std::optional<QMenu> Node::conMenu;

// Initializes a new Node
void Node::Init()
{
	setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);

	setCacheMode(QGraphicsItem::DeviceCoordinateCache);

	setAcceptHoverEvents(true);
	setZValue(0);
}

/**
 * @brief Construct a new Node:: Node object
 *
 * @param document JSON specification of the Node
 * @param name Name of the node
 */
Node::Node(QJsonObject document, std::string_view name)
	:style(document, name)
{
	Init();
}

/**
 * @brief Construct a new UI::Node::Node object
 *
 * @param other Node to be copied
 */
UI::Node::Node(const Node& other) noexcept
	:style(other.style),
	body_size(other.body_size),
	sinks(other.sinks),
	sources(other.sources),
	pdelta_sink(other.pdelta_sink),
	pdelta_source(other.pdelta_source)
{
	Sink_conns.resize(other.sinks.size());
	// TODO: remove before presenting
	printf("copy called\n");
	Init();
}

/**
 * @brief Destroy the UI::Node::Node object
 *
 */
UI::Node::~Node()
{
	ConnMapper::Trim(this);
}

/**
 * @brief Calculates sizes between Sinks (and Sources)
 *
 * @param minsize
 */
void UI::Node::CalculateSize(QSizeF minsize) noexcept
{
	qreal max_port = std::max(sources.size(), sinks.size()) * PortStyle::port_bbox + NodeStyle::item_padding * 2;
	qreal max_height = NodeStyle::title_height + std::max(std::max(minsize.height() + 2 * NodeStyle::item_padding, max_port), NodeStyle::min_height);
	qreal max_width = std::max(minsize.width() + PortStyle::port_bbox, NodeStyle::min_width);
	body_size = { max_width, max_height };

	auto height = max_height - NodeStyle::title_height - NodeStyle::item_padding * 2;

	// Calculate distance between sinks (or sources)
	pdelta_sink = height / (sinks.size() + 1);
	pdelta_source = height / (sources.size() + 1);
}


/**
 * @brief Calculates bounding rectangle of Node
 *
 * @return QRectF
 */
QRectF Node::boundingRect() const
{
	// Compensate for pen width
	return QRectF(
		-NodeStyle::pen_width / 2.0 - PortStyle::port_bbox,
		-NodeStyle::pen_width / 2.0,
		body_size.width() + NodeStyle::pen_width / 2.0 + 2.0 * PortStyle::port_bbox,
		body_size.height() + NodeStyle::pen_width / 2.0
	).normalized();
}

/**
 * @brief Gets correct position of Sink or Source
 *
 * @param po Port object
 * @param pos Index of the port
 * @return QPointF Calculated position
 */
QPointF Node::GetPortPos(Port po, size_t pos)
{
	return mapToScene({
		po == Port::Sink ? 0 : body_size.width(),
		po == Port::Sink ? NodeStyle::title_height + PortStyle::port_bbox + pdelta_sink * (pos + 1) :
		NodeStyle::title_height + pdelta_source * (pos + 1) + PortStyle::port_bbox });
}

/**
 * @brief Pains node with header caption and connection points
 *
 * @param painter
 * @param option
 * @param widget
 */
void Node::paint(QPainter* painter,
	const QStyleOptionGraphicsItem* option,
	QWidget* widget)
{
	DrawNodeRect(painter);
	DrawCaptionName(painter);
	DrawConnectionPoints(painter);
}

/**
 * @brief Callback for handling mouse press events
 *
 * @param event
 */
void Node::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	auto pos = event->pos();
	// Detects colision with ports
	if (pos.y() < NodeStyle::title_height + NodeStyle::item_padding ||
		pos.y() > body_size.height() - NodeStyle::item_padding ||
		pos.x() > PortStyle::port_bbox / 2 && pos.x() < body_size.width() - PortStyle::port_bbox / 2)
		return QGraphicsItem::mousePressEvent(event);

	// Port lookup
	if (auto port = PortHit(pos))
	{
		if (port->first == Port::Sink && Sink_conns[port->second])
			return ConnMapper::AttachTemporary(static_unique_pointer_cast<Connection>(std::move(Sink_conns[port->second])));
		ConnMapper::MakeTemporary(*this, port->first, port->second);
	}
}

/**
 * @brief Callback for context menu events
 *
 * @param event
 */
void Node::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
	if (conMenu) {
		event->accept();
		conMenu->popup(event->screenPos());
		return;
	}
	// Creates context menu if it doesn't exist
	conMenu.emplace();
	auto* a = conMenu->addAction("Edit");
	auto* b = conMenu->addAction("Export");
	conMenu->popup(event->screenPos());

	conMenu->connect(b, &QAction::triggered, [this](bool) {Export(); });
	event->accept();
}


/**
 * @brief Callback for handling item changes
 *
 * @param change
 * @param value
 * @return QVariant
 */
QVariant Node::itemChange(GraphicsItemChange change, const QVariant& value)
{
	// Also update item positions on move
	if (change == ItemPositionChange)
		MoveConnections(value.toPointF());

	return QGraphicsItem::itemChange(change, value);
}

/**
 * @brief Moves connections by the same delta as Node
 *
 * @param newpos New position
 */
void Node::MoveConnections(QPointF newpos)
{
	auto delta = newpos - pos();

	// Update positions for all Sinks
	for (auto& s : Sink_conns)
	{
		if (s) {
			static_cast<Connection&>(*s.get()).Move(delta, Port::Sink);
		}
	}
	for (auto s : ConnMapper::Get(this))
	{
		s->Move(delta, Port::Source);
	}
}

/**
 * @brief Update connection graph on Node update
 *
 */
void UI::Node::Update()
{
	ConnMapper::UpdateGraph(this);
}

/**
 * @brief Draws body of the Node
 *
 * @param painter
 */
void Node::DrawNodeRect(QPainter* painter)
{
	constexpr qreal edge_size = 10.0;

	// path for the caption of this node
	QPainterPath path_title;
	path_title.setFillRule(Qt::WindingFill);
	path_title.addRoundedRect(QRect(0, 0, body_size.width(), NodeStyle::title_height), edge_size, edge_size);
	path_title.addRect(0, NodeStyle::title_height - edge_size, edge_size, edge_size);
	path_title.addRect(body_size.width() - edge_size, NodeStyle::title_height - edge_size, edge_size, edge_size);
	painter->setPen(Qt::NoPen);
	painter->setBrush(style.Title());
	painter->drawPath(path_title.simplified());


	// path for the content of this node
	QPainterPath path_content;
	path_content.setFillRule(Qt::WindingFill);
	path_content.addRoundedRect(QRect(0, NodeStyle::title_height, body_size.width(), body_size.height() - NodeStyle::title_height), edge_size, edge_size);
	path_content.addRect(0, NodeStyle::title_height, edge_size, edge_size);
	path_content.addRect(body_size.width() - edge_size, NodeStyle::title_height, edge_size, edge_size);
	painter->setPen(Qt::NoPen);
	painter->setBrush(style.Background());
	painter->drawPath(path_content.simplified());

	if (isSelected()) {
		painter->setPen({ {"#FFFFFFFF"},2.0 });
	} else {
		painter->setPen(Qt::NoPen);
	}
	painter->setBrush(Qt::BrushStyle::NoBrush);
	painter->drawPath((path_title + path_content).simplified());

}

/**
 * @brief Draw Node caption in header
 *
 * @param painter
 */
void Node::DrawCaptionName(QPainter* painter)
{
	QString name{ style.StyleName().data() };
	QFont f = painter->font();
	f.setBold(true);

	QFontMetrics metrics(f);
	auto rect = metrics.boundingRect(name);

	QPointF position((body_size.width() - rect.width()) / 2.0,
		(NodeStyle::title_height - rect.height() / 2.0));

	painter->setFont(f);
	painter->setPen(style.FontColor());
	painter->drawText(position, name);

	f.setBold(false);
	painter->setFont(f);
}

/**
 * @brief Draws connection points on the Node outline
 *
 * @param painter
 */
void UI::Node::DrawConnectionPoints(QPainter* painter)
{
	auto& style = PortStyle::Grayscale;

	painter->setPen(style.port);
	auto ypos = NodeStyle::title_height + NodeStyle::item_padding + pdelta_sink - PortStyle::diameter / 2;
	// Draw each Sink connection point
	for (auto si = 0; si < sinks.size(); si++)
	{
		painter->setBrush(Sink_conns[si] ? style.brSinkUsed : style.brSink);
		painter->drawEllipse(-PortStyle::diameter / 2, ypos, PortStyle::diameter, PortStyle::diameter);
		ypos += pdelta_sink;
	}

	painter->setBrush(style.brSource);
	ypos = NodeStyle::title_height + NodeStyle::item_padding + pdelta_source - PortStyle::diameter / 2;
	// Draw each Source connection point
	for (const auto& so : sources)
	{
		painter->drawEllipse(body_size.width() - PortStyle::diameter / 2, ypos, PortStyle::diameter, PortStyle::diameter);
		ypos += pdelta_source;
	}
}

/**
 * @brief Detects collision with port (Sink or Source)
 *
 * @param point
 * @return std::optional<std::pair<Port, uint8_t>>
 */
std::optional<std::pair<Port, uint8_t>> UI::Node::PortHit(QPointF point)
{
	if (point.x() < PortStyle::port_bbox)
	{
		auto startheight = NodeStyle::title_height + NodeStyle::item_padding + pdelta_sink - PortStyle::port_bbox / 2;
		if (point.y() < startheight)return std::nullopt;
		for (uint8_t si = 0; si < sinks.size(); si++, startheight += pdelta_sink)
		{
			if (point.y() < startheight + PortStyle::port_bbox)
			{
				return { {Port::Sink,si} };
			}
		}
		return std::nullopt;
	}

	auto startheight = NodeStyle::title_height + NodeStyle::item_padding + pdelta_source - PortStyle::port_bbox / 2;
	if (point.y() < startheight)return std::nullopt;
	for (uint8_t si = 0; si < sources.size(); si++, startheight += pdelta_sink)
	{
		if (point.y() < startheight + PortStyle::port_bbox)
		{
			return { {Port::Source,si} };
		}
	}
	return std::nullopt;
}
