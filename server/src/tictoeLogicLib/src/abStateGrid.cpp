#include "abStateGrid.h"
#include <exception>

/*
StateGrid::StateGrid(uint8_t n_states, uint8_t size) {
  this->n_states = n_states;
  this->size = size;

  this->state_grid = new uint8_t[size*size];
  for(uint8_t i = 0; i < size*size; i++) this->state_grid[i] = 0;
}

StateGrid::~StateGrid() {
  delete [] this->state_grid;
}

void StateGrid::setStateAt(uint8_t i, uint8_t state) {
  if (state == 0) throw std::logic_error("Cannot set the state to of a grid item to unoccupied");

  if (state_grid[i] != 0) throw std::logic_error("Illegal move: Cannot change the state of an already occupied grid item");

  this->state_grid[i] = state;
}

uint8_t StateGrid::getOverallState(uint8_t** valid_combos) {
  uint8_t combo_sum;

  // Check for each possible state if a combo of valid_combos is being done
  for (uint8_t i = 1; i <= this->n_states; i++) {
    for (uint8_t j = 0; j < 2*(this->size + 1); j++) {
      combo_sum = 0;
      for (uint8_t k = 0; k < this->size; k++) {
        if (this->state_grid[valid_combos[j][k]] == i) combo_sum++;
      }
      if (combo_sum == this->size) return i;
    }
    return 0;
  }
}

*/
