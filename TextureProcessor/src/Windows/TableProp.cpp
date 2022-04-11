#include <Windows/TableProp.h>
#include <UI/ProjectEvent.h>
#include <UI/ColorWidget.h>
#include <QApplication>
#include <QComboBox>

UI::Windows::TableProperties::TableProperties(QWidget* tab_relay)
	:QDockWidget("Node Properties"),
	table(4, 2), tab(tab_relay)
{
	QHeaderView* header = table.verticalHeader();
	header->setDefaultSectionSize(10);
	header->sectionResizeMode(QHeaderView::Fixed);
	header->hide();
	table.horizontalHeader()->setStretchLastSection(true);

	setWidget(&table);
	/////////////////////////////NAME///////////////////////////////
	auto name = new QTableWidgetItem; // header
	name->setData(Qt::DisplayRole, "Name");
	name->setFlags(name->flags() & ~Qt::ItemIsEditable);
	table.setItem(0, 0, name);

	name = new QTableWidgetItem; // value
	name->setData(Qt::DisplayRole, "");
	table.setItem(0, 1, name);
	/////////////////////////////GROUP///////////////////////////////
	auto cat = new QTableWidgetItem; // header
	cat->setData(Qt::DisplayRole, "Category");
	cat->setFlags(cat->flags() & ~Qt::ItemIsEditable);
	table.setItem(1, 0, cat);

	table.setCellWidget(1, 1, cats = new QComboBox);
	cats->setEditable(true);
	/////////////////////////////HEADER///////////////////////////////

	auto xhead = new QTableWidgetItem; // header
	xhead->setData(Qt::DisplayRole, QStringLiteral("Header Color"));
	xhead->setFlags(xhead->flags() & ~Qt::ItemIsEditable);
	table.setItem(2, 0, xhead);

	table.setCellWidget(2, 1, head = new ColorWidget);

	/////////////////////////////FONT///////////////////////////////
	auto xfont = new QTableWidgetItem; // header
	xfont->setData(Qt::DisplayRole, QStringLiteral("Font Color"));
	xfont->setFlags(xfont->flags() & ~Qt::ItemIsEditable);
	table.setItem(3, 0, xfont);

	table.setCellWidget(3, 1, font = new ColorWidget);

	connect(&table, &QTableWidget::itemChanged, this, &TableProperties::OnItemChanged);
	connect(head, &ColorWidget::ColorChanged, this, &TableProperties::HeadColorChanged);
	connect(font, &ColorWidget::ColorChanged, this, &TableProperties::FontColorChanged);
}

void UI::Windows::TableProperties::OnItemChanged(QTableWidgetItem* item)
{
	if (item->column() != 1)return;
	switch (item->row())
	{
	case 0:
		emit NameChanged(item->text());
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

void UI::Windows::TableProperties::SetTitleColor(const QColor& c) noexcept
{
	head->SetColor(c);
}
void UI::Windows::TableProperties::SetFontColor(const QColor& c) noexcept
{
	font->SetColor(c);
}
