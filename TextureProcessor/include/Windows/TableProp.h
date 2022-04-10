#pragma once
#include <QDockWidget>
#include <QTableWidget>
#include <QHeaderView>

class QComboBox;

namespace UI::Windows
{
	class TableProperties : public QDockWidget
	{
	public:
		TableProperties(QWidget* tab);
		void OnItemChanged(QTableWidgetItem* item);
		void SetName(const QString& xname);
		void SetCatList(std::vector<QString> c);
		QString GetCategory()const noexcept;
		void SetCategory(const QString& c) noexcept;
	private:
		QTableWidget table;
		QWidget* tab;
		QComboBox* cats;
	};
}
