#include <Logic/ShaderProbe.h>
#include <Logic/ShaderNode.h>
#include <Interfaces/INode.h>
#include <Editor/NodeParser.h>
#include <ranges>

QString ver::ShaderProbe::MakeShader(std::span<UI::INode*> outputs)
{
	QString shader;
	for (size_t i = 0; i < outputs.size(); i++)
		shader += std::format("layout(location = {})out vec4 {};\n\n", i, outputs[i]->Name()).c_str();

	for (auto* i : outputs)
		i->Accept(*this);

	for (auto& i : out_structs)
		shader += QString::fromStdWString(i);
	
	for (auto& i : shaders)
		shader += QString::fromStdWString(i);
	
	return shader;
}

void ver::ShaderProbe::ReadNode(const std::string& connected_node)
{
	if (complexity.contains(connected_node))
		return;
	nodes.at(connected_node)->Accept(*this);
}

void ver::ShaderProbe::AddDesc(TextureDescriptor* td)
{
	if (descs.contains(td))return;
	descs.emplace(td);

	std::wstring str = std::format(L"struct {}{{\n", td->style.StyleName().toStdWString());
	for (auto&& i : td->sources)
		str += std::format(L"\tvec4 {};\n", i.name.toStdWString());
	str += L"}\n\n";
	out_structs.emplace_back(std::move(str));
}

void ver::ShaderProbe::AddShader(std::wstring sh)
{
	shaders.emplace_back(std::move(sh));
}

void ver::ShaderProbe::RegisterNode(std::string_view sh, bool complex)
{
	complexity.emplace(sh, complex);
}

bool ver::ShaderProbe::IsComplex(std::string_view node)
{
	if (auto i = complexity.find(node); i != complexity.end())
		return i->second;
	return false;
}

void ver::ShaderProbe::AddOutput(std::wstring connected_node, std::optional<std::wstring> port)
{
	outputs.emplace(std::move(connected_node), std::move(port));
}

