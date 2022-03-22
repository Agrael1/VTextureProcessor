#pragma once
#include <memory>


namespace ver
{
	class Node;
	struct Descriptor
	{
		virtual ~Descriptor() = default;
		virtual std::unique_ptr<Node> MakeModel()const = 0;
		virtual bool valid()const noexcept = 0;
		size_t use_count()const noexcept { return refcount; };
		mutable size_t refcount = 0;
	};
}