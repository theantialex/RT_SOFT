#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main() {
    int fd; 
    unsigned int t = 0;
    fd = open("/dev/drv",O_RDONLY);
    while (t < 10) {
        read(fd, &t, sizeof(t));
        printf("%d\n", t);
    }
    close(fd);
}