#include <UI/TextureNode.h>


UI::TextureNode::TextureNode(QJsonObject document, std::string_view name)
	:Node(document, name), model(document), texture(128, 128, QImage::Format::Format_ARGB32)
{
	CalculateSize({ 128,128 });
	auto height = body_size.height() - NodeStyle::title_height - NodeStyle::item_padding * 2;
	pdelta_sink = height / (SinksCount() + 1);
	pdelta_source = height / (SourcesCount() + 1);
}

void UI::TextureNode::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Node::paint(painter, option);
	DrawConnectionPoints(painter);
	DrawTexture(painter);
}
void UI::TextureNode::DrawConnectionPoints(QPainter* painter)
{
	auto& style = PortStyle::Grayscale;
	painter->setPen(style.port);
	painter->setBrush(style.brSink);

	auto ypos = NodeStyle::title_height + NodeStyle::item_padding + pdelta_sink - PortStyle::diameter / 2;
	for (const auto& si: model.GetSinks())
	{
		painter->drawEllipse(-PortStyle::diameter / 2, ypos, PortStyle::diameter, PortStyle::diameter);
		ypos += pdelta_sink;
	}

	
	painter->setBrush(style.brSource);
	ypos = NodeStyle::title_height + NodeStyle::item_padding + pdelta_source - PortStyle::diameter / 2;
	for (const auto& so : model.GetSources())
	{
		painter->drawEllipse(body_size.width() - PortStyle::diameter / 2, ypos, PortStyle::diameter, PortStyle::diameter);
		ypos += pdelta_source;
	}
}

void UI::TextureNode::DrawTexture(QPainter* painter)
{
	QPointF point{ body_size.width() / 2 - 64, EffectiveHeight() / 2 - 64 + NodeStyle::title_height + NodeStyle::item_padding };
	painter->drawImage(point, texture);
}

void UI::TextureNode::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	auto pos = event->pos();
	if (pos.y() < NodeStyle::title_height + NodeStyle::item_padding ||
		pos.y() > body_size.height() - NodeStyle::item_padding ||
		pos.x() > PortStyle::port_bbox / 2 && pos.x() < body_size.width() - PortStyle::port_bbox / 2)
		return Node::mousePressEvent(event);
	
	if (auto port = PortHit(pos))
	{
		event->accept();
	}
}

std::optional<std::pair<Port, uint8_t>> UI::TextureNode::PortHit(QPointF point)
{
	if (point.x() < PortStyle::port_bbox)
	{
		auto startheight = NodeStyle::title_height + NodeStyle::item_padding + pdelta_sink - PortStyle::port_bbox / 2;
		if (point.y() < startheight)return std::nullopt;
		for (uint8_t si = 0; si < SinksCount(); si++, startheight += pdelta_sink)
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
	for (uint8_t si = 0; si < SourcesCount(); si++, startheight += pdelta_sink)
	{
		if (point.y() < startheight + PortStyle::port_bbox)
		{
			return { {Port::Source,si} };
		}
	}
	return std::nullopt;
}
