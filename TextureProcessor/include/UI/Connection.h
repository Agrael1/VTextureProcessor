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

class QGraphicsScene;

namespace UI
{
	struct INode;
	class Port;
	class Connection;

	class ConnectionMap
	{
	public:
		ConnectionMap() = default;
		~ConnectionMap();
		ConnectionMap(ConnectionMap&) = delete;
		void operator=(ConnectionMap&) = delete;
	public:
		void MakeTemporary(Port& port);
		void ConnectTemporary(Port& port);
		void AttachTemporary(std::unique_ptr<IConnection>&& in);
		void ClearTemporary();
		std::unique_ptr<IConnection> DetachTemporary();

		void UpdateGraph(INode& from);
		void Map(INode& n, Connection& c);
		void Unmap(INode& n, Connection& c);
		void Trim(INode& n);

		void Serialize(QJsonObject& doc);
		std::span<Connection*> Get(const INode& n);
	private:
		std::unordered_map<const INode*, std::vector<Connection*>> map;
		std::unique_ptr<IConnection> tmp;
	};

	IConnection& Query(Connection* in);
	ConnectionMap& QueryConnectionMap(QGraphicsScene* scene);
}

