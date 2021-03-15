#pragma once
#include <QGraphicsScene>
#include <UI/UINode.h>
#include <unordered_map>

class FlowScene : public QGraphicsScene
{
public:
	FlowScene(QObject* parent);
public:
	UI::Node& CreateNode(std::string_view name);
protected:
	void drawBackground(QPainter* painter, const QRectF& rect) override;
private:
	QColor Cbackground;
	QColor Clight;
	QColor Cdark;

	QPen Plight;
	QPen Pdark;

	QBrush Bbackground;

	std::unordered_map<std::string_view, std::unique_ptr<UI::Node>> nodes;
};