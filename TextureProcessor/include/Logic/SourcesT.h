/**
 * @file SourcesT.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief Templates for custom sources
 * Modified from Veritas engine WinD3D project (file Pass.*)
 * https://github.com/Agrael1/VeritasD3D
 */
#pragma once
#include <Logic/Source.h>

template <PortType ty = PortType::Grayscale>
class DirectTextureSource : public Source
{
public:
	DirectTextureSource(std::string_view name, std::string_view shader_value, std::shared_ptr<QOpenGLTexture>& buffer)
		:Source(name, ty),
		shader_value(shader_value),
		buffer(buffer)
	{}
	static auto Make(std::string_view name, std::string_view shader_value, std::shared_ptr<QOpenGLTexture>& buffer)
	{
		return std::make_unique<DirectTextureSource>(name, shader_value, buffer);
	}
	std::string_view YieldShader()override
	{
		return shader_value;
	}
	std::shared_ptr<QOpenGLTexture> YieldTexture()
	{
		return buffer;
	}
private:
	std::shared_ptr<QOpenGLTexture>& buffer;
	std::string_view shader_value;
};

using GrayscaleSource = DirectTextureSource<PortType::Grayscale>;
