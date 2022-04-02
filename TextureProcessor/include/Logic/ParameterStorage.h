#pragma once
#include <Logic/DynamicTypes.h>
#include <variant>

namespace ver::dc
{
	struct param_storage
	{
	public:
		param_storage(Type xt) :t(xt)
		{
			switch (t)
			{
#define X(el) case Type::el: val.emplace<typename Map<Type::el>::param>();break;
				LEAF_ELEMENT_TYPES
#undef X
			default:
				break;
			}
		}
	public:
		template <Type t>
		auto& get()
		{
			return std::get<typename Map<t>::param>(val);
		}
		template <Type t>
		auto& get()const
		{
			return std::get<typename Map<t>::param>(val);
		}
	private:
		template <Type t> requires has_min<t>
		void set_min_impl(QVariant v)
		{
			get<t>().min = v.value<typename Map<t>::SysType>();
		}
		template <Type t>
		void set_min_impl(QVariant v)
		{}
		template <Type t> requires has_min<t>
		void set_max_impl(QVariant v)
		{
			get<t>().max = v.value<typename Map<t>::SysType>();
		}
		template <Type t>
		void set_max_impl(QVariant v)
		{}
	public:
		void set_default(QVariant v)
		{
			switch (t)
			{
#define X(el) case Type::el: get<Type::el>().def = v.value<typename Map<Type::el>::SysType>();break;
				LEAF_ELEMENT_TYPES
#undef X
			default:
				break;
			}
		}
		void set_min(QVariant v)
		{
			switch (t)
			{
#define X(el) case Type::el: set_min_impl<Type::el>(v);break;
				LEAF_ELEMENT_TYPES
#undef X
			default:
				break;
			}
		}
		void set_max(QVariant v)
		{
			switch (t)
			{
#define X(el) case Type::el: set_max_impl<Type::el>(v);break;
				LEAF_ELEMENT_TYPES
#undef X
			default:
				break;
			}
		}


	public:
#define X(el) typename Map<Type::el>::param
#undef SEP
#define SEP() ,
		std::variant<LEAF_ELEMENT_TYPES> val;
		Type t;
#undef SEP
#define SEP()
#undef X
	};


	struct Options
	{
		Options(Type t) :param(t) {}
		uint16_t index = 0;
		uint8_t enable_def : 1 = 0;
		uint8_t enable_min : 1 = 0;
		uint8_t enable_max : 1 = 0;
		uint8_t enable_alias : 1 = 0;
		uint8_t reserved : 4 = 0;
		std::string alias;
		param_storage param;
	};
}