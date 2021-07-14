#pragma once
#include <cell_world.h>
#include <cell_world_rl/history.h>

namespace cell_world::rl {
    struct Predictor {
        Predictor ();
        explicit Predictor (double);
        virtual Move get_move(const History &) = 0;
        History history;
        double probability;
    };
}