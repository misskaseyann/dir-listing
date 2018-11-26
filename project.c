#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

/*******************************
 * Lab Week 12
 * File System Interface: Information
 * Programming Assignment (ls - Directory Listing)
 * By Kasey Stowell + Kehlsey Lewis
 *******************************/

int main (int argc, char *argv[]) {
	// Directory file sits here.
	DIR *dirPtr;
	// Pointer to directory entries.
	struct dirent *entryPtr;
	// inode information.
	struct stat statBuf;
	// -n and -i bools.
	int inode_n = 0, file_id = 0;
	// File path.
	char fp[50] = ".";

	// Check command line arguments.
	for (int i = 1; i < argc; i++) {
		if (strcmp("-i", argv[i]) == 0) {
			file_id = 1;
			printf("-i was found\n");
		} else if (strcmp("-n", argv[i]) == 0) {
			inode_n = 1;
			printf("-n was found\n");
		}
	}

	// Was a file path given?
	if (argc > 1 && strchr(argv[argc - 1], '/') != NULL) {
		printf("file path given.\n");
		strcpy(fp, argv[argc - 1]);
	}
	
	
}