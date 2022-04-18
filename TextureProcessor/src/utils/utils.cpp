#include <utils/utils.h>

std::filesystem::path ver::generate(const std::filesystem::path& p, std::string_view separator)
{
	namespace fs = std::filesystem;
	std::filesystem::path rp{ p };
	auto f = rp.filename().replace_extension().string();
	auto x = rp.extension().string();
	f += separator;
	auto v = 0;

	// Increment while file exists
	while (fs::exists(rp))
		rp.replace_filename(f + std::to_string(v++) + x);

	return rp;
}

std::string ver::normalize_name(std::string in)
{
	std::ranges::replace(in, ' ', '_'); 
	auto [first, last] = std::ranges::remove_if(in, [](char c) {return !std::isalnum(c) && c != '_'; });
	in.erase(first, last);
	return in;
}
