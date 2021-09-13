#include <Windows/TabRelay.h>
#include <Windows/SceneTab.h>


using namespace UI::Windows;

TabRelay::TabRelay(QWidget* parent, SceneTab*& sc)
	:QTabWidget(parent), cur_scene(sc)
{
	setMovable(true);
	setTabsClosable(true);
	connect(this, &QTabWidget::tabCloseRequested, this, &TabRelay::OnTabClosed);
	connect(this, &QTabWidget::currentChanged, this, &TabRelay::OnCurrentChanged);
}

void TabRelay::OnCurrentChanged(int index)
{
	auto* cur = GetCurrent();
	if (!cur)return;
	cur_scene = dynamic_cast<SceneTab*>(cur);
	cur->OnChange();
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
