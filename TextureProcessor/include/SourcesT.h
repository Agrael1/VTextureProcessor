#pragma once
#include <Source.h>

template <class T, Source::Type ty = Source::Type::Grayscale>
class DirectTextureSource : public Source
{
public:
	DirectTextureSource(std::string_view name, std::string_view shader_value, const T& buffer)
		:Source(name, ty),
		shader_value(shader_value),
		buffer(buffer)
	{}
	static auto Make(std::string_view name, std::string_view shader_value, const T& buffer)
	{
		return pv::make_polymorphic_value<DirectTextureSource>(name, shader_value, buffer);
	}
	std::string_view YieldShader()override
	{
		return shader_value;
	}
private:
	const T& buffer;
	std::string_view shader_value;
};

