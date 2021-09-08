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
			auto* b = menu.addAction("EPort");
			menu.connect(b, &QAction::triggered, [this](bool) {node->EPort(); });
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
