#pragma once
#include <UI/UINode.h>
#include <Logic/DynamicNode.h>

namespace UI
{
	class PortsProperty;

	//class DynamicNode :public NodeUI
	//{
	//	using base_class = NodeUI;
	//public:
	//	DynamicNode(const std::filesystem::path& p);
	//	~DynamicNode();
	//public:
	//	void Rename(const QString& name);
	//	const QString& StyleName()const noexcept
	//	{
	//		return style->StyleName();
	//	}
	//	static std::pair<QJsonObject, std::string> Parse(const std::filesystem::path& p);

	//	virtual void UpdateProperties(Windows::PropertyElement& properties) override;
	//	void UpdatePorts();
	//private:
	//	std::shared_ptr<PortsProperty> container;
	//};
}