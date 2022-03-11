#pragma once
#include <utils/const_utils.h>

namespace ver
{
	constexpr auto config_ext = _tl(".vtxc");
	constexpr auto proj_ext = _tl(".vtxproj");
	constexpr auto node_ext = _tl(".json");
	
	template<ver::string_literal ext>
	consteval auto filter_p()
	{
		return _ncat<"(*", ext, ");;">();
	}
	template<ver::string_literal f>
	consteval auto config()
	{
		return _ncat<f, config_ext>();
	}
	template<ver::string_literal f>
	consteval auto project()
	{
		return _ncat<f, proj_ext>();
	}

	constexpr auto proj_filter = filter_p<proj_ext>();
	constexpr auto node_filter = filter_p<node_ext>();
}