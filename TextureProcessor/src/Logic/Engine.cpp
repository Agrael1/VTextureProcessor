/**
 * @file Engine.cpp
 * @author Ilya Doroshenko (xdoros01)
 * @brief Handler for GLSL Shaders
 */

#include <Logic/Engine.h>
#include <QOpenGLTexture>
#include <QOpenGLExtraFunctions>

std::optional<Engine> Engine::e;

constexpr GLenum bufs[] = {
GL_COLOR_ATTACHMENT0
, GL_COLOR_ATTACHMENT1 
, GL_COLOR_ATTACHMENT2 
, GL_COLOR_ATTACHMENT3 
, GL_COLOR_ATTACHMENT4 
, GL_COLOR_ATTACHMENT5 
, GL_COLOR_ATTACHMENT6 
, GL_COLOR_ATTACHMENT7 
, GL_COLOR_ATTACHMENT8 
, GL_COLOR_ATTACHMENT9 
, GL_COLOR_ATTACHMENT10
, GL_COLOR_ATTACHMENT11
, GL_COLOR_ATTACHMENT12
, GL_COLOR_ATTACHMENT13
, GL_COLOR_ATTACHMENT14
, GL_COLOR_ATTACHMENT15
, GL_COLOR_ATTACHMENT16
, GL_COLOR_ATTACHMENT17
, GL_COLOR_ATTACHMENT18
, GL_COLOR_ATTACHMENT19
, GL_COLOR_ATTACHMENT20
, GL_COLOR_ATTACHMENT21
, GL_COLOR_ATTACHMENT22
, GL_COLOR_ATTACHMENT23
, GL_COLOR_ATTACHMENT24
, GL_COLOR_ATTACHMENT25
, GL_COLOR_ATTACHMENT26
, GL_COLOR_ATTACHMENT27
, GL_COLOR_ATTACHMENT28
, GL_COLOR_ATTACHMENT29
, GL_COLOR_ATTACHMENT30
, GL_COLOR_ATTACHMENT31};

 /**
  * @brief Creates GL context
 */
Engine::Context::Context()
{
	QSurfaceFormat surfaceFormat;
	surfaceFormat.setMajorVersion(4);
	surfaceFormat.setMinorVersion(2);

	context.setFormat(surfaceFormat);
	context.create();

	surface.setFormat(surfaceFormat);
	surface.create();
	if (!surface.isValid()) qDebug("Unable to create the Offscreen surface");

	context.makeCurrent(&surface);
	funcs.initializeOpenGLFunctions();
}

/**
 * @brief Creates Engine with specified texture size
 * @param size size of the input and output textures
*/
Engine::Engine()
	:vs(QOpenGLShader::Vertex), vlay(QOpenGLBuffer::VertexBuffer)
{
	vs.compileSourceCode(vertexShaderSource);
	shaders.addShader(&vs);

	vbuf.create();
	vbuf.bind();
}

Engine::~Engine() {
	Empty().destroy();
}

void Engine::BindScene(UI::FlowScene* scene, QSize size)
{
	auto& i = Instance();
	i.frames.emplace(std::piecewise_construct, 
		std::forward_as_tuple(scene), 
		std::forward_as_tuple(size, i.con.Format()));
}
void Engine::UnbindScene(UI::FlowScene* const scene)
{
	auto& i = Instance();
	if (!i.frames.contains(scene))return;
	if (i.current == &i.frames.at(scene))
		i.current = nullptr;
	i.frames.erase(scene);
}

void Engine::SwitchScene(UI::FlowScene* scene)
{
	auto& i = Instance();
	i.current = &i.frames.at(scene);
	i.current->bind();
	auto sz = i.current->size();
	i.con.funcs.glViewport(0, 0, sz.width(), sz.height());
}

/**
 * @brief Creates static empty texture
 * @return ref to global empty texture
*/
QOpenGLTexture& Engine::Empty()
{
	static QOpenGLTexture empty{ QOpenGLTexture::Target2D };
	if (!empty.isCreated())
	{
		empty.setSize(1, 1, 1);
		empty.allocateStorage();
	}
	return empty;
}

/**
 * @brief Renders specified node with input and output textures
 * @param ps Node shader
 * @param inputs input textures to be bound
 * @param tile enable tiling on textures
 * @param outputs retrieved textures
 * @param buffer constant buffer
 * @return Image to be drawn to the node
*/
void Engine::Render(QOpenGLShader& ps, std::span<std::shared_ptr<QImage>> inputs, std::span<std::shared_ptr<QImage>> outputs, ver::dc::Buffer& buffer)
{
	if (!current)return;

	Current();
	shaders.addShader(&ps);
	shaders.link();
	shaders.bind();

	std::vector<std::optional<QOpenGLTexture>> xinputs(inputs.size());
	for (uint32_t s = 0; const auto & x : inputs)
	{
		if (x)xinputs[s].emplace(*x, QOpenGLTexture::MipMapGeneration::DontGenerateMipMaps);
		s++;
	}

	for (uint32_t s = 0; auto & i: xinputs)
	{
		if (i)
		{
			if (false)i->setWrapMode(QOpenGLTexture::Repeat);
			else i->setWrapMode(QOpenGLTexture::ClampToEdge);
			i->bind(s++);
		}
		else Empty().bind(s++);
	}

	auto catt = current->sizes();
	for (size_t i = catt.size(); i < outputs.size(); i++)
		current->addColorAttachment(catt[0]);


	if (buffer)BindBuffer(buffer);

	con.funcs.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	con.funcs.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();
	f->glDrawBuffers(current->sizes().size(), bufs);

	con.funcs.glDrawArrays(GL_TRIANGLES, 0, 3);
	shaders.removeShader(&ps);

	for (int i = 0; auto & o : outputs)
		*o = current->toImage(false, i++);
}

/**
 * @brief Constant buffer binding
 * @param buffer buffer with data to be bound
*/
void Engine::BindBuffer(ver::dc::Buffer& buffer)
{
	for (auto r : buffer)
	{
		switch (r.GetType())
		{
		case ver::dc::Type::Float:
			shaders.setUniformValue(r.GetName().data(), (const float&)r);
			break;
		case ver::dc::Type::Float2:
			shaders.setUniformValue(r.GetName().data(), (const QVector2D&)r);
			break;
		case ver::dc::Type::Float3:
			shaders.setUniformValue(r.GetName().data(), (const QVector3D&)r);
			break;
		case ver::dc::Type::Float4:
			shaders.setUniformValue(r.GetName().data(), (const QVector4D&)r);
			break;
		case ver::dc::Type::Matrix:
			shaders.setUniformValue(r.GetName().data(), (const QMatrix4x4&)r);
			break;
		case ver::dc::Type::Bool:
		case ver::dc::Type::Integer:
			shaders.setUniformValue(r.GetName().data(), (const int&)r);
			break;
		default:
			break;
		}
	}
}

void Engine::Current()
{
	con.context.makeCurrent(&con.surface);
}
