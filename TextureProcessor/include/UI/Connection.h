/**
 * @file Connection.h
 * @author Ilya Doroshenko (xdoros01), David Černý (xcerny74)
 * @brief Class for managing connections between Nodes
 */
#pragma once
#include <unordered_map>
#include <span>
#include <Logic/PortType.h>
#include <UI/Interfaces/IConnection.h>

namespace UI
{
	struct IXNode;
	class XPort;
	class XConnection;

	class XConnMapper
	{
	public:
		static XConnMapper& Instance();

		static void MakeTemporary(XPort& port);
		static void ConnectTemporary(XPort& port);
		static void AttachTemporary(std::unique_ptr<IXConnection>&& in);
		static void ClearTemporary();
		static std::unique_ptr<XConnection> DetachTemporary();

		static void UpdateGraph(IXNode& from);
		static void Map(IXNode& n, XConnection& c);
		static void Unmap(IXNode& n, XConnection& c);
		static void Trim(IXNode& n);
		static std::span<XConnection*> Get(const IXNode& n);
	private:
		XConnMapper() = default;
		~XConnMapper();
		XConnMapper(XConnMapper&) = delete;
		XConnMapper(XConnMapper&&) = delete;
		void operator=(XConnMapper&) = delete;
		void operator=(XConnMapper&&) = delete;
	private:
		std::unordered_map<const IXNode*, std::vector<XConnection*>> map;
		std::unique_ptr<XConnection> tmp;
	};

	IXConnection& Query(XConnection* in);
}

