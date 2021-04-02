#include <ShaderNode.h>
#include <SourcesT.h>

using namespace ver;

ShaderNode::NodePrivate::NodePrivate(QString&& code)
	:shader(QOpenGLShader::Fragment), shadercode(code.toStdString())
{
	shader.compileSourceCode(code);
}

ShaderNode::ShaderNode(QJsonObject document)
{
	auto node = document["Node"].toObject();
	QString xshader;
	auto val = document["Value"];
	if (!val.isArray())
	{
		return;//TODO: file
	}
	for (auto x : val.toArray())
	{
		xshader += x.toString();
	}

	auto sources = node["Sources"].toArray();
	for (auto it : sources)
	{
		auto source = it.toObject();
		auto type = source["Type"].toString();
		if (type == "Grayscale")
		{
		}
	}

	auto sinks = node["Sinks"].toArray();
	for (auto it : sinks)
	{
		auto sink = it.toObject();
		sink["Name"];
	}


}