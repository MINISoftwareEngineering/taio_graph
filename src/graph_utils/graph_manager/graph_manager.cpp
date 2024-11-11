#include "graph_manager.h"

#pragma region general
int GraphManager::getGraphSize(GraphData& graph_data)
{
	int edges_count = 0;
	for each (const auto & out_edges in graph_data.out_edges_by_node)
		edges_count += out_edges.second.size();

	return edges_count;
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

bool GraphManager::tryGetRandomNode(int nodes_count, int& random_node)
{
	return getRandomValue(nodes_count - 1, random_node);
}

void GraphManager::removeOutEdges(GraphData& graph_data, int node)
{
	for each (int in_node in graph_data.out_edges_by_node[node])
		graph_data.in_edges_by_node[in_node].erase(node);
	graph_data.out_edges_by_node.erase(node);
}

void GraphManager::addEdge(GraphData& graph_data, edge new_edge)
{
	graph_data.in_edges_by_node[new_edge.out].insert(new_edge.in);
	graph_data.out_edges_by_node[new_edge.in].insert(new_edge.out);
}

int GraphManager::getEdgesDensityMetic(GraphData& graph_data)
{
	// TO CONSIDER: can be O(1) if updated after every change in a graph

	if (graph_data.getNodesCount() == 0)
		return -1;

	int edges_count = getGraphSize(graph_data);

	int metric = edges_count / graph_data.getNodesCount();
	if (metric * graph_data.getNodesCount() < edges_count)
		metric += 1;

	return metric;
}

bool GraphManager::hasEdge(GraphData& graph_data, int out_node, int in_node)
{
	return graph_data.out_edges_by_node[out_node].find(in_node) != graph_data.out_edges_by_node[out_node].end();
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

		for each (const auto & in_pair in graph_data.in_edges_by_node)
		{
			int in_node = in_pair.first;
			const std::unordered_set<int>& in_edges = in_pair.second;

			if (!isNodeWithoutOutEdges(graph_data, in_node))
				continue;

			for each (int out_node in in_edges)
			{
				graph_data.out_edges_by_node[out_node].erase(in_node);

				if (graph_data.out_edges_by_node[out_node].empty())
					graph_data.out_edges_by_node.erase(out_node);
			}

			nodes_to_remove_in_edges.push_back(in_node);
		}

		for each (int in_node in nodes_to_remove_in_edges)
		{
			//for each (int out_node in graph_data.in_edges_by_node[in_node])
			//{
			//	graph_data(out_node, in_node) = false;
			//}

			graph_data.in_edges_by_node.erase(in_node);
			any_edge_removed = true;
		}

		for each (const auto & out_pair in graph_data.out_edges_by_node)
		{
			int out_node = out_pair.first;
			const std::unordered_set<int>& out_edges = out_pair.second;

			if (!isNodeWithoutInEdges(graph_data, out_node))
				continue;
			
			for each (int in_node in out_edges)
			{
				graph_data.in_edges_by_node[in_node].erase(out_node);

				if (graph_data.in_edges_by_node[in_node].empty())
					graph_data.in_edges_by_node.erase(in_node);
			}

			nodes_to_remove_out_edges.push_back(out_node);
		}

		for each (int out_node in nodes_to_remove_out_edges)
		{
			//for each (int in_node in graph_data.out_edges_by_node[out_node])
			//{
			//	graph_data(out_node, in_node) = false;
			//}

			graph_data.out_edges_by_node.erase(out_node);
			any_edge_removed = true;
		}

	} while (any_edge_removed);
}
#pragma endregion

#pragma region longestCycles
void GraphManager::findLongestCycles(GraphData& graph_data)
{
	throw new std::runtime_error("Method not implemented.");

	std::vector<std::vector<int>> longest_cycles;

	// TODO: actually compute those cycles

	graph_data.assignLongestCycles(longest_cycles);
}
#pragma endregion

#pragma region hamiltonCycleApproximation
bool GraphManager::tryGetRandomUnvisitedNeighbourNode(GraphData& graph_data, int node, std::unordered_set<int>& visited_nodes, int& random_neighbour_node)
{
	return getRandomValue(graph_data.out_edges_by_node[node], random_neighbour_node, visited_nodes);
}

