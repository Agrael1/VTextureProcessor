/**
 * @file Engine.cpp
 * @author Ilya Doroshenko (xdoros01)
 * @brief Handler for GLSL Shaders
 */

#include <Presenter/Engine.h>

/**
 * @brief Creates GL context
*/
Engine::Context::Context()
{
	QSurfaceFormat surfaceFormat;
	surfaceFormat.setMajorVersion(3);
	surfaceFormat.setMinorVersion(3);

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
Engine::Engine(QSize size)
	:vs(QOpenGLShader::Vertex), frame(size, con.Format()), vlay(QOpenGLBuffer::VertexBuffer)
{
	con.funcs.glViewport(0, 0, size.width(), size.height());
	frame.bind();

	vs.compileSourceCode(vertexShaderSource);
	shaders.addShader(&vs);

	vbuf.create();
	vbuf.bind();
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
		empty.create();
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
QImage Engine::Render(QOpenGLShader& ps, std::span<std::shared_ptr<QOpenGLTexture>> inputs, bool tile, std::span<std::shared_ptr<QOpenGLTexture>> outputs, ver::dc::Buffer& buffer)
{
	shaders.addShader(&ps);
	shaders.link();
	shaders.bind();

	for (uint32_t s = 0; auto & i: inputs)
	{
		if (i)
		{
			if (tile)
				i->setWrapMode(QOpenGLTexture::ClampToEdge);
			i->bind(s++);
		}
		else Empty().bind(s++);
	}

	if (buffer)BindBuffer(buffer);

	con.funcs.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	con.funcs.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	con.funcs.glDrawArrays(GL_TRIANGLES, 0, 3);
	shaders.removeShader(&ps);

	if(tile)
		for (auto& i : inputs)
			if (i)i->setWrapMode(QOpenGLTexture::Repeat);

	for (int i = 0; auto & o : outputs)
		o = std::make_shared<QOpenGLTexture>(frame.toImage(false, i++));

	return frame.toImage(true, 0);
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