#pragma once
#include <utils/string_literal.h>
#define BOOST_STATIC_STRING_STANDALONE
#include <boost/static_string/static_string.hpp>


namespace ver
{
	namespace sts = boost::static_strings;

	template<size_t N>
	consteval auto _tl(const char(&str)[N])
	{
		return ver::string_literal<N>(str);
	}

	template<ver::string_literal...args>
	consteval auto _ncat()
	{
		constexpr auto l = ((args.length()) + ...);
		sts::static_string<l> a;
		(a.append(args.c_str()), ...);
		return a;
	}


	template<std::integral T>
	consteval size_t IntLength(T f)
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
	constexpr auto int2str()
	{
		constexpr auto l = IntLength(value);
		auto xvalue = value;
		ver::string_literal<l> str;

		if (value == 0)
		{
			str[0] = '0';
			return sts::static_string<l>(str.c_str());
		}
		if (value < 0)
		{
			str[0] = '-';
			xvalue = -xvalue;
		}
		for (auto end = int(str.length() - 1); end >= 0; end--, xvalue /= 10)
			str[end] = "0123456789"[xvalue % 10];
		return sts::static_string<l>(str.c_str());
	}


	consteval auto rgb_to_hex(uint8_t r, uint8_t g, uint8_t b)
	{
		ver::string_literal<7> str;
		str[0] = '#';
		for (auto end = 2; end > 0; end--, r >>= 4)
			str[end] = "0123456789abcdef"[r & 0x0f];
		for (auto end = 4; end > 2; end--, g >>= 4)
			str[end] = "0123456789abcdef"[g & 0x0f];
		for (auto end = 6; end > 4; end--, b >>= 4)
			str[end] = "0123456789abcdef"[b & 0x0f];
		return str;
	}
}