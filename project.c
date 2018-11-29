#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

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
		// Do we have extra arguments to process.
		stat(entryPtr->d_name, &statBuf);

		// Add user and group ID for each file.
		if (file_id) {
			// Print out mode.
			// https://stackoverflow.com/questions/10323060/printing-file-permissions-like-ls-l-using-stat2-in-c
			printf( (S_ISDIR(statBuf.st_mode)) ? "d" : "-");
		    printf( (statBuf.st_mode & S_IRUSR) ? "r" : "-");
		    printf( (statBuf.st_mode & S_IWUSR) ? "w" : "-");
		    printf( (statBuf.st_mode & S_IXUSR) ? "x" : "-");
		    printf( (statBuf.st_mode & S_IRGRP) ? "r" : "-");
		    printf( (statBuf.st_mode & S_IWGRP) ? "w" : "-");
		    printf( (statBuf.st_mode & S_IXGRP) ? "x" : "-");
		    printf( (statBuf.st_mode & S_IROTH) ? "r" : "-");
		    printf( (statBuf.st_mode & S_IWOTH) ? "w" : "-");
		    printf( (statBuf.st_mode & S_IXOTH) ? "x" : "-");

			// Print the number of links.
			printf("%-8d ", statBuf.st_nlink);

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

			// Print the total size, in bytes.
			printf("%-8lld ", statBuf.st_size);

			// Format the time.
			time_t t = statBuf.st_mtime;
			struct tm lt;
			char mtime[80];
			localtime_r(&t, &lt);
			strftime(mtime, sizeof(mtime), "%b %d %T", &lt);
			// Print time of last modification.
			printf("%s ", mtime);

		}

		// Add inode # for each file.
		if (inode_n) {
			printf("%lu ", statBuf.st_ino);
		}

		// Print name of file.
		printf("%-20s\n", entryPtr->d_name);
	}

	// Close everything up and exit.
	closedir(dirPtr);
	return 0;
}
