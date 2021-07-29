#pragma once
#include <array>
#include <string>
#define BOOST_STATIC_STRING_STANDALONE
#include <boost/static_string/static_string.hpp>


namespace ver
{
	namespace sts = boost::static_strings;

	template<size_t N>
	struct StringLiteral {
		constexpr StringLiteral(const char(&str)[N]) {
			std::copy_n(str, N, value);
		}
		constexpr StringLiteral() = default;
		constexpr StringLiteral(sts::static_string<N> str) {
			std::copy(str.begin(), str.begin() + N + 1, value);
		}
		constexpr auto size()const noexcept
		{
			return N;
		}
		constexpr auto length()const noexcept
		{
			return N;
		}
		char value[N + 1]{};
	};

	template<size_t N>
	consteval auto _tl(const char(&str)[N])
	{
		return sts::static_string<N>(str);
	}

	constexpr auto config_ext = _tl(".vtxc");
	constexpr auto proj_ext = _tl(".vtxproj");

	template<StringLiteral...args>
	consteval auto _ncat()
	{
		constexpr auto l = ((args.length()) + ...);
		sts::static_string<l> a;
		(a.append(args.value),...);
		return a;
	}

	template<StringLiteral ext>
	consteval auto filter_p()
	{
		return _ncat<"(*",ext,");;">();
	}
	template<StringLiteral f>
	consteval auto config()
	{
		return _ncat<f, config_ext>();
	}
	template<StringLiteral f>
	consteval auto project()
	{
		return _ncat<f, proj_ext>();
	}

	template<std::integral auto f>
	consteval size_t IntLength()
	{
		size_t l = 0, n = f;
		if (f == 0) return size_t(1);
		if (f < 0) { l++; n = -f; };

		while (n > 0)
		{
			l++;
			n = n / 10;
		}
		return l;
	}

	template<std::integral auto value>
	consteval auto int2str()
	{
		auto xvalue = value;
		constexpr auto l = IntLength<value>();
		StringLiteral<l> str;
		
		if (value == 0)
		{
			str.value[0] = '0';
			return sts::static_string<l>(str.value);
		}
		if (value < 0)
		{
			str.value[0] = '-';
			xvalue = -xvalue;
		}
		for (auto end = int(str.length() - 1); end >= 0; end--, xvalue /= 10)
			str.value[end] = "0123456789"[xvalue % 10];
		return sts::static_string<l>(str.value);
	}
	
	constexpr auto proj_filter = filter_p<proj_ext>();
}