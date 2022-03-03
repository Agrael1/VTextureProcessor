#pragma once
#include <Logic/ShaderNode.h>
#include <UI/PropertyHandler.h>

namespace ver
{
	class DynamicNode :public ShaderNode
	{
	public:
		struct property_handler
		{
			static void ConstructProperties(UI::Windows::PropertyElement& properties, ver::Node& node);
		};
	public:
		using ShaderNode::ShaderNode;
	public:
		void SetName(std::string_view name)
		{

		}
	};
}
