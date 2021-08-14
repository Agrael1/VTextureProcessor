#pragma once
#include <UI/Node.h>
#include <Logic/Engine.h>


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
		XFlowCodex();
	public:
		static XFlowCodex& Instance()noexcept
		{
			static XFlowCodex instance;
			return instance;
		}
		static void DestroyEngine()
		{
			Instance().engine.reset();
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

		std::unique_ptr<IXNode> GetNode(std::string_view nodety)const;
		const std::unique_ptr<IXNode>* TryGetNode(std::string_view nodety)const;

		void SetMaxRef(std::string_view nodety, size_t cnt);
		size_t AddRef(std::string_view nodety);
	private:
		void ParseJson(const QJsonDocument& json);
	private:
		std::unordered_map<std::string, RefCountPair> codex;
		std::unordered_map<std::string, std::vector<std::string_view>> cats;
		std::optional<Engine> engine;
	};
}