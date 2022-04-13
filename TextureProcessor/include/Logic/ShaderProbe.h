#pragma once
#include <unordered_set>
#include <memory>
#include <span>

namespace UI
{
	struct INode;
}

namespace ver
{
	struct TextureDescriptor;
	class ShaderProbe
	{
		using container = std::unordered_map<std::string, std::unique_ptr<UI::INode>>;
	public:
		ShaderProbe(const container& xnodes):nodes(xnodes){}
	public:
		QString MakeShader(std::span<UI::INode*> outputs);
		void ReadNode(const std::string& connected_node);
		void AddDesc(TextureDescriptor* td);
	private:
		const container& nodes;
		std::unordered_set<TextureDescriptor*> descs;
	};
}