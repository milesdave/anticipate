#define _DEFAULT_SOURCE
#define _XOPEN_SOURCE

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct tm tm_t;

const int MAX_CHARS = 256;

void usage(void);

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
