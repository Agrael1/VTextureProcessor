/**
 * @file FlowView.h
 * @author Ilya Doroshenko (xdoros01), David Černý (xcerny74)
 * @brief Flow view for handling the Node editor canvas view
 */
#pragma once
#include <QGraphicsView>

class QTreeWidgetItem;

namespace UI
{
	class FlowScene;
	class FlowView : public QGraphicsView
	{
	public:
		FlowView(QGraphicsScene* scene);
	private:
		void wheelEvent(QWheelEvent* event) override;
		void contextMenuEvent(QContextMenuEvent* event)override;
		void scaleUp();
		void scaleDown();
	};
}

