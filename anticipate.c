#define _DEFAULT_SOURCE
#define _XOPEN_SOURCE

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct tm tm_t;

const int MAX_CHARS = 256;
const int COLOUR_WINDOW_ID = 1;
const int COLOUR_SHADOW_ID = 2;
const int WINDOW_WIDTH = 55;
const int WINDOW_HEIGHT = 9;

void usage(void);
void setupNcurses(void);
WINDOW* createWindow(void);

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		usage();
		return 1;
	}

	tm_t targetTime;
	time_t targetTimeSec;

	if((strptime(argv[1], "%Y-%m-%d %H:%M:%S", &targetTime)) == NULL)
	{
		printf("Could not parse date/time!\n");
		usage();
		return 1;
	}

	targetTimeSec = mktime(&targetTime);

	setupNcurses();
	WINDOW* window = createWindow();

	delwin(window);
	endwin();

	return 0;
}

void usage(void)
{
	printf("Usage: anticipate DATETIME TEXT\n\n"
		"       DATETIME  One string in the format YYYY-MM-DD HH:MM:SS\n"
		"                 (time is in 24-hour format)\n"
		"       TEXT      What's the thing you're anticipating?\n"
		"                 (one string)\n");
}

void setupNcurses(void)
{
	initscr();
	start_color();
	use_default_colors();
	curs_set(0);
	cbreak();
	keypad(stdscr, true);
	noecho();
	nodelay(stdscr, true);
	refresh();

	init_pair(COLOUR_WINDOW_ID, COLOR_WHITE, COLOR_RED);
	init_pair(COLOUR_SHADOW_ID, COLOR_BLACK, COLOR_BLACK);
}

WINDOW* createWindow(void)
{
	int termWidth;
	int termHeight;
	getmaxyx(stdscr, termHeight, termWidth);

	int windowX = termWidth / 2 - (WINDOW_WIDTH / 2);
	int windowY = termHeight / 2 - (WINDOW_HEIGHT / 2);

	WINDOW* window = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, windowY, windowX);
	box(window, 0, 0);
	wbkgd(window, COLOR_PAIR(COLOUR_WINDOW_ID));
	wrefresh(window);

	return window;
}
