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
	struct INode;

	class Port :public QGraphicsLayoutItem, public QGraphicsItem
	{
		static constexpr QPointF center = { PortStyle::port_bbox / 2, PortStyle::port_bbox / 2 };
	public:
		Port(INode& parent, uint8_t port_num, ver::PortType ty);
		Port(Port&& in)noexcept;
	public:
		constexpr virtual ver::PortSide GetType()const noexcept
		{
			return ver::PortSide::None;
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
		INode& parent;
		uint8_t port_num;
		ver::PortType ty;
	};

	class Sink : public Port
	{
		friend class Connection;
	public:
		Sink(INode& parent, uint8_t port_num, ver::Sink& model);
		Sink(Sink&& in)noexcept:Port(std::move(in)),model(in.model), connection(std::move(in.connection)){}
	public:
		virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
		void MoveConnections(QPointF delta);
		void UpdateConnectionPos()
		{
			if (connection)connection->UpdatePosition();
		}
		constexpr virtual ver::PortSide GetType()const noexcept override
		{
			return ver::PortSide::Sink;
		}
	protected:
		void mousePressEvent(QGraphicsSceneMouseEvent* event)override;
	private:
		std::unique_ptr<IConnection> connection;
		ver::Sink& model;
	};

	class Source : public Port
	{
		friend class Connection;
	public:
		Source(INode& parent, uint8_t port_num, ver::Source& model);
		Source(Source&& in)noexcept :Port(std::move(in)), model(in.model){}
	public:
		virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
		void MoveConnections(QPointF delta);
		constexpr virtual ver::PortSide GetType()const noexcept override
		{
			return ver::PortSide::Source;
		}
	protected:
		void mousePressEvent(QGraphicsSceneMouseEvent* event)override;
	private:
		ver::Source& model;
	};
}