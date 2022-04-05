/**
 * @file SourcesT.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief Templates for custom sources
 * Modified from Veritas engine WinD3D project (file Pass.*)
 * https://github.com/Agrael1/VeritasD3D
 */
#pragma once
#include <Logic/Source.h>

namespace ver
{
	class DirectTextureSource : public Source
	{
	public:
		DirectTextureSource(std::string_view name, std::shared_ptr<QImage>& buffer, ver::PortType ty)
			:Source(name, ty),
			buffer(buffer)
		{}
		static auto Make(std::string_view name, std::shared_ptr<QImage>& buffer, ver::PortType ty)
		{
			return std::make_unique<DirectTextureSource>(name, buffer, ty);
		}
		std::shared_ptr<QImage> YieldTexture()override
		{
			return buffer;
		}
	private:
		std::shared_ptr<QImage>& buffer;
	};

	template<PortType ty>
	struct _DTSO_T : DirectTextureSource
	{
		_DTSO_T(std::string_view name, std::shared_ptr<QImage>& buffer)
			:DirectTextureSource(name, buffer, ty) {}
		static std::unique_ptr<Source> Make(std::string_view name,  std::shared_ptr<QImage>& buffer)
		{
			return std::make_unique<DirectTextureSource>(name, buffer, ty);
		}
	};
	using GrayscaleSource = _DTSO_T<PortType::Grayscale>;
}

