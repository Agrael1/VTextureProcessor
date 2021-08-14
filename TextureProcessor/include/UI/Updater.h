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
	namespace Windows
	{
		class XPropertyElement;
	}

	struct Updater : public QWidget
	{
		virtual void SetChangedCallback(INode* to) = 0;
	};

	template<bool changes = true>
	struct XUpdater : public QWidget
	{
		virtual void SetChangedCallback(IXNode* to, Windows::XPropertyElement& self) = 0;
	};
	template<> struct XUpdater<false> : public QWidget
	{
		virtual void SetChangedCallback(IXNode* to) = 0;
	};
	using IUpdater = XUpdater<false>;
	using PropertyUpdater = XUpdater<true>;
}
