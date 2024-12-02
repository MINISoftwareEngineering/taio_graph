#include "graph_manager.h"
#include <functional>

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

void GraphManager::removeInEdges(GraphData& graph_data, int node)
{
	for each (int in_node in graph_data.in_edges_by_node[node])
		graph_data.out_edges_by_node[in_node].erase(node);
	graph_data.in_edges_by_node.erase(node);
}

void GraphManager::addEdge(GraphData& graph_data, edge new_edge)
{
	graph_data.in_edges_by_node[new_edge.end].insert(new_edge.start);
	graph_data.out_edges_by_node[new_edge.start].insert(new_edge.end);
}

int GraphManager::getEdgesDensity(GraphData& graph_data)
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

bool GraphManager::hasEdge(GraphData& graph_data, int start_node, int end_node)
{
	if (graph_data.out_edges_by_node.count(start_node) == 0)
		return false;
	return graph_data.out_edges_by_node[start_node].find(end_node) != graph_data.out_edges_by_node[start_node].end();
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

/// <summary>
/// void generateGraphPermutations(GraphData& graph_data, std::function<bool(GraphData)> callback)
/// </summary>
/// <param name="graph_data">
///		graph for which we perform the permutations
/// </param>
/// <param name="callback">
///		a callback function that returns true if we want to continue with the permutations or false if we want to end,
///		this allows us to perform operations on permuted graphs one by one instead of having to hold them all in memory
/// </param>
void GraphManager::generateGraphPermutations(GraphData& graph_data, std::function<bool(GraphData)> callback) 
{
	std::vector<int> nodes;

	for (int i = 0; i < graph_data.getNodesCount(); i++) {
		nodes.push_back(i);
	}

	std::sort(nodes.begin(), nodes.end());

	do {
		GraphData permuted_graph;
		std::unordered_map<int, int> nodes_mapping;
		for (int i = 0; i < nodes.size(); i++) {
			nodes_mapping[nodes[i]] = i;
		}

		for (const auto& [node, edges] : graph_data.out_edges_by_node) {
			int permuted_node_index = nodes_mapping[node];
			for (int edge : edges) {
				permuted_graph.out_edges_by_node[permuted_node_index].insert(nodes_mapping[edge]);
			}
		}

		for (const auto& [node, edges] : graph_data.in_edges_by_node) {
			int permuted_node_index = nodes_mapping[node];
			for (int edge : edges) {
				permuted_graph.in_edges_by_node[permuted_node_index].insert(nodes_mapping[edge]);
			}
		}

		permuted_graph.setNodesCount(graph_data.getNodesCount());
		permuted_graph.setInitialize(true);

		if (!callback(permuted_graph)) {
			break;
		}


	} while (std::next_permutation(nodes.begin(), nodes.end()));
}

#pragma endregion



#pragma region longestCycles

void GraphManager::traverseNeighbours(GraphData& graph_data, std::vector<std::vector<int>>& longest_cycles, std::vector<int>& current_path, int& longest_cycle_length)
{
	int last_vertex = current_path.back();
	std::unordered_set<int> out_neighbours = graph_data.out_edges_by_node[last_vertex];
	for (int neighbour : out_neighbours)
	{
		if (neighbour == current_path.front())
		{
			if (current_path.size() >= longest_cycle_length)
			{
				if (current_path.size() > longest_cycle_length)
				{
					longest_cycle_length = current_path.size();
					longest_cycles.clear();
				}

				std::vector<int> ordered_cycle;
				auto min_vertex = std::min_element(current_path.begin(), current_path.end());
				int min_index = std::distance(current_path.begin(), min_vertex);
				ordered_cycle.insert(ordered_cycle.end(), current_path.begin() + min_index, current_path.end());
				ordered_cycle.insert(ordered_cycle.end(), current_path.begin(), current_path.begin() + min_index);
				ordered_cycle.push_back(ordered_cycle.front());
				if (std::find(longest_cycles.begin(), longest_cycles.end(), ordered_cycle) == longest_cycles.end())
					longest_cycles.push_back(ordered_cycle);
			}
		}
		if (std::find(current_path.begin(), current_path.end(), neighbour) == current_path.end())
		{
			current_path.push_back(neighbour);
			traverseNeighbours(graph_data, longest_cycles, current_path, longest_cycle_length);
			current_path.pop_back();
		}
	}
}

void GraphManager::findLongestCycles(GraphData& graph_data)
{
	auto start = std::chrono::high_resolution_clock::now();

	std::vector<std::vector<int>> longest_cycles;
	std::vector<int> current_path;

	int longest_cycle_length = 0;

	for (int i = 0; i < graph_data.getNodesCount(); i++)
	{
		current_path.push_back(i);
		traverseNeighbours(graph_data, longest_cycles, current_path, longest_cycle_length);
		current_path.pop_back();
	}
	graph_data.assignLongestCycles(longest_cycles);

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	graph_data.assignLongestCyclesTime(duration);
}

#pragma endregion


#pragma region longestCyclesApproximation
void GraphManager::tryFindLongestCycles(GraphData& graph_data)
{
	auto start = std::chrono::high_resolution_clock::now();

	std::vector<std::vector<int>> resulting_cycles;
	std::stack<std::pair<int, int>> stack_of_cycles;
	GraphData temp_graph;
	temp_graph.out_edges_by_node = graph_data.out_edges_by_node;
	temp_graph.in_edges_by_node = graph_data.in_edges_by_node;
	temp_graph.setNodesCount(graph_data.getNodesCount());
	temp_graph.setInitialize(true);
	transformToGraphWithoutEdgesAdjecentToLeafNode(temp_graph);
	std::vector<int> numbers(temp_graph.getNodesCount(), 0);
	std::vector<int> parent(temp_graph.getNodesCount(), -1);
	std::vector<bool> visited(temp_graph.getNodesCount(), false);
	int lastLargestCycle = 0;
	for (int start = 0; start < temp_graph.getNodesCount(); ++start) {

		std::fill(numbers.begin(), numbers.end(), 0);
		std::fill(parent.begin(), parent.end(), -1);
		std::fill(visited.begin(), visited.end(), false);

		numbers[start] = 1;

		std::stack<int> stack;
		stack.push(start);

		while (!stack.empty()) {
			int current = stack.top();
			stack.pop();

			if (visited[current]) {
				continue;
			}

			visited[current] = true;
			std::vector<int> neighbors(temp_graph.out_edges_by_node[current].begin(), temp_graph.out_edges_by_node[current].end());

			while (!neighbors.empty()) {

				int neighbor = takeOutRandomValue(neighbors);

				if (numbers[neighbor] == 0) {
					numbers[neighbor] = numbers[current] + 1;
					parent[neighbor] = current;
					stack.push(neighbor);
				}
				else {
					int newDifference = numbers[current] - numbers[neighbor];
					if (newDifference > 2 && newDifference >= lastLargestCycle) {

						bool isProperCycle = false;
						int checkVert = current;

						while (checkVert != -1) {
							checkVert = parent[checkVert];
							if (checkVert == neighbor) {
								isProperCycle = true;
								break;
							}
						}

						if (newDifference > lastLargestCycle && isProperCycle) {
							lastLargestCycle = newDifference;
							resulting_cycles.clear();
							while (!stack_of_cycles.empty()) stack_of_cycles.pop();
							stack_of_cycles.push({ neighbor, current });
						}
						else if (isProperCycle) {
							stack_of_cycles.push({ neighbor, current });
						}
					}
				}
			}
		}

		while (!stack_of_cycles.empty()) {
			auto [startCycle, endCycle] = stack_of_cycles.top();
			stack_of_cycles.pop();

			std::vector<int> cycle;
			int currentVertex = endCycle;
			while (currentVertex != startCycle) {
				cycle.push_back(currentVertex);
				currentVertex = parent[currentVertex];
			}
			cycle.push_back(startCycle);
			std::reverse(cycle.begin(), cycle.end());

			bool isUnique = true;
			for (int i = 0; i < resulting_cycles.size(); i++) {
				if (isSameCycle(cycle, resulting_cycles[i])) {
					isUnique = false;
					break;
				}
			}

			if (isUnique) {
				resulting_cycles.push_back(cycle);
			}
		}
	}
	graph_data.assignApproximateLongestCycles(resulting_cycles);

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	graph_data.assignApproximateLongestCyclesTime(duration);
}
bool GraphManager::isSameCycle(const std::vector<int>& cycle1, const std::vector<int>& cycle2)
{
	if (cycle1.size() != cycle2.size()) {
		return false;
	}
	int startVertex = cycle1[0];
	int cycleSize = cycle1.size();
	auto it = std::find(cycle2.begin(), cycle2.end(), startVertex);
	if (it == cycle2.end()) {
		return false;
	}
	int startIdxInCycle2 = std::distance(cycle2.begin(), it);
	bool isSameClockwise = true;
	bool isSameCounterClockwise = true;
	for (int i = 0; i < cycleSize; ++i) {
		if (cycle1[i] != cycle2[(startIdxInCycle2 + i) % cycleSize]) {
			isSameClockwise = false;
		}
		if (cycle1[i] != cycle2[(startIdxInCycle2 - i + cycleSize) % cycleSize]) {
			isSameCounterClockwise = false;
		}
		if (!isSameClockwise && !isSameCounterClockwise) {
			return false;
		}
	}
	return true;
}
int GraphManager::takeOutRandomValue(std::vector<int>& vec) {
	if (vec.empty()) {
		return -1;
	}
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, vec.size() - 1);
	int randomIndex = dis(gen);
	int value = vec[randomIndex];
	std::swap(vec[randomIndex], vec.back());
	vec.pop_back();
	return value;
}
#pragma endregion


