#if _WIN32
#   include <Windows.h>
#endif
#if __APPLE__
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#   include <GLUT/glut.h>
#else
#   include <GL/gl.h>
#   include <GL/glu.h>
#   include <GL/glut.h>
#endif

//This sturcture represents the game record
typedef struct game_log {
	char* score_prefix;
	char* score_char;
	int score;
	char* total_time_prefix;
	char* minutes;
	char* seconds;
	char* game_over;
	int width;
	int height;
}Game_Log;

extern void game_log_init(Game_Log* game_log, int width, int height, char* minutes, char* seconds, char* score_char);
//current_time in seconds. Convert the time to a string in the format of : x minutes y seconds.
extern void convert_time_minute(int current_time, char* minutes);
extern void convert_time_second(int current_time, char* seconds);
extern void update_game_score(Game_Log* game_log);
extern void update_game_time(Game_Log* game_log, int current_time, char* minutes, char* second);
extern void reset_game_info(Game_Log* game_log);