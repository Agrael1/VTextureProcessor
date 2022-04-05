#pragma once
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>

#define LEAF_ELEMENT_TYPES \
	X(Float)SEP()\
	X(Float2)SEP()\
	X(Float3)SEP()\
	X(Float4)SEP()\
	X(Matrix)SEP()\
	X(Bool)SEP()\
	X(Integer)
#define SEP()

namespace ver::dc
{
	enum class Type
	{
		Empty,
#define X(el) el,
		LEAF_ELEMENT_TYPES
#undef X
	};

	constexpr const char* type_strings[] =
	{
#define X(el) #el,
		LEAF_ELEMENT_TYPES
#undef X
	};

	template<Type type>
	struct Map
	{
		static constexpr bool valid = false;
		using SysType = void;
		static constexpr const uint16_t floats = 0;
	};
	template<> struct Map< Type::Float >
	{
		using SysType = float;
		static constexpr size_t hlslSize = sizeof(SysType);
		static constexpr const uint16_t ocslot = 1;
		static constexpr const uint16_t floats = 1;
		static constexpr bool valid = true;
		static constexpr const char* code = "float";
		struct param { SysType min; SysType max; SysType def; };
	};
	template<> struct Map< Type::Float2 >
	{
		using SysType = QVector2D;
		static constexpr size_t hlslSize = sizeof(SysType);
		static constexpr const uint16_t ocslot = 1;
		static constexpr const uint16_t floats = 2;
		static constexpr bool valid = true;
		static constexpr const char* code = "vec2";
		struct param { SysType min; SysType max; SysType def; };
	};
	template<> struct Map< Type::Float3 >
	{
		using SysType = QVector3D;
		static constexpr size_t hlslSize = sizeof(SysType);
		static constexpr const uint16_t ocslot = 1;
		static constexpr const uint16_t floats = 3;
		static constexpr bool valid = true;
		static constexpr const char* code = "vec3";
		struct param { SysType min; SysType max; SysType def; };
	};
	template<> struct Map< Type::Float4 >
	{
		using SysType = QVector4D;
		static constexpr size_t hlslSize = sizeof(SysType);
		static constexpr const uint16_t ocslot = 1;
		static constexpr const uint16_t floats = 4;
		static constexpr bool valid = true;
		static constexpr const char* code = "vec4";
		struct param { SysType min; SysType max; SysType def; };
	};
	template<> struct Map< Type::Matrix >
	{
		using SysType = QMatrix4x4;
		static constexpr size_t hlslSize = sizeof(SysType);
		static constexpr const uint16_t ocslot = 4;
		static constexpr const uint16_t floats = 16;
		static constexpr const uint16_t stride = 4;
		static constexpr bool valid = true;
		static constexpr const char* code = "mat4";
		struct param { SysType def; };
	};
	template<> struct Map< Type::Bool >
	{
		using SysType = bool;
		static constexpr size_t hlslSize = 4u;
		static constexpr const uint16_t ocslot = 1;
		static constexpr const uint16_t floats = 1;
		static constexpr bool valid = true;
		static constexpr const char* code = "bool";
		struct param { SysType def; };
	};
	template<> struct Map< Type::Integer >
	{
		using SysType = int;
		static constexpr size_t hlslSize = sizeof(SysType);
		static constexpr const uint16_t ocslot = 1;
		static constexpr const uint16_t floats = 1;
		static constexpr const char* code = "int";
		static constexpr bool valid = true;
		struct param { SysType min; SysType max; SysType def; };
	};

#define X(el) static_assert(Map<Type::el>::valid, "Missing map implementation for " #el);
	LEAF_ELEMENT_TYPES
#undef X

		template<typename T>
	struct ReverseMap
	{
		static constexpr bool valid = false;
	};
#define X(el)\
	template<> struct ReverseMap<typename Map<Type::el>::SysType>\
	{\
		static constexpr bool valid = true;\
		static constexpr Type type = Type::el;\
	};
	LEAF_ELEMENT_TYPES
#undef X

	template<Type t>
	concept has_min = requires(typename Map<t>::param p) { p.min; };
	template<Type t>
	concept has_max = requires(typename Map<t>::param p) { p.max; };
	template<Type t>
	concept has_stride = requires(Map<t> p) { p.stride; };
	template<Type t>
	concept integral_core = std::integral<typename Map<t>::SysType> ||
		requires (typename Map<t>::SysType v) { { v[0] }->std::integral; };


	template <Type t>
	inline consteval uint16_t GetStride()
	{
		if constexpr (has_stride<t>)
			return Map<t>::stride;
		return Map<t>::floats;
	}
}