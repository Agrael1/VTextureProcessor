#pragma once
#include <UI/UINode.h>
#include <UI/FlowCodex.h>

namespace UI
{
	class FlowScene : public QGraphicsScene
	{
	public:
		FlowScene(QObject* parent);
	public:
		auto& GetGroupList()const noexcept
		{
			return codex.CatMap();
		}
		UI::Node& CreateNode(std::string_view name);
		void Clear();
	protected:
		void drawBackground(QPainter* painter, const QRectF& rect) override;
	private:
		QColor Cbackground;
		QColor Clight;
		QColor Cdark;

		QPen Plight;
		QPen Pdark;

		QBrush Bbackground;

		UI::FlowCodex codex;
		std::unordered_map<std::string, UI::Node> nodes;
	};
}