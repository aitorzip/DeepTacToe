#ifndef _STATE_GRID_H
#define _STATE_GRID_H

class StateGrid {

private:
  uint8_t n_states;
  uint8_t size;
  uint8_t* state_grid;

public:
  StateGrid(uint8_t n_states, uint8_t size);
  ~StateGrid();

  uint8_t getStateAt(uint8_t i) { return this->state_grid[i]; }
  void setStateAt(uint8_t i, uint8_t state);
  uint8_t getOverallState(uint8_t** valid_combos);
}

#endif
