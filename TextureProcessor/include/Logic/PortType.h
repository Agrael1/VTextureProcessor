/**
 * @file PortType.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief Basic Enumerators for node logic
 * https://github.com/Agrael1/VeritasD3D
 */
#pragma once 
#include <QString>
#include <QPen>
#include <array>

#define ENUMERATE()\
	X(None)\
	X(Grayscale)\
	X(Color)\
	X(Normal)

namespace ver
{
	enum class PortType :uint8_t
	{
#define X(x) x,
		ENUMERATE()
#undef X
		size
	};
	constexpr std::array<std::string_view, size_t(PortType::size)> PortStrings{
	#define X(x) #x,
		ENUMERATE()
	#undef X
	};


	constexpr PortType from_str(std::string_view i)
	{
		using enum PortType;
#define X(x) if(i == #x)return x;
		ENUMERATE()
#undef X
			return None;
	}
	inline PortType from_str(QStringView i)
	{
		using enum PortType;
#define X(x) if(i == u#x)return x;
		ENUMERATE()
#undef X
			return None;
	}

	constexpr std::string_view to_str(PortType p)
	{
		return PortStrings[size_t(p)];
	}

	enum class PortSide :uint8_t
	{
		None,
		Sink,
		Source
	};
	inline bool any(PortSide p)
	{
		return p != PortSide::None;
	}
	inline bool any(PortType p)
	{
		return p != PortType::None;
	}
	constexpr inline bool compatible(PortType sink, PortType source)
	{
		switch (sink)
		{
		case ver::PortType::Normal:
		case ver::PortType::Grayscale:
			return source == sink;
		case ver::PortType::Color:
			return source == sink || source == ver::PortType::Grayscale;
		default:
			break;
		}
		return false;
	}

	struct PortDesc
	{
		QString name;
		PortType type;
	};
}
namespace UI
{
	struct PortStyle
	{
	public:
		constexpr static const qreal diameter = 10.0;
		constexpr static const qreal port_bbox = 20.0;
		constexpr static const qreal pen_width = 1.0;
	public:
		static const PortStyle* Get(ver::PortType ty)
		{
			switch (ty)
			{
			case ver::PortType::Grayscale:return &Grayscale;
			case ver::PortType::Color:return &Color;
			case ver::PortType::Normal:return &Normal;
			default:return nullptr;
			}
		}
	public:
		QPen port{ QColor{u"#FFFFFFFF"}, pen_width };
		QBrush brSink{ QColor{u"#FF828282"} };
		QBrush brSinkUsed{ QColor{u"#FFD9DDDC"} };
		QBrush brSource{ QColor{u"#FF909090"} };
	public:
		static const PortStyle Grayscale;
		static const PortStyle Color;
		static const PortStyle Normal;
	};
}

#undef ENUMERATE