/**
 * @file Connection.h
 * @author Ilya Doroshenko (xdoros01), David Černý (xcerny74)
 * @brief Class for managing connections between Nodes
 */
#pragma once
#include <Interfaces/IConnection.h>
#include <Logic/PortType.h>
#include <unordered_map>
#include <memory>
#include <span>


namespace UI
{
	struct INode;
	class Port;
	class Connection;

	class ConnectionMap
	{
	public:
		static ConnectionMap& Instance();

		static void MakeTemporary(Port& port);
		static void ConnectTemporary(Port& port);
		static void AttachTemporary(std::unique_ptr<IConnection>&& in);
		static void ClearTemporary();
		static std::unique_ptr<Connection> DetachTemporary();

		static void UpdateGraph(INode& from);
		static void Map(INode& n, Connection& c);
		static void Unmap(INode& n, Connection& c);
		static void Trim(INode& n);
		static std::span<Connection*> Get(const INode& n);
	private:
		ConnectionMap() = default;
		~ConnectionMap();
		ConnectionMap(ConnectionMap&) = delete;
		ConnectionMap(ConnectionMap&&) = delete;
		void operator=(ConnectionMap&) = delete;
		void operator=(ConnectionMap&&) = delete;
	private:
		std::unordered_map<const INode*, std::vector<Connection*>> map;
		std::unique_ptr<Connection> tmp;
	};

	IConnection& Query(Connection* in);
}

