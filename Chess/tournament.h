#ifndef _TOURNAMENT_H
#define _TOURNAMENT_H
#include "chessSystem.h" //Because we need the results types.
#include "map.h"

/** Type for representing a chess tournament that organizes chess games */
typedef struct tournament_t* Tournament;

/**
 * tournamentCreate: Creates an empty tournament and inserts the max games per player and location for it.
 */
Tournament tournamentCreate(int max_games_per_player, const char* tournament_location);

/** 
 * tournamentCopy: A copy function for a tournament we give to the map ADT.
 * We actually want the map to insert the given tournament instead of copying it,
 * so this function just returns the input.
 */
MapDataElement tournamentCopy(MapDataElement tournament);

/**
 * tournamentDestroy: Frees a chess tournament and all its contents from memory.
 */
void tournamentDestroy(MapDataElement tournament);

/**
 * tournamentAddGame: Adds a new match to a chess tournament and updateds the players statistics accordingly.
 */
ChessResult tournamentAddGame(Tournament tournament, int first_player, int second_player, 
	Winner winner, int play_time, Map players);

/**
 * tournamentRemovePlayer: Removes a player from an active tournament 
 * and updates the other participants statistics accordingly.
 */
void tournamentRemovePlayer(Tournament tournament, int player_id, Map players);

/**
 * tournamentEnd: Ends an active tournament and finds it's winner.
 */
ChessResult tournamentEnd(Tournament tournament);

/**
 * tournamentRemoveStatistics: 
 * Removes the tournament's players accomplishments from this tournament from their total accomplishments.
 * This function is used to update the players statistics when removing a tournament.
 */
void tournamentRemoveStatistics(Tournament tournament, Map players);

/**
 * tournamentSaveStatistics: 
 * Does nothing if the tournamet is still going and prints it's statistics into the file if it ended.
 */
void tournamentSaveStatistics(Tournament tournament, FILE* file);

/**
 * tournamentFinished: Returns true if the tournamet has ended and false otherwise.
 */
bool tournamentFinished(Tournament tournament);

#endif //_TOURNAMENT_H