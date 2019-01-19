#include <unistd.h>
#include <libgen.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>

#define BUF_LEN     20

int read_test(int fd) {
    size_t len = BUF_LEN;
    ssize_t rd;
    char buf[BUF_LEN + 1] = {0};

    printf("read\n");

    while ((rd = read(fd, buf, len)) > 0) {
        for (ssize_t i = 0; i < rd; i++)
            printf("%c", buf[i]);
    }
    printf("\nerr %ld\n", rd);

    return rd;
}

int write_test(int fd) {
    uint8_t msg[BUF_LEN] = "send message\r\n";
    int len = strnlen((char *)msg, BUF_LEN - 1);

    int err = write(fd, msg, len);
    if (err < 0)
        perror("write");
    printf("write\n");

    return err;
}

// connects to the device given and then starts communicating with it
int comm(char *dev) {
    int fd;
    int err;
    speed_t baud = B115200;
    struct termios options;

    fd = open(dev, O_RDWR);
    if (fd < 0) {
        perror("open");
        return fd;
    }

    tcgetattr(fd, &options);

    cfsetspeed(&options, baud); // set input and output baud rates
    options.c_cflag &= ~PARENB; // no parity
    options.c_cflag &= ~CSTOPB; // 1 stop bit
    options.c_cflag &= ~CSIZE; // 8 bits
    options.c_cflag |= CS8 | CLOCAL; // 8 bits
    options.c_lflag = ICANON; //
    options.c_oflag &= ~OPOST;

    tcsetattr(fd, TCSANOW, &options);

    /* write settings */
    tcsetattr(fd, TCSANOW, &options);

    write_test(fd);
    read_test(fd);

    err = close(fd);
    if (err == -1)
        perror("fclose");

    return err;
}

// usage message
void print_usage(char *progname) {
    printf("Usage: %s [ options ]\n"
            "A USART communcation program\n"
            "Options:\n"
            "    -d [device file] sets the device file (required)\n"
            "    -h displays this usage message\n", basename(progname));
}

// main function gets options and call comm to runn program
int main(int argc, char **argv) {
    int c;
    char *tty_dev = NULL;

    while ((c = getopt(argc, argv, "hd:")) != -1) {
        switch (c) {
            case 'h':
                print_usage(argv[0]);
                exit(0);
            case 'd':
                tty_dev = optarg;
                break;
            default:
                print_usage(argv[0]);
                exit(1);
        }
    }

    if (tty_dev == NULL) {
        print_usage(argv[0]);
        exit(1);
    }

    comm(tty_dev);

    return 0;
}
