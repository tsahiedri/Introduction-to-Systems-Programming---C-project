#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "map.h"
#include "game.h"
#include "player.h"
#include"insideFunctions.h"


struct game_t {
    int first_player;
    int second_player;
    Winner winner;
    int play_time;
    bool a_player_was_removed;
};

Game gameCreate(int first_player, int second_player, Winner winner,
    int play_time, Map tournament_players, Map chess_players)
{
    Game game = malloc(sizeof(*game));
    if (game == NULL)
    {
        return NULL;
    }
    game->first_player = first_player;
    game->second_player = second_player;
    game->winner = winner;
    game->play_time = play_time;
    game->a_player_was_removed = false;
    Player first_in_tournament = mapGet(tournament_players, &first_player);
    Player first_in_chess = mapGet(chess_players, &first_player);
    Player second_in_tournament = mapGet(tournament_players, &second_player);
    Player second_in_chess = mapGet(chess_players, &second_player);
    if (winner == DRAW)
    {
        playerAddDraw(first_in_tournament, play_time);
        playerAddDraw(first_in_chess, play_time);
        playerAddDraw(second_in_tournament, play_time);
        playerAddDraw(second_in_chess, play_time);
    }
    else if (winner == FIRST_PLAYER)
    {
        playerAddWin(first_in_tournament, play_time);
        playerAddWin(first_in_chess, play_time);
        playerAddLose(second_in_tournament, play_time);
        playerAddLose(second_in_chess, play_time);
    }
    else if (winner == SECOND_PLAYER)
    {
        playerAddLose(first_in_tournament, play_time);
        playerAddLose(first_in_chess, play_time);
        playerAddWin(second_in_tournament, play_time);
        playerAddWin(second_in_chess, play_time);
    }
    return game;
}

void gameRemovePlayer(Game game, int player_id, Map tournament_players, Map players)
{
    if ((player_id != game->first_player && player_id != game->second_player) || game->a_player_was_removed == true)
    {
        return;
    }
    Player player_to_change_tournament;
    Player player_to_change_chess;
    game->a_player_was_removed = true;
    Winner old_winner = game->winner;
    if (player_id == game->first_player)
    {
        if (old_winner == SECOND_PLAYER)
        {
            return;
        }
        player_to_change_tournament = mapGet(tournament_players, &(game->second_player));
        player_to_change_chess = mapGet(players, &(game->second_player));
        game->winner = SECOND_PLAYER;
    }
    else if (player_id == game->second_player)
    {
        if (old_winner == FIRST_PLAYER)
        {
            return;
        }
        player_to_change_tournament = mapGet(tournament_players, &(game->first_player));
        player_to_change_chess = mapGet(players, &(game->first_player));
        game->winner = FIRST_PLAYER;
    }
    if (old_winner == DRAW)
    {
        playerChangeDrawToWin(player_to_change_tournament);
        playerChangeDrawToWin(player_to_change_chess);
    }
    else
    {
        playerChangeLoseToWin(player_to_change_tournament);
        playerChangeLoseToWin(player_to_change_chess);
    }

}

void gameFree(MapDataElement game)
{
    free(game);
}

MapDataElement gameCopy(MapDataElement game)
{
    return game;
}

MapKeyElement gameCopyId(MapKeyElement id)
{
    int* copy = malloc(2 * sizeof(int));
    if (copy == NULL)
    {
        return NULL;
    }
    copy[0] = ((int*)id)[0];
    copy[1] = ((int*)id)[1];
    return (MapKeyElement)copy;
}

int  gameCompareId(MapKeyElement id1, MapKeyElement id2)
{
    int id1_bigger = max(((int*)id1)[0], ((int*)id1)[1]);
    int id1_smaller = min(((int*)id1)[0], ((int*)id1)[1]);
    int id2_bigger = max(((int*)id2)[0], ((int*)id2)[1]);
    int id2_smaller = min(((int*)id2)[0], ((int*)id2)[1]);
    if (id1_bigger == id2_bigger)
    {
        return id1_smaller - id2_smaller;
    }
    return id1_bigger - id2_bigger;
}

void gameFreeId(MapKeyElement id)
{
    free(id);
}

bool gamePlayerRemoved(Game game)
{
    return game->a_player_was_removed;
}