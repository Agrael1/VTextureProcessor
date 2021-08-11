#include <UI/Node.h>
#include <QPainter>

using namespace UI;


UI::XNode::XNode(QJsonObject document, std::string_view name/*, Engine& e*/)
	:gll(new QGraphicsLinearLayout), style(document, name)/*, model(document, e)*/
{
	Init();
}

void UI::XNode::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	DrawBackground(painter);
	QGraphicsWidget::paint(painter, option, widget);
}

void UI::XNode::DrawBackground(QPainter* painter)
{
	constexpr qreal edge_size = 10.0;

	auto xwidth = geometry().width();
	// path for the caption of this node
	QPainterPath path_title;
	path_title.setFillRule(Qt::WindingFill);
	path_title.addRoundedRect(QRectF(0, 0, xwidth, NodeStyle::title_height), edge_size, edge_size);
	path_title.addRect(0, NodeStyle::title_height - edge_size, xwidth, edge_size);
	painter->setPen(Qt::NoPen);
	painter->setBrush(style.Title());
	painter->drawPath(path_title.simplified());


	// path for the content of this node
	QPainterPath path_content;
	path_content.setFillRule(Qt::WindingFill);
	path_content.addRoundedRect(QRectF(0, NodeStyle::title_height, xwidth, geometry().height() - NodeStyle::title_height), edge_size, edge_size);
	path_content.addRect(0, NodeStyle::title_height, xwidth, edge_size);
	painter->setPen(Qt::NoPen);
	painter->setBrush(style.Background());
	painter->drawPath(path_content.simplified());

	if (isSelected()) {
		painter->setPen({ {"#FFFFFFFF"},2.0 });
	}
	else {
		painter->setPen(Qt::NoPen);
	}
	painter->setBrush(Qt::BrushStyle::NoBrush);
	painter->drawPath((path_title + path_content).simplified());
}
void UI::XNode::DrawCaptionName(QPainter* painter)
{
	QString name{ style.StyleName().data() };
	QFont f = painter->font();
	f.setBold(true);

	QFontMetrics metrics(f);
	auto rect = metrics.boundingRect(name);

	QPointF position((geometry().width() - rect.width()) / 2.0,
		(NodeStyle::title_height - rect.height() / 2.0));

	painter->setFont(f);
	painter->setPen(style.FontColor());
	painter->drawText(position, name);

	f.setBold(false);
	painter->setFont(f);
}
void UI::XNode::Init()
{
	setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);

	setCacheMode(QGraphicsItem::DeviceCoordinateCache);

	setAcceptHoverEvents(true);
	setZValue(0.0f);

	gll->setOrientation(Qt::Vertical);
	gll->setContentsMargins(NodeStyle::h_offset, NodeStyle::title_height + NodeStyle::h_offset, NodeStyle::h_offset, NodeStyle::h_offset);
	gll->addItem(&m);
	gll->addItem(&m2);
	setLayout(gll);
}

void UI::Module::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->drawImage(QPointF{0,0}, img);
}