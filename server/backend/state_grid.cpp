#include <exception>

#include "state_grid.h"

StateGrid::StateGrid(uint8_t n_states, uint8_t size) {
  this->n_states = n_states;
  this->size = size;

  this->state_grid = new uint8_t[size*size];
  for(int i = 0; i < size*size; i++) {
    this->state_grid[i] = 0;
  }
}

StateGrid::~StateGrid() {
  delete [] this->state_grid;
}

void StateGrid::setStateAt(uint8_t i, uint8_t state) {
  if (state == 0) {
    throw std::logic_error("Cannot set the state to of a grid item to unoccupied");
  }

  if (state_grid[i] != 0) {
    throw std::logic_error("Illegal move: Cannot change the state of an already occupied grid item");
  }

  this->state_grid[i] = state;
}

uint8_t StateGrid::getOverallState(uint8_t** valid_combos) {
  uint8_t* combo;
  uint8_t combo_sum;

  // Check for each possible state if a combo of valid_combos is being done
  for (i = 1; i <= this->n_states; i++) {
    for (int j = 0; j < 2*(this->size + 1); j++) {
      combo = valid_combos[j];
      combo_sum = 0;
      for (int k = 0; k < this->size; k++) {
        if (this->state_grid[combo[k]] == i) combo_sum++;
      }
      if (combo_sum == this->size) return i;
    }
    return 0;
  }
}

void StateGrid::reset() {
  for(int i = 0; i < this->size*this->size; i++) {
    this->state_grid[i] = 0;
  }
}
