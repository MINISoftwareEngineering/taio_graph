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
	void findMinimumExtentionForHamiltonCycle(GraphData& graph_data);

private:
	void removeOutEdges(GraphData& graph_data, int node);
	bool tryGetRandomUnvisitedNode(GraphData& graph_data, std::unordered_set<int> visited_nodes, int& random_unvisited_node);
	bool tryGetRandomUnvisitedNeighbourNode(GraphData& graph_data, int node, std::unordered_set<int>& visited_nodes, int& random_neighbour_node);
	void followRandomPathRec(GraphData& graph_data, std::unordered_set<int>& visited_nodes, std::vector<edge>& graph_extention, int start_node, int current_node);
};