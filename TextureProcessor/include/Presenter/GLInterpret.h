#pragma once
#include <span>
#include <Logic/DynamicConstant.h>

class Engine
{
	struct Context
	{
		Context()
		{
			QSurfaceFormat surfaceFormat;
			surfaceFormat.setMajorVersion(4);
			surfaceFormat.setMinorVersion(3);

			context.setFormat(surfaceFormat);
			context.create();

			surface.setFormat(surfaceFormat);
			surface.create();
			if (!surface.isValid()) qDebug("Unable to create the Offscreen surface");

			context.makeCurrent(&surface);
			funcs.initializeOpenGLFunctions();
		}
		auto Format()
		{
			QOpenGLFramebufferObjectFormat fboFormat;
			fboFormat.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
			return fboFormat;
		}
		QOffscreenSurface surface;
		QOpenGLContext context;
		QOpenGLPaintDevice device;
		QOpenGLFunctions funcs;
	};
public:
	Engine(QSize size)
		:vs(QOpenGLShader::Vertex), frame(size, con.Format()), vlay(QOpenGLBuffer::VertexBuffer)
	{
		con.funcs.glViewport(0, 0, size.width(), size.height());
		frame.bind();

		vs.compileSourceCode(vertexShaderSource);
		shaders.addShader(&vs);

		vbuf.create();
		vbuf.bind();
	}
	~Engine()
	{
		Empty().destroy();
	}
public:
	QImage Render(QOpenGLShader& ps, std::span<std::shared_ptr<QOpenGLTexture>> outputs, ver::dc::Buffer* buffer = nullptr)
	{
		shaders.addShader(&ps);
		shaders.link();
		shaders.bind();

		if (buffer)BindBuffer(*buffer);

		con.funcs.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		con.funcs.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		con.funcs.glDrawArrays(GL_TRIANGLES, 0, 3);
		shaders.removeShader(&ps);

		for (int i = 0; auto& o : outputs)
			o = std::make_shared<QOpenGLTexture>(frame.toImage(true, i++));

		return frame.toImage(true, 0);
	}
	auto& Functions()
	{
		return con.funcs;
	}
	QOpenGLTexture& Empty()
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
private:
	void BindBuffer(ver::dc::Buffer& buffer)
	{
		for (auto r : buffer)
		{
			switch (r.GetType())
			{
			case ver::dc::Type::Float:
				shaders.setUniformValue(int(r.GetSlot()), (const float&)r);
				break;
			case ver::dc::Type::Float2:
				shaders.setUniformValue(int(r.GetSlot()), (const QVector2D&)r);
				break;
			case ver::dc::Type::Float3:
				shaders.setUniformValue(int(r.GetSlot()), (const QVector3D&)r);
				break;
			case ver::dc::Type::Float4:
				shaders.setUniformValue(int(r.GetSlot()), (const QVector4D&)r);
				break;
			case ver::dc::Type::Matrix:
				shaders.setUniformValue(int(r.GetSlot()), (const QMatrix4x4&)r);
				break;
			case ver::dc::Type::Bool:
			case ver::dc::Type::Integer:
				shaders.setUniformValue(int(r.GetSlot()), (const int&)r);
				break;
			default:
				break;
			}
		}
	}
private:
	Context con;
	QOpenGLFramebufferObject frame;

	QOpenGLBuffer vlay;
	QOpenGLVertexArrayObject vbuf;

	QOpenGLShader vs;
	QOpenGLShaderProgram shaders;

	static constexpr const char* vertexShaderSource =
	 R"(#version 330 core
		out vec2 texcoords;
		void main() {
			vec2 vertices[3] = vec2[3](vec2(-1, -1), vec2(3, -1), vec2(-1, 3));
			gl_Position = vec4(vertices[gl_VertexID], 0, 1);
			texcoords = 0.5 * gl_Position.xy + vec2(0.5);
		})";
};