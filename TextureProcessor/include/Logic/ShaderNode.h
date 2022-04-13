/**
 * @file ShaderNode.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief ShaderNode model logic
 */
#pragma once
#include <Logic/Node.h>
#include <Logic/DynamicConstant.h>
#include <Logic/PropertyView.h>
#include <Logic/Descriptor.h>
#include <Logic/PortType.h>
#include <UI/NodeStyle.h>
#include <QOpenGLShader>

class Engine;

namespace UI::Windows {
	class PropertyElement;
}

namespace ver
{
	class TextureDescriptor : public Descriptor
	{
	public:
		TextureDescriptor() = default;
		TextureDescriptor(QJsonObject document);
	public:
		virtual std::unique_ptr<Node> MakeModel() override;
		void Assemble();
		bool CompileShader(const QString& xshader) {
			return shader.compileSourceCode(xshader);
		}
		const QString& StyleName()const noexcept { return style.StyleName(); }
		virtual bool valid()const noexcept;
	private:
		void SetOptions(QJsonObject obj, dc::Options& opt);
		void SetProperties(const QJsonArray& props);
	public:
		QOpenGLShader shader{ QOpenGLShader::Fragment };
		UI::NodeStyle style;
		QString shader_body;
		std::vector<PortDesc> sinks;
		std::vector<PortDesc> sources;
		ver::dc::Layout buffer;
		std::vector<dc::Options> params;
		mutable QString last_error;
	};

	class ShaderNode : public Node
	{
	public:
		ShaderNode(TextureDescriptor& td);
	public:
		dc::Buffer& GetBuffer() { return buf; }
		std::span<std::shared_ptr<QImage>> GetLayout()noexcept
		{
			return outputs;
		}
		void Update()override;
		
		void GetProperties(UI::Windows::PropertyElement& props);

		virtual void Serialize(QJsonObject& doc)override;
		virtual bool Deserialize(QJsonObject in)override;
		virtual void ExportSilent(std::string_view name)override;
		virtual std::string Export()override;
		virtual UI::NodeStyle& GetStyle()override
		{
			return desc.style;
		}
		void Accept(ver::ShaderProbe& probe)override;
	protected:
		std::vector<std::shared_ptr<QImage>> inputs;
		std::vector<std::shared_ptr<QImage>> outputs;
		TextureDescriptor& desc;

		dc::Buffer buf;
	};
}

