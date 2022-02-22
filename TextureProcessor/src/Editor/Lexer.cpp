#include <Editor/Lexer.h>
#include <ranges>
#include <optional>
#include <format>


constexpr std::array<std::string_view, 16> statements{
	"break",
	"case",
	"continue",
	"default",
	"discard",
	"do",
	"else",
	"for",
	"if",
	"in",
	"inout",
	"layout",
	"out",
	"return",
	"switch",
	"while"
};
constexpr std::array<std::string_view, 92> kwords{
	"attribute",
	"bool",
	"bvec2",
	"bvec3",
	"bvec4",
	"const",
	"dmat2",
	"dmat2x2",
	"dmat2x3",
	"dmat2x4",
	"dmat3",
	"dmat3x2",
	"dmat3x3",
	"dmat3x4",
	"dmat4",
	"dmat4x2",
	"dmat4x3",
	"dmat4x4",
	"double",
	"dvec2",
	"dvec3",
	"dvec4",
	"false",
	"float",
	"int",
	"isampler1D",
	"isampler1DArray",
	"isampler2D",
	"isampler2DArray",
	"isampler2DMS",
	"isampler2DMSArray",
	"isampler2DRect",
	"isampler3D",
	"isamplerBuffer",
	"isamplerCube",
	"isamplerCubeArray",
	"ivec2",
	"ivec3",
	"ivec4",
	"mat2",
	"mat2x2",
	"mat2x3",
	"mat2x4",
	"mat3",
	"mat3x2",
	"mat3x3",
	"mat3x4",
	"mat4",
	"mat4x2",
	"mat4x3",
	"mat4x4",
	"sampler1D",
	"sampler1DArray",
	"sampler1DArrayShadow",
	"sampler1DShadow",
	"sampler2D",
	"sampler2DArray",
	"sampler2DArrayShadow",
	"sampler2DMS",
	"sampler2DMSArray",
	"sampler2DRect",
	"sampler2DRectShadow",
	"sampler2DShadow",
	"sampler3D",
	"samplerBuffer",
	"samplerCube",
	"samplerCubeArray",
	"samplerCubeArrayShadow",
	"samplerCubeShadow",
	"struct",
	"uint",
	"true",
	"uniform",
	"usampler1D",
	"usampler1DArray",
	"usampler2D",
	"usampler2DArray",
	"usampler2DMS",
	"usampler2DMSArray",
	"usampler2DRect",
	"usampler3D",
	"usamplerBuffer",
	"usamplerCube",
	"usamplerCubeArray",
	"uvec2",
	"uvec3",
	"uvec4",
	"varying",
	"vec2",
	"vec3",
	"vec4",
	"void",
};


consteval auto statements_range_i2()
{
	std::vector<char> a{};
	size_t i = 0;
	char c = 0;

	for (auto x : statements) {
		char r = x[0];
		if (c != r)
			a.push_back(r);
		c = r;
	}
	return a;
}
consteval auto statements_range()
{
	std::array<char, statements_range_i2().size()> vsz{};
	auto v = statements_range_i2();
	size_t a = 0;
	for (auto& i : vsz)
		i = v[a++];
	return vsz;
}
consteval auto kwords_range_i2()
{
	std::vector<char> a{};
	size_t i = 0;
	char c = 0;

	for (auto x : kwords) {
		char r = x[0];
		if (c != r)
			a.push_back(r);
		c = r;
	}
	return a;
}
consteval auto kwords_range()
{
	std::array<char, kwords_range_i2().size()> vsz{};
	auto v = kwords_range_i2();
	size_t a = 0;
	for (auto& i : vsz)
		i = v[a++];
	return vsz;
}

constexpr auto sr = statements_range();
constexpr auto kr = kwords_range();




