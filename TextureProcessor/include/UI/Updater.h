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
	namespace Windows
	{
		class PropertyElement;
	}

	template<bool changes = true>
	struct Updater : public QWidget
	{
		virtual void SetChangedCallback(INode* to, Windows::PropertyElement& self) = 0;
	};
	template<> struct Updater<false> : public QWidget
	{
		virtual void SetChangedCallback(INode* to) = 0;
	};
	using IUpdater = Updater<false>;
	using PropertyUpdater = Updater<true>;
}
