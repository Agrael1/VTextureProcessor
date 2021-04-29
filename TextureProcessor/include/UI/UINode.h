#pragma once
#include <UI/NodeStyle.h>
#include <optional>
#include <PortType.h>


namespace UI
{
	class Node : public QGraphicsItem
	{
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
	protected:
		void CalculateSize(QSizeF minsize = {})noexcept;
		qreal EffectiveHeight()const noexcept { return body_size.height() - NodeStyle::title_height - 2 * NodeStyle::item_padding; }
		
		QSizeF body_size;
	private:
		virtual void SetUniqueName(std::string_view xname) = 0;
		void Init();
		void DrawNodeRect(QPainter* painter);
		void DrawCaptionName(QPainter* painter);
		void DrawConnectionPoints(QPainter* painter);
	protected:
		std::optional<std::pair<Port, uint8_t>> PortHit(QPointF point);
	protected:
		std::basic_string<PortType> sinks;
		std::basic_string<PortType> sources;
		qreal pdelta_sink;
		qreal pdelta_source;
	private:
		NodeStyle style;
		constexpr static const qreal diameter = 10.0;
		constexpr static const qreal offset = 5;
	};
}

