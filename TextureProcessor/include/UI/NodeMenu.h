#pragma once
#include <QMenu>
#include <Interfaces/INode.h>

namespace UI
{
	class NodeMenu
	{
	public:
		NodeMenu()
		{
			menu.addAction("Edit");
			auto* b = menu.addAction("Export");
			menu.connect(b, &QAction::triggered, [this](bool) {node->Export(); });
		}
		void Execute(IXNode* xnode, QPoint pos)
		{
			node = xnode;
			menu.popup(pos);
		}

	private:
		IXNode* node = nullptr;
		QMenu menu;
	};
}
