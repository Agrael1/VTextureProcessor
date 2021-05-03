#pragma once
#include <UI/TextureNode.h> 
#include <unordered_map>
#include <utils/polymorphic_value.h>
#include <Presenter/GLInterpret.h>

namespace pv = isocpp_p0201;

namespace UI
{
	class FlowCodex
	{
		struct RefCountPair : public pv::polymorphic_value<UI::Node>
		{
			template<typename T, typename ...Args>
			static RefCountPair set(Args&&... in)
			{
				return { pv::polymorphic_value<UI::Node>(pv::make_polymorphic_value<T>(std::forward<Args>(in)...)) };
			}
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
		std::pair<const pv::polymorphic_value<UI::Node>&, size_t> MakeNode(std::string_view in)const;
	private:
		std::unordered_map<std::string, RefCountPair> codex;
		std::unordered_map<std::string, std::vector<std::string_view>> cats;
		Engine engine;
	};
}