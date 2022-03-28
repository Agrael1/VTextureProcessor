#pragma once
#include <Logic/ShaderNode.h>

namespace UI
{
	class NodeUI;
	class PortsProperty;
}

namespace ver
{
	class DynamicDescriptor :public TextureDescriptor
	{
		friend class DynamicNode;
	public:
		DynamicDescriptor();
		DynamicDescriptor(QJsonObject document, std::string_view styleName);
	public:
		virtual std::unique_ptr<Node> MakeModel() override;
		void SetParent(UI::NodeUI* xnode){node = xnode;}
		void ModifySinks();
		void ModifySources();
		void Recompile();
	private:
		UI::NodeUI* node = nullptr;
		std::shared_ptr<UI::PortsProperty> prop;
	};


	class DynamicNode :public ShaderNode
	{
		using base_class = ShaderNode;
	public:
		using ShaderNode::ShaderNode;
	public:
		void UpdateSinks();
		void UpdateSources();
		void GetProperties(UI::Windows::PropertyElement& props)override;
	};
}
