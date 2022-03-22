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
		TextureDescriptor(QJsonObject document, std::string_view styleName);
	public:
		virtual std::unique_ptr<Node> MakeModel() override;
		bool CompileShader(QString xshader) {
			if (shader.compileSourceCode(xshader))
			{
				shadercode = xshader.toStdString();
				return true;
			}
			return false;
		}
		virtual bool valid()const noexcept{
			return shader.isCompiled();
		}
	private:
		void SetProperties(const QJsonArray& props, QString& scode);
	public:
		QOpenGLShader shader{ QOpenGLShader::Fragment };
		UI::NodeStyle style;
		std::string shadercode;
		std::vector<PortDesc> sinks;
		std::vector<PortDesc> sources;
		ver::dc::Layout buffer;
		std::vector<std::pair<size_t, dc::param_storage>> params;
	};

	class ShaderNode : public Node
	{
		static constexpr auto xdesc = MakeDesc({
			{DescType::Buffer, "Buffer"},
			{DescType::Boolean, "Tiling"},
			{DescType::BooleanUpd, "Buffer"}
			});
	public:
		ShaderNode(TextureDescriptor& td);
	public:
		dc::Buffer& GetBuffer() { return buf; }
		bool& Tiling() { return tiling; }
		std::span<std::shared_ptr<QImage>> GetLayout()noexcept
		{
			return outputs;
		}
		void Update()override;
		PropertyView GetProperties()override
		{
			PropertyView pv;
			if (buffer)
				pv.TieProp<xdesc[0]>(buf);
			pv.TieProp<xdesc[1]>(tiling);
			if (buf)
				pv.TieProp<xdesc[2]>(buffer);
			return pv;
		}
		virtual QJsonObject Serialize()override;
		virtual void Deserialize(QJsonObject in)override;
		virtual void ExportSilent(std::string_view name)override;
		virtual std::string Export()override;
		virtual UI::NodeStyle& GetStyle()override
		{
			return desc.style;
		}
	private:
		void SetProperties(const QJsonArray& props, QString& scode);
	protected:
		std::vector<std::shared_ptr<QImage>> inputs;
		std::vector<std::shared_ptr<QImage>> outputs;
		TextureDescriptor& desc;

		dc::Buffer buf;
		bool tiling = false;
		bool buffer = true;
	};
}

