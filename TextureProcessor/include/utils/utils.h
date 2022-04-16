#include <filesystem>
#include <ranges>
#include <cwctype>

namespace ver
{
	std::filesystem::path generate(const std::filesystem::path& p, std::string_view separator = "");
	constexpr bool iequals(std::wstring_view lhs, std::wstring_view rhs) {
		auto to_lower{ std::ranges::views::transform(std::towlower) };
		return std::ranges::equal(lhs | to_lower, rhs | to_lower);
	}
}
