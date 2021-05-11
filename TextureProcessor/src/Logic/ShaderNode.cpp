#include <Logic/ShaderNode.h>
#include <Logic/SourcesT.h>
#include <Logic/SinksT.h>
#include <Presenter/GLInterpret.h>

using namespace ver;

ShaderNode::NodePrivate::NodePrivate(QString&& code)
	:shader(QOpenGLShader::Fragment), shadercode(code.toStdString())
{
	shader.compileSourceCode(code);
}

ShaderNode::ShaderNode(QJsonObject document, Engine& e)
	: e(e)
{
	auto node = document["Node"].toObject();
	QString xshader{ "#version 420 \n" };

	if (node.contains("Properties"))
		SetProperties(node["Properties"].toArray(), xshader);


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
	outputs.reserve(sources.size());

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
	inputs.reserve(sinks.size());
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
	:shader(other.shader), e(other.e), buf(other.buf)
{
	sinks.reserve(other.SinksCount());
	sources.reserve(other.SourcesCount());
	outputs.reserve(other.SourcesCount());
	inputs.reserve(other.SinksCount());

	for (auto& s : other.sources)
	{
		switch (s->GetType())
		{
		case PortType::Grayscale:
		{
			auto r = std::make_shared<QOpenGLTexture>(QOpenGLTexture::Target2D);
			r->setSize(256, 256);
			r->create();
			r->allocateStorage();

			RegisterSource(GrayscaleSource::Make(s->GetName(),
				shader->shadercode, outputs.emplace_back(std::move(r))));
		}
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
	return e.Render(shader->shader, inputs, tiling, outputs, buf);
}


void ShaderNode::SetProperties(const QJsonArray& props, QString& scode)
{
	if (props.isEmpty())return;
	dc::Layout lay;

	std::vector<std::string> tags;
	std::vector<QVariant> vars;
	for (auto it : props)
	{
		auto p = it.toObject();
		scode += "uniform ";
		auto qtype = p["Type"].toString();
		scode += qtype + " ";
		auto qtag = p["Tag"].toString();
		scode += qtag + ";\n";
		lay += { tags.emplace_back(qtag.toStdString()), dc::LayoutElement{ qtype.toStdString() }};
	}


	buf.Replace(std::move(lay));
	for (size_t i = 0; auto it : props)
	{
		auto p = it.toObject();
		if (p.contains("Val"))
			buf[tags[i]]= p["Val"].toVariant();
		i++;
	}
}