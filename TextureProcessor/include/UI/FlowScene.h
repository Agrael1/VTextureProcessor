#pragma once
#include <UI/UINode.h>
#include <UI/FlowCodex.h>

class FlowScene : public QGraphicsScene
{
public:
	FlowScene(QObject* parent);
public:
	UI::Node& CreateNode(std::wstring_view name);
protected:
	void drawBackground(QPainter* painter, const QRectF& rect) override;
private:
	QColor Cbackground;
	QColor Clight;
	QColor Cdark;

	QPen Plight;
	QPen Pdark;

	QBrush Bbackground;

	FlowCodex codex;
	std::vector<UI::Node> nodes;
};