#pragma once
#include <UI/Node.h>
#include <Logic/DynamicNode.h>

namespace UI
{
	class PortsProperty;

	class DynamicNode :public XNode<ver::ShaderNode>
	{
		using base_class = XNode<ver::ShaderNode>;
	public:
		DynamicNode();
		DynamicNode(const std::filesystem::path& p);
	public:
		void Rename(const QString& name);
		const QString& StyleName()const noexcept
		{
			return style->StyleName();
		}
		static std::pair<QJsonObject, std::string> Parse(const std::filesystem::path& p);

		virtual void UpdateProperties(Windows::PropertyElement& properties) override;
	private:
		std::shared_ptr<PortsProperty> container;
	};
}