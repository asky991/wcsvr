
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <string>
using   namespace   std;

#ifdef WIN32
#include "winsvr.h"
#else
#include "linuxsvr.h"
#endif // WIN32




#include "version.h"
#define  WCSER_NAME			"wcsvr"

int show_info(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	log_info(argc, argv);
	if (argc > 1 && show_info(argc, argv)==0)
	{
		return 0;
	}
	if (argc > 1 && init_svr(argc, argv) == 0) return 0;

	//init_proc();

	return 0;
}

int show_info(int argc, char* argv[])
{
	for (int i = 1; i<argc; i++)
	{

		if (0 == strcmp("-v", argv[i]) || 0 == strcmp("-version", argv[i]))
		{
			printf("%s \nPRODUCTVERSION : %s\nFILEVERSION : %s\n(%s,%s)\n", WCSER_NAME, VER_PRODUCTVERSION_STR, VER_FILEVERSION_STR, __DATE__, __TIME__);
		}
		else if (0 == strcmp("-h", argv[i]))
		{
			printf("Usage: %s [options]\n", WCSER_NAME);
			printf("Options:\n");
			printf("  void\tRun %s\n", WCSER_NAME);
			printf("  -v\tDisplay %s version information\n", WCSER_NAME);
			printf("  -h\tDisplay help information\n");
		}
		else
		{
			return -1;
		}

	}
	cout << "Please enter any key to exit...";
	getchar();
	return 0;
}