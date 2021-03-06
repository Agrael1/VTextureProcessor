/**
 * @file Engine.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief Engine base model for shader compilation
 */
#pragma once
#include <Logic/DynamicConstant.h>
#include <QOpenGLFramebufferObject>
#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QOffscreenSurface>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class QOpenGLTexture;
namespace UI
{
	class FlowScene;
}


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
	Engine();
public:
	static Engine& Instance()noexcept{
		if (!Engine::e)Engine::e.emplace();
		return *Engine::e;
	}
	static void Destroy()noexcept{
		Engine::e.reset();
	}
	~Engine();
public:
	static void BindScene(UI::FlowScene* scene, QSize size);
	static void UnbindScene(UI::FlowScene* scene);
	static void SwitchScene(UI::FlowScene* scene);

	void Render(QOpenGLShader& ps, std::span<std::shared_ptr<QImage>> inputs, std::span<std::shared_ptr<QImage>> outputs, ver::dc::Buffer& buffer);
	QOpenGLFunctions& Functions() { return con.funcs; }
	QOpenGLTexture& Empty();
private:
	void BindBuffer(ver::dc::Buffer& buffer);
	void Current();
private:
	Context con;
	std::unordered_map<UI::FlowScene*, QOpenGLFramebufferObject> frames;
	QOpenGLFramebufferObject* current;

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
	static std::optional<Engine> e;
};