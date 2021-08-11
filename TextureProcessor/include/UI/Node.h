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
		Module(std::shared_ptr<QOpenGLTexture>&& tex) : 
			img(128, 128, QImage::Format::Format_ARGB32), tex(std::move(tex))
		{
			setGraphicsItem(this);
		}
		Module(Module&& in)noexcept
			:img(std::move(in.img))
			,tex(std::move(in.tex))
		{
			setGraphicsItem(this);
		}
		Module() :  img(128, 128, QImage::Format::Format_ARGB32)
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
		std::shared_ptr<QOpenGLTexture> tex;
		QImage img;
	};

	//template<class XModel> requires std::derived_from<XModel, ver::Node>
	class XNode : public IXNode//, public ISerialize
	{
	public:
		//XNode() :gll(new QGraphicsLinearLayout) { Init(); };
		XNode(QJsonObject document, std::string_view name, Engine& e);
		XNode(const XNode& in);
		void paint(QPainter* painter,
			const QStyleOptionGraphicsItem* option,
			QWidget* widget = nullptr) override;
		virtual std::unique_ptr<IXNode> Clone(std::string&& name)const override;
		virtual std::string_view Name()const override;
	private:
		void DrawBackground(QPainter* painter);
		void DrawCaptionName(QPainter* painter);
		void Init();
		void ConstructModules();
	private:
		NodeStyle style;
		ver::ShaderNode model;

		QGraphicsLinearLayout* gll;
		std::vector<Module> modules;
	};
}

