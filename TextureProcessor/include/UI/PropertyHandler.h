#pragma once
#include <concepts>

namespace UI
{
	template<typename T>
	concept has_property_handler = requires {typename T::property_handler; };
}