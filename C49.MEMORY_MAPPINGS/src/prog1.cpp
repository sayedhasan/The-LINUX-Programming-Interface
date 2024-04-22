#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <cstring>
#include <fcntl.h>
#include <cstdlib>
#include <unistd.h>

int
main(int argc, char* argv[])
{
  if (argc != 2 || strcmp(argv[1], "--help") == 0) {
    fprintf(stderr, "Usage: %s file\n", argv[0]);
    exit(EXIT_SUCCESS);
  }
  
  int fd = open(argv[1], O_RDONLY);
  if (fd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  // obtain the size of the file
  struct stat info;
  if (fstat(fd, &info) == -1) {
    perror("fstat");
    exit(EXIT_FAILURE);
  }
  
  // map the file to memory
  char* addr = static_cast<char*>(mmap(nullptr, info.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
  if (addr == MAP_FAILED) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }

  if (write(STDOUT_FILENO, addr, info.st_size) != info.st_size) {
    perror("partial/failed write");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}
