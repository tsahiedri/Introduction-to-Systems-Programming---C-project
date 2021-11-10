#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tournament.h"
#include "map.h"
#include "player.h"
#include "game.h"
#include "insideFunctions.h"
#include "chessSystem.h"

#define TOURNAMENT_FINISHED 1
#define TOURNAMENT_ON 0

struct tournament_t {
	Map games;
	char* tournament_location;
	int max_games_per_player;
	bool status;
	int winner;
	int total_games_time;
	int longest_game_time;
	int num_of_games;
	Map players;
};

Tournament tournamentCreate(int max_games_per_player, const char* tournament_location)
{
	Tournament tournament = malloc(sizeof(*tournament));
	if (tournament == NULL)
	{
		return NULL;
	}
	tournament->max_games_per_player = max_games_per_player;
	tournament->tournament_location = malloc(sizeof(char) * (strlen(tournament_location) + 1));
	if (tournament->tournament_location == NULL)
	{
		free(tournament);
		return NULL;
	}
	strcpy(tournament->tournament_location, tournament_location);
	tournament->total_games_time = 0;
	tournament->longest_game_time = 0;
	tournament->num_of_games = 0;
	tournament->winner = 0;
	tournament->status = TOURNAMENT_ON;
	tournament->players = mapCreate(playerCopy, copyId, playerDestroy, freeId, compareIds);
	tournament->games = mapCreate(gameCopy, gameCopyId, gameFree, gameFreeId, gameCompareId);
	return tournament;
}

MapDataElement tournamentCopy(MapDataElement tournament)
{
	return tournament;
}

void tournamentDestroy(MapDataElement tournament)
{
	free(((Tournament)tournament)->tournament_location);
	mapDestroy(((Tournament)tournament)->players);
	mapDestroy(((Tournament)tournament)->games);
	free(tournament);
}

ChessResult tournamentAddGame(Tournament tournament, int first_player, int second_player, 
	Winner winner, int play_time, Map players)
{
	if (tournament->status == TOURNAMENT_FINISHED)
	{
		return CHESS_TOURNAMENT_ENDED;
	}
	Player first = mapGet(tournament->players, &first_player);
	Player second = mapGet(tournament->players, &second_player);
	int game_id[2] = { first_player, second_player };
	Game requested_game = mapGet(tournament->games, &game_id);
	if (requested_game != NULL && (gamePlayerRemoved(requested_game) == false))
	{
		return CHESS_GAME_ALREADY_EXISTS;
	}
	if (play_time < 0)
	{
		return CHESS_INVALID_PLAY_TIME;
	}
	if (playerExceededGames(first, tournament->max_games_per_player) || 
		playerExceededGames(second, tournament->max_games_per_player))
	{
		return CHESS_EXCEEDED_GAMES;
	}
	if (first == NULL)
	{
		first = playerAdd(tournament->players, first_player);
		if (first == NULL)
		{
			return CHESS_OUT_OF_MEMORY;
		}
		if (mapContains(players, &first_player) == false)
		{
			if (playerAdd(players, first_player) == NULL)
			{
				mapRemove(tournament->players, &first_player);
				return CHESS_OUT_OF_MEMORY;
			}
		}
	}
	if (second == NULL)
	{
		second = playerAdd(tournament->players, second_player);
		if (second == NULL)
		{
			return CHESS_OUT_OF_MEMORY;
		}
		if (mapContains(players, &second_player) == false)
		{
			if (playerAdd(players, second_player) == NULL)
			{
				mapRemove(tournament->players, &second_player);
				return CHESS_OUT_OF_MEMORY;
			}
		}
	}
	tournament->longest_game_time = max(play_time, tournament->longest_game_time);
	tournament->total_games_time += play_time;
	Game game = gameCreate(first_player, second_player, winner, play_time, tournament->players, players);
	if (game == NULL)
	{
		return CHESS_OUT_OF_MEMORY;
	}
	mapPut(tournament->games, game_id, game);
	tournament->num_of_games++;
	return CHESS_SUCCESS;
}

void tournamentRemovePlayer(Tournament tournament, int player_id, Map players)
{
	if (mapContains(tournament->players, &player_id) == false || tournament->status == TOURNAMENT_FINISHED)
	{
		return;
	}
	MAP_FOREACH(MapKeyElement, game_id, tournament->games)
	{
		gameRemovePlayer(mapGet(tournament->games, game_id), player_id, tournament->players, players);
		free(game_id);
	}
	mapRemove(tournament->players, &player_id);
}

ChessResult tournamentEnd(Tournament tournament)
{
	if (tournament == NULL)
	{
		return CHESS_TOURNAMENT_NOT_EXIST;
	}
	if (tournament->status == TOURNAMENT_FINISHED)
	{
		return CHESS_TOURNAMENT_ENDED;
	}
	if (mapGetSize(tournament->games) == 0)
	{
		return CHESS_NO_GAMES;
	}
	MapKeyElement first_player = mapGetFirst(tournament->players);
	Player current_winner = mapGet(tournament->players, first_player);
	free(first_player);
	int highest_score = playerGetScore(current_winner);
	tournament->winner = playerGetID(current_winner);
	MAP_FOREACH(MapKeyElement, player_id, tournament->players)
	{
		Player player = mapGet(tournament->players, player_id);
		free(player_id);
		if (playerGetScore(player) > highest_score)
		{
			tournament->winner = playerGetID(player);
			current_winner = player;
			highest_score = playerGetScore(player);
		}
		else if (playerGetScore(player) == highest_score)
		{
			if (playerGetLosses(player) < playerGetLosses(current_winner))
			{
				tournament->winner = playerGetID(player);
				current_winner = player;
				highest_score = playerGetScore(player);
			}
			else if (playerGetLosses(player) == playerGetLosses(current_winner))
			{
				if (playerGetWins(player) > playerGetWins(current_winner))
				{
					tournament->winner = playerGetID(player);
					current_winner = player;
					highest_score = playerGetScore(player);
				}
				else if (playerGetWins(player) == playerGetWins(current_winner))
				{
					if (playerGetID(player) < playerGetID(current_winner))
					{
						tournament->winner = playerGetID(player);
						current_winner = player;
						highest_score = playerGetScore(player);
					}
				}
			}

		}
	}
	tournament->status = TOURNAMENT_FINISHED;
	return CHESS_SUCCESS;
}

void tournamentRemoveStatistics(Tournament tournament, Map players)
{
	MAP_FOREACH(MapKeyElement , id, tournament->players)
	{
		playerRemoveStatistics(mapGet(tournament->players, id), players);
		free(id);
	}
}

void tournamentSaveStatistics(Tournament tournament, FILE* file)
{
	if (tournament->status == TOURNAMENT_ON)
	{
		return;
	}
	int num_of_players = mapGetSize(tournament->players);
	double average_game_time = (double)(tournament->total_games_time) / tournament->num_of_games;
	fprintf(file, "%d\n%d\n%.2f\n%s\n%d\n%d\n", tournament->winner, tournament->longest_game_time,
		average_game_time, tournament->tournament_location, tournament->num_of_games, num_of_players);
}

bool tournamentFinished(Tournament tournament)
{
	return (tournament->status == TOURNAMENT_FINISHED);
}