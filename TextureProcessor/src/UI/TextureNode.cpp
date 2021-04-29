#include <UI/TextureNode.h>
#include <UI/Connection.h>


UI::TextureNode::TextureNode(QJsonObject document, std::string_view name)
	:Node(document, name), model(document), texture(128, 128, QImage::Format::Format_ARGB32)
{
	for (auto& s : model.GetSinks())
		sinks.push_back(s->GetType());
	for (auto& s : model.GetSources())
		sources.push_back(s->GetType());
	CalculateSize({ 128,128 });
}

void UI::TextureNode::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Node::paint(painter, option);
	DrawTexture(painter);
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
		auto& conn = *new Connection(*this, port->first, port->second);
		conn.grabMouse();
	}
}


