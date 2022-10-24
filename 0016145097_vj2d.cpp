#include <iostream>
#include <iomanip>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <csignal>

using namespace std;

struct slog_info_polje {
    long double *polje;
    int br_el;
    int blok;
} *info_polje;

void proces (int i) {
    int start = info_polje->blok * i;
    int kraj = start + info_polje->blok;

    if (kraj > info_polje->br_el) {
        kraj = info_polje->br_el;
    }

    printf("Dijete %d\n", i);

    for (int i = start; i < kraj; i++) {
            long double e = exp(info_polje->polje[i]);
            printf("%Lf\n", e);
    }
}

key_t id_dij_mem;

int broj_procesa;
void kraj (int sig) {
    for (int i = 0; i < broj_procesa; i++) wait(NULL);
    shmdt(info_polje);
    shmctl(id_dij_mem, IPC_RMID, NULL);
    exit(0);
}

int main (int argc, char **argv) {
    system("clear");

    if (argc != 3)
    {
        cout << "1. argument = velicina polja\n2. argument = velicina bloka za obradu\n";
        return 0;
    }

    id_dij_mem = shmget(IPC_PRIVATE, sizeof(slog_info_polje), 0600);

    info_polje = (slog_info_polje *) shmat(id_dij_mem, NULL, 0);
    info_polje->br_el = atoi(argv[1]);
    info_polje->blok = atoi(argv[2]);
    info_polje->polje = new long double [info_polje->br_el];

    sigset(SIGINT, kraj);

    srand(time(0));
    for (int i = 0; i < info_polje->br_el; i++)
    {
        info_polje->polje[i] = (long double) rand()/(RAND_MAX - 1) * 10;
        cout << i << ". " << setprecision(12) << info_polje->polje[i] << endl;
    }

    broj_procesa = info_polje->br_el / info_polje->blok;
    if (info_polje->br_el % info_polje->blok) broj_procesa++;

    for (int i = 0; i < broj_procesa; i++)
    {
        switch (fork())
        {
        case 0:
        {
            proces(i);
            exit(0);
        }
        case -1:
        {
            cout << "Fatalna pogreška!\n";
            exit(1);
        }
        }
    }

    kraj(0);
}
