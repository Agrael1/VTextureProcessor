#include <Logic/ShaderProbe.h>
#include <Logic/ShaderNode.h>
#include <Interfaces/INode.h>
#include <Editor/NodeParser.h>
#include <ranges>

QString ver::ShaderProbe::MakeShader(std::span<UI::INode*> outputs)
{
	QString shader;
	for (size_t i = 0; i < outputs.size(); i++)
		shader += std::format("layout(location = {})out vec4 {};\n", i, outputs[i]->Name()).c_str();

	for (auto* i : outputs)
		i->Accept(*this);


	for (auto* i : descs)
		shader += i->shader_body;
	
	return shader;
}

void ver::ShaderProbe::ReadNode(const std::string& connected_node)
{
	nodes.at(connected_node)->Accept(*this);
}

void ver::ShaderProbe::AddDesc(TextureDescriptor* td)
{
	descs.emplace(td);
}
