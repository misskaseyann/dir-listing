#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

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
	// Password information.
	struct passwd *pwd;
	// Group ID information.
	struct group *grp;
	// -n and -i bools.
	int inode_n = 0, file_id = 0;
	// File path.
	char fp[50] = ".";

	// Check command line arguments.
	for (int i = 1; i < argc; i++) {
		if (strcmp("-i", argv[i]) == 0) {
			inode_n = 1;
		} else if (strcmp("-n", argv[i]) == 0) {
			file_id = 1;
		}
	}

	// Was a file path given.
	if (argc > 1 && strchr(argv[argc - 1], '/') != NULL) {
		strcpy(fp, argv[argc - 1]);
	}
	
	// Open the directory.
	if ((dirPtr = opendir(fp)) == NULL) {
		printf("Not a directory.\n");
		return 1;
	}

	// Loop through entries in directory file.
	while ((entryPtr = readdir(dirPtr))) {
		printf("%-20s ", entryPtr->d_name);
		// Do we have extra arguments to process?
		stat(entryPtr->d_name, &statBuf);

		// Add user and group ID for each file.
		if (file_id) {
			// Print out owners name if found using getpwuid()
			if ((pwd = getpwuid(statBuf.st_uid)) != NULL)
				printf("%-8.8s ", pwd->pw_name);
			else
				printf("%-8d ", statBuf.st_uid);

			// Print out group name if found using getgrgid()
			if ((grp = getgrgid(statBuf.st_gid)) != NULL)
				printf("%-8.8s ", grp->gr_name);
			else
				printf("%-8d ", statBuf.st_gid);
		}

		// Add inode # for each file.
		if (inode_n) {
			printf("%lu ", statBuf.st_ino);
		}

		printf("\n");
	}

	// Close everything up and exit.
	closedir(dirPtr);
	return 0;
}
