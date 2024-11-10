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

bool GraphManager::tryGetRandomUnvisitedNeighbourNode(GraphData& graph_data, int node, std::unordered_set<int>& visited_nodes, int& random_neighbour_node)
{
	return getRandomValue(graph_data.out_edges_by_node[node], random_neighbour_node, visited_nodes);
}

bool GraphManager::tryGetRandomUnvisitedNode(GraphData& graph_data, std::unordered_set<int> visited_nodes, int& random_unvisited_node)
{
	std::vector<int> unvisited_nodes;
	for (int i = 0; i < graph_data.getNodesCount(); ++i)
		if (visited_nodes.find(i) == visited_nodes.end())
			unvisited_nodes.push_back(i);

	if (unvisited_nodes.empty()) {
		random_unvisited_node = -1;
		return false;
	}

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, unvisited_nodes.size() - 1);

	random_unvisited_node = unvisited_nodes[dis(gen)];
	return true;
}

void GraphManager::removeOutEdges(GraphData& graph_data, int node)
{
	for each (int in_node in graph_data.out_edges_by_node[node])
		graph_data.in_edges_by_node[in_node].erase(node);
	graph_data.out_edges_by_node.erase(node);
}

void GraphManager::findMinimumExtentionForHamiltonCycle(GraphData& graph_data)
{
	std::unordered_set<int> visited_nodes;
	std::vector<edge> graph_extention;
	int start_node = graph_data.out_edges_by_node.begin()->first;

	followRandomPathRec(graph_data, visited_nodes, graph_extention, start_node, start_node);
}

void GraphManager::followRandomPathRec(GraphData& graph_data, std::unordered_set<int>& visited_nodes, std::vector<edge>& graph_extention, int start_node, int current_node)
{
	visited_nodes.insert(current_node);
	std::cout << "visited node: " + std::to_string(current_node) + "\n";
	
	int next_node;
	bool success = tryGetRandomUnvisitedNeighbourNode(graph_data, current_node, visited_nodes, next_node);
	removeOutEdges(graph_data, current_node);

	if (!success)
	{
		if (!tryGetRandomUnvisitedNode(graph_data, visited_nodes, next_node))
			return;

		graph_extention.push_back({ current_node, next_node });
		// getUnvisitedUnreachableNodeWithLeastInEdges();
	}

	followRandomPathRec(graph_data, visited_nodes, graph_extention, start_node, next_node);
}
