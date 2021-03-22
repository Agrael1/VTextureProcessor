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

void ContextMenu::AppendGroup(std::wstring_view groupName)
{
    auto& item = *new QTreeWidgetItem(&selection);
    item.setText(0, QString::fromWCharArray(groupName.data(), int(groupName.size())));
    item.setData(0, Qt::UserRole, QString::fromWCharArray(skipper.data(),int(skipper.size())));
    current_group = &item;
}

void ContextMenu::AppendItem(std::wstring_view item)
{
    auto& xitem = *new QTreeWidgetItem(current_group);
    xitem.setText(0, QString::fromWCharArray(item.data(), int(item.size())));
    xitem.setData(0, Qt::UserRole, QString::fromWCharArray(item.data(), int(item.size())));
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
