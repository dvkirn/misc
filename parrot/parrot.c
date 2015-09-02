/*
 * parrot.c
 * Program to illustrate /dev/dsp device
 * Records several seconds of sound, then echoes it back.
 * Runs until Control-C is pressed.
 *
 * Originally from here:
 *   http://www.oreilly.de/catalog/multilinux/excerpt/ch14-05.htm
 *
 * Stripped down by Shish for easier demonstration
 */

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <linux/soundcard.h>

#define LENGTH 3    /* how many seconds of speech to store */
#define RATE 48000   /* the sampling rate */
#define SIZE 16      /* sample size: 8 or 16 bits */
#define CHANNELS 2  /* 1 = mono 2 = stereo */

/* this buffer holds the digitized audio */
unsigned char buf[LENGTH*RATE*SIZE*CHANNELS/8];

int main() {
    int fd; /* sound device file descriptor */
    int arg;    /* argument for ioctl calls */

    /* open sound device */
    fd = open("/dev/dsp", O_RDWR);
    if (fd < 0) {
        perror("open of /dev/dsp failed");
        return 1;
    }

    arg = SIZE;
    ioctl(fd, SOUND_PCM_WRITE_BITS, &arg);

    arg = CHANNELS;
    ioctl(fd, SOUND_PCM_WRITE_CHANNELS, &arg);

    arg = RATE;
    ioctl(fd, SOUND_PCM_WRITE_RATE, &arg);

    while (1) {
        printf("Say something:\n");
        read(fd, buf, sizeof(buf)); /* record some sound */

        printf("You said:\n");
        write(fd, buf, sizeof(buf)); /* play it back */

        /* wait for playback to complete before recording again */
        ioctl(fd, SOUND_PCM_SYNC, 0); 
    }
}
