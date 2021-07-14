#pragma once
#include <cell_world.h>

namespace cell_world::rl {
    struct History : json_cpp::Json_vector<Model_public_state> {
        History();
        void add(const Model_public_state &state, bool simulated = false);
        std::span <Model_public_state> get();
        const Model_public_state &current_state() const;

    private:
        unsigned int last_index;
        unsigned int last_confirmed_index;
    };
}