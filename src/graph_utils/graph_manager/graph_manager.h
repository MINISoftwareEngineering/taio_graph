#pragma once
#include <config.h>
#include <graph_data.h>
#include <functional>

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
	bool hasEdge(GraphData& graph_data, int start_node, int end_node);
public:
	void generateGraphPermutations(GraphData& graph_data, std::function<bool(GraphData)> callback);
#pragma endregion

#pragma region longestCycles
public:
	void findLongestCycles(GraphData& graph_data);
#pragma endregion

#pragma region longestCyclesApproximation
public:
	bool tryFindLongestCycles(GraphData& graph_data);
private:
	bool isSameCycle(const std::vector<int>& cycle1, const std::vector<int>& cycle2);
	int takeOutRandomValue(std::vector<int>& vec);
#pragma endregion

#pragma region hamiltonCycle
#pragma endregion

#pragma region hamiltonCycleApproximation
public:
	bool tryFindMinimumExtentionForHamiltonCycleAndAllHamiltonCycles(GraphData& graph_data, int retry_factor = 1);

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
	bool tryFindMinimumExtentionForHamiltonCycle(GraphData& graph_data, int retry_factor);
	bool findAllHamiltonCycles(GraphData& graph_data, int retry_factor);
#pragma endregion

#pragma region metric
public:
	int getMetricDistance(GraphData graph_1, GraphData graph_2);
private:
	int getEditDistance(GraphData graph_1, GraphData graph_2, int size_difference);
#pragma endregion

#pragma region metricApproximation
public:
	int tryGetMetricDistance(GraphData graph_1, GraphData graph_2);
	void sortGraph(GraphData& graph);
#pragma endregion
};