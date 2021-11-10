#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "chessSystem.h"
#include "tournament.h"
#include "player.h"
#include "map.h"
#include "insideFunctions.h"
#define ERROR -1

struct chess_system_t
{
    Map tournaments;
    Map players;
};

ChessSystem chessCreate()
{
    ChessSystem  system = malloc(sizeof(*system));
    if (system == NULL)
    {
        return NULL;
    }
    system->tournaments = mapCreate(tournamentCopy,
        copyId, tournamentDestroy, freeId, compareIds);
    if (system->tournaments == NULL)
    {
        free(system);
        return NULL;
    }
    system->players = mapCreate(playerCopy,
        copyId, playerDestroy, freeId, compareIds);
    if (system->players == NULL)
    {
        mapDestroy(system->tournaments);
        free(system);
        return NULL;
    }
    return system;
}

void chessDestroy(ChessSystem chess)
{
    if (chess == NULL)
    {
        return;
    }
    mapDestroy(chess->tournaments);
    mapDestroy(chess->players);
    free(chess);
}

ChessResult chessAddTournament(ChessSystem chess, int tournament_id,
    int max_games_per_player, const char* tournament_location)
{
    if (chess == NULL || tournament_location == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if (tournament_id <= 0)
    {
        return CHESS_INVALID_ID;
    }
    if (mapContains(chess->tournaments, &tournament_id))
    {
        return CHESS_TOURNAMENT_ALREADY_EXISTS;
    }
    if (checkTournamentLocation(tournament_location) == false)
    {
        return CHESS_INVALID_LOCATION;
    }
    if (max_games_per_player <= 0)
    {
        return CHESS_INVALID_MAX_GAMES;
    }
    Tournament new_tournament = tournamentCreate(max_games_per_player, tournament_location);
    if (new_tournament == NULL)
    {
        return CHESS_OUT_OF_MEMORY;
    }
    MapResult result = mapPut(chess->tournaments, &tournament_id, new_tournament);
    return result == MAP_OUT_OF_MEMORY ? CHESS_OUT_OF_MEMORY : CHESS_SUCCESS;
}

ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player,
    int second_player, Winner winner, int play_time)
{
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if (tournament_id <= 0 || first_player <= 0 || second_player <= 0 || first_player == second_player)
    {
        return CHESS_INVALID_ID;
    }
    Tournament tournament = mapGet(chess->tournaments, &tournament_id);
    if (tournament == NULL)
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    return tournamentAddGame(tournament, first_player, second_player, winner, play_time, chess->players);
}

ChessResult chessRemoveTournament(ChessSystem chess, int tournament_id)
{
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if (tournament_id <= 0)
    {
        return CHESS_INVALID_ID;
    }
    Tournament to_destroy = mapGet(chess->tournaments, &tournament_id);
    if (to_destroy == NULL)
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    tournamentRemoveStatistics(to_destroy, chess->players);
    mapRemove(chess->tournaments, &tournament_id);
    return CHESS_SUCCESS;
}

ChessResult chessRemovePlayer(ChessSystem chess, int player_id)
{
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if (player_id <= 0)
    {
        return CHESS_INVALID_ID;
    }
    if (mapContains(chess->players, &player_id) == false)
    {
        return CHESS_PLAYER_NOT_EXIST;
    }
    mapRemove(chess->players, &player_id);
    MAP_FOREACH(MapKeyElement, tournament_id, chess->tournaments)
    {
        tournamentRemovePlayer(mapGet(chess->tournaments, tournament_id), player_id, chess->players);
        free(tournament_id);
    }
    return CHESS_SUCCESS;
}

ChessResult chessEndTournament(ChessSystem chess, int tournament_id)
{
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if (tournament_id <= 0)
    {
        return CHESS_INVALID_ID;
    }
    Tournament tournament = mapGet(chess->tournaments, &tournament_id);
    return tournamentEnd(tournament);
}

double chessCalculateAveragePlayTime(ChessSystem chess, int player_id, ChessResult* chess_result)
{
    if (chess == NULL)
    {
        *chess_result = CHESS_NULL_ARGUMENT;
        return ERROR;
    }
    if (player_id <= 0)
    {
        *chess_result = CHESS_INVALID_ID;
        return ERROR;
    }
    Player player = mapGet(chess->players, &player_id);
    if (player == NULL)
    {
        *chess_result = CHESS_PLAYER_NOT_EXIST;
        return ERROR;
    }
    *chess_result = CHESS_SUCCESS;
    return playerCalculateAveragePlayTime(player);
}

ChessResult chessSavePlayersLevels(ChessSystem chess, FILE* file)
{
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    int number_of_players = mapGetSize(chess->players);
    double* players_levels_array = malloc(sizeof(double) * number_of_players);
    int* players_levels_ID_array = malloc(sizeof(int) * number_of_players);
    if (players_levels_array == NULL || players_levels_ID_array == NULL)
    {
        free(players_levels_array);
        free(players_levels_ID_array);
        return CHESS_OUT_OF_MEMORY;
    }
    int i = 0;
    MAP_FOREACH(MapKeyElement, player_id, chess->players)
    {
        Player player = mapGet(chess->players, player_id);
        free(player_id);
        players_levels_array[i] = playerGetLevel(player);
        players_levels_ID_array[i] = playerGetID(player);
        i++;
    }
    sort(players_levels_array, players_levels_ID_array, number_of_players);
    for (int j = number_of_players - 1; j >= 0; j--)
    {
        fprintf(file, "%d %.2f\n", players_levels_ID_array[j], players_levels_array[j]);
    }
    free(players_levels_array);
    free(players_levels_ID_array);
    return CHESS_SUCCESS;
}

ChessResult chessSaveTournamentStatistics(ChessSystem chess, char* path_file)
{
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    bool a_tournament_ended = false;
    MAP_FOREACH(MapKeyElement, tournament_id, chess->tournaments)
    {
        Tournament tournament = mapGet(chess->tournaments, tournament_id);
        free(tournament_id);
        if (tournamentFinished(tournament) == true)
        {
            a_tournament_ended = true;
            break;
        }
    }
    if (a_tournament_ended == false)
    {
        return CHESS_NO_TOURNAMENTS_ENDED;
    }
    FILE* file = fopen(path_file, "w");
    if (file == NULL)
    {
        return CHESS_SAVE_FAILURE;
    }
    MAP_FOREACH(MapKeyElement, tournament_id, chess->tournaments)
    {
        tournamentSaveStatistics(mapGet(chess->tournaments, tournament_id), file);
        free(tournament_id);
    }
    fclose(file);
    return CHESS_SUCCESS;
}

static bool checkTournamentLocation(const char* location)
{
    if (*location<'A' || *location>'Z')
    {
        return false;
    }
    for (int i = 1; i < strlen(location); i++)
    {
        if (*(location + i) < 'a' || *(location + i) > 'z')
        {
            if (*(location + i) == ' ')
            {
                continue;
            }
            return false;
        }
    }
    return true;
}
















