#include <stdlib.h>
#include <fcntl.h> // File control library
#include <err.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
void cat(rfd)
	int rfd;
{
	// read from a file
	// to stdout
	int wfd;
	// with a buffer
	static char *buf;
	// with the correct block size
	static size_t bsize;
	// that we get from the file stats
	struct stat sbuf;

	wfd = fileno(stdout);  // Get file descriptor of destination file
			       // In this case stdout

	if(!buf){
	
		if(fstat(wfd, &sbuf)){
			err(1, "stdout");
		}
		bsize = sbuf.st_blksize;
		buf = malloc(bsize);
		if(!buf){
			err(1,0);
		}
	}
	ssize_t nr, nw;
	int offset = 0;

	nr = read(rfd, buf, bsize);
	while(nr > 0){
		for(offset = 0; nr > 0; nr-= nw, offset += nw){
			nw = write(wfd, buf+offset, nr);
			if(nw < 0){
				err(1, "stdout");
			}
		}
		nr = read(rfd, buf, bsize);
	}
}

int main(argc, argv)
	int argc;
	char *argv[];
{
	// Cat takes a list of file names
	// copy their contents to stdout
	// if one of filenme is '-', read from stdin
	// no flags
	int fd;
	
	++argv; // increment to skip program name (which is the first argument)
	fd = fileno(stdin); // Get file descriptor
	do{
		if(*argv){ // if argument is passed in to command
			if(strcmp(*argv, "-")== 0){ // if argument is "-"
				fd = fileno(stdin); // then file is stdin 
			}else{
				fd = open(*argv, O_RDONLY); // else get file descriptor of file passed as argument
			}
			if(fd < 0){
				err(1, "%s", *argv);  // if fd < 0 assert error
			}
			++argv;		
		}
		cat(fd); // Call function to print to console
	}while(*argv);
	if(fd != fileno(stdin)){
		close(fd);
	}
}
