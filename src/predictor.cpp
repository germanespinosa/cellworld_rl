#include <cell_world_rl/predictor.h>

namespace cell_world::rl{

    Predictor::Predictor(double probability) : probability(probability) {

    }

    Predictor::Predictor(): probability(1) {

    }
}