#pragma once
#include <optional>

class Engine
{
    struct Context
    {
        Context()
        {
            context.create();
            surface.create();
            context.makeCurrent(&surface);
            funcs.initializeOpenGLFunctions();
        }
        QOpenGLContext context;
        QOpenGLPaintDevice device;
        QOffscreenSurface surface;
        QOpenGLFunctions funcs;
    };
public:
	Engine(QSize size);
public:
	auto Render(std::string_view ps)
	{
        
	}
private:
    Context con;
    QOpenGLFramebufferObject frame;
    QOpenGLShaderProgram shaders;
    QOpenGLShader vs;

    static constexpr const char* vertexShaderSource =
     R"(#version 330 core
        out vec2 texcoords;
        void main() {
            vec2 vertices[3] = vec2[3](vec2(-1, -1), vec2(3, -1), vec2(-1, 3));
            gl_Position = vec4(vertices[gl_VertexID], 0, 1);
            texcoords = 0.5 * gl_Position.xy + vec2(0.5);
        })";
};