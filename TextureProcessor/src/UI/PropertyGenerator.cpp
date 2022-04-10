#include <UI/PropertyGenerator.h>
#include <UI/Sliders.h>
#include <Logic/DynamicConstant.h>
#include <Windows/Properties.h>
#include <QLabel>

using ver::dc::Options;




void UI::PropertyBuffer(Windows::PropertyElement& elem, ver::dc::Buffer& buf, std::span<ver::dc::Options> params)
{
	using enum ver::dc::Type;
	auto it = params.begin();
	Options* opt = nullptr;

	for (auto&& ref : buf)
	{
		opt = std::addressof(*it);
		auto t = ref.GetType();
		QString name = opt->enable_alias ? opt->alias.c_str() : ref.GetName().data();

		switch (t)
		{
		case ver::dc::Type::Float:
		{
			elem.AppendWidget<QLabel>(name);
			auto& s = elem.AppendWidget<FloatSlider>((float&)ref);
			auto& par = opt->param.get<Float>();
			if (opt->enable_min) s.SetMin(par.min);
			if (opt->enable_max) s.SetMax(par.max);
			s.Revalidate();
			break;
		}
		case ver::dc::Type::Float2:
		{
			elem.AppendWidget<QLabel>(name);
			auto& s = elem.AppendWidget<Vec2Slider>((QVector2D&)ref);
			auto& par = opt->param.get<Float2>();
			if (opt->enable_min) s.SetMin(par.min);
			if (opt->enable_max) s.SetMax(par.max);
			break;
		}
		case ver::dc::Type::Float3:
			break;
		case ver::dc::Type::Float4:
			break;
		case ver::dc::Type::Matrix:
			break;
		case ver::dc::Type::Bool:
			elem.AppendWidget<CheckBox>((bool&)ref, name);
			break;
		case ver::dc::Type::Integer:
		{
			elem.AppendWidget<QLabel>(name);
			auto& s = elem.AppendWidget<IntSlider>((int&)ref);
			auto& par = opt->param.get<Integer>();
			if (opt->enable_min) s.SetMin(par.min);
			if (opt->enable_max) s.SetMax(par.max);
			s.Revalidate();
		}
			break;
		default:
			break;
		}
		if (opt) { it++; opt = nullptr; }
	}
}
