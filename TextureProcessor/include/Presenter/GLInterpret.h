#pragma once

class Engine
{
public:
	Engine(QSize size);
public:
	auto Render(std::string_view ps)
	{
        
	}
private:
	QOpenGLContext context;
	QOpenGLPaintDevice device;
	//QOpenGLFunctions funcs;
    QOffscreenSurface surface;
    QOpenGLFramebufferObject frame;
    QOpenGLShaderProgram shaders;
    QOpenGLShader vs;

    static constexpr const char* vertexShaderSource =
        "attribute highp vec4 posAttr;\n"
        "attribute lowp vec4 colAttr;\n"
        "varying lowp vec4 col;\n"
        "uniform highp mat4 matrix;\n"
        "void main() {\n"
        "   col = colAttr;\n"
        "   gl_Position = matrix * posAttr;\n"
        "}\n";
};