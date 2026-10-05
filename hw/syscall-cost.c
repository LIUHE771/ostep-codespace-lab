#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define N 1000000

static double now_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1e9 + ts.tv_nsec;
}

int main(void) {
    char buf[1];
    double start = now_ns();
    for (int i = 0; i < N; i++) {
        ssize_t r = read(0, buf, 0);
        (void)r;
    }
    double end = now_ns();
    printf("average system call cost: %.1f ns\n", (end - start) / N);
    return 0;
}
