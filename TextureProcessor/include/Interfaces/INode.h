/**
 * @file INode.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief Interfaces for class management
 */
#pragma once
#include <QGraphicsWidget>
#include <Interfaces/ISerialize.h>
#include <memory>

namespace ver
{
	class ShaderProbe;
}

namespace UI
{
	namespace Windows{class PropertyElement;}
	class Port;

	struct NOVTABLE INode:public QGraphicsWidget, public ISerialize
	{
		virtual ~INode() = default;
		virtual std::string_view Name()const = 0;
		virtual void UpdateProperties(Windows::PropertyElement& properties) = 0;
		virtual void Update() = 0;
		virtual std::string Export() = 0;
		//virtual void Edit() = 0;
		virtual void ExportSilent(std::string_view in) = 0;
		virtual void StartConnection(uint8_t index) = 0;
		virtual void FinishConnection(uint8_t index) = 0;
		virtual void Disconnect() = 0;
		virtual void Accept(ver::ShaderProbe& probe) = 0;
	};
}
