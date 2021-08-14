/**
 * @file Updater.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief A simple interface for callback
 */
#pragma once
#include <QWidget>

namespace UI
{
	struct INode;
	struct IXNode;
	struct Updater : public QWidget
	{
		virtual void SetChangedCallback(INode* to) = 0;
	};
	struct IUpdater : public QWidget
	{
		virtual void SetChangedCallback(IXNode* to) = 0;
	};
}
