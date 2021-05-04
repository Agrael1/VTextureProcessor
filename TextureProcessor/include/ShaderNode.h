#pragma once
#include <Node.h>

class Engine;

namespace ver
{
	class ShaderNode : public Node
	{
		struct NodePrivate
		{
			NodePrivate(QString&& code);
			QOpenGLShader shader;
			std::string shadercode;
		};
	public:
		ShaderNode(QJsonObject document, Engine& e);
		ShaderNode(const ShaderNode& other);
	public:
		QImage Update();
	private:

	private:
		std::vector<std::shared_ptr<QOpenGLTexture>> inputs;
		std::vector<std::shared_ptr<QOpenGLTexture>> outputs;
		std::shared_ptr<NodePrivate> shader;
		Engine& e;
	};
}

