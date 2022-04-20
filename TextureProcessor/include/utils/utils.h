#include <filesystem>
#include <QString>

namespace ver
{
	std::filesystem::path generate(const std::filesystem::path& p, std::string_view separator);
	std::string normalize_name(std::string in);
	std::wstring normalize_name(std::wstring in);

	const QString MakeConstString(const char16_t* line, size_t length);
}
