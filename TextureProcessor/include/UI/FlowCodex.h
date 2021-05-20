/**
 * @file FlowCodex.h
 * @author Ilya Doroshenko (xdoros01), David Černý (xcerny74)
 * @brief Class for loading Node definitions from files
 */
#pragma once
#include <UI/TextureNode.h> 
#include <unordered_map>
#include <utils/polymorphic_value.h>
#include <Logic/Engine.h>
#include <filesystem>

namespace pv = isocpp_p0201;

namespace UI
{
	class FlowCodex
	{
		static constexpr std::string_view NodesDir = "nodes";
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
		FlowCodex();
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
		const pv::polymorphic_value<UI::Node>& GetNode(std::string_view nodety)const;
		const pv::polymorphic_value<UI::Node>* TryGetNode(std::string_view nodety)const;
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