#pragma once
#include <Logic/ShaderNode.h>

namespace UI
{
	class NodeUI;
	class PortsProperty;
	class PropertyContainer;
}

namespace ver
{
	class DynamicDescriptor :public TextureDescriptor
	{
		friend class DynamicNode;
	public:
		DynamicDescriptor();
		DynamicDescriptor(QJsonObject document, std::string_view styleName);
		~DynamicDescriptor();
	public:
		virtual std::unique_ptr<Node> MakeModel() override;
		void SetParent(UI::NodeUI* xnode){node = xnode;}
		void ModifySinks();
		void ModifySources();
		void Recompile();
		void ResetContainer();
		bool AcceptContainer();
		void GatherPropertyInfo();
		void UpdateProperties(UI::Windows::PropertyElement& props);
	public:
		UI::NodeUI* node = nullptr;
		std::shared_ptr<UI::PortsProperty> prop;
		std::shared_ptr<UI::PropertyContainer> pcont;
	};


	class DynamicNode :public ShaderNode
	{
		using base_class = ShaderNode;
	public:
		DynamicNode(DynamicDescriptor& td);
	public:
		void UpdateSinks();
		void UpdateSources();
		void UpdateBuffer();
		void GetProperties(UI::Windows::PropertyElement& props)override;
		DynamicDescriptor& Desc()
		{
			return static_cast<DynamicDescriptor&>(desc);
		}
	private:
		bool bchange = false;
	};
}
