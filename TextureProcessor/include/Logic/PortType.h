/**
 * @file PortType.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief Basic Enumerators for node logic
 * https://github.com/Agrael1/VeritasD3D
 */
#pragma once 
#include <string_view>
#include <array>

#define ENUMERATE()\
	X(None)\
	X(Grayscale)\
	X(Color)\
	X(Normal)

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
#undef ENUMERATE