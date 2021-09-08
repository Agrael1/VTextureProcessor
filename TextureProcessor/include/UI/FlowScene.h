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
		class XProperties;
	}
	class FlowScene : public QGraphicsScene, public ISerialize
	{
	public:
		FlowScene(QObject* parent, Windows::XProperties& props, QSize dims);
	public:
		void DeleteSelected();
		void Clear();
		void EPortAll();
		QSize Dimensions(QJsonObject)const noexcept;

		virtual QJsonObject Serialize()override;
		virtual void Deserialize(QJsonObject)override;
	protected:
		bool event(QEvent* e)override;
		UI::INode& InsertNode(std::string_view name);
		UI::INode* TryInsertNode(std::string_view name, size_t ref);
		UI::INode& CreateNode(std::string_view name);
		void drawBackground(QPainter* painter, const QRectF& rect) override;
		void OnSelectionChanged();
		void OnItemSelected(QTreeWidgetItem* item, int);
	private:
		mutable QSize dims;
		QColor Cbackground;
		QColor Clight;
		QColor Cdark;

		QPen Plight;
		QPen Pdark;

		QBrush Bbackground;
		UI::Windows::XProperties& props;
		UI::FlowCodex codex;

		std::unordered_map<std::string, std::unique_ptr<INode>> nodes;
		std::vector<INode*> outputs;

		QPoint last_event;
		ContextMenu menu;
		NodeMenu nodemenu;
	};
}