#pragma region hamiltonCycle
void GraphManager::traverseOtherVertices(GraphData& graph_data, std::vector<std::pair<int, int>>& smallest_extension, std::vector<std::pair<int, int>>& current_extension, std::vector<int>& current_path, int& hamilton_cycle_count)
{
	int last_vertex = current_path.back();
	std::unordered_set<int> out_neighbours = graph_data.out_edges_by_node[last_vertex];

	if (current_path.size() == graph_data.getNodesCount())
	{
		if (out_neighbours.find(current_path.front()) == out_neighbours.end())
			current_extension.push_back(std::pair<int, int>(last_vertex, current_path.front()));

		if (current_extension.size() == 0)
		{
			smallest_extension.clear();
			hamilton_cycle_count++;
		}


		if (hamilton_cycle_count == 0)
		{
			if (smallest_extension.size() == 0 || smallest_extension.size() > current_extension.size()) // No extensions found yet or the new one is shorter
				smallest_extension = current_extension;
		}

		if (out_neighbours.find(current_path.front()) == out_neighbours.end())
			current_extension.pop_back();
		return;
	}


	for (int i = 0; i < graph_data.getNodesCount(); i++)
	{
		if (std::find(current_path.begin(), current_path.end(), i) == current_path.end())
		{
			if (out_neighbours.find(i) == out_neighbours.end())
				current_extension.push_back(std::pair<int, int>(last_vertex, i));
			current_path.push_back(i);
			traverseOtherVertices(graph_data, smallest_extension, current_extension, current_path, hamilton_cycle_count);
			if (out_neighbours.find(i) == out_neighbours.end())
				current_extension.pop_back();
			current_path.pop_back();
		}
	}

}

