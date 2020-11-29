#ifndef SIM_ALG_HPP
#define SIM_ALG_HPP

#include <vector>
#include "kmint/map/map.hpp"


namespace sim
{
	struct result
	{
		std::list<size_t> path;
		std::vector<size_t> visited;
	};

	class algorithms
	{
	public:
		static result dijkstra(const kmint::graph::basic_node<kmint::map::map_node_info>& node,
		                       const kmint::graph::basic_node<kmint::map::map_node_info>& target, 
							   const kmint::map::map_graph& graph);
	};
}

#endif
