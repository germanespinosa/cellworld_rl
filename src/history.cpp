#include <cell_world_rl/history.h>

using namespace std;
namespace cell_world::rl {

    History::History():last_index(0){

    }

    void History::add(const Model_public_state &state, bool simulated)  {
        if (simulated) {
            if (last_index >= size()) {
                push_back(state);
            } else {
                (*this)[last_index] = state;
            }
            last_index++;
        } else {
            if (last_confirmed_index >= size()) {
                push_back(state);
            } else {
                (*this)[last_confirmed_index] = state;
            }
            last_confirmed_index ++;
            last_index = last_confirmed_index;
        }
    }

    std::span<Model_public_state> History::get()  {
        return slice(last_index);
    }
}