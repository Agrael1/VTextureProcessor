/**
 * @file SinksT.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief Templated sinks with custom logic 
 * Modified from Veritas engine WinD3D project (file SinksT.h)
 * https://github.com/Agrael1/VeritasD3D
 */
#pragma once
#include <Logic/Source.h>
#include <Logic/Sink.h>
#include <utils/Exception.h>


namespace ver
{
	class DirectTextureSink : public Sink
	{
	public:
		static std::unique_ptr<Sink> Make(std::string_view registeredName, std::shared_ptr<QImage>& target, PortType ty)
		{
			return std::make_unique<DirectTextureSink>(registeredName, target, ty);
		}
		void Unlink()override
		{
			target = nullptr;
		}
		bool Link(Source& source) override
		{
			auto p = source.YieldTexture();
			if (!p)
			{
				auto msg = std::format("Binding input[{}] to output [{}.{}]. Source has no texture in it.\n",
					GetRegisteredName(),
					GetOutputNodeName(),
					GetSourceName());
				printf(msg.c_str());
				return false;
			}
			if (source.GetType() != GetType())
			{
				printf("Types do not match\n");
				return false;
			}
			target = std::move(p);
			return true;
		}
		DirectTextureSink(std::string_view registeredName, std::shared_ptr<QImage>& target, PortType ty)
			:
			Sink(registeredName, ty),
			target(target)
		{}
	private:
		std::shared_ptr<QImage>& target;
	};

	template<PortType ty>
	struct _DTS_T : DirectTextureSink
	{
		_DTS_T(std::string_view registeredName, std::shared_ptr<QImage>& target)
			:DirectTextureSink(registeredName, target, ty){}
		static std::unique_ptr<Sink> Make(std::string_view registeredName, std::shared_ptr<QImage>& target)
		{
			return std::make_unique<DirectTextureSink>(registeredName, target, ty);
		}
	};
	using GrayscaleSink = _DTS_T<PortType::Grayscale>;
}


