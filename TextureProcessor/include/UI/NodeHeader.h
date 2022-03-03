#pragma once
#include <QGraphicsProxyWidget>

class NodeHeader : QGraphicsProxyWidget
{
public:
	NodeHeader();
	QLabel& Label();
};