#pragma once
#include <config.h>
#include <graph_data.h>

struct GraphManager
{
#pragma region general
public:
	int getGraphSize(GraphData& graph_data);

private:
	int getEdgesDensity(GraphData& graph_data);
	void transformToGraphWithoutEdgesAdjecentToLeafNode(GraphData& graph_data);
	void addEdge(GraphData& graph_data, edge new_edge);
	bool isNodeWithoutOutEdges(GraphData& graph_data, int node_row);
	bool isNodeWithoutInEdges(GraphData& graph_data, int node_col);
	void removeOutEdges(GraphData& graph_data, int node);
	void removeInEdges(GraphData& graph_data, int node);
	bool tryGetRandomNode(int nodes_count, int& random_node);
	bool hasEdge(GraphData& graph_data, int out_node, int in_node);
#pragma endregion

#pragma region longestCycles
public:
	void findLongestCycles(GraphData& graph_data);
#pragma endregion

#pragma region hamiltonCycleApproximation
public:
	bool tryFindMinimumExtentionForHamiltonCycle(GraphData& graph_data, int retry_factor = 1);

private:
	struct FollowRandomPathRecData
	{
		int start_node;
		GraphData graph_data;
		std::unordered_set<int> visited_nodes;
		graph_extention_t graph_extention;
		path_t followed_path;
	};
	void rotateCycleToStartFromTheSmallestIndex(path_t& cycle);
	bool tryGetRandomUnvisitedNode(int nodes_count, std::unordered_set<int>& visited_nodes, int& node_with_least_in_edges);
	bool tryGetRandomUnvisitedNeighbourNode(GraphData& graph_data, int node, std::unordered_set<int>& visited_nodes, int& random_neighbour_node);
	bool tryGetUnvisitedNodeWithLeastInEdges(GraphData& graph_data, std::unordered_set<int>& visited_nodes, int& random_unvisited_node);
	void followRandomPathRec(FollowRandomPathRecData& rec_data, int current_node, bool allow_extending = true, int forced_next_node = -1);
#pragma endregion
};