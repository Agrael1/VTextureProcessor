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


	template<typename T, typename... Ts>
	consteval bool contains() {
		return std::disjunction_v<std::is_same<T, Ts>...>;
	}

	template<class... A>
	struct Join;

	template<class T1>
	struct Join<T1> {

		template <class... A>
		struct tx { using t = std::variant<T1, A...>; };
		template<>
		struct tx<void> { using t = std::variant<T1>; };

		template <class... A>
		using a_t = typename tx<A...>::t;
		using t = typename tx<void>::t;
	};

	template<class T1, class...T2>
	struct Join<T1, T2...> : public Join<T2...>
	{
		using base = Join<T2...>;

		template <class... A>
		struct tx {
			using t = std::conditional_t<contains<T1, T2...>(),
				typename Join<T2...>::template tx<A...>::t,
				typename Join<T2...>::template tx<T1, A...>::t>;
		};
		template <>
		struct tx<void> { using t = std::conditional_t < contains<T1, T2...>(), typename Join<T2...>::t, typename Join<T2...>::template a_t<T1>>; };

		template <class... A>
		using a_t = typename tx<A...>::t;
		using t = typename tx<void>::t;
	};

	template<class... T>
	using unique_variant = typename Join<T...>::t;

	class PropertyView
	{
#define _X(a) DescMap<DescType::a>::tied_t*
#define X(a) _X(a),
		using prop_ref = unique_variant<TYPES(_)>;
		using view_t = std::pair<arg_v, prop_ref>;
#undef _X
#undef X

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