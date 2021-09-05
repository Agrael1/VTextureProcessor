#pragma once
#include <QGraphicsItem>
#include <QGraphicsLayoutItem>
#include <Interfaces/IConnection.h>
#include <UI/NodeStyle.h>


namespace ver
{
	class Sink;
	class Source;
}

namespace UI
{
	struct IXNode;

	class XPort :public QGraphicsLayoutItem, public QGraphicsItem
	{
		static constexpr QPointF center = { PortStyle::port_bbox / 2, PortStyle::port_bbox / 2 };
	public:
		XPort(IXNode& parent, uint8_t port_num);
		XPort(XPort&& in)noexcept;
	public:
		constexpr virtual Port GetType()const noexcept
		{
			return Port::None;
		}
		auto& Node() const noexcept
		{
			return parent;
		}
		auto& Node() noexcept
		{
			return parent;
		}
		uint8_t Index()const noexcept
		{
			return port_num;
		}
		void setGeometry(const QRectF& geom) override
		{
			prepareGeometryChange();
			QGraphicsLayoutItem::setGeometry(geom);
			setPos(geom.topLeft());
		}
		virtual QRectF boundingRect() const override;
		virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
		QPointF CenterScene()const
		{
			return scenePos() + center;
		}
	protected:
		virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF& constraint = QSizeF()) const override;
	protected:
		IXNode& parent;
		uint8_t port_num;
	};

	class Sink : public XPort
	{
		friend class XConnection;
	public:
		Sink(IXNode& parent, uint8_t port_num, ver::Sink& model);
		Sink(Sink&& in)noexcept:XPort(std::move(in)),model(in.model), connection(std::move(in.connection)){}
	public:
		virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
		void MoveConnections(QPointF delta);
		void UpdateConnectionPos()
		{
			if (connection)connection->UpdatePosition();
		}
		constexpr virtual Port GetType()const noexcept override
		{
			return Port::Sink;
		}
	protected:
		void mousePressEvent(QGraphicsSceneMouseEvent* event)override;
	private:
		std::unique_ptr<IXConnection> connection;
		ver::Sink& model;
	};

	class Source : public XPort
	{
		friend class XConnection;
	public:
		Source(IXNode& parent, uint8_t port_num, ver::Source& model)
			:XPort(parent, port_num), model(model)
		{}
		Source(Source&& in)noexcept :XPort(std::move(in)), model(in.model){}
	public:
		virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
		void MoveConnections(QPointF delta);
		constexpr virtual Port GetType()const noexcept override
		{
			return Port::Source;
		}
	protected:
		void mousePressEvent(QGraphicsSceneMouseEvent* event)override;
	private:
		ver::Source& model;
	};
}