#pragma once

#define STATEMENT_ENUM()\
	X(break)\
	X(case)\
	X(continue)\
	X(default)\
	X(discard)\
	X(do)\
	X(else)\
	X(for)\
	X(if)\
	X(in)\
	X(inout)\
	X(layout)\
	X(out)\
	X(return)\
	X(switch)\
	X(while)

#define KEYWORD_ENUM()\
	X(attribute)\
	X(bool)\
	X(bvec2)\
	X(bvec3)\
	X(bvec4)\
	X(const)\
	X(dmat2)\
	X(dmat2x2)\
	X(dmat2x3)\
	X(dmat2x4)\
	X(dmat3)\
	X(dmat3x2)\
	X(dmat3x3)\
	X(dmat3x4)\
	X(dmat4)\
	X(dmat4x2)\
	X(dmat4x3)\
	X(dmat4x4)\
	X(double)\
	X(dvec2)\
	X(dvec3)\
	X(dvec4)\
	X(false)\
	X(float)\
	X(int)\
	X(isampler1D)\
	X(isampler1DArray)\
	X(isampler2D)\
	X(isampler2DArray)\
	X(isampler2DMS)\
	X(isampler2DMSArray)\
	X(isampler2DRect)\
	X(isampler3D)\
	X(isamplerBuffer)\
	X(isamplerCube)\
	X(isamplerCubeArray)\
	X(ivec2)\
	X(ivec3)\
	X(ivec4)\
	X(mat2)\
	X(mat2x2)\
	X(mat2x3)\
	X(mat2x4)\
	X(mat3)\
	X(mat3x2)\
	X(mat3x3)\
	X(mat3x4)\
	X(mat4)\
	X(mat4x2)\
	X(mat4x3)\
	X(mat4x4)\
	X(sampler1D)\
	X(sampler1DArray)\
	X(sampler1DArrayShadow)\
	X(sampler1DShadow)\
	X(sampler2D)\
	X(sampler2DArray)\
	X(sampler2DArrayShadow)\
	X(sampler2DMS)\
	X(sampler2DMSArray)\
	X(sampler2DRect)\
	X(sampler2DRectShadow)\
	X(sampler2DShadow)\
	X(sampler3D)\
	X(samplerBuffer)\
	X(samplerCube)\
	X(samplerCubeArray)\
	X(samplerCubeArrayShadow)\
	X(samplerCubeShadow)\
	X(struct)\
	X(true)\
	X(uint)\
	X(uniform)\
	X(usampler1D)\
	X(usampler1DArray)\
	X(usampler2D)\
	X(usampler2DArray)\
	X(usampler2DMS)\
	X(usampler2DMSArray)\
	X(usampler2DRect)\
	X(usampler3D)\
	X(usamplerBuffer)\
	X(usamplerCube)\
	X(usamplerCubeArray)\
	X(uvec2)\
	X(uvec3)\
	X(uvec4)\
	X(varying)\
	X(vec2)\
	X(vec3)\
	X(vec4)\
	X(void)

enum e_stmt
{
#define X(a) s_##a,
	STATEMENT_ENUM()
#undef X
};
enum e_kwd
{
#define X(a) k_##a,
	KEYWORD_ENUM()
#undef X
};

constexpr std::array<std::wstring_view, 16> statements{
#define X(a) L#a,
	STATEMENT_ENUM()
#undef X
};
constexpr std::array<std::wstring_view, 92> kwords{
#define X(a) L#a,
	KEYWORD_ENUM()
#undef X
};


consteval auto statements_range_i2()
{
	std::vector<wchar_t> a{};
	size_t i = 0;
	wchar_t c = 0;

	for (auto x : statements) {
		wchar_t r = x[0];
		if (c != r)
			a.push_back(r);
		c = r;
	}
	return a;
}
consteval auto statements_range()
{
	std::array<wchar_t, statements_range_i2().size()> vsz{};
	auto v = statements_range_i2();
	size_t a = 0;
	for (auto& i : vsz)
		i = v[a++];
	return vsz;
}
consteval auto kwords_range_i2()
{
	std::vector<wchar_t> a{};
	size_t i = 0;
	wchar_t c = 0;

	for (auto x : kwords) {
		wchar_t r = x[0];
		if (c != r)
			a.push_back(r);
		c = r;
	}
	return a;
}
consteval auto kwords_range()
{
	std::array<wchar_t, kwords_range_i2().size()> vsz{};
	auto v = kwords_range_i2();
	size_t a = 0;
	for (auto& i : vsz)
		i = v[a++];
	return vsz;
}

#undef STATEMENT_ENUM
#undef KEYWORD_ENUM

