#pragma once
#include <UI/Node.h>
#include <Logic/Engine.h>

namespace UI::RE
{
	class FlowCodex
	{
		static constexpr std::string_view NodesDir = "nodes";
		struct RefCountPair
		{
			template<typename ...Args>
			static RefCountPair set(Args&&... in)
			{
				return { .node{std::make_unique<XNode>(std::forward<Args>(in)...)} };
			}
			std::unique_ptr<XNode> node;
			mutable size_t refcount = 0;
		};
		FlowCodex();
	public:
		FlowCodex& Instance()noexcept
		{
			static FlowCodex instance;
			return instance;
		}
		const auto& CatMap()const noexcept
		{
			return cats;
		}
		void ClearCounts()
		{
			for (auto&& x : codex)
				x.second.refcount = 0;
		}
		const std::unique_ptr<XNode>& GetNode(std::string_view nodety)const;
		const std::unique_ptr<XNode>* TryGetNode(std::string_view nodety)const;
		void SetMaxRef(std::string_view nodety, size_t cnt);
		size_t AddRef(std::string_view nodety);
	private:
		void ParseJson(const QJsonDocument& json);
	private:
		std::unordered_map<std::string, RefCountPair> codex;
		std::unordered_map<std::string, std::vector<std::string_view>> cats;
		Engine engine;
	};
}