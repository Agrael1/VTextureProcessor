#pragma once
#include <UI/Node.h>
#include <Logic/DynamicNode.h>

namespace UI
{
	class DynamicNode :public XNode<ver::DynamicNode>
	{
		using base_class = XNode<ver::ShaderNode>;
	public:
		DynamicNode();
	public:
		void Rename(const QString& name);

		virtual void UpdateProperties(Windows::PropertyElement& properties) override;
	private:
	};
}