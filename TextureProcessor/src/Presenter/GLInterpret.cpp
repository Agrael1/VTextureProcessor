#include <Presenter/GLInterpret.h>

Engine::Engine(QSize size)
	:vs(QOpenGLShader::Vertex), frame(size)
{
	con.funcs.glViewport(0, 0, size.width(), size.height());
	vs.compileSourceCode(vertexShaderSource);
	shaders.addShader(&vs);
}