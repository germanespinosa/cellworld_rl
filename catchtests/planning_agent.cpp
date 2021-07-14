#include <catch.h>
#include <cell_world_tools.h>
#include <cell_world_rl.h>
#include <cell_world_tools/random_agent.h>

using namespace cell_world;
using namespace cell_world::rl;
using namespace std;
using namespace json_cpp;

struct Test_predictor : Predictor {
    explicit Test_predictor(const Connection_pattern &connection_pattern):
            connections(connection_pattern){
    }
    Move get_move(const History &history) override{
        return connections.random_move();
    };
    const Connection_pattern &connections;
};


struct Test_planning_agent : Planning_agent {
    explicit Test_planning_agent( const Cell &start_cell, const Cell_group &cells, const Connection_pattern &connection_pattern ,const Graph &visibility, unsigned int particle_count, unsigned int limit) :
            start_cell(start_cell), Planning_agent(cells, visibility, particle_count, limit),
            predictor(connection_pattern){
        predictor.probability = 1;
        generative_model.add_observer();
        generative_model.add_predictor(predictor);
    };

    const Cell &start_episode(Generative_model &generative_model, Belief_state &belief_state) override {
        return start_cell;
    };

    Move get_move(const Model_public_state &state, Generative_model &generative_model, Belief_state &belief_state) override {
        return {0,1};
    };

    Agent_status_code update_state(const Model_public_state &state, Generative_model &generative_model, Belief_state &belief_state) override {
        return Agent_status_code::Running;
    };
    Cell start_cell;
    Test_predictor predictor;
};

TEST_CASE("Planning_Agent"){
    string world_name("world_5_5");
    auto world = Json_create<World>(Web_resource::from("world").key(world_name).get());
    Cell_group cells(world.create_cell_group());
    Map map(cells);
    Graph visibility(world.create_graph(Json_create<Graph_builder>(Web_resource::from("graph").key(world_name).key("visibility").get())));
    Test_planning_agent tpa (map[{-7,0}], cells, world.connection_pattern, visibility, 10, 10000);
    Random_agent da(map[{7,0}], world.connection_pattern);
    Model model (cells);
    model.add_agent(tpa);
    model.add_agent(da);
    model.start_episode();
    cout << "model:" << model.state.public_state.get_progress() << endl;
    CHECK(tpa.public_state().cell == map[{-7,0}]);
    CHECK(tpa.belief_state.particle_count == 10);
    model.update();
    cout << "model:" << model.state.public_state.get_progress() << endl;
    CHECK(tpa.belief_state.particle_count == 10);
    model.update();
    cout << "model:" << model.state.public_state.get_progress() << endl;
    model.update();
    cout << "model:" << model.state.public_state.get_progress() << endl;
    CHECK(tpa.belief_state.particle_count == 10);
//    cout << "particle:" << tpa.belief_state[1].state.public_state << endl;
//    CHECK(tpa.belief_state.observations.size() == 2);
//    model.update();
//    CHECK(tpa.belief_state.particle_count == 100);
//    model.update();
//    CHECK(tpa.belief_state.observations.size() == 4);
//    CHECK(tpa.belief_state.particle_count == 100);


//    tpa.generative_model.set_state(tpa.belief_state[0]);
//    tpa.generative_model.evolve();
//    tpa.generative_model.evolve();
//    Model model(cells);
//    model.add_agent(tpa);
//    model.start_episode();
}