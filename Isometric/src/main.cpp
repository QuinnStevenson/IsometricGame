//Using SDL, SDL_image, standard IO, strings, and file streams
#include "GameHandler.h"
#include <stdio.h>

using namespace std;

int main(int argc, char* args[])
{
	//Send all reports to STDOUT
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);

	GameHandler newHandler;
	newHandler.begin();

	//Will not print if no leaks found
	_CrtDumpMemoryLeaks();
	return 0;
}