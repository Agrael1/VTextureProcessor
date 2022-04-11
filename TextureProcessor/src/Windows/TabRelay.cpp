#include <Windows/TabRelay.h>
#include <Windows/SceneTab.h>
#include <UI/ProjectEvent.h>


using namespace UI::Windows;

TabRelay::TabRelay(QWidget* parent)
	:QTabWidget(parent)
{
	setMovable(true);
	setTabsClosable(true);
	connect(this, &QTabWidget::currentChanged, this, &TabRelay::OnCurrentChanged);
	connect(this, &QTabWidget::tabCloseRequested, [this](int index) {
		auto* w = static_cast<Tab*>(widget(index));
		if (prev_tab == w)
		{
			w->OnLeave();
			prev_tab = nullptr;
		}
		tabs.erase(w->Path().wstring());
		delete w;
		});
}

UI::Windows::TabRelay::~TabRelay()
{
	disconnect(this, &QTabWidget::currentChanged, this, &TabRelay::OnCurrentChanged);
}

void TabRelay::OnCurrentChanged(int index)
{
	if (prev_tab)prev_tab->OnLeave();
	prev_tab = GetCurrent();
	if (!prev_tab)return;
	prev_tab->OnEnter();
}
void TabRelay::OnTabCreated()
{
	prev_tab = GetCurrent();
	prev_tab->OnEnter();
}

Tab* TabRelay::GetCurrent()
{
	return static_cast<Tab*>(currentWidget());
}

QString TabRelay::UKeyTemp(QStringView pattern)
{
	return pattern.toString() + '_' + QString::number(temp_index++);
}
