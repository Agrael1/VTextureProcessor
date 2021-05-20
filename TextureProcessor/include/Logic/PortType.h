/**
 * @file PortType.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief Basic Enumerators for node logic
 * https://github.com/Agrael1/VeritasD3D
 */
#pragma once 

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
	switch (p)
	{
#define X(x) case PortType::x: return #x;
		ENUMERATE()
#undef X
	default:
		return "None";
	}
}

enum class Port :uint8_t
{
	None,
	Sink,
	Source
};
inline bool any(Port p)
{
	return p != Port::None;
}
inline bool any(PortType p)
{
	return p != PortType::None;
}