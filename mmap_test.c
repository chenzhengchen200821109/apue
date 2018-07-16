/*
 * The following program prints part of the file specified int its first command-line argument
 * to standart output. The range of bytes to be printed is specified via offset and length values
 * int the second and third command-line arguments. The program creates a memory mapping of the
 * required pages of the file and then uses write(2) to output the desired bytes.
 */

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char *argv[])
{
    char *addr;
    int fd;
    struct stat sb;
    off_t offset, pa_offset;
    size_t length;
    ssize_t s;

    if (argc < 3 || argc > 4) {
        fprintf(stderr, "%s file offset [length]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if ((fd = open(argv[1], O_RDONLY)) == -1)
        handle_error("open");
    if (fstat(fd, &sb) == -1) /* to obtain file size */
        handle_error("fstat");
    offset = atoi(argv[2]);
    pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1); /* offset for mmap() must be page aligned */
    if (offset >= sb.st_size) {
        fprintf(stderr, "offset is past end of file\n");
        exit(EXIT_FAILURE);
    }
    if (argc == 4) {
        length = atoi(argv[3]);
        if (offset + length > sb.st_size)
            length = sb.st_size - offset; /* cannot display bytes past end of file */
    } else { /* no length arg, display to end of file */
        length = sb.st_size - offset;
    }
    if ((addr = mmap(NULL, length + offset - pa_offset, PROT_READ, MAP_PRIVATE, fd, pa_offset)) == MAP_FAILED)
        handle_error("mmap");
    if ((s = write(STDOUT_FILENO, addr + offset - pa_offset, length)) != length) {
        if (s == -1)
            handle_error("write");
        fprintf(stderr, "partial write");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
