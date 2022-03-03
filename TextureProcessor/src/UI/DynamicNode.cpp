#include <UI/DynamicNode.h>
#include <QGraphicsProxyWidget>

using namespace UI;

DynamicNode::DynamicNode()
	:XNode<ver::ShaderNode>({}, "")
{
	l_main = new GraphicsLinearLayout(Qt::Orientation::Horizontal);
	style->SetFontColor(Qt::white);
	Init();
}

void UI::DynamicNode::Rename(const QString& name)
{
	style->SetStyleName(name);
	auto& h = Header();
	h.setText(name);
	h.adjustSize();
	h.setMinimumSize(h.size());
	l_central->invalidate();
	Update();
}
