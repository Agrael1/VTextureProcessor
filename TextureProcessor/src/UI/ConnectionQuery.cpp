#include <UI/FlowScene.h>

namespace UI
{
	ConnectionMap& QueryConnectionMap(QGraphicsScene* scene)
	{
		return static_cast<UI::FlowScene*>(scene)->Connections();
	}
}