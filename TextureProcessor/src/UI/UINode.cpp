#include <UI/UINode.h>
#include <UI/PortsGeometry.h>

constexpr const qreal _hard_min_width = 150.0;
constexpr const qreal _hard_min_height = 120.0;
constexpr const qreal _top_margin = 30.0;
constexpr const qreal _bottom_margin = 15.0;
constexpr const qreal _item_padding = 5.0;
constexpr const qreal _lr_padding = 10.0;

constexpr const qreal _pen_width = 1.0;
constexpr const qreal _socket_size = 6.0;

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
    :style(other.style)
{
    printf("copy called\n");
    Init();
}
QRectF Node::boundingRect() const
{
    return QRectF(
        -_pen_width / 2.0 - _socket_size,
        -_pen_width / 2.0,
        style.MinWidth() + _pen_width / 2.0 + 2.0 * _socket_size,
        style.MinHeight() + _pen_width / 2.0
    ).normalized();
}

void Node::paint(QPainter* painter,
	const QStyleOptionGraphicsItem* option,
	QWidget* widget)
{
    DrawNodeRect(painter);
    DrawConnectionPoints(painter);
    DrawCaptionName(painter);
}

void Node::DrawNodeRect(QPainter* painter)
{
    constexpr qreal edge_size = 10.0;

    // path for the caption of this node
    QPainterPath path_title;
    path_title.setFillRule(Qt::WindingFill);
    path_title.addRoundedRect(QRect(0, 0, style.MinWidth(), NodeStyle::title_height), edge_size, edge_size);
    path_title.addRect(0, NodeStyle::title_height - edge_size, edge_size, edge_size);
    path_title.addRect(style.MinWidth() - edge_size, NodeStyle::title_height - edge_size, edge_size, edge_size);
    painter->setPen(Qt::NoPen);
    painter->setBrush(style.Title());
    painter->drawPath(path_title.simplified());


    // path for the content of this node
    QPainterPath path_content;
    path_content.setFillRule(Qt::WindingFill);
    path_content.addRoundedRect(QRect(0, NodeStyle::title_height, style.MinWidth(), style.MinHeight() - NodeStyle::title_height), edge_size, edge_size);
    path_content.addRect(0, NodeStyle::title_height, edge_size, edge_size);
    path_content.addRect(style.MinWidth() - edge_size, NodeStyle::title_height, edge_size, edge_size);
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
    painter->drawPath((path_title+path_content).simplified());

}

void Node::DrawConnectionPoints(QPainter* painter)
{
    //painter->setPen(Sinks);
    //painter->setBrush(style.BrushSinks);
    //auto ypos = title_height + diameter;
    //for (unsigned int i = 0; i < sinks; ++i, ypos+=diameter+offset)
    //{
    //    painter->drawEllipse(-diameter/2, ypos, diameter, diameter);
    //}

    //painter->setPen(Sources);
    //painter->setBrush(BrushSources);
    //ypos = MinHeight() - 2 * diameter;
    //for (unsigned int i = 0; i < sources; ++i, ypos -= diameter + offset)
    //{
    //    painter->drawEllipse(MinWidth()-diameter / 2, ypos, diameter, diameter);
    //}
}
void Node::DrawCaptionName(QPainter* painter)
{
    QString name{ style.StyleName().data() };
    QFont f = painter->font();
    f.setBold(true);
    
    QFontMetrics metrics(f);
    auto rect = metrics.boundingRect(name);

    QPointF position((style.MinWidth() - rect.width()) / 2.0,
        (NodeStyle::title_height - rect.height()/2.0));

    painter->setFont(f);
    painter->setPen(style.FontColor());
    painter->drawText(position, name);

    f.setBold(false);
    painter->setFont(f);
}