#pragma once
#include <Node.h>
#include <UI/NodeStyle.h>
#include <optional>


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
		QRectF boundingRect() const override;
		void paint(QPainter* painter,
			const QStyleOptionGraphicsItem* option,
			QWidget* widget = nullptr) override;
		virtual std::string_view GetName()const noexcept = 0;
		virtual size_t SourcesCount()const noexcept { return 0; }
		virtual size_t SinksCount()const noexcept { return 0; }

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
	private:
		void Init();
		void DrawNodeRect(QPainter* painter);
		void DrawCaptionName(QPainter* painter);
	private:
		
		NodeStyle style;
		constexpr static const qreal diameter = 10.0;
		constexpr static const qreal offset = 5;
	};
}

