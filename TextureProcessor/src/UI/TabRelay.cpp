#include <UI\TabRelay.h>

TabRelay::TabRelay(QWidget* parent, SceneTab*& sc)
	:QTabWidget(parent), cur_scene(sc)
{
	setMovable(true);
	setTabsClosable(true);
}