void GraphManager::findHamiltonCycle(GraphData& graph_data)
{
	auto start = std::chrono::high_resolution_clock::now();

	std::vector<std::pair<int, int>> smallest_extension;
	std::vector<std::pair<int, int>> current_extension;
	std::vector<int> current_path;

	int hamilton_cycle_count = 0;

	for (int i = 0; i < graph_data.getNodesCount(); i++)
	{
		current_path.push_back(i);
		traverseOtherVertices(graph_data, smallest_extension, current_extension, current_path, hamilton_cycle_count);
		current_path.pop_back();
	}

	hamilton_cycle_count /= graph_data.getNodesCount();
	graph_data.assignHamiltonCyclesAndExtensions(smallest_extension, hamilton_cycle_count);

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	graph_data.AssignPreciseHamiltonCycleTime(duration);
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
	int iterations = std::max(getEdgesDensity(graph_data), 1) * retry_factor;

	for (int i = 0; i < iterations; ++i)
	{
		int start_node;
		if (!tryGetRandomNode(graph_data.getNodesCount(), start_node))
			return false;

		FollowRandomPathRecData rec_data = { start_node, graph_data };
		followRandomPathRec(rec_data, start_node);

		int current_extention_size = graph_data.getHamiltonCycleGraphExtentionSize();
		if (rec_data.graph_extention.size() >= current_extention_size && current_extention_size != -1)
			continue;

		graph_data.setHamiltonCycleGraphExtention(rec_data.graph_extention);
		rotateCycleToStartFromTheSmallestIndex(rec_data.followed_path);
		graph_data.addHamiltonCycle(rec_data.followed_path);

		if (graph_data.getHamiltonCycleGraphExtention().empty())
			break;
	}

	return true;
}

