#include <UI/ContextMenu.h>


ContextMenu::ContextMenu(QWidget* parent)
    :QMenu(parent), layout(this)
{
    layout.setAlignment(Qt::AlignTop);
    layout.setContentsMargins(0, 0, 0, 0);
    layout.setSpacing(0);

    filter.setPlaceholderText("Search...");
    filter.setMinimumWidth(500);
    filter.setClearButtonEnabled(true);

    selection.setHeaderLabel("Select Node Type to Add");

    layout.addWidget(&filter);
    layout.addWidget(&selection);
}
