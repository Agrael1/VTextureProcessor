#pragma once
namespace ver
{
	class Node;
}

namespace UI
{
	namespace Windows
	{
		class Properties;
	}
	void AppendProperties(Windows::Properties& window, ver::Node& node);
}