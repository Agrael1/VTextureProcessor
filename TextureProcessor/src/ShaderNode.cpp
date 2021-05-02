#include <ShaderNode.h>
#include <SourcesT.h>
#include <SinksT.h>
#include <Presenter/GLInterpret.h>

using namespace ver;

ShaderNode::NodePrivate::NodePrivate(QString&& code)
	:shader(QOpenGLShader::Fragment), shadercode(code.toStdString())
{
	shader.compileSourceCode(code);
}

ShaderNode::ShaderNode(QJsonObject document, Engine& e)
	:e(e)
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
		auto type = sink["Type"].toString();
		if (type == "Grayscale")
		{
			auto sname = sink["Name"].toString().toStdString();
			RegisterSink(GrayscaleSink::Make(sname, inputs.emplace_back()));
		}
	}
}
ShaderNode::ShaderNode(const ShaderNode& other)
	:shader(other.shader), e(other.e)
{
	sinks.reserve(other.SinksCount());
	sources.reserve(other.SourcesCount());
	for (auto& s : other.sources)
	{
		switch (s->GetType())
		{
		case PortType::Grayscale:
			RegisterSource(GrayscaleSource::Make(s->GetName(),
				shader->shadercode, outputs.emplace_back(std::make_shared<QOpenGLTexture>(QOpenGLTexture::Target2D))));
		default:
			break;
		}
	}
	for (auto& s : other.sinks)
	{
		switch (s->GetType())
		{
		case PortType::Grayscale:
			RegisterSink(GrayscaleSink::Make(s->GetRegisteredName(),
				inputs.emplace_back()));
		default:
			break;
		}
	}
}

QImage ShaderNode::Update()
{
	//for (auto& i: inputs)
		//i->bind();
	//for (auto& i : outputs)
		//i->bind();
	return e.Render(shader->shader);
}