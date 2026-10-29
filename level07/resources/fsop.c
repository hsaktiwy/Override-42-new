#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define PORT 4244
#define DATA_SIZE 64
#define MAX_STREAMS 16
struct stream;
struct vtable {
    void (*flush)(struct stream *);
    void (*close)(struct stream *);
};
struct stream {
    struct vtable *vt;
    char data[DATA_SIZE];
};
static struct stream *streams[MAX_STREAMS];
__attribute__((used)) static void win(void) {
    system("/bin/sh");
}
static void real_flush(struct stream *s) {
    write(STDOUT_FILENO, s->data, DATA_SIZE);
}
static void real_close(struct stream *s) { (void)s; }
static struct vtable default_vt = { real_flush, real_close };
static void handle_alarm(int sig) { (void)sig; _exit(1); }
static long rd_long(int fd) {
    char b[32];
    ssize_t n = read(fd, b, sizeof(b) - 1);
    if (n <= 0) _exit(0);
    b[n] = 0;
    return strtol(b, NULL, 0);
}
static void put(int fd, const char *s) { write(fd, s, strlen(s)); }
static void session(int fd) {
    char line[64];
    signal(SIGALRM, handle_alarm);
    alarm(60);
    put(fd, "\033[0;36m  [MATRIX] File stream corrupted.\n");
    put(fd, "  [FSOP] Stream multiplexer v1.2\033[0m\n");
    snprintf(line, sizeof line, "[FSOP] loader @ %p\n", (void *)win);
    put(fd, line);
    for (;;) {
        put(fd, "\n[FSOP] 1)create 2)write 3)flush 4)quit\n> ");
        switch (rd_long(fd)) {
        case 1: {
            put(fd, "slot: ");
            long i = rd_long(fd);
            if (i < 0 || i >= MAX_STREAMS) break;
            streams[i] = malloc(sizeof(struct stream));
            streams[i]->vt = &default_vt;
            memset(streams[i]->data, 0, DATA_SIZE);
            snprintf(line, sizeof line, "[FSOP] stream %ld @ %p\n",
                     i, (void *)streams[i]);
            put(fd, line);
            break;
        }
        case 2: {
            put(fd, "slot: ");
            long i = rd_long(fd);
            if (i < 0 || i >= MAX_STREAMS || !streams[i]) break;
            put(fd, "len: ");
            long n = rd_long(fd);
            put(fd, "data: ");
            read(fd, streams[i]->data, n);
            break;
        }
        case 3: {
            put(fd, "slot: ");
            long i = rd_long(fd);
            if (i < 0 || i >= MAX_STREAMS || !streams[i]) break;
            streams[i]->vt->flush(streams[i]);
            break;
        }
        default:
            return;
        }
    }
}
static void run_server(void) {
    int srv, cli, opt = 1;
    struct sockaddr_in a;
    socklen_t l = sizeof(a);
    srv = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    memset(&a, 0, sizeof(a));
    a.sin_family = AF_INET;
    a.sin_addr.s_addr = INADDR_ANY;
    a.sin_port = htons(PORT);
    bind(srv, (struct sockaddr *)&a, sizeof(a));
    listen(srv, 8);
    while (1) {
        cli = accept(srv, (struct sockaddr *)&a, &l);
        if (cli < 0) continue;
        if (fork() == 0) {
            close(srv);
            dup2(cli, 0); dup2(cli, 1);
            session(cli);
            close(cli);
            _exit(0);
        }
        close(cli);
    }
}
int main(void) {
    signal(SIGCHLD, SIG_IGN);
    run_server();
    return 0;
}
