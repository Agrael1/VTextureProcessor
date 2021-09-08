#pragma once
#include <Interfaces/INode.h>

namespace UI
{
	class FlowCodex
	{
		static constexpr std::string_view NodesDir = "nodes";
		struct RefCountPair : public std::unique_ptr<INode>
		{
			template<typename T, typename ...Args>
			static RefCountPair set(Args&&... in)
			{
				return { std::make_unique<T>(std::forward<Args>(in)...) };
			}
			mutable size_t refcount = 0;
		};
	public:
		FlowCodex();
	public:
		const auto& CatMap()const noexcept
		{
			return cats;
		}
		void ClearCounts()
		{
			for (auto&& x : codex)
				x.second.refcount = 0;
		}

		std::unique_ptr<UI::INode> GetNode(std::string_view nodety)const;
		std::unique_ptr<UI::INode> GetNode(std::string_view nodety, size_t ref) const;
		bool contains(std::string_view nodety)const;
	private:
		void ParseJson(const QJsonDocument& json);
	private:
		std::unordered_map<std::string, RefCountPair> codex;
		std::unordered_map<std::string, std::vector<std::string_view>> cats;
	};
}