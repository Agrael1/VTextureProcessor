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
		void XUpdate();
		dc::Buffer& GetBuffer() { return buf; }
		bool& Tiling() { return tiling; }
		std::span<std::shared_ptr<QImage>> GetLayout()noexcept
		{
			return outputs;
		}
	private:
		void SetProperties(const QJsonArray& props, QString& scode);
	private:
		std::vector<std::shared_ptr<QImage>> inputs;
		std::vector<std::shared_ptr<QImage>> outputs;
		std::shared_ptr<NodePrivate> shader;
		dc::Buffer buf;
		Engine& e;
		bool tiling = false;
	};

	class OutputNode : public Node
	{
	public:
		OutputNode(QJsonObject document, Engine& e);
		OutputNode(const OutputNode& in);
	public:
		std::span<std::shared_ptr<QImage>> GetLayout()noexcept
		{
			return { &inout, 1 };
		}
	private:
		std::shared_ptr<QImage> inout;
	};
}

