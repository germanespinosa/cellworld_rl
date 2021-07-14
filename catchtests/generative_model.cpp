#include <catch.h>
#include <cell_world_tools.h>
#include <cell_world_rl.h>

using namespace cell_world;
using namespace cell_world::rl;
using namespace std;
using namespace json_cpp;

struct Test_predictor : Predictor {
    Test_predictor(){
        connections.load("{{\"x\"=0,\"y\"=-1},{\"x\"=0,\"y\"=1},{\"x\"=-1,\"y\"=0},{\"x\"=1,\"y\"=0}}");
    }
    Move get_move(const History &history) override{
        return connections.random_move();
    };
    Connection_pattern connections;
};

struct Test_agent : Stateless_agent {
    explicit Test_agent(const Cell &start_cell) : start_cell(start_cell) {
    };
    const Cell &start_episode() override {
        return start_cell;
    };
    Move get_move(const Model_public_state &) override {
        return {1,0};
    };
    Agent_status_code update_state(const Model_public_state &state) override {
        return Agent_status_code::Running;
    };
    Cell start_cell;
};

struct Test_planning_agent : Planning_agent {
    explicit Test_planning_agent( const Cell &start_cell, const Cell_group &cells, const Graph &visibility) :
            start_cell(start_cell), Planning_agent(cells, visibility, 100, 1000) {
        predictor.probability = 1;
        generative_model.add_predictor(predictor);
        generative_model.add_observer();
    };

    const Cell &start_episode(Generative_model &generative_model, Belief_state &belief_state) override {
        return start_cell;
    };

    Move get_move(const Model_public_state &state, Generative_model &generative_model, Belief_state &belief_state) override {
        return {-1,0};
    };

    Agent_status_code update_state(const Model_public_state &state, Generative_model &generative_model, Belief_state &belief_state) override {
        return Agent_status_code::Running;
    };

    Cell start_cell;
    Test_predictor predictor;
};


TEST_CASE("generative_model") {
//    string world_name("world_0_0");
//    World world (Json_create<World>(Web_resource::from("world").key(world_name).get()));
//    Cell_group cells(world.create_cell_group());
//    Map map(cells);
//    Graph visibility(world.create_graph(Json_create<Graph_builder>(Web_resource::from("graph").key(world.name).key("visibility").get())));
//    Test_agent ta1(map[{-7,0}]);
//    Test_planning_agent ta2( map[{7,0}], cells, visibility);

//    Model model(cells);
//    model.add_agent(ta1);
//    model.add_agent(ta2); // observer
//
//    model.start_episode();
//    CHECK(ta1.public_state().cell == map[{-7,0}]);
//    CHECK(ta2.public_state().cell == map[{7,0}]);
//
//    model.update();
//    CHECK(ta1.public_state().cell == map[{-6,0}]);
//    CHECK(ta2.public_state().cell == map[{7,0}]);
//
//    model.update();
//    CHECK(ta1.public_state().cell == map[{-6,0}]);
//    CHECK(ta2.public_state().cell == map[{6,0}]);
//
//    model.update();
//    CHECK(ta1.public_state().cell == map[{-5,0}]);
//    CHECK(ta2.public_state().cell == map[{6,0}]);

// create the generative model with one predictor and one observer
}

TEST_CASE("history") {
}