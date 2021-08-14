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
		class XProperties;
	}
	void AppendProperties(Windows::XProperties& window, ver::Node& node, IXNode& entity);
}