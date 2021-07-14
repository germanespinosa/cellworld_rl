#include <cell_world_rl/generative_model.h>
#include <utility>
using namespace std;

namespace cell_world::rl {

    Generative_model::Generative_model(
        const Cell_group &cells,
        const Graph &visibility ):
                Model(cells),
                visibility(visibility),
                inverted_visibility(Visibility::invert(visibility)){
    }

    Model_state Generative_model::create_particle() {
        observer.start_cell = start_state.agents_state[observer.index].cell;
        for (auto &predictor: predictors){
            auto &predictor_cell = start_state.agents_state[predictor.index].cell;
            if (visibility[observer.start_cell].contains(predictor_cell)){
                predictor.start_cell = predictor_cell;
            } else {
                if (Chance::coin_toss(predictor.predictor.probability)){
                    predictor.start_cell = inverted_visibility[observer.start_cell].random_cell();
                } else {
                    predictor.start_cell = Cell::ghost_cell();
                }
            }
        }
        if (state.public_state.status == Model_public_state::Running) {
            end_episode();
        }
        start_episode();
        return state;
    }

   void Generative_model::add_predictor(Predictor &predictor) {
        auto &p = predictors.emplace_back(predictor);
        p.index = (int)_agents.size();
        add_agent(p);
    }

    void Generative_model::add_observer() {
        observer.index = (int)_agents.size();
        add_agent(observer);
    }

    void Generative_model::set_start_state(const Model_public_state &state) {
        start_state = state;
    }

    void Generative_model::start_episode() {
        Model::start_episode();
    }

    // Predictor Container

    Move Predictor_container::get_move(const Model_public_state &state) {
        predictor.history.add(state);
        return predictor.get_move(predictor.history);
    }

    Agent_status_code Predictor_container::update_state(const Model_public_state &state) {
        predictor.history.add(state);
        return Agent_status_code::Running;
    }

    const Cell &Predictor_container::start_episode() {
        return start_cell;
    }

    Predictor_container::Predictor_container(Predictor &predictor): predictor(predictor), index(0) {

    }

    // Observer

    Move Observer::get_move(const Model_public_state &) {
        return next_move;
    }

    void Observer::set_move(Move move) {
        next_move = std::move(move);
    }

    const Cell &Observer::start_episode() {
        return start_cell;
    }
}