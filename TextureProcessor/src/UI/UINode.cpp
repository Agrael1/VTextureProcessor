#include <UI/UINode.h>
#include <QPainter>

constexpr const qreal _hard_min_width = 150.0;
constexpr const qreal _hard_min_height = 120.0;
constexpr const qreal _top_margin = 30.0;
constexpr const qreal _bottom_margin = 15.0;
constexpr const qreal _item_padding = 5.0;
constexpr const qreal _lr_padding = 10.0;

constexpr const qreal _pen_width = 1.0;
constexpr const qreal _socket_size = 6.0;

UINode::UINode()
    :sinks(4), sources(4), size(150, (2 * std::max(sources, sinks)+2)*diameter)
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

QRectF UINode::boundingRect() const
{
    return QRectF(
        -_pen_width / 2.0 - _socket_size,
        -_pen_width / 2.0,
        size.width() + _pen_width / 2.0 + 2.0 * _socket_size,
        size.height() + _pen_width / 2.0
    ).normalized();
}

void UINode::paint(QPainter* painter,
	const QStyleOptionGraphicsItem* option,
	QWidget* widget)
{
    DrawNodeRect(painter);
    DrawConnectionPoints(painter);
}

void UINode::DrawNodeRect(QPainter* painter)
{
    constexpr qreal edge_size = 10.0;
    

    // path for the caption of this node
    QPainterPath path_title;
    path_title.setFillRule(Qt::WindingFill);
    path_title.addRoundedRect(QRect(0, 0, size.width(), title_height), edge_size, edge_size);
    path_title.addRect(0, title_height - edge_size, edge_size, edge_size);
    path_title.addRect(size.width() - edge_size, title_height - edge_size, edge_size, edge_size);
    painter->setPen(Qt::NoPen);
    painter->setBrush(Title);
    painter->drawPath(path_title.simplified());


    // path for the content of this node
    QPainterPath path_content;
    path_content.setFillRule(Qt::WindingFill);
    path_content.addRoundedRect(QRect(0, title_height, size.width(), size.height() - title_height), edge_size, edge_size);
    path_content.addRect(0, title_height, edge_size, edge_size);
    path_content.addRect(size.width() - edge_size, title_height, edge_size, edge_size);
    painter->setPen(Qt::NoPen);
    painter->setBrush(Background);
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

void UINode::DrawConnectionPoints(QPainter* painter)
{
    painter->setPen(Sinks);
    painter->setBrush(BrushSinks);
    auto ypos = title_height + diameter;
    for (unsigned int i = 0; i < sinks; ++i, ypos+=diameter+offset)
    {
        painter->drawEllipse(-diameter/2, ypos, diameter, diameter);
    }

    painter->setPen(Sources);
    painter->setBrush(BrushSources);
    ypos = title_height + diameter;
    for (unsigned int i = 0; i < sources; ++i, ypos += diameter + offset)
    {
        painter->drawEllipse(size.width()-diameter / 2, ypos, diameter, diameter);
    }
}