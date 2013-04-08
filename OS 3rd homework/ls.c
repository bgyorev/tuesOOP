#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

int ls(char* path, int option);
int has_option(int argc, char* argv[]);
int print_things(char* path);

int main(int argc, char* argv[]) {
	if(argc == 1) {
		ls(".", 0);
	}else{
		int i;
		for(i = 1; i < argc; i++) {
			int option = has_option(argc, argv);
			if(strcmp(argv[i], "-l") == 0) {
				if(argc == 2)
					ls(".", option);
				
			}else{
				if(argc>2)
					printf("%s:\n", argv[i]);
				ls(argv[i], option);
			}
			
		}
	}	

	return 0;
}

/*Function ls
the function that prints the files in the directory
Parameters:
char* path - this is the path to the file
int option - works like a boolean to check if option -l is activated*/

int ls(char* path, int option) {

	struct dirent **d;
	int dir, i, is_file = 0;
	
	dir = scandir(path, &d, 0, alphasort);

	if(dir < 0){
		int file = scandir(".", &d, 0, alphasort);
		for(i = 0; i < file; i++) {
			if(strcmp(path, d[i]->d_name)==0){
				if(option){
					print_things(path);
					printf("%s\n", path);	
				}else{
					printf("%s\n", path);
				}
				is_file = 1;
			}
			
		}
		if(!is_file){
			fprintf(stderr, "%s : %s\n", path, strerror(errno));
			return 1;
		}else{
			return 0;
		}
	}

	if(option){
		for(i = 0; i < dir; i++) {
				print_things(d[i]->d_name);
				if(strcmp(d[i]->d_name,".") == 0 || strcmp(d[i]->d_name,"..") == 0)
					continue;
				printf("%s\n", d[i]->d_name);	
				free(d[i]);	
			}	

	}else{
		for(i = 0; i < dir; i++) {
			if(strcmp(d[i]->d_name,".") == 0 || strcmp(d[i]->d_name,"..") == 0)
				continue;
			if(i%3 == 0) {
				printf("%-20s \n", d[i]->d_name);	
			}else{
				printf("%-20s ", d[i]->d_name);
			}
		
			free(d[i]);	
	}
	printf("\n");
	free(d);
		}
}

/*Function print_things
printing everything for the file, when -l is activated
Parameters:
char* path - the path to the file*/
int print_things(char* path) {
	struct stat fileStat;
	if(stat(path,&fileStat) < 0)    
		return 1;
				
		printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
		printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
		printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
		printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
		printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
		printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
		printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
		printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
		printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
		printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
		printf(" ");
	
		printf("%-2d",fileStat.st_nlink);
		
		struct passwd *pw = getpwuid(fileStat.st_uid);
		struct group  *gr = getgrgid(fileStat.st_gid);				
		
		printf("%-8s", pw->pw_name);
		printf("%-8s", gr->gr_name);
		printf("%-6d",fileStat.st_size);
				
		char buffer[30];
		time_t mtime;
		struct tm *timeinfo;
	
		mtime = fileStat.st_mtime;
		timeinfo = localtime(&mtime);
		strftime(buffer, 30, "%Y-%m-%d %H:%M",timeinfo);
			
		printf(" %-15s ", buffer);			
}

/*Function - has_option
check if option -l is activated
Parameters:
int argc - number of command line arguments
char* argv[] - command line arguments value*/
int has_option(int argc, char* argv[]){
	int i;
	for(i = 1; i < argc; i++){
		if(strcmp(argv[i], "-l") == 0)
			return 1;
	}	
	return 0;
}
