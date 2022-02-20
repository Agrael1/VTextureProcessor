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
			menu.addAction("Export", [this](bool) {node->Export(); });
		}
		void Execute(INode* xnode, QPoint pos)
		{
			node = xnode;
			menu.popup(pos);
		}

	private:
		INode* node = nullptr;
		QMenu menu;
	};
}
