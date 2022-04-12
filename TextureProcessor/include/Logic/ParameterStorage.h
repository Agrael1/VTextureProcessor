#pragma once
#include <Logic/DynamicTypes.h>
#include <variant>

namespace ver::dc
{
	struct param_storage
	{
		template <Type t>
		using m_ty = Map<t>::SysType;
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
		void set_min_impl(QJsonValueRef v)
		{
			Deserialize(get<t>().min, v);
		}
		template <Type t>
		void set_min_impl(QJsonValueRef v)
		{}
		template <Type t> requires has_max<t>
		void set_max_impl(QJsonValueRef v)
		{
			Deserialize(get<t>().max, v);
		}
		template <Type t>
		void set_max_impl(QJsonValueRef v)
		{}
	private:
		template <Type t> requires has_min<t>
		QJsonValue get_min_impl()const
		{
			return Serialize(get<t>().min);
		}
		template <Type t>
		QJsonValue get_min_impl()const
		{
			return {};
		}
		template <Type t> requires has_max<t>
		QJsonValue get_max_impl()const
		{
			return Serialize(get<t>().max);
		}
		template <Type t>
		QJsonValue get_max_impl()const
		{
			return {};
		}
	public:
		void set_default(QJsonValueRef v)
		{
			switch (t)
			{
#define X(el) case Type::el: Deserialize(get<Type::el>().def, v);break;
				LEAF_ELEMENT_TYPES
#undef X
			default:
				break;
			}
		}
		void set_min(QJsonValueRef v)
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
		void set_max(QJsonValueRef v)
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
		QJsonValue get_def()const
		{
			switch (t)
			{
#define X(el) case Type::el: return Serialize(get<Type::el>().def);
				LEAF_ELEMENT_TYPES
#undef X
			default:
				break;
			}
			return {};
		}
		QJsonValue get_min()const
		{
			switch (t)
			{
#define X(el) case Type::el: return get_min_impl<Type::el>();
				LEAF_ELEMENT_TYPES
#undef X
			default:
				break;
			}
			return {};
		}
		QJsonValue get_max()const
		{
			switch (t)
			{
#define X(el) case Type::el: return get_max_impl<Type::el>();
				LEAF_ELEMENT_TYPES
#undef X
			default:
				break;
			}
			return {};
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
		union
		{
			struct
			{
				uint8_t enable_def : 1;
				uint8_t enable_min : 1;
				uint8_t enable_max : 1;
				uint8_t enable_alias : 1;
				uint8_t reserved : 4;
			};
			uint8_t flags = 0;
		};
		std::string alias;
		param_storage param;
	};
}