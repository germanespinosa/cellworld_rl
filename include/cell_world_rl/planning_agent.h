#pragma once
#include <cell_world.h>
#include <cell_world_rl/generative_model.h>
#include <cell_world_rl/belief_state.h>

namespace cell_world::rl {
    struct Planning_agent : Stateless_agent {
        explicit Planning_agent( const Cell_group &cells, const Graph &visibility, unsigned int particle_count, int limit );
        virtual const Cell &start_episode(Generative_model &generative_model, Belief_state &belief_state) = 0;
        virtual Move get_move(const Model_public_state &state, Generative_model &, Belief_state &belief_state) = 0;
        virtual Agent_status_code update_state(const Model_public_state &state, Generative_model &generative_model, Belief_state &belief_state) = 0;
        const Cell &start_episode() override;
        Move get_move(const Model_public_state &state) override;
        Agent_status_code update_state(const Model_public_state &state) override;

        Generative_model generative_model;
        Belief_state belief_state;
    };
}