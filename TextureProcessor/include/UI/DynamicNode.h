#pragma once
#include <UI/Node.h>
#include <Logic/ShaderNode.h>

namespace UI
{
	class DynamicNode :public XNode<ver::ShaderNode>
	{
	public:
		DynamicNode();
	public:
		void Rename(const QString& name);
	};
}