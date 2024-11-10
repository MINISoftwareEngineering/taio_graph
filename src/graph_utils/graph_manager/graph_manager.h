#pragma once
#include <config.h>
#include <graph_data.h>

struct GraphManager
{
#pragma region general
public:
	int getGraphSize(GraphData& graph_data);
	
private:
	bool isNodeWithoutOutEdges(GraphData& graph_data, int node_row);
	bool isNodeWithoutInEdges(GraphData& graph_data, int node_col);
	void removeOutEdges(GraphData& graph_data, int node);
	bool tryGetRandomNode(int nodes_count, int& random_node);
#pragma endregion

#pragma region longestCycles
public:
	void findLongestCycles(GraphData& graph_data);
	void transformToGraphWithoutEdgesAdjecentToLeafNode(GraphData& graph_data);
#pragma endregion

#pragma region hamiltonCycleAproximation
public:
	bool tryFindMinimumExtentionForHamiltonCycle(GraphData& graph_data);

private:
	struct FollowRandomPathRecData
	{
		int start_node;
		GraphData graph_data;
		std::unordered_set<int> visited_nodes;
		std::vector<edge> graph_extention;
	};
	bool tryGetRandomUnvisitedNode(int nodes_count, std::unordered_set<int>& visited_nodes, int& node_with_least_in_edges);
	bool tryGetRandomUnvisitedNeighbourNode(GraphData& graph_data, int node, std::unordered_set<int>& visited_nodes, int& random_neighbour_node);
	bool tryGetUnvisitedNodeWithLeastInEdges(GraphData& graph_data, std::unordered_set<int>& visited_nodes, int& random_unvisited_node);
	void followRandomPathRec(FollowRandomPathRecData& rec_data, int current_node);
#pragma endregion
};