#pragma once
#include <unordered_map>
#include <memory>
#include <string>

namespace ver {
	struct Descriptor;
}
class QJsonDocument;
namespace UI
{
	class INode;
	class FlowCodex
	{
		static constexpr std::string_view NodesDir = "nodes";
	public:
		FlowCodex();
		~FlowCodex();
	public:
		const auto& CatMap()const noexcept
		{
			return cats;
		}

		std::unique_ptr<UI::INode> GetNode(std::string_view nodety)const;
		bool contains(std::string_view nodety)const;
		void ClearCounts();
	private:
		void ParseJson(const QJsonDocument& json);
	private:
		std::unordered_map<std::string, std::unique_ptr<ver::Descriptor>> codex;
		std::unordered_map<std::string, std::vector<std::string_view>> cats;
	};
}