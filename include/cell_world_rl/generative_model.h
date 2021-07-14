#pragma once
#include <cell_world.h>
#include <cell_world_rl/predictor.h>

namespace cell_world::rl {

    struct Observer : Stateless_agent {
        Move get_move(const Model_public_state &) override;
        const Cell &start_episode() override;
        void set_move(Move);
        Cell start_cell;
        Move next_move;
        unsigned int index;
    };

    struct Predictor_container : Stateless_agent {
        explicit Predictor_container(Predictor &);
        Move get_move(const Model_public_state &) override;
        const Cell &start_episode() override;
        Agent_status_code update_state(const Model_public_state &) override;
        Predictor &predictor;
        Cell start_cell;
        unsigned int index;
    };

    struct Generative_model : Model {
        explicit Generative_model(const Cell_group &, const Graph &);
        void start_episode() override;
        void add_predictor(Predictor &);
        void add_observer();
        Model_state create_particle();
        void set_start_state(const Model_public_state &state);
        Observer observer;
        std::vector<Predictor_container> predictors;
        Model_public_state start_state;
        Graph visibility;
        Graph inverted_visibility;
    };
}