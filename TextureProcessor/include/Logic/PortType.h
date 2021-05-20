/**
 * @file PortType.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief Basic Enumerators for node logic
 * https://github.com/Agrael1/VeritasD3D
 */
#pragma once 

enum class PortType :uint8_t
{
	None,
	Grayscale
};

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