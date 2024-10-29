#pragma once
#include <config.h>
#include <graph_data.h>

struct GraphManager
{
	int getGraphSize(GraphData& graph_data);
	void transformToGraphWithoutEdgesAdjecentToLeafNode(GraphData& graph_data);
	bool isNodeWithoutOutEdges(GraphData& graph_data, int node_row);
	bool isNodeWithoutInEdges(GraphData& graph_data, int node_col);
};