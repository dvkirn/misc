#include <stdio.h>
#include <unistd.h>

typedef struct {
    char flags;
    char dx;
    char dy;
} mousein;

void char_to_binstr(char c, char *buf) {
    int i;
    for(i=0; i<8; i++) {
        buf[i] = ((c >> i) & 1) ? '1' : '0';
    }
    buf[8] = '\0';
}

int main(int argc, char *argv[]) {
    FILE *fd;
    mousein ev;
    char flags[9];

    fd = fopen("/dev/input/mice", "rw");
    if(!fd) {
        perror("Failed to open mouse device");
        return 1;
    }

    while(1) {
        fread(&ev, sizeof(mousein), 1, fd);

        char_to_binstr(ev.flags, flags);
        printf("flags: %s  movement: (%d, %d)\n", flags, ev.dx, ev.dy);

        // simulate "time spent doing other stuff"
        usleep(100000);
    }

    return 1;
}
