#include <UI/ContextMenu.h>


/**
 * @brief Construct a new Context Menu:: Context Menu object
 *
 * @param parent
 */
ContextMenu::ContextMenu(QWidget* parent)
	:QMenu(parent), layout(this)
{
	// Creates context menu times and connects filter text field

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

	// Node type filter
	filter.connect(&filter, &QLineEdit::textEdited, this, &ContextMenu::OnFilterChanged);
}

/**
 * @brief Creates a new group in the context menu
 *
 * @param groupName Name of the new group
 */
void ContextMenu::AppendGroup(std::string_view groupName)
{
	auto& item = *new QTreeWidgetItem(&selection);
	item.setText(0, groupName.data());
	item.setData(0, Qt::UserRole, skipper.data());
	current_group = &item;
}

/**
 * @brief Adds a new item to a context menu group
 *
 * @param item Name of the new item
 */
void ContextMenu::AppendItem(std::string_view item)
{
	auto& xitem = *new QTreeWidgetItem(current_group);
	xitem.setText(0, item.data());
	xitem.setData(0, Qt::UserRole, item.data());
}

/**
 * @brief Expands all subtrees in the context menu
 *
 */
void ContextMenu::Finish()
{
	selection.expandAll();
}

/**
 * @brief Renders the context menu
 *
 * @param where
 */
void ContextMenu::Execute(const QPoint& where)
{
	filter.setFocus();
	exec(where);
}

/**
 * @brief Change group name on double click
 *
 * @param item Item that was double clicked
 * @param column Column of the context menu
 */
void ContextMenu::OnItemDoubleClicked(QTreeWidgetItem* item, int column)
{
	if (item && item->data(column, Qt::UserRole).toString() == skipper.data())
	{
	}
}

/**
 * @brief Callback when filter was edited (hides children not matched by the filter)
 *
 * @param text New filter text
 */
void ContextMenu::OnFilterChanged(const QString& text)
{
	auto cnt = selection.topLevelItemCount();

	for (int j = 0; j < cnt; j++)
	{
		auto* topLvlItem = selection.topLevelItem(j);
		auto child_count = topLvlItem->childCount();
		int hidden = 0;

		// Max context menu depth is 2 (groups and children)
		// Hides unmatched children
		for (int i = 0; i < child_count; ++i)
		{
			auto child = topLvlItem->child(i);
			auto modelName = child->data(0, Qt::UserRole).toString();
			const bool match = (modelName.contains(text, Qt::CaseInsensitive));
			child->setHidden(!match);
			hidden += !match;
		}
		topLvlItem->setHidden(hidden == child_count);
	}
}
