#pragma once
#include <Logic/ShaderNode.h>

namespace UI
{
	class Node;
}

namespace ver
{
	class DynamicDescriptor :public TextureDescriptor
	{
	public:
		using TextureDescriptor::TextureDescriptor;
	public:
		void SetParent(UI::Node* xnode){node = xnode;}

	private:
		UI::Node* node;
	};
	//class DynamicNode :public ShaderNode
	//{
	//public:
	//	using ShaderNode::ShaderNode;
	//public:
	//	void ModifySinks(std::vector<ver::PortDesc> descs);
	//	void ModifySources(std::vector<ver::PortDesc> descs);
	//};
}
