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

	shader = std::make_shared<NodePrivate>(std::move(xshader));

	auto sources = node["Sources"].toArray();
	for (auto it : sources)
	{
		auto source = it.toObject();
		auto type = source["Type"].toString();
		if (type == "Grayscale")
		{
			auto sname = source["Name"].toString().toStdString();
			RegisterSource(GrayscaleSource::Make(sname,
				shader->shadercode, outputs.emplace_back(std::make_shared<QOpenGLTexture>(QOpenGLTexture::Target2D))));
		}
	}

	auto sinks = node["Sinks"].toArray();
	for (auto it : sinks)
	{
		auto sink = it.toObject();
		sink["Name"];
	}
}

ShaderNode::ShaderNode(const ShaderNode& other)
	:shader(other.shader)
{
	sinks.reserve(other.SinksCount());
	sources.reserve(other.SourcesCount());
	for (auto& s : other.sources)
	{
		switch (s->GetType())
		{
		case Source::Type::Grayscale:
			RegisterSource(GrayscaleSource::Make(s->GetName(),
				shader->shadercode, outputs.emplace_back(std::make_shared<QOpenGLTexture>(QOpenGLTexture::Target2D))));
		default:
			break;
		}
	}
}