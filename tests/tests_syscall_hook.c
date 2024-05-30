#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/memfd.h>

int main() {
    int fd;

    fd = syscall(SYS_memfd_create, "memfd_example", MFD_CLOEXEC);
    if (fd == -1) {
        perror("memfd_create");
        exit(EXIT_FAILURE);
    }

    printf("memfd created successfully! file descriptor: %d\n", fd);

    close(fd);

    return 0;
}