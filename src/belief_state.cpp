#include <cell_world_rl/belief_state.h>
#include <cell_world_tools.h>

using namespace std;
namespace cell_world::rl {

    Belief_state::Belief_state(Generative_model &generative_model, unsigned int particle_count, int limit):
            std::vector<Particle>(particle_count), generative_model(generative_model), particle_count(0), limit(limit), map(generative_model.visibility.cells){
    }

    Model_state &Belief_state::random_particle() {
        return (*this)[index[Chance::dice(index.size())]].state;
    }

    void Belief_state::try_replace_particle(Particle &particle) {
        particle.valid = false;
        unsigned int try_count = 0;
        while (!particle.valid && try_count++ < limit) {
            particle.state = generative_model.create_particle();
            particle.valid = true;
            generative_model.set_state(particle.state);
            unsigned int move_index = 0;
            for (auto &observation:observations){
                Move move{0,0};
                if (observation.current_turn != generative_model.observer.index)
                    move = observer_moves[move_index++];
                generative_model.observer.set_move(move);
                generative_model.set_state(particle.state);
                generative_model.update(observation.get_progress());
                particle.valid = is_valid(generative_model.state, observation);
                if (!particle.valid)
                    break;
            }
        }
        if (particle.valid)
            particle.state = generative_model.state;
    }

    bool Belief_state::is_valid(Model_state &particle, const Model_public_state &observation) {
        auto &particle_observer_state = particle.public_state.agents_state[generative_model.observer.index];
        auto &particle_observer_cell = particle_observer_state.cell;
        auto &observation_observer_state = observation.agents_state[generative_model.observer.index];
        auto &observation_observer_cell = observation_observer_state.cell;
        if (observation_observer_cell != particle_observer_cell ) return false;
        auto &observer_cell = observation_observer_cell;
        auto &visibility = generative_model.visibility[observer_cell];
        for (unsigned int a = 0; a < observation.agents_state.size(); a++){
            if (a != generative_model.observer.index) {
                auto particle_agent_cell = particle.public_state.agents_state[a].cell;
                auto observation_agent_cell = observation.agents_state[a].cell;
                if (observation_agent_cell != particle_agent_cell &&
                    (visibility.contains(observation_agent_cell) || visibility.contains(particle_agent_cell)) )
                    return false;
            }
        }
        return true;
    }

    unsigned int Belief_state::record_observation(const Model_public_state &observation) {
        observations.push_back(observation);
        if ( observation.current_turn != generative_model.observer.index ){
            auto move = observation.agents_state[generative_model.observer.index].cell.coordinates - observations[observations.size()-2].agents_state[generative_model.observer.index].cell.coordinates;
            observer_moves.push_back(move);
        }
        for (auto &particle: *this) {
            generative_model.set_state(particle.state);
            generative_model.update(observation.get_progress());
            particle.valid = is_valid(generative_model.state, observation);
            if (particle.valid)
                particle.state = generative_model.state;
            else
                try_replace_particle(particle);
        }
        return update_index();
    }

    unsigned int Belief_state::start_episode() {
        return update_particles(true);
    }

    unsigned int Belief_state::update_index() {
        index.clear();
        for (unsigned int particle_index=0; particle_index<size(); particle_index++ ){
            if ((*this)[particle_index].valid){
                index.push_back(particle_index);
            }
        }
        particle_count = index.size();
        return particle_count;
    }

    unsigned int Belief_state::update_particles(bool all) {
        for (auto &particle: *this)
            if (all || !particle.valid) try_replace_particle(particle);
        return update_index();
    }


    vector<Coordinates_list> beliefs(Belief_state &belief_state){
        unsigned int max=0;
        auto hits = vector<vector<unsigned int> (belief_state.map)
        for (auto &particle:belief_state) {
            if (particle.valid) {
                max = b > max ? b : max;
            }
        }
        unsigned int div = max / groups + 1;
        vector<Coordinates_list> bg (groups);
        for (unsigned int i=0; i<bs.hits.size() ; i++){
            unsigned int index = bs.hits[i]/div;
            bg[index].push_back(bs.particles_coordinates[i]);
        }
        return bg;
    }

    void Belief_state::print() {
        Screen_map sm (map);
        auto &prey_visibility = generative_model.visibility[generative_model.];
        auto belief_state_grouped = beliefs(prey.planner.filter.get_belief_state(), 4);
        if (prey_visibility.contains(predator_cell)) {
            sm.add_special_cell(predator_cell, ms.two.front(Blue).back(Yellow));
        }
        sm.add_special_cell(prey.internal_state().option, ms.goal.front(Red));
        sm.add_special_cell(prey_prev,ms.one.front(Red));
        sm.add_group(prey_visibility, ms.clear.back(Yellow));

        auto g = gradient(Blue,White);
        sm.add_group(belief_state_grouped[0],g[0]);
        sm.add_group(belief_state_grouped[1],g[1]);
        sm.add_group(belief_state_grouped[2],g[2]);
        sm.add_group(belief_state_grouped[3],g[3]);

        sm.add_special_cell(prey_cell, ms.get_direction(prey_move));
        cout << sm << endl;
    }
}