class LexContext
{
public:
	LexContext(std::string_view code)
		:code(code) {}
public:
	char fetch_one()noexcept
	{
		char x = prefetch_one();
		if (!x)return 0;
		advance_one();
		if (x == '\n') { line++; column = 0; }
		return x;
	}
	std::optional<token> try_get_statement(char c)
	{
		if (std::ranges::find(sr, c) == sr.end())return{};
		switch (c)
		{
		case 'b':
			return stmt_from_subset(0, 1);
		case 'c':
			if (prefetch_one() == 'a')
				return stmt_from_subset(1, 2);
			if (prefetch_one() == 'o')
				return stmt_from_subset(2, 2);
			break;
		case 'd':
			if (prefetch_one() == 'o')
				return create_advance(statements[5], token::type::statement);
			if (prefetch_one() == 'e')
				return stmt_from_subset(3, 2);
			if (prefetch_one() == 'i')
				return stmt_from_subset(4, 2);
			break;
		case 'e':
			return stmt_from_subset(6, 1);
		case 'f':
			return stmt_from_subset(7, 1);
		case 'i':
			if (prefetch_one() == 'f' && term(2))
				return create_advance(statements[8], token::type::statement);
			if (prefetch_one() == 'n')
			{
				if (auto st = create_advance(statements[9], token::type::statement))
					return st;
				return stmt_from_subset(10, 2);
			}
			break;
		case 'l':
			return stmt_from_subset(11, 1);
		case 'o':
			return stmt_from_subset(12, 1);
		case 'r':
			return stmt_from_subset(13, 1);
		case 's':
			return stmt_from_subset(14, 1);
		case 'w':
			return stmt_from_subset(15, 1);
		default:
			break;
		}
		return{};
	}
	std::optional<token> try_get_kw(char c)
	{
		if (std::ranges::find(kr, c) == kr.end())return{};
		switch (c)
		{
		case 'a':
			return kw_from_subset(0, 1);
		case 'b':
			if (prefetch_one() == 'o')
				return kw_from_subset(1, 2);
			if (code.substr(0, 3) != kwords[3].substr(1, 3)||!(c = prefetch_n(3)))
				return {};
			if (c > '1' && c < '5')
				return create_advance(kwords[c - '1' + 2], token::type::keyword);
		case 'c':
			return kw_from_subset(6, 1);
		case 'd':
			if (prefetch_one() == 'o')
				return kw_from_subset(18, 2);
		default:
			break;
		}

		return {};
	}
	std::optional<token> try_get_ident(char c)
	{
		size_t roffset = offset - 1;
		const char* av = code.data() - 1;
		while (!not_lit(prefetch_one()))fetch_one();
		return token{ .xtype = token::type::identifier, .value = {av, offset - roffset},.offset = roffset };
	}
	bool empty()const noexcept {
		return code.empty() || !code[0];
	}
	static bool not_lit(char c) {
		return !isalnum(c) && c != '_';
	}
private:
	char prefetch_one()const noexcept
	{
		if (empty())return 0;
		return code[0];
	}
	char prefetch_n(size_t n)const noexcept
	{
		if (empty() || code.length() <= n)return 0;
		return code[n];
	}
	bool term(size_t length)const noexcept {
		return code.length() <= (length) || not_lit(code[length]);
	}

	std::optional<token> stmt_from_subset(size_t stateN, size_t xoffset) {
		return from_subset(xoffset, statements[stateN], token::type::statement);
	}
	std::optional<token> kw_from_subset(size_t stateN, size_t xoffset) {
		return from_subset(xoffset, kwords[stateN], token::type::keyword);
	}
	std::optional<token> from_subset(size_t xoffset, std::string_view xst, token::type tt)
	{
		auto cmpw = xst.substr(xoffset);
		if (cmpw == code.substr(xoffset - 1, cmpw.length()) && term(xst.length() - 1))
		{
			auto r = offset - 1;
			advance(xst);
			return token{ tt, xst, r };
		}
		return{};
	}
	std::optional<token> create_advance(std::string_view sw, token::type tt)
	{
		if (term(sw.length() - 1))
		{
			auto r = offset - 1;
			advance(sw);
			return token{ tt, sw, r };
		}
		return {};
	}

	void advance(std::string_view sw)
	{
		auto l = sw.length() - 1; //without 1 byte
		offset += l;
		line += l;
		code = code.substr(l);
	}
	void advance_one()
	{
		offset++;
		line++;
		code = code.substr(1);
	}

private:
	size_t line = 0;
	size_t column = 0;
	size_t offset = 0;

	std::string_view code;
};



ver::generator<token> GetToken(std::string_view code)
{
	LexContext lex{ code };

	while (!lex.empty())
	{
		char c = lex.fetch_one();
		if (isdigit(c))
			continue;
		if (LexContext::not_lit(c))
			continue;
		if (auto st = lex.try_get_statement(c); st)
		{
			co_yield *st;
			continue;
		}
		if (auto st = lex.try_get_kw(c); st)
		{
			co_yield *st;
			continue;
		}
		if (auto st = lex.try_get_ident(c); st)
		{
			co_yield *st;
			continue;
		}
	}
}


std::string token::to_string() const noexcept
{
	return std::format("token {}\nvalue: {}\noffset: {}\n", type_s[size_t(xtype)], value, offset);
}
