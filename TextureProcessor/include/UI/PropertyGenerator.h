#pragma once
namespace ver
{
	class Node;
}

namespace UI
{
	struct INode;
	namespace Windows
	{
		class PropertyElement;
	}
	void PlaceProperties(Windows::PropertyElement& props, ver::Node& node, INode& entity);
}