#ifndef _DEEP_TAC_TOE_GAME_H
#define _DEEP_TAC_TOE_GAME_H

struct delta_state_t {
  uint8_t mini_grid_id;
  uint8_t new_mini_grid_state;

  uint8_t box_id;
  uint8_t new_box_state;

  uint8_t n_allowed_mini_grids;
  uint8_t* allowed_mini_grids;

  uint8_t next_player_turn;
  uint8_t winner;
  bool end;
} ;

class DeepTacToeGame {

private:
  uint8_t* players;

  StateGrid main_grid;
  StateGrid* mini_grids;

  uint8_t* allowed_mini_grids;
  uint8_t n_allowed_mini_grids;

  uint8_t** valid_combos;

  uint8_t n_players;
  uint8_t max_n_players;
  uint8_t size;
  uint8_t turn;

public:
  DeepTacToeGame(uint8_t size, uint8_t max_n_players);
  ~DeepTacToeGame();

  delta_state_t move(uint8_t player, uint8_t mini_grid_id, uint8_t box_id);
  uint8_t addPlayer();
}

#endif
