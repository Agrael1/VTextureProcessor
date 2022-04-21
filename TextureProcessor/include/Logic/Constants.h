#pragma once
#include <utils/const_utils.h>

namespace ver
{
	constexpr u16string_literal config_ext = u".vtxc";
	constexpr u16string_literal proj_ext = u".vtxproj";
	constexpr u16string_literal node_ext = u".json";

	template<ver::u16string_literal ext>
	consteval auto filter_p()
	{
		return _u16ncat < ver::u16string_literal{ u"(*" }, ext, ver::u16string_literal{ u");;" } > ();
	}
	template<ver::u16string_literal f>
	consteval auto config()
	{
		return _u16ncat<f, config_ext>();
	}
	template<ver::u16string_literal f>
	consteval auto project()
	{
		return _u16ncat<f, proj_ext>();
	}

	constexpr auto proj_filter = filter_p<proj_ext>();
	constexpr auto node_filter = filter_p<node_ext>();

	static constexpr ver::u16string_literal app_name = u"Veritas Texture Editor";
}