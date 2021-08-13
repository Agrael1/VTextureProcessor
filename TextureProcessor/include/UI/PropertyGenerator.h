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
		class Properties;
	}
	void AppendProperties(Windows::Properties& window, ver::Node& node, IXNode& entity);
}