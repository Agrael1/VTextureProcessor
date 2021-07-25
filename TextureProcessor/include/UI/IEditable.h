#pragma once
#include <utils/Macros.h>

class QWidget;

struct NOVTABLE IEditable
{
	virtual ~IEditable() = default;
	virtual QWidget* Widget() noexcept = 0;
	virtual void Save() = 0;
	virtual void SaveAs() = 0;
	virtual void Clear() = 0;
	virtual void Load() = 0;
};