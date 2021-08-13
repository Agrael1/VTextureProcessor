#pragma once
#include <variant>
#include <vector>
#include <bitset>
#include <array>
#include <Logic/DynamicConstant.h>
#include <Logic/Constants.h>

namespace ver
{
#define TYPES(m) \
	X(Buffer)\
	m##X(Boolean)

	enum class DescType
	{
#define X(a) a,
#define _X(a) a
		TYPES(_)
#undef _X
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

	using arg_v = std::pair<DescType, const char*>;

	template<size_t N>
	class PropertyDesc
	{
	public:
		constexpr PropertyDesc(const arg_v(&args)[N])noexcept
		{
			std::copy_n(args, N, desc.begin());
		}
		constexpr std::span<const arg_v, N> Get()const noexcept
		{
			return desc;
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

	class PropertyView
	{
#define _X(a) DescMap<DescType::a>::tied_t*
#define X(a) _X(a),
		using prop_ref = std::variant<TYPES(_)>;
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
			refs.emplace_back(view_t{v, prop_ref{ &ref }});
		}
	private:
		std::vector<view_t> refs;
	};

#undef TYPES
}