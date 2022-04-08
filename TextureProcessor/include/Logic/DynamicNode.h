#pragma once
#include <Logic/ShaderNode.h>
#include <unordered_set>

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
		DynamicDescriptor(QJsonObject document);
		~DynamicDescriptor();
	public:
		virtual std::unique_ptr<Node> MakeModel() override;
		void SetParent(UI::NodeUI* xnode){node = xnode;}
		void ModifySinks();
		void ModifySources();
		void Recompile();
		QJsonObject Save();

		void ResetContainer();
		bool AcceptContainer();
		void GatherPropertyInfo();
		void UpdateProperties(UI::Windows::PropertyElement& props);
	public:
		UI::NodeUI* node = nullptr;
		std::shared_ptr<UI::PortsProperty> prop;
		std::shared_ptr<UI::PropertyContainer> pcont;
		std::function<void()> prop_callback;
	};


	class DynamicNode :public ShaderNode
	{
	public:
		using ShaderNode::ShaderNode;
	public:
		void GetProperties(UI::Windows::PropertyElement& props)override;
		DynamicDescriptor& Desc()
		{
			return static_cast<DynamicDescriptor&>(desc);
		}
	private:
		bool bchange = false;
	};
}
