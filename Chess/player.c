#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tournament.h"
#include "map.h"
#include "player.h"
#include "game.h"
#include "insideFunctions.h"
#define WIN_POINTS_SCORE 2
#define TIE_POINTS_SCORE 1
#define WIN_POINTS_LEVEL 6
#define TIE_POINTS_LEVEL 2
#define LOSE_POINTS_LEVEL -10

struct player_t {
    int num_of_wins;
    int num_of_losses;
    int num_of_ties;
    int num_of_games;
    int total_game_time;
    int player_id;
};

int playerGetID(Player player)
{
    return player->player_id;
}

int playerGetLosses(Player player)
{
    return player->num_of_losses;
}

int playerGetWins(Player player)
{
    return player->num_of_wins;
}

MapDataElement playerCopy(MapDataElement player)
{
    return player;
}

double playerCalculateAveragePlayTime(Player player)
{
    return ((double)player->total_game_time / player->num_of_games);
}

bool playerExceededGames(Player player, int max_games)
{
    if (player == NULL)
    {
        return false;
    }
    return player->num_of_games >= max_games;
}

Player playerCreate(int id)
{
    Player player = malloc(sizeof(*player));
    if (player == NULL)
    {
        return NULL;
    }
    player->num_of_games = 0;
    player->num_of_losses = 0;
    player->num_of_ties = 0;
    player->num_of_wins = 0;
    player->total_game_time = 0;
    player->player_id = id;
    return player;
}

int playerGetScore(Player player)
{
    return ((player->num_of_wins) * WIN_POINTS_SCORE + (player->num_of_ties) * TIE_POINTS_SCORE);
}

double playerGetLevel(Player player)
{
    return ((WIN_POINTS_LEVEL * player->num_of_wins + LOSE_POINTS_LEVEL * player->num_of_losses + 
        TIE_POINTS_LEVEL * player->num_of_ties) / (double)player->num_of_games);
}

Player playerAdd(Map players, int id)
{
    Player player = playerCreate(id);
    if (player == NULL)
    {
        return NULL;
    }
    mapPut(players, &id, player);
    return player;
}

void playerDestroy(MapDataElement player)
{
    free(player);
}

void playerRemoveStatistics(Player player, Map players)
{
    Player outside_player = mapGet(players, &(player->player_id));
    outside_player->num_of_games -= player->num_of_games;
    if (outside_player->num_of_games == 0)
    {
        mapRemove(players, &(player->player_id));
        return;
    }
    outside_player->num_of_losses -= player->num_of_losses;
    outside_player->num_of_ties -= player->num_of_ties;
    outside_player->num_of_wins -= player->num_of_wins;
    outside_player->total_game_time -= player->total_game_time;
}

void playerAddDraw(Player player, int game_time)
{
    player->num_of_games++;
    player->num_of_ties++;
    player->total_game_time += game_time;
}

void playerAddWin(Player player, int game_time)
{
    player->num_of_games++;
    player->num_of_wins++;
    player->total_game_time += game_time;
}

void playerAddLose(Player player, int game_time)
{
    player->num_of_games++;
    player->num_of_losses++;
    player->total_game_time += game_time;
}

void playerChangeLoseToWin(Player player)
{
    player->num_of_losses--;
    player->num_of_wins++;
}

void playerChangeDrawToWin(Player player)
{
    player->num_of_ties--;
    player->num_of_wins++;
}