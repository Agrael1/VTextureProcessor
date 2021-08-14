#define USE_TYPES
#include <UI/PropertyGenerator.h>
#include <UI/Sliders.h>
#include <Logic/Node.h>
#include <Logic/DynamicConstant.h>
#include <Windows/REProperties.h>

using namespace UI;

static void PropertyBuffer(Windows::XPropertyElement& elem, const char* name, ver::dc::Buffer& buf)
{
	for (auto ref : buf)
	{
		auto t = ref.GetType();
		switch (t)
		{
		case ver::dc::Type::Float:
			elem.AppendWidget<QLabel>(ref.GetName().data());
			elem.AppendWidget<FloatSlider>((float&)ref);
			break;
		case ver::dc::Type::Float2:
			elem.AppendWidget<QLabel>(ref.GetName().data());
			elem.AppendWidget<Vec2Slider>((QVector2D&)ref);
			break;
		case ver::dc::Type::Float3:
			break;
		case ver::dc::Type::Float4:
			break;
		case ver::dc::Type::Matrix:
			break;
		case ver::dc::Type::Bool:
			elem.AppendWidget<CheckBox>((bool&)ref, ref.GetName().data());
			break;
		case ver::dc::Type::Integer:
			elem.AppendWidget<QLabel>(ref.GetName().data());
			elem.AppendWidget<IntSlider>((int&)ref);
			break;
		default:
			break;
		}
	}
}
static void PropertyBoolean(Windows::XPropertyElement& elem, const char* name, bool& buf)
{
	elem.AppendWidget<CheckBox>(buf, name);
}

void UI::AppendProperties(Windows::XProperties& window, ver::Node& node, IXNode& entity)
{
	Windows::XPropertyElement elem{ entity, node.GetName() };
	auto props = node.GetProperties();
	for (const auto& x : props.Get())
	{
		switch (x.first.first)
		{
#define X(a) case ver::DescType::a: Property##a(elem, x.first.second, *std::get<ver::DescMap<ver::DescType::a>::tied_t*>(x.second));break;
			XTYPES
#undef X
		default:
			break;
		}
	}

	window.AppendProperty(std::move(elem));
}
