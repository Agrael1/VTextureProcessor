#pragma once
namespace ver::dc
{
	struct Options;
	class Buffer;
	struct param_storage;
}

namespace UI
{
	struct INode;
	namespace Windows
	{
		class PropertyElement;
	}
	void PropertyBuffer(Windows::PropertyElement& elem, ver::dc::Buffer& buf, std::span<ver::dc::Options> params);
}