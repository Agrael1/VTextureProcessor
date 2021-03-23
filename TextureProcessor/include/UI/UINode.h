#pragma once
#include <Node.h>
#include <UI/NodeStyle.h>
#include <optional>


namespace UI
{
	class Node : public QGraphicsItem
	{
	public:
		Node() = delete;
		Node(QJsonObject document, std::string_view name);
		Node(const Node& other) noexcept;
	public:
		QRectF boundingRect() const override;
		void paint(QPainter* painter,
			const QStyleOptionGraphicsItem* option,
			QWidget* widget = nullptr) override;
		std::string_view GetName()const noexcept
		{
			return model.GetName();
		}
		std::string_view GetStyleName()const noexcept
		{
			return style.StyleName();
		}

	private:
	private:
		void Init();
		void DrawNodeRect(QPainter* painter);
		void DrawConnectionPoints(QPainter* painter);
		void DrawCaptionName(QPainter* painter);
	private:
		ver::Node model;
		NodeStyle style;
		constexpr static const qreal diameter = 10.0;
		constexpr static const qreal offset = 5;
	};
}

