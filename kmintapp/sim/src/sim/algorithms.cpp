#include "sim/algorithms.hpp"
#include <queue>
#include <thread>
#include <vector>

using namespace kmint;

namespace sim
{
	struct comparer
	{
		bool operator() (std::pair<size_t, float> first, std::pair<size_t, float> second) const
		{
			return first.second > second.second;
		}
	};


	result algorithms::dijkstra(const graph::basic_node<map::map_node_info>& node,
	                            const graph::basic_node<map::map_node_info>& target,
	                            const map::map_graph& graph)
	{		
		std::priority_queue<std::pair<size_t,float>,std::vector<std::pair<size_t,float>>,comparer> priority_queue;
		std::vector<float> costs(graph.num_nodes(), INT_MAX);
		std::unordered_map<size_t, size_t> parents;
		result result;
		
		priority_queue.push(std::make_pair(node.node_id(), 0));
		costs[node.node_id()] = 0;

		while (!priority_queue.empty())
		{
			auto [id,cost] = priority_queue.top();
			priority_queue.pop();

			if (id == target.node_id())
				break;

			for (const auto& edge : graph[id])
			{
				auto edge_id = edge.to().node_id();
				auto weight = edge.weight();
				auto edge_location = graph[edge_id].location();
				auto location = graph[target.node_id()].location();

				auto xDiff = edge_location.x() > location.x() ? edge_location.x() - location.x() : location.x() - edge_location.x();
				auto yDiff = edge_location.y() > location.y() ? edge_location.y() - location.y() : location.y() - edge_location.y();

				xDiff = xDiff == 0.0f ? 0.0f : xDiff / 32;
				yDiff = yDiff == 0.0f ? 0.0f : yDiff / 32;
				
				result.visited.push_back(edge_id);	
				auto newCost = costs[id] + weight;
				
				if(costs[edge_id] > newCost)
				{
					costs[edge_id] = costs[id] + weight;
					priority_queue.push(std::make_pair(edge_id, costs[id] + weight + (xDiff + yDiff)));
					parents[edge_id] = id;
				}
			}
		}


		size_t current = target.node_id();

		while (!(parents.find(current) == parents.end()))
		{
			auto& parent_id = parents[current];
			result.path.push_front(parent_id);
			current = parent_id;
		}		
		
		return result;
	}

}
