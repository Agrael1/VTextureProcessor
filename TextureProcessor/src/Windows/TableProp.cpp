#include <Windows/TableProp.h>
#include <UI/ProjectEvent.h>
#include <QApplication>
#include <QComboBox>

UI::Windows::TableProperties::TableProperties(QWidget* tab_relay)
	:QDockWidget("Node Properties"),
	table(2, 2), tab(tab_relay)
{
	QHeaderView* header = table.verticalHeader();
	header->setDefaultSectionSize(10);
	header->sectionResizeMode(QHeaderView::Fixed);
	header->hide();
	table.horizontalHeader()->setStretchLastSection(true);

	setWidget(&table);

	auto name = new QTableWidgetItem; // header
	name->setData(Qt::DisplayRole, "Name");
	name->setFlags(name->flags() & ~Qt::ItemIsEditable);
	table.setItem(0, 0, name);

	name = new QTableWidgetItem; // value
	name->setData(Qt::DisplayRole, "");
	table.setItem(0, 1, name);

	auto cat = new QTableWidgetItem; // header
	cat->setData(Qt::DisplayRole, "Category");
	cat->setFlags(cat->flags() & ~Qt::ItemIsEditable);
	table.setItem(1, 0, cat);

	table.setCellWidget(1, 1, cats = new QComboBox);
	cats->setEditable(true);

	connect(&table, &QTableWidget::itemChanged, this, &TableProperties::OnItemChanged);
}

void UI::Windows::TableProperties::OnItemChanged(QTableWidgetItem* item)
{
	if (item->column() != 1)return;
	switch (item->row())
	{
	case 0:
		QApplication::postEvent(tab, new NameChangedEvent(item->text()));
		break;
	default: break;
	}
}

void UI::Windows::TableProperties::SetName(const QString& xname)
{
	table.item(0,1)->setData(Qt::DisplayRole, xname);
}

void UI::Windows::TableProperties::SetCatList(std::vector<QString> c)
{
	for (auto&& x : c)cats->addItem(x);
}

QString UI::Windows::TableProperties::GetCategory() const noexcept
{
	return cats->currentText();
}
void UI::Windows::TableProperties::SetCategory(const QString& c) noexcept
{
	cats->setCurrentText(c);
}
