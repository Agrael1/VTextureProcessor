#include <Presenter/GLInterpret.h>

Engine::Engine(QSize size)
	:vs(QOpenGLShader::Vertex)
{
	context.create();
	surface.create();
	context.makeCurrent(&surface);

	funcs.initializeOpenGLFunctions();
	funcs.glViewport(0, 0, size.width(), size.height());
	frame.emplace(size);


	vs.compileSourceCode(vertexShaderSource);
	shaders.addShader(&vs);
}