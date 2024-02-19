#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <stdbool.h>

#include "crowarmor/ioctl.h"

int main()
{
    int fd = open("/dev/crowarmor", O_RDWR);
    if (fd < 0)
    {
        perror("Error opening device driver");
        exit(EXIT_FAILURE);
    }

    struct crowarmor crow;

    if (ioctl(fd, IOCTL_READ_CROWARMOR, &crow) == -1)
    {
        perror("Error calling IOCTL_READ_CROWARMOR");
        close(fd);
        exit(EXIT_FAILURE);
    }

    assert(crow.is_actived == 1);

    if (close(fd) == -1)
    {
        perror("Error closing device driver");
        exit(EXIT_FAILURE);
    }

    return 0;
}