#pragma once
#include <cassert>
#include <memory>
#include <string>
#include <optional>
#include <vector>
#include <span>

#define LEAF_ELEMENT_TYPES \
	X(Float)\
	X(Float2)\
	X(Float3)\
	X(Float4)\
	X(Matrix)\
	X(Bool)\
	X(Integer)

namespace ver::dc
{
	enum class Type
	{
		Empty,
#define X(el) el,
		LEAF_ELEMENT_TYPES
#undef X
	};

	template<Type type>
	struct Map
	{
		static constexpr bool valid = false;
	};
	template<> struct Map< Type::Float >
	{
		using SysType = float;
		static constexpr size_t hlslSize = sizeof(SysType);
		static constexpr bool valid = true;
		static constexpr const char* code = "F1";
	};
	template<> struct Map< Type::Float2 >
	{
		using SysType = QVector2D;
		static constexpr size_t hlslSize = sizeof(SysType);
		static constexpr bool valid = true;
		static constexpr const char* code = "F2";
	};
	template<> struct Map< Type::Float3 >
	{
		using SysType = QVector3D;
		static constexpr size_t hlslSize = sizeof(SysType);
		static constexpr bool valid = true;
		static constexpr const char* code = "F3";
	};
	template<> struct Map< Type::Float4 >
	{
		using SysType = QVector4D;
		static constexpr size_t hlslSize = sizeof(SysType);
		static constexpr bool valid = true;
		static constexpr const char* code = "F4";
	};
	template<> struct Map< Type::Matrix >
	{
		using SysType = QMatrix4x4;
		static constexpr size_t hlslSize = sizeof(SysType);
		static constexpr bool valid = true;
		static constexpr const char* code = "F4x4";
	};
	template<> struct Map< Type::Bool >
	{
		using SysType = bool;
		static constexpr size_t hlslSize = 4u;
		static constexpr bool valid = true;
		static constexpr const char* code = "B";
	};
	template<> struct Map< Type::Integer >
	{
		using SysType = int;
		static constexpr size_t hlslSize = sizeof(SysType);
		static constexpr const char* code = "IN";
		static constexpr bool valid = true;
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


	class LayoutElement
	{
	public:
		constexpr LayoutElement() noexcept = default;
		constexpr LayoutElement(Type typeIn) noexcept
			:type(typeIn)
		{
			if (!std::is_constant_evaluated())
			{
				assert(typeIn != Type::Empty);
			}
		}
	public:
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
	private:
		Type type = Type::Empty;
	};



	class Layout
	{
	public:
		Layout() noexcept = default;
	public:
		Layout& Add(Type addedType, std::string name) noexcept
		{
			lay.emplace_back(std::move(name), addedType);
			return *this;
		}
		Layout& Add(std::initializer_list<std::pair<std::string, LayoutElement>> pairs) noexcept
		{
			lay.insert(lay.end(), pairs);
			return *this;
		}
		Layout& operator+=(std::pair<std::string, LayoutElement> element) noexcept
		{
			lay.emplace_back(std::move(element));
			return *this;
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
	private:
		std::vector<std::pair<std::string, LayoutElement>> lay;
	};


	class Buffer;

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
			if (type && ReverseMap<std::remove_const_t<T>>::type == type)
				static_cast<T&>(*this) = rhs;
			assert(false && "Attemt to set non existent element");
		}
	private:
		ElementRef(std::byte* pBytes, LayoutElement type) noexcept
			:type(type), pBytes(pBytes)
		{

		}
	private:
		LayoutElement type;
		std::byte* pBytes;
	};

	class Buffer
	{
	public:
		Buffer(const Layout&& lay) noexcept
			:lay(lay)
		{
			bytes.resize(lay.GetSizeInBytes());
		}
	public:
		ElementRef operator[](std::string_view key) noexcept
		{
			auto x = lay.GetOffsetAndType(key);
			return { bytes.data() + x.first, x.second };
		}
	public:
		constexpr std::span<const std::byte> GetData() const noexcept
		{
			return bytes;
		}
		size_t GetSizeInBytes() const noexcept
		{
			return lay.GetSizeInBytes();
		}
	private:
		const Layout lay;
		std::vector<std::byte> bytes;
	};

}
