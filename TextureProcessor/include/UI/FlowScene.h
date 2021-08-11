/**
 * @file FlowScene.h
 * @author Ilya Doroshenko (xdoros01), David Černý (xcerny74)
 * @brief Logic behind Node editor canvas
 */
#pragma once
#include <UI/FlowCodex.h>
#include <QGraphicsScene>

#include <UI/Node.h>

namespace UI
{
	namespace Windows {
		class Properties;
	}
	class FlowScene : public QGraphicsScene, public ISerialize
	{
	public:
		FlowScene(QObject* parent, Windows::Properties& props);
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
		UI::IXNode& InsertNode(std::string_view name);

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
		UI::Windows::Properties& props;

		UI::FlowCodex codex;
		std::unordered_map<std::string, pv::polymorphic_value<UI::Node>> nodes;
		std::vector<UI::Node*> outputs;

		std::unique_ptr<IXNode> test;
	};
}