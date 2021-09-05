#pragma once
#include <Interfaces/INode.h>

namespace UI::RE
{
	class XFlowCodex
	{
		static constexpr std::string_view NodesDir = "nodes";
		struct RefCountPair : public std::unique_ptr<IXNode>
		{
			template<typename T, typename ...Args>
			static RefCountPair set(Args&&... in)
			{
				return { std::make_unique<T>(std::forward<Args>(in)...) };
			}
			mutable size_t refcount = 0;
		};
	public:
		XFlowCodex();
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

		std::unique_ptr<IXNode> GetNode(std::string_view nodety)const;
		bool contains(std::string_view nodety)const;

		void SetMaxRef(std::string_view nodety, size_t cnt);
	private:
		void ParseJson(const QJsonDocument& json);
	private:
		std::unordered_map<std::string, RefCountPair> codex;
		std::unordered_map<std::string, std::vector<std::string_view>> cats;
	};
}