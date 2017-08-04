#include "TesterApplication.h"


#include <EASTL/string.h>
#include <Windows.h>
#include <conio.h>



using namespace hct;
using namespace eastl;
void main(int argc, char *argv[])
{
	string args;
	for (int i = 1; i < argc; ++i)
	{
		if (1 != i)args.append(" ");
		args.append(argv[i]);
	}
	TesterApplication app;
	char strFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileNameA(NULL, strFilePath, MAX_PATH);
	(strrchr(strFilePath, ('\\')))[1] = 0;
	SetCurrentDirectoryA(strFilePath);
	string path(strFilePath);
	app.Initialize(path, args);
	app.Dowork();
	printf("Press any key to exit.\r\n");
	_getch();
	app.Exit();
};