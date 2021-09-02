/**
 * @file FlowScene.h
 * @author Ilya Doroshenko (xdoros01), David Černý (xcerny74)
 * @brief Logic behind Node editor canvas
 */
#pragma once
#include <UI/REFlowCodex.h>
#include <QGraphicsScene>
#include <UI/ContextMenu.h>

namespace UI
{
	struct IXNode;
	namespace Windows {
		class XProperties;
	}
	class FlowScene : public QGraphicsScene, public ISerialize
	{
	public:
		FlowScene(QObject* parent, Windows::XProperties& props, QSize dims);
		~FlowScene();
	public:
		void DeleteSelected();
		void Clear();
		void ExportAll();
		QSize Dimensions()const noexcept { return dims; }

		virtual QJsonObject Serialize()override;
		virtual void Deserialize(QJsonObject)override;
	protected:
		bool event(QEvent* e)override;
		UI::IXNode& InsertNode(std::string_view name);
		UI::IXNode* TryInsertNode(std::string_view name);
		UI::IXNode& CreateNode(std::string_view name);
		void drawBackground(QPainter* painter, const QRectF& rect) override;
		void OnSelectionChanged();
		void OnItemSelected(QTreeWidgetItem* item, int);
	private:
		QSize dims;
		QColor Cbackground;
		QColor Clight;
		QColor Cdark;

		QPen Plight;
		QPen Pdark;

		QBrush Bbackground;
		UI::Windows::XProperties& props;
		UI::RE::XFlowCodex codex;

		std::unordered_map<std::string, std::unique_ptr<IXNode>> nodes;
		std::vector<IXNode*> outputs;

		QPoint last_event;
		ContextMenu menu;
	};
}