/**
 * @file Updater.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief A simple interface for callback
 */
#pragma once

namespace UI
{
	struct INode;
	struct Updater : public QWidget
	{
		virtual void SetChangedCallback(INode* to) = 0;
	};
}
