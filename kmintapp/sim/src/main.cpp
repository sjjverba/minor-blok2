#include "kmint/main.hpp"     // voor de main loop
#include "kmint/graphics.hpp" // kleuren en afbeeldingen
#include "kmint/map/map.hpp"
#include "kmint/math/vector2d.hpp" // voor window en app
#include "kmint/play.hpp"          // voor stage
#include "kmint/ui.hpp"            // voor window en app
#include "sim/cow.hpp"
#include "sim/hare.hpp"

using namespace kmint; // alles van libkmint bevindt zich in deze namespace

// let op de enter als tussen de declaratie en definitie (regel 22),
// gaat fout als er direct uit de pdf gekopieerd wordt.
static const char* map_description = R"graph(32 24 32
resources/firstmap.png
G 1 1
I 1 2
C 1 2
H 1 1
W 0 0
B 1 8

WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
WWWHGGGGGGGGGGGGGGGGGGGGGGGGHWWW
WWWGGGGGGGGGGGGGGGGGGGGGGGGGGWWW
WWWGGGGGGGGGGGGGBGGGGGGGGGGGGWWW
WWWGGGGGGWWWWWWWBWWWWWWGGGGGGWWW
WWWGGGGGGWWWWWWWBWWWWWWGGGGGGWWW
WWWGGGGGGWWWWWWWBWWWWWWGGGGGGWWW
WWWGGGGGGWWWWWIIBIWWWWWGGGGGGWWW
WWWGGGGGGWWWWWIIIIWWWWWGGGGGGWWW
WWWGGGGGGWWWWWIIIIWWWWWGGGGGGWWW
WWWGGGGGBBBBBBBICIIIIIIGGGGGGWWW
WWWGGGGGGWWWWWIIIIWWWWWGGGGGGWWW
WWWGGGGGGWWWWWIIBIWWWWWGGGGGGWWW
WWWGGGGGGWWWWWWWBWWWWWWGGGGGGWWW
WWWGGGGGGWWWWWWWBWWWWWWGGGGGGWWW
WWWGGGGGGWWWWWWWBWWWWWWGGGGGGWWW
WWWGGGGGGGGGGGGGBGGGGGGGGGGGGWWW
WWWGGGGGGGGGGGGGGGGGGGGGGGGGGWWW
WWWHGGGGGGGGGGGGGGGGGGGGGGGGHWWW
WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
)graph";


std::size_t find_cow_node(const map::map_graph &graph) {
  for (std::size_t i = 0; i < graph.num_nodes(); ++i) {
    if (graph[i].node_info().kind == 'C') {
      return i;
    }
  }
  throw "could not find starting point";
}

int main() {
  // een app object is nodig om
  ui::app app{};

  //  maak een venster aan
  ui::window window{app.create_window({1024, 768}, "hello")};

  // maak een podium aan
  play::stage s{{1024, 768}};

  // maak een kaart
  map::map m{map::read_map(map_description)};
  s.build_actor<play::background>(math::size(1024, 768),
                                  graphics::image{m.background_image()});
  s.build_actor<play::map_actor>(math::vector2d{0.0f, 0.0f}, m.graph());

  // maak de koe en de haas
  auto &graph = m.graph();
  auto cow_node_index = find_cow_node(graph);
  sim::cow &cow = s.build_actor<sim::cow>(graph, graph[cow_node_index]);
  sim::hare &hare = s.build_actor<sim::hare>(graph);
  hare.set_cow(cow);
  cow.set_hare(hare);

  // Maak een event_source aan (hieruit kun je alle events halen, zoals
  // toetsaanslagen)
  ui::events::event_source event_source{};

  // main_loop stuurt alle actors aan.
  main_loop(s, window, [&](delta_time dt, loop_controls &ctl) {
    // gebruik dt om te kijken hoeveel tijd versterken is
    // sinds de vorige keer dat deze lambda werd aangeroepen
    // loop controls is een object met eigenschappen die je kunt gebruiken om de
    // main-loop aan te sturen.

    for (ui::events::event &e : event_source) {
      // event heeft een methode handle_quit die controleert
      // of de gebruiker de applicatie wilt sluiten, en zo ja
      // de meegegeven functie (of lambda) aanroept om met het
      // bijbehorende quit_event
      //
      e.handle_quit([&ctl](ui::events::quit_event qe) { ctl.quit = true; });
    }
  });
}