bool GraphManager::findAllHamiltonCycles(GraphData& graph_data, int retry_factor)
{
	GraphData extended_graph_data = graph_data;
	for each (edge e in graph_data.getHamiltonCycleGraphExtention())
		addEdge(extended_graph_data, e);
	int graphSize = getGraphSize(extended_graph_data);

	if (!graph_data.getHamiltonCycleGraphExtention().empty())
	{
		for each (edge e in graph_data.getHamiltonCycleGraphExtention())
		{
			int iterations = graphSize * std::max((int)log(retry_factor), 1);
			for (int i = 0; i < iterations; ++i)
			{
				FollowRandomPathRecData rec_data = { e.start, extended_graph_data };
				followRandomPathRec(rec_data, e.start, false, e.end);

				if (rec_data.followed_path.size() > 0)
				{
					rotateCycleToStartFromTheSmallestIndex(rec_data.followed_path);
					graph_data.addHamiltonCycle(rec_data.followed_path);
				}
			}
		}

		return true;
	}

	int iterations = graphSize * std::max((int)log(retry_factor), 1);
	for (int i = 0; i < iterations; ++i)
	{
		FollowRandomPathRecData rec_data = { 0, extended_graph_data };
		followRandomPathRec(rec_data, 0, false);

		if (rec_data.followed_path.size() > 0)
			graph_data.addHamiltonCycle(rec_data.followed_path);
	}

	return true;
}

bool GraphManager::tryFindMinimumExtentionForHamiltonCycleAndAllHamiltonCycles(GraphData& graph_data, int retry_factor)
{
	bool result = measure_execution_time(
		graph_data.findMinimumExtentionForHamiltonCycleExecutionTimeMs, 
		[&]() { return tryFindMinimumExtentionForHamiltonCycle(graph_data, retry_factor); }
	);

	if (!result) 
		return false;

	result = measure_execution_time(
		graph_data.findAllHamiltonCyclesExecutionTimeMs,
		[&]() { return findAllHamiltonCycles(graph_data, retry_factor); }
	);

	return result;
}

void GraphManager::rotateCycleToStartFromTheSmallestIndex(path_t& cycle)
{
	auto minIt = std::min_element(cycle.begin(), cycle.end());
	std::rotate(cycle.begin(), minIt, cycle.end());
}

