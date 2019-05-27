#include <unistd.h>
#include <signal.h>

int main(int argc, char **argv) {
    kill(getppid(), SIGQUIT);
}
