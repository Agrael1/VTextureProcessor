#pragma once
#include <QGraphicsLinearLayout>
#include <QGraphicsWidget>
#include <UI/NodeStyle.h>
#include <Logic/ShaderNode.h>

#include <UI/INode.h>


namespace UI
{
	//template <class T>
	class Module : public QGraphicsLayoutItem, public QGraphicsItem
	{
	public:
		Module(QGraphicsItem* parent = nullptr) : QGraphicsItem(parent)
			, img(128, 128, QImage::Format::Format_ARGB32)
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
			QWidget* widget)override;
	protected:
		virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF& constraint = QSizeF()) const override
		{
			return img.size();
		}
	private:
		QImage img;
	};

	//template<class XModel> requires std::derived_from<XModel, ver::Node>
	class XNode : public QGraphicsWidget//, public ISerialize
	{
	public:
		XNode() :gll(new QGraphicsLinearLayout) { Init(); };
		XNode(QJsonObject document, std::string_view name/*, Engine& e*/);
		void paint(QPainter* painter,
			const QStyleOptionGraphicsItem* option,
			QWidget* widget = nullptr) override;
	private:
		void DrawBackground(QPainter* painter);
		void DrawCaptionName(QPainter* painter);
		void Init();
	private:
		NodeStyle style;
		//ver::ShaderNode model;

		QGraphicsLinearLayout* gll;
		Module m;
		Module m2;
	};
}