void GraphManager::followRandomPathRec(FollowRandomPathRecData& rec_data, int current_node, bool allow_extending, int forced_next_node)
{
	rec_data.visited_nodes.insert(current_node);
	rec_data.followed_path.push_back(current_node);
	//std::cout << "visited node: " + std::to_string(current_node) + "\n";

	bool has_edge_to_start_node = hasEdge(rec_data.graph_data, current_node, rec_data.start_node);

	int next_node;
	bool success = true;
	if (forced_next_node != -1)
		next_node = forced_next_node;
	else
		success = tryGetRandomUnvisitedNeighbourNode(rec_data.graph_data, current_node, rec_data.visited_nodes, next_node);
	
	if (current_node != rec_data.start_node)
		removeInEdges(rec_data.graph_data, current_node);
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

#pragma region metric
int GraphManager::getMetricDistance(GraphData graph_1, GraphData graph_2)
{
	int distance = 0;
	GraphData larger_graph = graph_1;
	GraphData smaller_graph = graph_2;

	if (graph_1.getNodesCount() < graph_2.getNodesCount())
	{
		smaller_graph = graph_1;
		larger_graph = graph_2;
	}

	// TODO: do it for every possible smaller_graph nodes mapping to larger_graph nodes and add smallest distance
	#pragma region addingOrRemovingEdgesCount
	int size_difference = larger_graph.getNodesCount() - smaller_graph.getNodesCount();
	distance = std::numeric_limits<int>::max();
	smaller_graph.setNodesCount(larger_graph.getNodesCount());

	generateGraphPermutations(smaller_graph, [&distance, this, size_difference, &larger_graph](GraphData permutedGraph) {
			
			int current_distance = getEditDistance(larger_graph, permutedGraph, size_difference);

			if (current_distance < distance) {
				distance = current_distance;
			}

			if (distance == 0)
				return false;

			return true;
		});

	#pragma endregion

	return distance;
}

int GraphManager::getEditDistance(GraphData graph_1, GraphData graph_2, int size_difference) {
	GraphData larger_graph = graph_1;
	GraphData smaller_graph = graph_2;

	if (graph_1.getNodesCount() < graph_2.getNodesCount())
	{
		smaller_graph = graph_1;
		larger_graph = graph_2;
	}

	int current_distance = size_difference;

	std::set<edge> different_edges;
	for (int start_node = 0; start_node < smaller_graph.getNodesCount(); ++start_node) {
		for (int end_node = 0; end_node < smaller_graph.getNodesCount(); ++end_node)
		{
			int edge_count = 0;
			if (hasEdge(smaller_graph, start_node, end_node))
				++edge_count;
			if (hasEdge(larger_graph, start_node, end_node))
				++edge_count;
			if (edge_count == 1)
				different_edges.insert({ start_node, end_node });

		}
	}

	current_distance += different_edges.size();
	return current_distance;
}
#pragma endregion


#pragma region metricApproximation
int GraphManager::tryGetMetricDistance(GraphData graph_1, GraphData graph_2) {
	sortGraph(graph_1);
	sortGraph(graph_2);

	GraphData larger_graph = graph_1;
	GraphData smaller_graph = graph_2;

	if (graph_1.getNodesCount() < graph_2.getNodesCount())
	{
		smaller_graph = graph_1;
		larger_graph = graph_2;
	}

	int size_difference = larger_graph.getNodesCount() - smaller_graph.getNodesCount();
	smaller_graph.setNodesCount(larger_graph.getNodesCount());

	return getEditDistance(graph_1, graph_2, size_difference);
}

void GraphManager::sortGraph(GraphData& graph)
{
	std::vector<int> nodes;
	for (int i = 0; i < graph.getNodesCount(); i++) {
		nodes.push_back(i);
	}


	std::stable_sort(nodes.begin(), nodes.end(), [&graph](int a, int b) {
		size_t out_edges_a = graph.out_edges_by_node[a].size();
		size_t out_edges_b = graph.out_edges_by_node[b].size();
		return out_edges_a > out_edges_b;
		});

	std::stable_sort(nodes.begin(), nodes.end(), [&graph](int a, int b) {
		size_t in_edges_a = graph.in_edges_by_node[a].size();
		size_t in_edges_b = graph.in_edges_by_node[b].size();
		return in_edges_a > in_edges_b;
		});

	std::unordered_map<int, std::unordered_set<int>> sorted_out_edges;
	std::unordered_map<int, std::unordered_set<int>> sorted_in_edges;

	int sorted_index = 0;
	for (int node : nodes)
	{
		sorted_out_edges[sorted_index] = std::move(graph.out_edges_by_node[node]);
		sorted_in_edges[sorted_index] = std::move(graph.in_edges_by_node[node]);
		sorted_index++;
	}

	graph.out_edges_by_node = std::move(sorted_out_edges);
	graph.in_edges_by_node = std::move(sorted_in_edges);
}

#pragma endregion
