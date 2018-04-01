#define _DEFAULT_SOURCE
#define _XOPEN_SOURCE

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef struct tm tm_t;

const int MAX_CHARS = 256;
const int COLOUR_WINDOW_ID = 1;
const int COLOUR_SHADOW_ID = 2;
const int WINDOW_WIDTH = 55;
const int WINDOW_HEIGHT = 9;
const char* IN_TEXT = "- in -";

void usage(void);
void setupNcurses(void);
WINDOW* createWindow(void);
void shadow(const WINDOW* window);

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

	int messageLength = strlen(argv[2]);
	int messageX = (WINDOW_WIDTH / 2) - (messageLength / 2);

	int inLength = strlen(IN_TEXT);
	int inX = (WINDOW_WIDTH / 2) - (inLength / 2);

	int lastCountdownLength = 0;
	int lastCountdownX = 0;

	bool quit = false;
	while(!quit)
	{
		time_t now = time(NULL);
		time_t secondsToTarget = difftime(targetTimeSec, now);

		int seconds = secondsToTarget % 60;
		int minutes = (secondsToTarget % 3600) / 60;
		int hours = (secondsToTarget % 86400) / 3600;
		int days = (secondsToTarget % (86400 * 30)) / 86400;

		char* dayStr = days == 1 ? "day" : "days";
		char* hourStr = hours == 1 ? "hour" : "hours";
		char* minStr = minutes == 1 ? "minute" : "minutes";
		char* secStr = seconds == 1 ? "second" : "seconds";

		char countdownText[MAX_CHARS];
		snprintf(countdownText, MAX_CHARS, "%d %s, %d %s, %d %s and %d %s",
			days, dayStr, hours, hourStr, minutes, minStr, seconds, secStr);

		int countdownLength = strlen(countdownText);
		int countdownX = (WINDOW_WIDTH / 2) - (countdownLength / 2);
		int countdownY = (WINDOW_HEIGHT / 2) + 1;

		// This erases the previous countdown line, which is needed because the
		// new line can be smaller and leave behind characters from the previous.
		for(int i = 0; i < lastCountdownLength; i++)
			mvwprintw(window, countdownY, lastCountdownX + i, " ");

		mvwprintw(window, countdownY - 2, messageX, argv[2]);
		mvwprintw(window, countdownY - 1, inX, IN_TEXT);
		mvwprintw(window, countdownY, countdownX, countdownText);

		lastCountdownLength = countdownLength;
		lastCountdownX = countdownX;

		refresh();
		wrefresh(window);

		sleep(1);

		int userInput = getch();
		if(userInput == 'q')
			quit = true;
	}

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

	int windowX = (termWidth / 2) - (WINDOW_WIDTH / 2);
	int windowY = (termHeight / 2) - (WINDOW_HEIGHT / 2);

	WINDOW* window = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, windowY, windowX);
	box(window, 0, 0);
	wbkgd(window, COLOR_PAIR(COLOUR_WINDOW_ID));
	shadow(window);
	wrefresh(window);

	return window;
}

void shadow(const WINDOW* window)
{
	int winWidth;
	int winHeight;
	int winX;
	int winY;

	getmaxyx(window, winHeight, winWidth);
	getbegyx(window, winY, winX);

	mvchgat(winY + winHeight, winX + 1, winWidth, A_NORMAL, COLOUR_SHADOW_ID, NULL);
	for(int i = 0; i < winHeight - 1; i++)
		mvchgat(winY + i + 1, winX + winWidth, 1, A_NORMAL, COLOUR_SHADOW_ID, NULL);
}
