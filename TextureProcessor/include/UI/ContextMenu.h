﻿/**
 * @file ContextMenu.h
 * @author Ilya Doroshenko (xdoros01), David Černý (xcerny74)
 * @brief Class for managing context menu contents and callbacks
 */
#pragma once

class ContextMenu : public QMenu
{
	friend class MenuBuilder;
public:
	ContextMenu(QWidget* parent);
public:
	void AppendGroup(std::string_view groupName);
	void AppendItem(std::string_view item);
	void Finish();
	void Execute(const QPoint& where);

	template<typename Functor>
	void SetItemClickCallback(Functor&& fn)
	{
		connect(&selection, &QTreeWidget::itemClicked, std::forward<Functor>(fn) );
	}
	void OnItemDoubleClicked(QTreeWidgetItem* item, int column);
	void OnFilterChanged(const QString& str);
private:
	QVBoxLayout layout;
	QLineEdit filter;
	QTreeWidget selection;
	QTreeWidgetItem* current_group;
};

constexpr std::string_view skipper = "XXXX";