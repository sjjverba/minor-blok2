#ifndef KMINT_PLAY_QUAD_TREE_HPP
#define KMINT_PLAY_QUAD_TREE_HPP

#include "kmint/math/rectangle.hpp"
#include "kmint/play/actor.hpp"
#include "kmint/util/overload.hpp"
#include <array>
#include <cstddef>
#include <memory>
#include <variant>
#include <optional>

namespace kmint {
namespace play {

class actor;

template <std::size_t MaxActors> class quad_tree {
public:
  //! Initialize a quad tree that spans a given \a area
  /*!
    The quad tree is a data structure to efficiently find actors in a
    specific region. 

    \param area the area that is managed by the \c quad_tree.
  */
  explicit quad_tree(math::rectangle area) noexcept : area_{area} {};
  //! Add an actor to the \c quad_tree
  /*!
    This adds an actor to the \c quad_tree such that it later may be found 
    using the \sa quad_tree::find method.
    
    The actor must be within the area of the quad tree.

    \param a the actor to add
  */
  void add(actor &a) {
    if (!contains(area_, a.location()))
      return;
    auto next = std::visit(
      overload{[&](free &f) -> std::optional<saturated> {
                   if (f.n == MaxActors) {
                     return split(f, a);
                   } else {
                     return {};
                   }
                 },
               [&](saturated &s) -> std::optional<saturated> {
                   s.children[index_for(a)]->add(a);
                   return {};
                 }},
        state_);
    if (next) {
      state_ = std::move(*next);
    }
  }

  //! Find all actors in the given \a bounds and writes them to \a destination
  /*!
     This method will search the area managed by the \c quad_tree for any actors
     that are within the \a bounds and outputs them via the output iterator \a destination.
     \param bounds the bounds in which to search for actors
     \param destination the output iterator via which to write the actors found
  */
  template <typename OutputIt>
  void find(math::rectangle bounds, OutputIt destination) const {
    if (!intersect(bounds, area_))
      return;
    std::visit(overload{[&](free const &f) {
                          for (std::size_t i{0}; i < f.n; ++i) {
                            auto *a = f.actors[i];

                            if (contains(bounds, a->location())) {

                              *destination++ = f.actors[i];
                            }
                          }
                        },
                        [&](saturated const &s) {
                          for (auto const &child : s.children) {
                            child->find(bounds, destination);
                          }
                        }},
               state_);
  }

private:
  struct free {
    std::array<actor *, MaxActors> actors{};
    std::size_t n{};
  };
  struct saturated {
    std::unique_ptr<quad_tree> children[4]{};
  };

  saturated split(free &f, actor &new_actor) {

    saturated next_state{};

    auto q = to_quadrants(area_);
    for (std::size_t i{0}; i < 4; ++i) {
      next_state.children[i] =
          std::make_unique<quad_tree>(q.counterClockwise[i]);
    }

    for (auto *current_actor : f.actors) {
      next_state.children[index_for(*current_actor)]->add(*current_actor);
    }
    next_state.children[index_for(new_actor)]->add(new_actor);

    return next_state;
  }
  std::size_t index_for(actor &a) {
    auto c = math::center(area_);
    auto x = a.location().x();
    auto y = a.location().y();
    if (y <= c.y()) {
      if (x <= c.x()) {
        return 1;
      } else {
        return 0;
      }
    } else if (x <= c.x()) {
      return 2;
    }
    return 3;
  };

  math::rectangle area_;
  std::variant<free, saturated> state_{free{}};
};
} // namespace play
} // namespace kmint

#endif /* KMINT_PLAY_QUAD_TREE_HPP */
