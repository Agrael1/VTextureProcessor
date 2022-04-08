/**
 * @file DynamicConstant.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief Dynamic constant buffer for heterogenous data to bind on a gpu side
 * Gathered from Veritas engine WinD3D project
 * https://github.com/Agrael1/VeritasD3D
 */
#pragma once
#include <Logic/ParameterStorage.h>
#include <cassert>
#include <optional>
#include <vector>
#include <variant>
#include <span>
#include <unordered_set>


namespace ver::dc
{
	class LayoutElement
	{
	public:
		constexpr LayoutElement() noexcept = default;
		constexpr LayoutElement(Type typeIn) noexcept
			:type(typeIn)
		{}
		constexpr LayoutElement(std::string_view signature)noexcept
		{
#define X(el) if(signature == Map<Type::el>::code){type = Type::el; return;}
			LEAF_ELEMENT_TYPES
#undef X
				type = Type::Empty;
		}
	public:
		constexpr Type Get()const noexcept
		{
			return type;
		}
		constexpr std::string_view GetSignature() const noexcept
		{
			switch (type)
			{
#define X(el) case Type::el: return Map<Type::el>::code;
				LEAF_ELEMENT_TYPES
#undef X
			default:
				assert("Bad type in signature generation" && false);
				return "???";
			}
		}
		constexpr bool Exists() const noexcept
		{
			return type != Type::Empty;
		}
	public:
		operator bool()const noexcept
		{
			return Exists();
		}
	public:
		constexpr size_t GetSizeInBytes() const noexcept
		{
			switch (type)
			{
#define X(el) case Type::el: return Map<Type::el>::hlslSize;
				LEAF_ELEMENT_TYPES
#undef X
			default:
				assert("Bad type in signature generation" && false);
				return 0;
			}
		}
		constexpr uint16_t GetSlot() const noexcept
		{
			switch (type)
			{
#define X(el) case Type::el: return Map<Type::el>::ocslot;
				LEAF_ELEMENT_TYPES
#undef X
			default:
				assert("Bad type in signature generation" && false);
				return 0;
			}
		}
	private:
		Type type = Type::Empty;
	};

	class Layout
	{
	public:
		using Entry = std::pair<std::string, LayoutElement>;
	public:
		Layout() noexcept = default;
	public:
		Layout& Add(Type addedType, std::string name) noexcept
		{
			lay.emplace_back(std::move(name), addedType);
			return *this;
		}
		Layout& Add(Entry e) noexcept
		{
			lay.emplace_back(std::move(e));
			return *this;
		}
		Layout& Add(std::initializer_list<std::pair<std::string, LayoutElement>> pairs) noexcept
		{
			lay.insert(lay.end(), pairs);
			return *this;
		}
		Layout& operator+=(Entry element) noexcept
		{
			lay.emplace_back(std::move(element));
			return *this;
		}
		std::span<const Entry> Get()const noexcept
		{
			return lay;
		}
		auto& operator[](size_t in)
		{
			return lay[in];
		}
		size_t count()const noexcept
		{
			return lay.size();
		}
		bool contains(std::string_view key)const noexcept
		{
			for (const auto& x : lay)
				if (key == x.first)
					return true;
			return false;
		}
		bool is_distinct()const
		{
			std::unordered_set<std::string_view> distinct;
			std::ranges::for_each(lay, [&distinct](const auto& a) {distinct.emplace(a.first); });
			return distinct.size() == lay.size();
		}
	public:
		std::pair<size_t, LayoutElement> GetOffsetAndType(std::string_view key) const
		{
			size_t out = 0;
			LayoutElement out2;
			for (const auto& x : lay)
			{
				if (key == x.first)
				{
					out2 = x.second;
					break;
				}
				out += x.second.GetSizeInBytes();
			}
			return { out, out2 };
		}
		size_t GetSizeInBytes() const noexcept
		{
			size_t out = 0;
			for (const auto& x : lay)
			{
				out += x.second.GetSizeInBytes();
			}
			return out;
		}
		std::string GetSignature() const noexcept
		{
			using namespace std::string_literals;
			auto sig = "St{"s;
			for (const auto& el : lay)
			{
				sig += el.first + ":"s + el.second.GetSignature().data() + ";"s;
			}
			sig += "}"s;
			return sig;
		}
		uint16_t GetSlot(std::string_view key)
		{
			uint16_t out = 0;
			LayoutElement out2;
			for (const auto& x : lay)
			{
				if (key == x.first)
				{
					out2 = x.second;
					break;
				}
				out += x.second.GetSlot();
			}
			return out;
		}
	private:
		std::vector<Entry> lay;
	};

	class Buffer;


	template <typename T, typename Array, std::size_t... I>
	T v2T(const Array& a, std::index_sequence<I...>)
	{
		return T(a[I].toFloat()...);
	}
	template<dc::Type type, class T = dc::Map<type>::SysType, typename Indices = std::make_index_sequence<dc::Map<type>::floats>>
	T FromVariant(const QVariant& va)
	{
		if constexpr (std::same_as<T, int> || std::same_as<T, float> || std::same_as<T, bool>)
			return T{ va.value<T>() };
		return v2T<T>(va.toList(), Indices{});
	}

	template<dc::Type type, class T = dc::Map<type>::SysType>
	QVariant GetVariant(const std::byte* va)
	{
		if constexpr (std::same_as<T, int> || std::same_as<T, float> || std::same_as<T, bool>)
			return *((T*)va);
		QVariantList qw;
		qw.reserve(int(dc::Map<type>::floats));
		for (auto el : std::span<const float>((const float*)va, dc::Map<type>::floats))
			qw.append(el);
		return qw;
	}

