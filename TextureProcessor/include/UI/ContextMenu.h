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
private:
	QVBoxLayout layout;
	QLineEdit filter;
	QTreeWidget selection;
	QTreeWidgetItem* current_group;
};

constexpr std::string_view skipper = "XXXX";