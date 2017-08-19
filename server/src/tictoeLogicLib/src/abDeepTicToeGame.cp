#include <exception>

#include "deep_tac_toe_game.h"

DeepTacToeGame::DeepTacToeGame(uint8_t size, uint8_t max_n_players) {
  uint8_t combo_counter = 0;
  uint8_t count = 0;

  this->size = size;
  this->n_players = 0;
  this->max_n_players = max_n_players;
  this->players =  new uint8_t[max_n_players];
  this->turn = 0;

  // Set up the big grid, and the mini grids
  this->main_grid = StateGrid(n_players + 1, size);

  this->mini_grids = new StateGrid[size*size];
  for (uint8_t i = 0; i < size*size; i++) this->mini_grids[i] = StateGrid(n_players +1, size);

  // Initially, all mini grids are playable
  this->allowed_mini_grids = new uint8_t[size*size];
  for (uint8_t i = 0; i < size*size; i++) this->allowed_mini_grids[i] = i;
  this->n_allowed_mini_grids = size*size;

  // Initialize players
  for (uint8_t i = 0; i < max_n_players; i++) this->players[i] = i+1;

  // Setup the valid winning combinations
  this->valid_combos = new uint8_t*[2*(size + 1)];

  // Horizontal combos
  for (uint8_t i = 0; i < size; i++) {
    this->valid_combos[combo_counter] = new uint8_t[size];
    for(int j = i*size; j < size*(i+1); j++) {
      this->valid_combos[combo_counter][count] = j;
      count++;
    }
    combo_counter++;
  }

  // Vertical combos
  for (int i = 0; i < size; i++) {
    count = 0;
    this->valid_combos[combo_counter] = new uint8_t[size];
    for(int j = i; j < i + (size-1)*size + 1; j = j + size) {
      this->valid_combos[combo_counter][count] = j;
      count++;
    }
    combo_counter++;
  }

  // Diagonal combos
  this->valid_combos[combo_counter] = new uint8_t[size];
  count = 0
  for (int j = 0; j < size*size; j = j + size + 1) {
    this->valid_combos[combo_counter][count] = j;
    count++;
  }
  combo_counter++;
  this->valid_combos[combo_counter] = new uint8_t[size];
  count = 0
  for (int j = size - 1; j < (size - 1)*size + 1; j = j + size - 1) {
    this->valid_combos[combo_counter][count] = j;
    count++;
  }

}

DeepTacToeGame::~DeepTacToeGame() {
  delete [] this->players;
  delete [] this->mini_grids;
  for(uint8_t i = 0; i < 2*(this->size + 1); i++) delete [] this->valid_combos[i];
  delete [] this->valid_combos;
  delete [] this->allowed_mini_grids;
  delete this->main_grid;
}

delta_state_t DeepTacToeGame::move(uint8_t player, uint8_t mini_grid_id, uint8_t box_id) {
  delta_state_t delta_state;
  uint8_t count = 0;
  bool allowed = false;

  // Check if it is player's turn
  if (player != this->players[this->turn]) throw std::logic_error("Not player's turn!");

  // Check that the player is allowed to move in this region
  for (uint8_t i = 0; i < this.allowed_mini_grids_size; i++) {
    if (mini_grid_id == this.allowed_mini_grids[i]) allowed = true;
  }
  if (!allowed) throw std::logic_error("Illegal move: Can't move in this area");

  // Change mini amd main grid state. Throw exception if already occupied
  try {
    this->mini_grids[mini_grid_id].setStateAt(box_id, player);
    delta_state.new_mini_grid_state = this->mini_grids[mini_grid_id].getOverallState(this->valid_combos);
    this->main_grid.setStateAt(mini_grid_id, delta_state.new_mini_grid_state);
  } catch (...) {
    throw;
  }

  // Update allowed mini grids for next turn.
  // Mini grid must be unoccupied, if not next player can move anywhere where the
  // mini grid has not been yet closed.
  if (this->main_grid.getStateAt(box_id) == 0) {
    this->allowed_mini_grids[0] = box_id;
    this->n_allowed_mini_grids = 1;
  } else {
    for(uint8_t i = 0; i < this->size*this->size; i++) {
      if (this->main_grid.getStateAt(i) == 0) {
        this->allowed_mini_grids[count] = i;
        count++;
      }
    }
    this->n_allowed_mini_grids = count;
  }

  // Next turn
  this->turn = (this->turn + 1) % this->n_players;

  // Fill the return structure
  delta_state.mini_grid_id = mini_grid_id;
  delta_state.box_id = box_id;
  delta_state.new_box_state = player;
  delta_state.n_allowed_mini_grids = this->n_allowed_mini_grids;
  delta_state.allowed_mini_grids = this->allowed_mini_grids;
  delta_state.next_player_turn = this->players[this->turn];
  delta_state.winner = this->main_grid.getOverallState(this->valid_combos);
  if ((this->n_allowed_mini_grids > 0) && (delta_state.winner == 0)) delta_state.end = false;
  else delta_state.end = true;

  return delta_state;
}

uint8_t DeepTacToeGame::addPlayer() {
  if (this->n_players == this->max_n_players) throw std::logic_error("Can't add more players. Game is full.");
  this->n_players++;
  return this->n_players;
}

uint8_t DeepTacToeGame::removePlayer() {
  if (this->n_players == 0) throw std::logic_error("Can't remove more players. Game is empty.")
  this->n_players--;
  return this->n_players;
}
