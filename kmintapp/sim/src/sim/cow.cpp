// your code here
// Don't forget to add this file in your CMakeLists.txt
#include "sim/cow.hpp"
#include "sim/hare.hpp"
#include "kmint/random.hpp"
#include "sim/algorithms.hpp"

using namespace kmint;

namespace sim {

	static const char* cow_image = "resources/cow.png";
	cow::cow(map::map_graph& g, map::map_node& initial_node)
		: play::map_bound_actor{ initial_node },
		drawable_{ *this, kmint::graphics::image{cow_image, 0.1} },
		graph{ g } {}


	void cow::act(delta_time dt) {
		t_passed_ += dt;
		if (to_seconds(t_passed_) >= 1) {
			graph.untag_all();
			// pick random edge
			int next_index = random_int(0, node().num_edges());
			this->node(node()[next_index].to());
			t_passed_ = from_seconds(0);

			auto result = algorithms::dijkstra(this->node(), hare_->node(), graph);

			for (auto value : result.visited)
			{
				graph[value].tag(graph::node_tag::visited);
			}
			
			for (auto value : result.path)
			{
				graph[value].tag(graph::node_tag::path);
			}		
		}
	}


} // namespace sim