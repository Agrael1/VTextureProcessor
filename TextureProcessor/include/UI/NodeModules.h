#pragma once
#include <QGraphicsLinearLayout>
#include <memory>

namespace UI
{
	class Module : public QGraphicsLayoutItem, public QGraphicsItem
	{
	public:
		Module(const std::shared_ptr<QImage>& tex);
		Module(Module&& in)noexcept;
	public:
		QRectF boundingRect() const override;
		void setGeometry(const QRectF& geom) override;
		void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)override;
		void Update();
	protected:
		virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF& constraint = QSizeF()) const override;
	private:
		std::shared_ptr<QImage> tex;
		QImage img;
	};
}