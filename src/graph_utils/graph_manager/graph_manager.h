#pragma once
#include <config.h>
#include <graph_data.h>

struct GraphManager
{
	int getGraphSize(GraphData& graph_data);
	void transformToGraphWithoutEdgesAdjecentToLeafNode(GraphData& graph_data);
	bool isNodeWithoutOutEdges(GraphData& graph_data, int node_row);
	bool isNodeWithoutInEdges(GraphData& graph_data, int node_col);
	void findLongestCycles(GraphData& graph_data);
	bool tryFindMinimumExtentionForHamiltonCycle(GraphData& graph_data);

private:
	struct FollowRandomPathRecData
	{
		int start_node;
		GraphData graph_data;
		std::unordered_set<int> visited_nodes;
		std::vector<edge> graph_extention;
	};

	void removeOutEdges(GraphData& graph_data, int node);
	bool tryGetRandomUnvisitedNode(int nodes_count, std::unordered_set<int>& visited_nodes, int& node_with_least_in_edges);
	bool tryGetRandomNode(int nodes_count, int& random_node);
	bool tryGetRandomUnvisitedNeighbourNode(GraphData& graph_data, int node, std::unordered_set<int>& visited_nodes, int& random_neighbour_node);
	bool tryGetUnvisitedNodeWithLeastInEdges(GraphData& graph_data, std::unordered_set<int>& visited_nodes, int& random_unvisited_node);
	void followRandomPathRec(FollowRandomPathRecData& rec_data, int current_node);
};