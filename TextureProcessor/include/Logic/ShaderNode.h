/**
 * @file ShaderNode.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief ShaderNode model logic
 */
#pragma once
#include <Logic/Node.h>
#include <Logic/DynamicConstant.h>
#include <QOpenGLShader>

class Engine;

namespace ver
{
	class ShaderNode : public Node
	{
		//Shared value between same type of nodes
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
		dc::Buffer& GetBuffer() { return buf; }
		bool& Tiling() { return tiling; }
	private:
		void SetProperties(const QJsonArray& props, QString& scode);
	private:
		std::vector<std::shared_ptr<QOpenGLTexture>> inputs;
		std::vector<std::shared_ptr<QOpenGLTexture>> outputs;
		std::shared_ptr<NodePrivate> shader;
		dc::Buffer buf;
		Engine& e;
		bool tiling = false;
	};
}