bool GraphManager::tryGetUnvisitedNodeWithLeastInEdges(GraphData& graph_data, std::unordered_set<int>& visited_nodes, int& node_with_least_in_edges)
{
	// TODO: optimize to always keep track of unvisited nodes to avoid checking every node (visited_nodes & getNodesCount --> unvisited_nodes)

	int least_in_edges = INT_MAX;

	for (int node = 0; node < graph_data.getNodesCount(); ++node)
	{
		if (visited_nodes.find(node) != visited_nodes.end())
			continue;

		const auto& in_edges = graph_data.in_edges_by_node.find(node);

		if (in_edges == graph_data.in_edges_by_node.end())
		{
			node_with_least_in_edges = node;
			return true;
		}

		if (in_edges->second.size() < least_in_edges)
		{
			least_in_edges = in_edges->second.size();
			node_with_least_in_edges = node;
		}
	}

	if (least_in_edges == INT_MAX)
	{
		node_with_least_in_edges = -1;
		return false;
	}

	return true;
}

bool GraphManager::tryGetRandomUnvisitedNode(int nodes_count, std::unordered_set<int>& visited_nodes, int& random_unvisited_node)
{
	return getRandomValue(nodes_count - 1, random_unvisited_node, visited_nodes);
}

bool GraphManager::tryFindMinimumExtentionForHamiltonCycle(GraphData& graph_data, int retry_factor)
{
	int iterations = std::max(getEdgesDensityMetic(graph_data), 1) * retry_factor;

	for (int i = 0; i < iterations; ++i)
	{
		//std::cout << std::to_string(i) + " try: \n";

		int start_node;
		if (!tryGetRandomNode(graph_data.getNodesCount(), start_node))
			return false;

		FollowRandomPathRecData rec_data = { start_node, graph_data };
		followRandomPathRec(rec_data, start_node);
		rotateCycleToStartFromTheSmallestIndex(rec_data.followed_path);

		int current_extention_size = graph_data.getHamiltonCycleGraphExtentionSize();
		if (rec_data.graph_extention.size() >= current_extention_size && current_extention_size != -1)
			continue;

		graph_data.setHamiltonCycleGraphExtention(rec_data.graph_extention);
		graph_data.addHamiltonCycle(rec_data.followed_path);

		if (graph_data.getHamiltonCycleGraphExtention().empty())
			break;
	}

	GraphData extended_graph_data = graph_data;
	for each (edge e in graph_data.getHamiltonCycleGraphExtention())
	{
		addEdge(extended_graph_data, e);
	}
	
	iterations = getGraphSize(extended_graph_data) * std::max((int)log(retry_factor), 1);
	transformToGraphWithoutEdgesAdjecentToLeafNode(extended_graph_data);
	for (int i = 0; i < iterations; ++i)
	{
		FollowRandomPathRecData rec_data = { 0, extended_graph_data };
		followRandomPathRec(rec_data, 0, false);

		if (rec_data.followed_path.size() > 0)
			graph_data.addHamiltonCycle(rec_data.followed_path);
	}

	return true;
}

void GraphManager::rotateCycleToStartFromTheSmallestIndex(path_t& cycle)
{
	auto minIt = std::min_element(cycle.begin(), cycle.end());
	std::rotate(cycle.begin(), minIt, cycle.end());
}

void GraphManager::followRandomPathRec(FollowRandomPathRecData& rec_data, int current_node, bool allow_extending)
{
	rec_data.visited_nodes.insert(current_node);
	rec_data.followed_path.push_back(current_node);
	//std::cout << "visited node: " + std::to_string(current_node) + "\n";

	bool has_edge_to_start_node = hasEdge(rec_data.graph_data, current_node, rec_data.start_node);

	int next_node;
	bool success = tryGetRandomUnvisitedNeighbourNode(rec_data.graph_data, current_node, rec_data.visited_nodes, next_node);
	if (allow_extending)
		removeOutEdges(rec_data.graph_data, current_node);

	if (!success)
	{
		if (!allow_extending)
		{
			if (!has_edge_to_start_node || rec_data.followed_path.size() != rec_data.graph_data.getNodesCount())
				rec_data.followed_path = {};
			return;
		}

		//if (!tryGetRandomUnvisitedNode(rec_data.graph_data.getNodesCount(), rec_data.visited_nodes, next_node)) // for speedup, but might be less accurate
		if (!tryGetUnvisitedNodeWithLeastInEdges(rec_data.graph_data, rec_data.visited_nodes, next_node))
		{
			if (!has_edge_to_start_node)
				rec_data.graph_extention.insert({ current_node, rec_data.start_node });

			return;
		}

		rec_data.graph_extention.insert({ current_node, next_node });
	}

	followRandomPathRec(rec_data, next_node, allow_extending);
}
#pragma endregion
