#include <Presenter/GLInterpret.h>

Engine::Engine(QSize size)
	:frame(size), vs(QOpenGLShader::Vertex)
{
	context.create();
	funcs.initializeOpenGLFunctions();
	surface.create();
	context.makeCurrent(&surface);
	funcs.glViewport(0, 0, size.width(), size.height());

	vs.compileSourceCode(vertexShaderSource);
	shaders.addShader(&vs);
}