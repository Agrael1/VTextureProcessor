#pragma once
#include <variant>
#include <vector>
#include <array>
#include <span>

namespace ver::dc
{
	class Buffer;
}

namespace ver
{
#define TYPES(m) \
	X(Buffer)\
	X(BooleanUpd)\
	m##X(Boolean)
#define MXRX(a) X(a)
#define TYPES_() TYPES(MXR)
#define XTYPES TYPES_()

	enum class DescType
	{
#define X(a) a,
		XTYPES
		Count
#undef X
	};


	template<DescType x>
	struct DescMap
	{
		using tied_t = void;
	};
	template<>struct DescMap<DescType::Buffer>
	{
		using tied_t = dc::Buffer;
	};
	template<>struct DescMap<DescType::Boolean>
	{
		using tied_t = bool;
	};
	template<>struct DescMap<DescType::BooleanUpd>
	{
		using tied_t = bool;
	};

	using arg_v = std::pair<DescType, const char*>;

	template<size_t N>
	class PropertyDesc
	{
	public:
		constexpr PropertyDesc(const arg_v(&args)[N])noexcept
		{
			std::copy_n(args, N, desc.begin());
		}
		constexpr auto operator[](size_t i)const
		{
			return desc[i];
		}
	private:
		std::array<arg_v, N> desc;
	};

	template<size_t N>
	consteval auto MakeDesc(const arg_v(&args)[N])
	{
		return PropertyDesc<N>{args};
	}


	template <class... Ts>
	struct typelist;

	template <class List, class... Ts>
	struct unique_variant;

	template <class... Uniqs>
	struct unique_variant<typelist<Uniqs...>> {
		using type = std::variant<Uniqs...>;
	};

	template <class... Uniqs, class Head, class... Tail>
	struct unique_variant<typelist<Uniqs...>, Head, Tail...> {
		using type = std::conditional_t<
			(std::is_same_v<Head, Uniqs> || ...),
			typename unique_variant<typelist<Uniqs...      >, Tail...>::type,
			typename unique_variant<typelist<Uniqs..., Head>, Tail...>::type
		>;
	};
	template <class... Ts>
	using unique_variant_t = typename unique_variant<typelist<>, Ts...>::type;

	template <class T>
	struct enum2type;

	template<typename T, T... size>
	struct enum2type<std::integer_sequence<T, size...>> {
		using type = unique_variant_t<typename DescMap<(DescType)size>::tied_t*...>;
	};	
	using unique_variant_tg = enum2type<std::make_integer_sequence<size_t, size_t(DescType::Count)>>::type;
	

	class PropertyView
	{
		using prop_ref = unique_variant_tg;
		using view_t = std::pair<arg_v, prop_ref>;
	public:
		//template<arg_v...v>
		//static PropertyView Make(typename DescMap<v.first>::tied_t&... ref)
		//{
		//	PropertyView pv;
		//	(pv.refs.push_back(TieProp<v>(ref)),...);
		//	return pv;
		//}
		template<arg_v v>
		void TieProp(typename DescMap<v.first>::tied_t& ref)
		{
			refs.emplace_back(view_t{ v, prop_ref{ &ref } });
		}
		std::span<view_t> Get()
		{
			return refs;
		}
	private:
		std::vector<view_t> refs;
	};



#ifndef USE_TYPES
#undef TYPES
#undef MXRX
#undef TYPES_
#undef XTYPES
#endif
}