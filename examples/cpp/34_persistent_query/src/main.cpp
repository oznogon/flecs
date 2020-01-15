#include <persistent_query.h>
#include <iostream>

/* Component types */
struct Position {
    float x;
    float y;
};

struct Velocity {
    float x;
    float y;
};

int main(int argc, char *argv[]) {
    /* Create the world, pass arguments for overriding the number of threads,fps
     * or for starting the admin dashboard (see flecs.h for details). */
    flecs::world world(argc, argv);

    flecs::component<Position>(world, "Position");
    flecs::component<Velocity>(world, "Velocity");

    /* Create a query. Queries are 'persistent' meaning they are registered with
     * the world and continuously matched with new entities (tables). Queries
     * are the fastest way to iterate over entities, as a lot of processing is
     * done when entities are matched, outside of the main loop.
     *
     * Queries are the mechanism used by systems, and as such both accept the
     * same signature expressions, and have similar performance.  */
    flecs::query q(world, "Position, Velocity");

    /* Create a few entities that match the query */
    flecs::entity(world, "E1")
        .set<Position>({1, 2})
        .set<Velocity>({1, 1});

    flecs::entity(world, "E2")
        .set<Position>({3, 4})
        .set<Velocity>({1, 1});

    /* Don't add Velocity here, E3 will not match query */
    flecs::entity(world, "E3")
        .set<Position>({5, 6});

    /* Iterate over entities matching the query */
    for (auto rows : q) {
        flecs::column<Position> p(rows, 1); 
        flecs::column<Velocity> v(rows, 2);
   
        for (auto row : rows) {
            p[row].x += v[row].x;
            p[row].y += v[row].y;

            std::cout << "Moved " << rows.entity(row).name() << " to {" <<
                p[row].x << ", " << p[row].y << "}" << std::endl;
        }
    }
}