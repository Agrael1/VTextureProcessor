#pragma once
namespace ver
{
	class Node;
}

namespace UI
{
	struct IXNode;
	namespace Windows
	{
		class XPropertyElement;
	}
	void PlaceProperties(Windows::XPropertyElement& props, ver::Node& node, IXNode& entity);
}