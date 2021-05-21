/**
 * @file Engine.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief Engine base model for shader compilation
 */
#pragma once
#include <Logic/DynamicConstant.h>

class Engine
{
	struct Context
	{
		Context();
		QOpenGLFramebufferObjectFormat Format()
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
	Engine(QSize size);
	~Engine() { Current(); Empty().destroy(); }
public:
	QImage Render(QOpenGLShader& ps, std::span<std::shared_ptr<QOpenGLTexture>> inputs, bool tile, std::span<std::shared_ptr<QOpenGLTexture>> outputs, ver::dc::Buffer& buffer);
	QOpenGLFunctions& Functions() { return con.funcs; }
	static QOpenGLTexture& Empty();
private:
	void BindBuffer(ver::dc::Buffer& buffer);
	void Current();
private:
	Context con;
	QOpenGLFramebufferObject frame;

	QOpenGLBuffer vlay;
	QOpenGLVertexArrayObject vbuf;

	QOpenGLShader vs;
	QOpenGLShaderProgram shaders;

	static constexpr const char* vertexShaderSource =
		R"(#version 330 core
		out vec2 sv_texc;
		void main() {
			vec2 vertices[3] = vec2[3](vec2(-1, -1), vec2(3, -1), vec2(-1, 3));
			gl_Position = vec4(vertices[gl_VertexID], 0, 1);
			sv_texc = 0.5 * gl_Position.xy + vec2(0.5);
		})";
};