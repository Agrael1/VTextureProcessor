#pragma once
#include <UI/FlowCodex.h>

namespace UI
{
	class FlowScene : public QGraphicsScene
	{
	public:
		FlowScene(QObject* parent, Properties& props);
	public:
		auto& GetGroupList()const noexcept
		{
			return codex.CatMap();
		}
		UI::Node& CreateNode(std::string_view name);
		UI::Node* LocateNode(QPointF pos)noexcept;
		void DeleteSelected();
		void Clear();
	protected:
		void drawBackground(QPainter* painter, const QRectF& rect) override;
		void OnSelectionChanged();
	private:
		QColor Cbackground;
		QColor Clight;
		QColor Cdark;

		QPen Plight;
		QPen Pdark;

		QBrush Bbackground;
		Properties& props;

		UI::FlowCodex codex;
		std::unordered_map<std::string, pv::polymorphic_value<UI::Node>> nodes;
	};
}