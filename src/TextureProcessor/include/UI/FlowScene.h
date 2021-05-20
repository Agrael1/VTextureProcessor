/**
 * @file FlowScene.h
 * @author Ilya Doroshenko (xdoros01), David Černý (xcerny74)
 * @brief Logic behind Node editor canvas
 */
#pragma once
#include <UI/FlowCodex.h>

namespace UI
{
	class FlowScene : public QGraphicsScene, public ISerialize
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
		void ExportAll();
		virtual QJsonObject Serialize()override;
		virtual void Deserialize(QJsonObject)override;
	protected:
		UI::Node& InsertNode(std::string_view name, std::string&& unique_name);
		UI::Node* TryInsertNode(std::string_view name, std::string&& unique_name);
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
		std::vector<UI::Node*> outputs;
	};
}