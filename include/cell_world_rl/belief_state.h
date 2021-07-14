#pragma once
#include <cell_world.h>
#include <cell_world_rl/generative_model.h>

namespace cell_world::rl {
    struct Particle {
        Model_state state;
        bool valid;
    };

    struct Belief_state : std::vector<Particle> {
        Belief_state(Generative_model &, unsigned int, int);
        unsigned int start_episode();
        Model_state &random_particle();
        Generative_model &generative_model;
        unsigned int particle_count;
        unsigned int limit;
        unsigned int record_observation(const Model_public_state &observation);
        std::vector<Model_public_state> observations;
        Move_list observer_moves;
        std::vector<unsigned int> index;
        Map map;
    private:
        void try_replace_particle(Particle &);
        bool is_valid(Model_state &, const Model_public_state &);
        unsigned int update_particles(bool all=true);
        unsigned int update_index();
        void print();
    };
}