	class ElementRef
	{
		friend class Buffer;
	public:
		class Ptr
		{
			friend ElementRef;
		public:
			// conversion to read/write pointer to supported SysType
			template<typename T>
			operator T* () const noexcept
			{
				static_assert(ReverseMap<std::remove_const_t<T>>::valid, "Unsupported SysType used in pointer conversion");
				return &static_cast<T&>(*ref);
			}
		private:
			Ptr(ElementRef* ref) noexcept :ref(ref) {}
			ElementRef* ref;
		};
	public:
		operator bool()const noexcept
		{
			return type;
		}

		template<typename S>
		bool SetIfExists(const S& val) noexcept
		{
			if (type)
			{
				*this = val;
				return true;
			}
			return false;
		}
		Ptr operator&() const noexcept
		{
			return const_cast<ElementRef*>(this);
		}

		template<typename T>
		operator T& () const noexcept
		{
			static_assert(ReverseMap<std::remove_const_t<T>>::valid, "Unsupported SysType used in conversion");
			return *reinterpret_cast<T*>(pBytes);
		}

		template<typename T>
		void operator=(const T& rhs) const noexcept
		{
			static_assert(ReverseMap<std::remove_const_t<T>>::valid, "Unsupported SysType used in assignment");
			if (type && ReverseMap<std::remove_const_t<T>>::type == type.Get())
			{
				static_cast<T&>(*this) = rhs;
				return;
			}
			assert(false && "Attemt to set non existent element");
		}
		void operator=(const QVariant& rhs) const noexcept
		{
			switch (type.Get())
			{
#define X(el) case Type::el: return this->operator=(FromVariant<Type::el>(rhs));
				LEAF_ELEMENT_TYPES
#undef X
			default:
				break;
			}
			assert(false && "Attemt to set non existent element");
		}
		void operator=(const param_storage& rhs) const noexcept
		{
			if (type.Get() != rhs.t)return;
			switch (type.Get())
			{
#define X(el) case Type::el: return this->operator=(rhs.get<Type::el>().def);
				LEAF_ELEMENT_TYPES
#undef X
			default:
				break;
			}
			assert(false && "Attemt to set non existent element");
		}
		QVariant ToVariant()const noexcept
		{
			switch (type.Get())
			{
#define X(el) case Type::el: return GetVariant<Type::el>(pBytes);
				LEAF_ELEMENT_TYPES
#undef X
			default:
				break;
			}
			assert(false && "Attemt to get non existent element");
			return{};
		}
		Type GetType()const noexcept
		{
			return type.Get();
		}
		uint16_t GetSlot()const noexcept
		{
			return slot;
		}
		std::string_view GetName()const noexcept
		{
			return name;
		}
	private:
		ElementRef(std::byte* pBytes, LayoutElement type, uint16_t slot, std::string_view name) noexcept
			:type(type), pBytes(pBytes), slot(slot), name(name)
		{

		}
	private:
		std::string_view name;
		LayoutElement type;
		uint16_t slot;
		std::byte* pBytes;
	};

	class Buffer
	{
		class Iterator
		{
		public:
			explicit Iterator(Buffer* b, ElementRef r, size_t index) :ref(r), b(b), index(index) {}
		public:
			void operator++()
			{
				ref = b->Get(++index);
			}
			bool operator==(Iterator& o)
			{
				return (b == o.b) && (index == o.index);
			}
			ElementRef operator*()const noexcept
			{
				return ref;
			}
		private:
			ElementRef ref;
			Buffer* b;
			size_t index;
		};
	public:
		Buffer() = default;
		Buffer(Layout xlay) noexcept
			:lay(std::move(xlay))
		{
			bytes.resize(lay.GetSizeInBytes());
		}
		Buffer(Layout xlay, std::span<const Options> def) noexcept
			:lay(std::move(xlay))
		{
			bytes.resize(lay.GetSizeInBytes());
			for (auto& i : def)
				if(i.enable_def)
					Get(i.index) = i.param;
		}
	public:
		ElementRef operator[](std::string_view key) noexcept
		{
			auto x = lay.GetOffsetAndType(key);
			return { bytes.data() + x.first, x.second, lay.GetSlot(key), key };
		}
		operator bool()const
		{
			return !bytes.empty();
		}
		Iterator begin()
		{
			return Iterator{ this, Get(0), 0 };
		}
		Iterator end()
		{
			return Iterator{ this, Get(lay.count()), lay.count() };
		}
	public:
		void Replace(Layout xlay, std::span<const Options> def)
		{
			lay = std::move(xlay);
			bytes.resize(lay.GetSizeInBytes());
			for (auto& i : def)
				if (i.enable_def)
					Get(i.index) = i.param;
		}
		constexpr std::span<const std::byte> GetData() const noexcept
		{
			return bytes;
		}
		size_t GetSizeInBytes() const noexcept
		{
			return lay.GetSizeInBytes();
		}
	private:
		ElementRef Get(size_t in)
		{
			if (in == lay.count())
				return { bytes.data() + bytes.size(), { Type::Empty }, 65535, "" };
			std::string_view name = lay[in].first;
			auto x = lay.GetOffsetAndType(name);
			return { bytes.data() + x.first, x.second, lay.GetSlot(name), name };
		}
	private:
		Layout lay;
		std::vector<std::byte> bytes;
	};

}
