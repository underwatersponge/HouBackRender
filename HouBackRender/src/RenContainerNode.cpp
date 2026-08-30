#include "RenContainerNode.h"

RenContainerNode::RenContainerNode()
{
}

RenContainerNode::RenContainerNode(const std::string& hipPath, const std::vector<std::string>& renNodes)
{
	Add(hipPath, renNodes);
}

void RenContainerNode::Add(const std::string& hipPath, const std::vector<std::string>& renNodes)
{
	if (!hipPath.empty())
	{
		m_HipFilePath = hipPath;
		m_RenNodes = renNodes;
	}
}

void RenContainerNode::Remove()
{
	m_HipFilePath = std::string();
	m_RenNodes.clear();
}

void RenContainerNode::AddRenNode(const std::string& renNode)
{
	m_RenNodes.push_back(renNode);
}

const std::string& RenContainerNode::GetHipFilePath() const
{
	return m_HipFilePath;
}

// 
const std::vector<std::string>& RenContainerNode::GetRenNodes() const
{
	return m_RenNodes;
}
