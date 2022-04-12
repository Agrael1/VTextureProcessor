#pragma once
#include <QWidget>

namespace UI
{
	struct AwareWidget : public QWidget
	{
		Q_OBJECT
	signals:
		void ValueChanged();
	};
}