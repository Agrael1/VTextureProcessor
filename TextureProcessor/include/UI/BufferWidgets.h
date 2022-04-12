#pragma once
#include <Logic/DynamicConstant.h>
#include <UI/Sliders.h>


namespace UI
{
	template<ver::dc::Type t>
	class BufferWidget : public AwareWidget
	{
		using map_t = ver::dc::Map<t>;
		using convref_t = map_t::SysType&;
		static constexpr const char* names[] = { "x","y","z","w" };
	public:
		BufferWidget(ver::dc::ElementRef ref, ver::dc::Options& opt)
			:name(opt.enable_alias ? opt.alias.c_str() : ref.GetName().data())
		{
			vl.setContentsMargins(0, 0, 0, 0);
			vl.addWidget(&name);

			auto min = std::span<float>{ (float*)std::addressof(opt.param.get<t>().min) , map_t::floats};
			auto max = std::span<float>{ (float*)std::addressof(opt.param.get<t>().max), map_t::floats };
			auto xref = std::span<float>{ (float*)std::addressof((convref_t)ref), map_t::floats };

			auto itmin = min.begin();
			auto itmax = max.begin();
			size_t xsize = 0;

			for (auto& i : xref)
			{
				auto w = new FloatSlider(i, names[xsize++]);
				if (opt.enable_min)w->SetMin(*itmin++);
				if (opt.enable_max)w->SetMax(*itmax++);
				connect(w, &FloatSlider::ValueChanged, this, &AwareWidget::ValueChanged);
				vl.addWidget(w);
			}
			setLayout(&vl);
		}
	private:
		QVBoxLayout vl;
		QLabel name;
	};
	template<>
	class BufferWidget<ver::dc::Type::Float> : public FloatSlider
	{
		using conv_t = ver::dc::Map<ver::dc::Type::Float>::SysType&;
	public:
		BufferWidget(ver::dc::ElementRef ref, ver::dc::Options& opt)
			:FloatSlider((conv_t)ref, opt.enable_alias ? opt.alias.c_str() : ref.GetName().data())
		{
			auto& x = opt.param.get<ver::dc::Type::Float>();
			if (opt.enable_min)SetMin(x.min);
			if (opt.enable_max)SetMax(x.max);
		}
	};
	template<>
	class BufferWidget<ver::dc::Type::Integer> : public IntSlider
	{
		using conv_t = ver::dc::Map<ver::dc::Type::Integer>::SysType&;
	public:
		BufferWidget(ver::dc::ElementRef ref, ver::dc::Options& opt)
			:IntSlider((conv_t)ref, opt.enable_alias ? opt.alias.c_str() : ref.GetName().data())
		{
			auto& x = opt.param.get<ver::dc::Type::Integer>();
			if (opt.enable_min)SetMin(x.min);
			if (opt.enable_max)SetMax(x.max);
		}
	};
	template<>
	class BufferWidget<ver::dc::Type::Matrix> : public UI::Matrix
	{
		using conv_t = ver::dc::Map<ver::dc::Type::Matrix>::SysType&;
	public:
		BufferWidget(ver::dc::ElementRef ref, ver::dc::Options& opt)
			:Matrix((conv_t)ref, opt.enable_alias ? opt.alias.c_str() : ref.GetName().data())
		{}
	};
	template<>
	class BufferWidget<ver::dc::Type::Bool> : public UI::CheckBox
	{
		using conv_t = ver::dc::Map<ver::dc::Type::Bool>::SysType&;
	public:
		BufferWidget(ver::dc::ElementRef ref, ver::dc::Options& opt)
			:CheckBox((conv_t)ref, opt.enable_alias ? opt.alias.c_str() : ref.GetName().data())
		{}
	};
}
