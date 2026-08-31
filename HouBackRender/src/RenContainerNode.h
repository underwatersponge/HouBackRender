#pragma once
#include <vector>
#include <string>
// Should be a meaningful name? what is better?

class RenContainerNode 
{
public:
	RenContainerNode();
	RenContainerNode(const std::string& hipPath = std::string(), const std::vector<std::string>& renNodes = std::vector<std::string>());

	void Add(const std::string& hipPath = std::string(), const std::vector<std::string>& renNodes = std::vector<std::string>());
	void Remove();

	void AddRenNode(const std::string& renNode);
	const std::string& GetHipFilePath() const;
	std::vector<std::string>& GetRenNodes();
private:
	std::string m_HipFilePath;
	std::vector<std::string> m_RenNodes;
};