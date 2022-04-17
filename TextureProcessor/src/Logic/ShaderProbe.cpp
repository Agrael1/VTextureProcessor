#include <Logic/ShaderProbe.h>
#include <Logic/ShaderNode.h>
#include <Interfaces/INode.h>
#include <Editor/NodeParser.h>
#include <ranges>

QString ver::ShaderProbe::MakeShader(std::span<UI::INode*> xoutputs)
{
	QString shader;
	for (size_t i = 0; i < xoutputs.size(); i++)
		shader += std::format("layout(location = {})out vec4 {};\n\n", i, xoutputs[i]->Name()).c_str();

	for (auto* i : xoutputs)
		i->Accept(*this);

	for (auto& i : out_structs)
		shader += QString::fromStdWString(i);

	for (auto& i : shaders)
		shader += QString::fromStdWString(i);

	size_t tmpcnt = 0;
	std::unordered_set<std::string_view> complex_vars;
	for (auto& i : outputs) {
		complex_vars.emplace(i.second.first);
	}

	shader += u"void main(){\n";
	for (auto i : complex_vars)
		shader += std::format("\t{} tmpvar_{} = {}_main(gl_FragCoord);\n",
			IsComplex(i) ? i : "vec4", i, i).c_str();

	for (auto* i : xoutputs)
	{
		auto n = i->Name();
		auto r = outputs.at(n);
		shader += std::format("\t{} = tmpvar_{};\n", n, 
			r.second.empty()?r.first:std::format("{}.{}", r.first, r.second)).c_str();
	}

	shader += u"}\n";
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

void ver::ShaderProbe::AddOutput(std::string_view output_node, std::string_view connected_node, std::string_view port)
{
	outputs.emplace(output_node, std::pair{ connected_node , port });
}

