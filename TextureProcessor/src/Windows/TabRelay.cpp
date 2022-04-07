#include <Windows/TabRelay.h>
#include <Windows/SceneTab.h>
#include <UI/ProjectEvent.h>


using namespace UI::Windows;

TabRelay::TabRelay(QWidget* parent)
	:QTabWidget(parent)
{
	setMovable(true);
	setTabsClosable(true);
	connect(this, &QTabWidget::tabCloseRequested, this, &TabRelay::OnTabClosed);
	connect(this, &QTabWidget::currentChanged, this, &TabRelay::OnCurrentChanged);
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

void UI::Windows::TabRelay::OnTabClosed(int prev_i)
{
	auto key = tabToolTip(prev_i).toStdString();
	if (auto it = temp_tabs.find(key); it != temp_tabs.end())
	{
		if (it->second.get() == prev_tab)
			prev_tab = nullptr;
		temp_tabs.erase(it);
		return;
	}
	if (auto it = tabs.find(key); it != tabs.end())
	{
		if (it->second.get() == prev_tab)
			prev_tab = nullptr;
		tabs.erase(it);
		return;
	}
}

Tab* TabRelay::GetCurrent()
{
	int prev_i = currentIndex();
	if (prev_i == -1)return nullptr;
	auto key = tabToolTip(prev_i).toStdString();
	if (auto it = temp_tabs.find(key); it != temp_tabs.end())
		return it->second.get();
	if (auto it = tabs.find(key); it != tabs.end())
		return it->second.get();
	return nullptr;
}

std::string TabRelay::UKeyTemp(std::string_view pattern)
{
	std::string p{ pattern };
	if (!temp_tabs.contains(p))
		return p;
	p += " ";
	std::string poll;
	size_t r = 0ull;
	do
	{
		poll = p + std::to_string(r++);
	} while (temp_tabs.contains(poll));
	return poll;
}
