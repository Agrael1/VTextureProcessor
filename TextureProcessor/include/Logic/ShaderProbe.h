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
		void AddShader(std::wstring sh);
		void RegisterNode(std::string_view sh, bool complex = false);
		bool IsComplex(std::string_view node);
		void AddOutput(std::string_view output_node, std::string_view connected_node, std::string_view port="");
	private:
		const container& nodes;
		std::unordered_set<TextureDescriptor*> descs;
		std::unordered_map<std::string_view, bool> complexity;
		std::vector<std::wstring> out_structs;
		std::vector<std::wstring> shaders;
		std::unordered_map<std::string_view, std::pair<std::string_view, std::string_view>> outputs;
	};
}