#pragma once
#include <UI/NodeStyle.h>
#include <optional>
#include <PortType.h>


namespace UI
{
	class Node : public QGraphicsItem
	{
		friend class Connection;
		friend class FlowScene;
	public:
		Node() = delete;
		Node(QJsonObject document, std::string_view name);
		Node(const Node& other) noexcept;
	public:
		QPointF GetPortPos(Port po, size_t pos);
		QRectF boundingRect() const override;
		void paint(QPainter* painter,
			const QStyleOptionGraphicsItem* option,
			QWidget* widget = nullptr) override;
		virtual std::string_view GetName()const noexcept = 0;

		std::string_view GetStyleName()const noexcept
		{
			return style.StyleName();
		}
		auto GetSourceType(size_t pos)
		{
			return sinks[pos];
		}
		auto GetSinkType(size_t pos)
		{
			return sources[pos];
		}

	private:
		virtual void SetUniqueName(std::string_view xname) = 0;
		//virtual void OnConnect(uint8_t sinkN, Node& source, uint8_t sourceN) = 0;
		//virtual void OnDisconnect(uint8_t sinkN, Node& source, uint8_t sourceN) = 0;
		void mousePressEvent(QGraphicsSceneMouseEvent* event)override;
		QVariant itemChange(GraphicsItemChange change, const QVariant& value)override;

		void SetConnection(std::unique_ptr<QGraphicsItem>&& in, uint8_t portN)
		{
			assert(portN < sinks.size());
			Sink_conns[portN] = std::move(in);
		}
		QGraphicsItem* GetConnection(uint8_t portN)
		{
			assert(portN < sinks.size());
			return Sink_conns[portN].get();
		}

		void Init();
		void DrawNodeRect(QPainter* painter);
		void DrawCaptionName(QPainter* painter);
		void DrawConnectionPoints(QPainter* painter);

		void MoveConnections(QPointF newpos);
	protected:
		void CalculateSize(QSizeF minsize = {})noexcept;
		qreal EffectiveHeight()const noexcept { return body_size.height() - NodeStyle::title_height - 2 * NodeStyle::item_padding; }
		std::optional<std::pair<Port, uint8_t>> PortHit(QPointF point);
		std::optional<std::pair<Port, uint8_t>> PortHitScene(QPointF scene_point)
		{
			return PortHit(mapFromScene(scene_point));
		}
	protected:
		std::basic_string<PortType> sinks;
		std::basic_string<PortType> sources;

		std::vector<std::unique_ptr<QGraphicsItem>> Sink_conns;
		QSizeF body_size;
		qreal pdelta_sink;
		qreal pdelta_source;

	private:
		NodeStyle style;
	};
}

