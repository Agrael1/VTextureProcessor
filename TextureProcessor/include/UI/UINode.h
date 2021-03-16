#pragma once
#include <Node.h>
#include <UI/NodeStyle.h>
#include <optional>


namespace UI
{
	class Node : public QGraphicsItem
	{
	public:
		Node();
		Node(QJsonObject document);
	public:
		QRectF boundingRect() const override;
		void paint(QPainter* painter,
			const QStyleOptionGraphicsItem* option,
			QWidget* widget = nullptr) override;

	private:
		void Init();
		void DrawNodeRect(QPainter* painter);
		void DrawConnectionPoints(QPainter* painter);
	private:
		std::optional<ver::Node> model;
		NodeStyle style;
		constexpr static const qreal diameter = 10.0;
		constexpr static const qreal offset = 5;
	};
}

