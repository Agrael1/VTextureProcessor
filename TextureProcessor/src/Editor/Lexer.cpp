#include <Editor/Lexer.h>
#include <ranges>
#include <optional>

constexpr std::array<std::string_view, 15> statements{
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
	"layout"
	"out",
	"return",
	"switch",
	"while"
};
constexpr std::array<std::string_view, 91> kwords{
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
	"struct""uint",
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




struct LexContext
{
	enum State
	{
		begin,
		strlit
	}state;
	size_t line = 0;
	size_t column = 0;
	size_t offset = 0;

	std::string_view code;

	char fetch_one()noexcept
	{
		char x = code[0];
		if (!x)return 0;
		offset++;
		column++;
		if (x == '\n') { line++; column = 0; }
		code = code.substr(1);
		return x;
	}
	std::optional<token> try_get_statement()
	{

	}
	std::optional<token> try_get_kw()
	{

	}
	std::optional<token> try_get_ident()
	{

	}
};



ver::generator<token> GetToken(std::string_view code)
{
	LexContext lex;


	while (true)
	{
		char c = lex.fetch_one();
		if (std::ranges::find(sr, c) != sr.end())
			if (auto st = lex.try_get_statement(); st)
			{
				co_yield *st;
				continue;
			}
		if (std::ranges::find(kr, c) != kr.end())
			if (auto st = lex.try_get_statement(); st)
			{
				co_yield *st;
				continue;
			}
		if (auto st = lex.try_get_ident(); st)
		{
			co_yield *st;
			continue;
		}

	}
}
