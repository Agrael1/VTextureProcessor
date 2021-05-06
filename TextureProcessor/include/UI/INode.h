#pragma once
#ifdef _WIN32
#define NOVTABLE __declspec(novtable)
#else
#define NOVTABLE
#endif

namespace UI
{
	class Node;
	struct NOVTABLE INode
	{
		virtual ~INode() = default;
		virtual void OnConnect(uint8_t sinkN, Node& source, uint8_t sourceN) = 0;
		virtual void OnDisconnect(uint8_t sinkN) = 0;
		virtual void Update() = 0;
	};
}
