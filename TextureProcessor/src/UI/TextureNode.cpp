#include <UI/TextureNode.h>
#include <UI/Connection.h>


UI::TextureNode::TextureNode(QJsonObject document, std::string_view name, Engine& engine)
	:Node(document, name), model(document, engine), texture(128, 128, QImage::Format::Format_ARGB32)
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



