#ifndef AB_DEEP_TAC_TOE_GAME_H
#define AB_DEEP_TAC_TOE_GAME_H

// DeepTacToeGame - Proprietary and confidential
// Author Aitor Ruano
// Copyright (C) Autobon.ai, LLC - All Rights Reserved
// Unauthorized copying of this file via any medium
// is strictly prohibited without the written permission

#include "abStateGrid.h"

/*
class DeepTacToeGame
{
    public:
        typedef std::vector<StateGrid>  TGVec;
        typedef std::vector<uint8_t>    TVec;

        struct DeltaState
        {
            uint8_t              _miniGridId;
            uint8_t              _newMiniGridState;
            uint8_t              _boxId;
            uint8_t              _newBoxState;
            std::vector<uint8_t> _allowedMiniGrids;
            uint8_t              _nextPlayerTurn;
            uint8_t              _winner;
            bool                 _end;
        };

         DeepTacToeGame(uint8_t size, uint8_t maxPlayers);
        ~DeepTacToeGame();

        DeltaState  move(uint8_t player, uint8_t miniGridId, uint8_t boxId);
        uint8_t     addPlayer(void);
        uint8_t     removePlayer(void);

    private:
        TVec       _players;

        StateGrid  _mainGrid;
        TGVec      _miniGrids;

        TGVec      _allowedMiniGrids;
        uint8_t**  _valid_combos;

        uint8_t    _players;
        uint8_t    _maxPlayers;
        uint8_t    _size;
        uint8_t    _turn;
};
*/

#endif // AB_DEEP_TAC_TOE_GAME_H
