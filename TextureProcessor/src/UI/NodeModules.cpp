#include <UI/NodeModules.h>
#include <UI/NodeStyle.h>
#include <QPainter>


UI::Module::Module(const std::shared_ptr<QImage>& tex) :
	img(128, 128, QImage::Format::Format_ARGB32), tex(tex)
{
	setGraphicsItem(this);
}

UI::Module::Module(Module&& in) noexcept
	:img(std::move(in.img))
	, tex(std::move(in.tex))
{
	setGraphicsItem(this);
}

QRectF UI::Module::boundingRect() const
{
	return QRectF(QPointF(0, 0), geometry().size());
}

void UI::Module::setGeometry(const QRectF& geom)
{
	prepareGeometryChange();
	QGraphicsLayoutItem::setGeometry(geom);
	setPos(geom.topLeft());
}

void UI::Module::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->drawImage(QPointF{ 0,0 }, img);
}

void UI::Module::Update()
{
	img = tex->scaled(img.size()).mirrored();
}

QSizeF UI::Module::sizeHint(Qt::SizeHint which, const QSizeF& constraint) const
{
	return img.size();
}
