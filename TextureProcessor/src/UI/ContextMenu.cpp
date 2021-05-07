#include <UI/ContextMenu.h>


ContextMenu::ContextMenu(QWidget* parent)
	:QMenu(parent), layout(this)
{
	layout.setAlignment(Qt::AlignTop);
	layout.setContentsMargins(0, 0, 0, 0);
	layout.setSpacing(0);

	filter.setPlaceholderText("Search...");
	filter.setMinimumWidth(300);
	filter.setClearButtonEnabled(true);

	selection.setHeaderLabel("Select Node Type to Add");
	selection.setMinimumHeight(400);

	layout.addWidget(&filter);
	layout.addWidget(&selection);
}

void ContextMenu::AppendGroup(std::string_view groupName)
{
	auto& item = *new QTreeWidgetItem(&selection);
	item.setText(0, groupName.data());
	item.setData(0, Qt::UserRole, skipper.data());
	current_group = &item;
}

void ContextMenu::AppendItem(std::string_view item)
{
	auto& xitem = *new QTreeWidgetItem(current_group);
	xitem.setText(0, item.data());
	xitem.setData(0, Qt::UserRole, item.data());
}
void ContextMenu::Finish()
{
	selection.expandAll();
}

void ContextMenu::Execute(const QPoint& where)
{
	filter.setFocus();
	exec(where);
}
void OnItemDoubleClicked(QTreeWidgetItem* item, int column)
{
	if (item && item->data(column, Qt::UserRole).toString() == skipper.data())
	{

	}
}