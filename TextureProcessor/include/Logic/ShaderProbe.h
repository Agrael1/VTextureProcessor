#pragma once
#include <unordered_map>
#include <memory>
#include <span>

namespace UI
{
	struct INode;
}

namespace ver
{
	class ShaderProbe
	{
		using container = std::unordered_map<std::string, std::unique_ptr<UI::INode>>;
	public:
		ShaderProbe(const container& xnodes):nodes(xnodes){}
	public:
		QString MakeShader(std::span<UI::INode*> outputs);
	private:
		const container& nodes;
	};
}