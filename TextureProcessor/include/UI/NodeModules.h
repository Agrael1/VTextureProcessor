#pragma once
#include <QGraphicsLinearLayout>
#include <QOpenGlTexture>
#include <QPainter>
#include <memory>

namespace UI
{
	//template <class T>
	class Module : public QGraphicsLayoutItem, public QGraphicsItem
	{
	public:
		Module(const std::shared_ptr<QImage>& tex) :
			img(128, 128, QImage::Format::Format_ARGB32), tex(tex)
		{
			setGraphicsItem(this);
		}
		Module(Module&& in)noexcept
			:img(std::move(in.img))
			, tex(std::move(in.tex))
		{
			setGraphicsItem(this);
		}
		Module() : img(128, 128, QImage::Format::Format_ARGB32)
		{
			setGraphicsItem(this);
		}
		QRectF boundingRect() const override
		{
			return QRectF(QPointF(0, 0), geometry().size());
		}
		void setGeometry(const QRectF& geom) override
		{
			prepareGeometryChange();
			QGraphicsLayoutItem::setGeometry(geom);
			setPos(geom.topLeft());
		}
		void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
			QWidget* widget)override
		{
			painter->drawImage(QPointF{ 0,0 }, img);
		}
		void Update()
		{
			img = tex->scaled(img.size()).mirrored();
		}
	protected:
		virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF& constraint = QSizeF()) const override
		{
			return img.size();
		}
	private:
		std::shared_ptr<QImage> tex;
		QImage img;
	};
}