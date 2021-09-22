#include <Windows/TableProp.h>
#include <UI/ProjectEvent.h>

UI::Windows::TableProperties::TableProperties(QWidget* tab_relay)
	:QDockWidget("Node Properties"),
	table(1, 2), tab_relay(tab_relay)
{
	QHeaderView* header = table.verticalHeader();
	header->setDefaultSectionSize(10);
	header->sectionResizeMode(QHeaderView::Fixed);
	header->hide();
	table.horizontalHeader()->setStretchLastSection(true);

	setWidget(&table);
	QTableWidgetItem* name = new QTableWidgetItem;
	name->setData(Qt::DisplayRole, "Name");
	name->setFlags(name->flags() & ~Qt::ItemIsEditable);
	table.setItem(0, 0, name);

	name = new QTableWidgetItem;
	name->setData(Qt::DisplayRole, "Node");
	table.setItem(0, 1, name);

	connect(&table, &QTableWidget::itemChanged, this, &TableProperties::OnItemChanged);
}

void UI::Windows::TableProperties::OnItemChanged(QTableWidgetItem* item)
{
	if (item->column() != 1)return;
	switch (item->row())
	{
	case 0:
		//QApplication::sendEvent(tab_relay, new NameChangedEvent(item->text()));
		break;
	default: break;
	}
}
