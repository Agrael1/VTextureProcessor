#pragma once

namespace UI
{
	struct INode;
	struct Updater : public QWidget
	{
		virtual void SetChangedCallback(INode* to) = 0;
	};
}

