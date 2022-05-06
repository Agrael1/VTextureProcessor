#pragma once
#include <string_view>
#include <array>

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
	X(property)\
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

enum class e_stmt
{
#define X(a) s_##a,
	STATEMENT_ENUM()
#undef X
	count
};
enum class e_kwd
{
#define X(a) k_##a,
	KEYWORD_ENUM()
#undef X
	count
};

constexpr std::array<std::wstring_view, size_t(e_stmt::count)> statements{
#define X(a) L#a,
	STATEMENT_ENUM()
#undef X
};
constexpr std::array<std::wstring_view, size_t(e_kwd::count)> kwords{
#define X(a) L#a,
	KEYWORD_ENUM()
#undef X
};


constexpr std::array<std::wstring_view, 76> builtin_funcs{
//trigonometry
L"acos",
L"acosh",
L"asin",
L"asinh",
L"atan",
L"atanh",
L"cos",
L"cosh",
L"degrees",
L"radians",
L"sin",
L"sinh",
L"tan",
L"tanh",
//Math
L"abs",
L"ceil",
L"clamp",
L"dFdx",
L"dFdy",
L"exp",
L"exp2",
L"floor",
L"fma",
L"fract",
L"fwidth",
L"inversesqrt",
L"isinf",
L"isnan",
L"log",
L"log2",
L"max",
L"min",
L"mix",
L"mod",
L"modf",
L"noise",
L"pow",
L"round",
L"roundEven",
L"sign",
L"smoothstep",
L"sqrt",
L"step",
L"trunc",
//Vector
L"cross",
L"distance",
L"dot",
L"equal",
L"faceforward",
L"length",
L"normalize",
L"notEqual",
L"reflect",
L"refract",
//Comparison
L"all",
L"any",
L"greaterThan",
L"greaterThanEqual",
L"lessThan",
L"lessThanEqual",
L"not",
//Matrix
L"determinant",
L"groupMemoryBarrier",
L"inverse",
L"matrixCompMult",
L"outerProduct",
L"transpose",
//integer
L"bitCount",
L"bitfieldExtract",
L"bitfieldInsert",
L"bitfieldReverse",
L"findLSB",
L"findMSB",
L"uaddCarry",
L"umulExtended",
L"usubBorrow"
};

#undef STATEMENT_ENUM
#undef KEYWORD_ENUM

