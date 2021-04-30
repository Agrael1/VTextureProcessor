#include <UI/UINode.h>

using namespace UI;

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

Node::Node(QJsonObject document, std::string_view name)
	:style(document, name)
{
	Init();
}

UI::Node::Node(const Node& other) noexcept
	:style(other.style),
	body_size(other.body_size),
	sinks(other.sinks),
	sources(other.sources),
	pdelta_sink(other.pdelta_sink),
	pdelta_source(other.pdelta_source)
{
	Sink_conns.resize(other.sinks.size());
	printf("copy called\n");
	Init();
}


void UI::Node::CalculateSize(QSizeF minsize) noexcept
{
	qreal max_port = std::max(sources.size(), sinks.size()) * PortStyle::port_bbox + NodeStyle::item_padding * 2;
	qreal max_height = NodeStyle::title_height + std::max(std::max(minsize.height() + 2 * NodeStyle::item_padding, max_port), NodeStyle::min_height);
	qreal max_width = std::max(minsize.width() + PortStyle::port_bbox, NodeStyle::min_width);
	body_size = { max_width, max_height };

	auto height = max_height - NodeStyle::title_height - NodeStyle::item_padding * 2;
	pdelta_sink = height / (sinks.size() + 1);
	pdelta_source = height / (sources.size() + 1);
}
QRectF Node::boundingRect() const
{
	return QRectF(
		-NodeStyle::pen_width / 2.0 - PortStyle::port_bbox,
		-NodeStyle::pen_width / 2.0,
		body_size.width() + NodeStyle::pen_width / 2.0 + 2.0 * PortStyle::port_bbox,
		body_size.height() + NodeStyle::pen_width / 2.0
	).normalized();
}
QPointF Node::GetPortPos(Port po, size_t pos)
{
	return mapToScene({ po == Port::Sink ? 0 : body_size.width(),
		po == Port::Sink ? NodeStyle::title_height + PortStyle::port_bbox + pdelta_sink * (pos + 1) :
		NodeStyle::title_height + pdelta_source * (pos + 1) + PortStyle::port_bbox });
}

void Node::paint(QPainter* painter,
	const QStyleOptionGraphicsItem* option,
	QWidget* widget)
{
	DrawNodeRect(painter);
	DrawCaptionName(painter);
	DrawConnectionPoints(painter);
}

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

	if (isSelected())
	{
		painter->setPen({ {"#FFFFFFFF"},2.0 });
	}
	else
	{
		painter->setPen(Qt::NoPen);
	}
	painter->setBrush(Qt::BrushStyle::NoBrush);
	painter->drawPath((path_title + path_content).simplified());

}
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
void UI::Node::DrawConnectionPoints(QPainter* painter)
{
	auto& style = PortStyle::Grayscale;
	painter->setPen(style.port);
	painter->setBrush(style.brSink);

	auto ypos = NodeStyle::title_height + NodeStyle::item_padding + pdelta_sink - PortStyle::diameter / 2;
	for (const auto& si : sinks)
	{
		painter->drawEllipse(-PortStyle::diameter / 2, ypos, PortStyle::diameter, PortStyle::diameter);
		ypos += pdelta_sink;
	}


	painter->setBrush(style.brSource);
	ypos = NodeStyle::title_height + NodeStyle::item_padding + pdelta_source - PortStyle::diameter / 2;
	for (const auto& so : sources)
	{
		painter->drawEllipse(body_size.width() - PortStyle::diameter / 2, ypos, PortStyle::diameter, PortStyle::diameter);
		ypos += pdelta_source;
	}
}

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