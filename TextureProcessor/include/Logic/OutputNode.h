#pragma once
#include <Logic/Descriptor.h>
#include <Logic/Node.h>
#include <UI/NodeStyle.h>

namespace ver
{
	class OutputDescriptor : public Descriptor
	{
	public:
		virtual std::unique_ptr<Node> MakeModel() override;
		virtual bool valid()const noexcept {
			return true;
		}
	};

	class OutputNode : public Node
	{
	public:
		OutputNode();
	public:
		std::span<std::shared_ptr<QImage>> GetLayout()noexcept
		{
			return { &out, 1 };
		}
		virtual UI::NodeStyle& GetStyle()override
		{
			return style;
		}
		void Update();
		std::string Export()override;
		virtual void ExportSilent(std::string_view name);
	private:
		std::shared_ptr<QImage> in;
		std::shared_ptr<QImage> out;
		UI::NodeStyle style;
	};
}
