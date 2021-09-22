#pragma once
#include <QDockWidget>
#include <QTableWidget>
#include <QHeaderView>

namespace UI::Windows
{
	class TableProperties : public QDockWidget
	{
	public:
		TableProperties(QWidget* tab_relay);
		void OnItemChanged(QTableWidgetItem* item);
	private:
		QTableWidget table;
		QWidget* tab_relay;
	};
}
