/**
 * @file INode.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief Interfaces for class management
 */
#pragma once
#include <QGraphicsWidget>
#include <UI/Interfaces/ISerialize.h>
#include <memory>

namespace UI
{
	namespace Windows{class XPropertyElement;}

	struct NOVTABLE IXNode:public QGraphicsWidget, public ISerialize
	{
		virtual ~IXNode() = default;
		virtual std::unique_ptr<IXNode> Clone(std::string&& name)const = 0;
		virtual std::string_view Name()const = 0;
		virtual void UpdateProperties(Windows::XPropertyElement& properties) = 0;
		virtual void Update() = 0;
		virtual std::string Export() = 0;
		virtual void ExportSilent(std::string_view in) = 0;
	};
}
