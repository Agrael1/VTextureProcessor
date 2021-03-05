#pragma once
#include <unordered_map>
#include <Link.h>


class GraphCodex
{
public:
	static GraphCodex& get()
	{
		static GraphCodex lazy;
		return lazy;
	}


	Node& MakeNode(std::string_view name)
	{
		return *((*(nodes.emplace(name, std::make_unique<Node>(name)).first)).second);
	}
	void EraseNode(std::string_view name)
	{
		nodes.erase(name);
	}
private:
	std::unordered_map<std::string_view, std::unique_ptr<Node>> nodes;
};

namespace vrg
{
	class unique_node
	{
	public:
		unique_node(std::string_view name)
			:impl(GraphCodex::get().MakeNode(name)){}
		~unique_node(){
			GraphCodex::get().EraseNode(impl.GetName());
		}
	public:
		constexpr auto* operator ->(){
			return &impl;
		}
	private:
		Node& impl;
	};
}

