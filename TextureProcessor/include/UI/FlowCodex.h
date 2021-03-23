#include <UI/UINode.h>
#include <unordered_map>

namespace UI
{
	class FlowCodex
	{
		template<class C>
		struct RefCountPair : C
		{
			template<typename ...Args>
			RefCountPair(Args&&... in)
				:C(std::forward<Args>(in)...)
			{}
			mutable size_t refcount = 0;
		};
	public:
		FlowCodex(QJsonDocument nodes);
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
		std::pair<const UI::Node&, size_t> MakeNode(std::string_view in)const;
	private:
		std::unordered_map<std::string, RefCountPair<UI::Node>> codex;
		std::unordered_map<std::string, std::vector<std::string_view>> cats;
	};
}