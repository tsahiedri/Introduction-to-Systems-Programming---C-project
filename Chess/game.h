#ifndef _GAME_H
#define _GAME_H
#include <stdbool.h>
#include "chessSystem.h"
#include "map.h"

/** Type for representing a single chess match */
typedef struct game_t* Game;

/**
 * gameCreate: Creates a new chess game with the given time and players and updates their statistics.
 */
Game gameCreate(int first_player, int second_player, Winner winner,
	int play_time, Map tournament_players, Map chess_players);

/**
 * gameCopy: A copy function for a game we give to the map ADT.
 * We actually want the map to insert the given game instead of copying it so this function just returns the input.
 */
MapDataElement gameCopy(MapDataElement game);

/**
 * gameCopyId: A copy function for a game id (a vector of 2 containing the players id's) we give to the map ADT.
 */
MapKeyElement gameCopyId(MapKeyElement id);

/**
 * gameCompareId: A function that compares 2 game ids we give to the map ADT.
 * If both games have the same player (the ids are the same or one is the other swapped) it returns 0.
 */
int  gameCompareId(MapKeyElement id1, MapKeyElement id2);

/**
 * gameFreeId: Frees a game id (int[2]) from the memory.
 */
void gameFreeId(MapKeyElement id);

/**
 * gameFree: Frees a game from the memory.
 */
void gameFree(MapDataElement game);

/**
 * gameRemovePlayer: 
 * Removes a given player from a game if he was in it by making the other player winner and updating the statistics.
 * If the given player wasn't in the game, or a player was already removed from the game, it does nothing.
 */
void gameRemovePlayer(Game game, int player_id, Map tournament_players, Map players);

/**
 * gamePlayerRemoved: Returns true if one of the players of the given game was removed and false otherwise.
 */
bool gamePlayerRemoved(Game game);


#endif // _GAME_H