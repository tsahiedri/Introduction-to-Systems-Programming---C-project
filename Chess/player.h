#ifndef _PLAYER_H
#define _PLAYER_H

#include <stdbool.h>
#include "map.h"

/** Type for representing a chess player that contains his id and statistics (wins, loses, etc.) */
typedef struct player_t* Player;

/**
 * playerCreate: Creates a player, sets is id to the given id and resets his statistics 
 */
Player playerCreate(int id);

/**
 * playerCopy: A copy function for a player we give to the map ADT.
 * We actually want the map to insert the given player instead of copying it so this function just returns the input.
 */
MapDataElement playerCopy(MapDataElement player);

/**
 * playerDestroy: Frees a player from memory.
 */
void playerDestroy(MapDataElement player);

/**
 * playerCalculateAveragePlayTime: Calculates and returns the average play time of the given player.
 */
double playerCalculateAveragePlayTime(Player player);

/**
 * playerGetLevel: Calculates and returns the level of the given player.
 */
double playerGetLevel(Player player);

/**
 * playerGetID: Return the id of the given player.
 */
int playerGetID(Player player);

/**
 * playerExceededGames: 
 * Returns true if the given player already played the max amount of games he's allowed and false otherwise.
 */
bool playerExceededGames(Player player, int max_games);

/**
 * playerGetScore: Calculates and returns the score of the given player.
 */
int playerGetScore(Player player);

/**
 * playerAdd: Creates a player with the given id and inserts it into the players map.
 */
Player playerAdd(Map players, int id);

/**
 * playerGetLosses: Returns the number of games the given player lost.
 */
int playerGetLosses(Player player);

/**
 * playerGetWins: Returns the number of games the given player won.
 */
int playerGetWins(Player player);

/**
 * playerRemoveStatistics: Finds the player in the map with the same id as the one given.
 * It then decrease the map player statistics by the statistics of the player given.
 * If the player in the map remains with no games, it deletes him.
 */
void playerRemoveStatistics(Player player, Map players);

/**
 * playerAddDraw: Updates the player's statistics given he played another game in the given length and drew.
 */
void playerAddDraw(Player player, int game_time);

/**
 * playerAddWin: Updates the player's statistics given he played another game in the given length and won.
 */
void playerAddWin(Player player, int game_time);

/**
 * playerAddLose: Updates the player's statistics given he played another game in the given length and lost.
 */
void playerAddLose(Player player, int game_time);

/**
 * playerChangeLoseToWin: Decreases the player's number of loses by 1 and increases his number of wins by 1.
 * This function is used if a player lost in a game but his opponent was removed.
 */
void playerChangeLoseToWin(Player player);

/**
 * playerChangeDrawToWin: Decreases the player's number of draws by 1 and increases his number of wins by 1.
 * This function is used if a player drew in a game but his opponent was removed.
 */
void playerChangeDrawToWin(Player player);

#endif //_PLAYER_H