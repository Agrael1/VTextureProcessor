#pragma once
#include <optional>

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
public:
	QImage Render(QOpenGLShader& ps)
	{
		shaders.addShader(&ps);
		shaders.link();
		shaders.bind();

		con.funcs.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		con.funcs.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		con.funcs.glDrawArrays(GL_TRIANGLES, 0, 3);
		shaders.removeShader(&ps);

		return frame.toImage();
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
		void main() {
			vec2 vertices[3] = vec2[3](vec2(-1, -1), vec2(3, -1), vec2(-1, 3));
			gl_Position = vec4(vertices[gl_VertexID], 0, 1);
		})";
};