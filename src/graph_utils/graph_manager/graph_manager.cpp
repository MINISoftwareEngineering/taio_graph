#include "graph_manager.h"

int GraphManager::getGraphSize(GraphData& graph_data)
{
	// TO CONSIDER: calculate when loading data and later adjust after any graph change

	int graph_size = 0;

	for (int row = 0; row < graph_data.getNodesCount(); ++row)
	{
		for (int col = 0; col < graph_data.getNodesCount(); ++col)
		{
			if (graph_data(row, col)) 
				++graph_size;
		}
	}

	return graph_size;
}

void GraphManager::transformToGraphWithoutEdgesAdjecentToLeafNode(GraphData& graph_data)
{
	bool any_edge_removed;
	std::vector<int> nodes_to_remove_out_edges;
	std::vector<int> nodes_to_remove_in_edges;

	do
	{
		any_edge_removed = false;
		nodes_to_remove_in_edges.clear();
		nodes_to_remove_out_edges.clear();

		for each (const auto& in_pair in graph_data.in_edges_by_node)
		{
			int in_node = in_pair.first;
			const std::unordered_set<int>& in_edges = in_pair.second;

			if (isNodeWithoutOutEdges(graph_data, in_node))
			{
				for each (int out_node in in_edges)
				{
					graph_data.out_edges_by_node[out_node].erase(in_node);
					
					if (graph_data.out_edges_by_node[out_node].empty())
						graph_data.out_edges_by_node.erase(out_node);
				}

				nodes_to_remove_in_edges.push_back(in_node);
			}
		}

		for each (int in_node in nodes_to_remove_in_edges)
		{
			for each (int out_node in graph_data.in_edges_by_node[in_node])
			{
				graph_data(out_node, in_node) = false;
			}

			graph_data.in_edges_by_node.erase(in_node);
			any_edge_removed = true;
		}

		for each (const auto& out_pair in graph_data.out_edges_by_node)
		{
			int out_node = out_pair.first;
			const std::unordered_set<int>& out_edges = out_pair.second;

			if (isNodeWithoutInEdges(graph_data, out_node))
			{
				for each (int in_node in out_edges)
				{
					graph_data.in_edges_by_node[in_node].erase(out_node);
					
					if (graph_data.in_edges_by_node[in_node].empty())
						graph_data.in_edges_by_node.erase(in_node);
				}

				nodes_to_remove_out_edges.push_back(out_node);
			}
		}

		for each (int out_node in nodes_to_remove_out_edges)
		{
			for each (int in_node in graph_data.out_edges_by_node[out_node])
			{
				graph_data(out_node, in_node) = false;
			}

			graph_data.out_edges_by_node.erase(out_node);
			any_edge_removed = true;
		}

	} while (any_edge_removed);
}

bool GraphManager::isNodeWithoutOutEdges(GraphData& graph_data, int node)
{
	return graph_data.out_edges_by_node.find(node) == graph_data.out_edges_by_node.end();

	//for (int col = 0; col < graph_data.getNodesCount(); ++col)
	//{
	//	if (graph_data(node, col)) 
	//		return true;
	//}
	//return false;
}

bool GraphManager::isNodeWithoutInEdges(GraphData& graph_data, int node)
{
	return graph_data.in_edges_by_node.find(node) == graph_data.in_edges_by_node.end();

	//for (int row = 0; row < graph_data.getNodesCount(); ++row)
	//{
	//	if (graph_data(row, node))
	//		return true;
	//}
	//return false;
}

void GraphManager::findLongestCycles(GraphData& graph_data)
{
	throw new std::runtime_error("Method not implemented.");

	std::vector<std::vector<int>> longest_cycles;
	
	// TODO: actually compute those cycles

	graph_data.assignLongestCycles(longest_cycles);
}
