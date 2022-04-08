/**
 * @file FlowScene.h
 * @author Ilya Doroshenko (xdoros01), David Černý (xcerny74)
 * @brief Logic behind Node editor canvas
 */
#pragma once
#include <UI/FlowCodex.h>
#include <UI/ContextMenu.h>
#include <UI/NodeMenu.h>

#include <QGraphicsScene>

namespace UI
{
	struct INode;
	namespace Windows {
		class Properties;
	}
	class FlowScene : public QGraphicsScene, public ISerialize
	{
	public:
		FlowScene(QObject* parent, Windows::Properties& props, QSize dims);
	public:
		void DeleteSelected();
		void Clear();
		void ExportAll();
		void OnSelectionChanged();
		QSize Dimensions(QJsonObject)const noexcept;

		virtual void Serialize(QJsonObject& doc)override;
		virtual bool Deserialize(QJsonObject doc)override;
	protected:
		bool event(QEvent* e)override;
		UI::INode* CreateNode(std::string_view name);
		void drawBackground(QPainter* painter, const QRectF& rect) override;
		void OnItemSelected(QTreeWidgetItem* item, int);
	private:
		mutable QSize dims;
		QColor Cbackground;
		QColor Clight;
		QColor Cdark;

		QPen Plight;
		QPen Pdark;

		QBrush Bbackground;
		UI::Windows::Properties& props;
		UI::FlowCodex codex;

		std::unordered_map<std::string, std::unique_ptr<INode>> nodes;
		std::vector<INode*> outputs;

		QPoint last_event;
		ContextMenu menu;
		NodeMenu nodemenu;
	};
}