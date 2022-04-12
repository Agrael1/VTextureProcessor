#include <Logic/ShaderProbe.h>
#include <Interfaces/INode.h>
#include <ranges>

QString ver::ShaderProbe::MakeShader(std::span<UI::INode*> outputs)
{
	QString shader;
	for (size_t i = 0; i < outputs.size(); i++)
		shader += std::format("layout(location = {})out vec4 {};\n", i, outputs[i]->Name()).c_str();


	return shader;
}
