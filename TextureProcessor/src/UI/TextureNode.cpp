#include <UI/TextureNode.h>

UI::TextureNode::TextureNode(QJsonObject document, std::string_view name)
	:Node(document, name), model(document), texture(128, 128, QImage::Format::Format_ARGB32)
{
	CalculateSize({ 128,128 });
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
	auto height = body_size.height() - NodeStyle::title_height - NodeStyle::item_padding * 2;
	auto delta = height / (SourcesCount() + 1);
	auto ypos = NodeStyle::title_height + NodeStyle::item_padding + delta - PortStyle::diameter / 2;

	for (const auto& si: model.GetSinks())
	{
		painter->drawEllipse(-PortStyle::diameter / 2, ypos, PortStyle::diameter, PortStyle::diameter);
		ypos += delta;
	}

	painter->setBrush(style.brSource);
	ypos = NodeStyle::title_height + NodeStyle::item_padding + delta - PortStyle::diameter / 2;
	for (const auto& so : model.GetSources())
	{
		painter->drawEllipse(body_size.width() - PortStyle::diameter / 2, ypos, PortStyle::diameter, PortStyle::diameter);
		ypos += delta;
	}
}

void UI::TextureNode::DrawTexture(QPainter* painter)
{
	QPointF point{ body_size.width() / 2 - 64, EffectiveHeight() / 2 - 64 + NodeStyle::title_height + NodeStyle::item_padding };
	painter->drawImage(point, texture);
}