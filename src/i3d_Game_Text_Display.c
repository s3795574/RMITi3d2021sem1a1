#include "i3d_Game_Text_Display.h"
#include < stdio.h >

void game_log_init(Game_Log* game_log, int width, int height, char* minutes, char* seconds, char* score_char) {
	game_log->score_char = score_char;
	game_log->score = 0;
	sprintf_s(game_log->score_char, 100, "%d", game_log->score);
	game_log->score_prefix = "Score: ";
	game_log->total_time_prefix = "Game Time: ";
	game_log->minutes = minutes;
	game_log->seconds = seconds;
	game_log->width = width;
	game_log->height = height;
	game_log->game_over = "Game Over.Press any key to play again...";
	game_log->start_game = "Press any key to start a new game!";
}

void convert_time_minute(int current_time, char* minutes){
	int minutes_local = current_time / 60;
	sprintf_s(minutes, 100, "%d minutes ", minutes_local);
}
void convert_time_second(int current_time, char* seconds) {
	int seconds_local = current_time % 60;
	sprintf_s(seconds, 100, "%d seconds", seconds_local);
}

void update_game_time(Game_Log* game_log, int current_time, char* minutes, char* second) {
	convert_time_minute(current_time, minutes);
	game_log->minutes = minutes;
	convert_time_second(current_time, second);
	game_log->seconds = second;
}

void update_game_score(Game_Log* game_log) {
	game_log->score += 1;
	sprintf_s(game_log->score_char, 100, "%d", game_log->score);
}

void reset_game_info(Game_Log* game_log) {

}