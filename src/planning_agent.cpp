#include <cell_world_rl/planning_agent.h>

namespace cell_world::rl {
    Planning_agent::Planning_agent(
            const Cell_group &cells,
            const Graph &visibility,
            unsigned int particle_count,
            int limit) :
            generative_model(cells, visibility),
            belief_state(generative_model, particle_count, limit ) {
    }

    const Cell &Planning_agent::start_episode() {
        return start_episode(generative_model,belief_state);
    }

    Move Planning_agent::get_move(const Model_public_state &state) {
        belief_state.record_observation(state);
        return get_move(state, generative_model, belief_state);
    }

    Agent_status_code Planning_agent::update_state(const Model_public_state &state) {
        if (state.status == Model_public_state::Starting) {
            generative_model.set_start_state(state);
            belief_state.start_episode();
        } else {
            belief_state.record_observation(state);
        }
        return update_state(state, generative_model, belief_state);
    }
}