#include <iostream>
#include <iomanip>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <csignal>
#include <pthread.h>
#include <stdio.h>

using namespace std;

struct slog_info_polje {
    long double *polje;
    int br_el;
    int blok;
    int m;
} *info_polje;

void *dretva (void *arg) {
    int i = *((int*)arg);
    int start = info_polje->blok * i;
    int kraj = start + info_polje->blok;

    if (kraj > info_polje->br_el) {
        kraj = info_polje->br_el;
    }

//    printf("Dretva %d\n", i);

    for (int i = start; i < kraj; i++) {
            long double e = exp(info_polje->polje[i]);
            printf("%Lf\n", e);
            exit(0);
    }
    exit(0);
}

int broj_dretvi;
pthread_t *polje_dretvi;

void kraj (int sig) {
    if (!sig) {
        for (int i = 0; i < broj_dretvi; i++) pthread_join(polje_dretvi[i], NULL);
    }
    else {
        for (int i = 0; i < broj_dretvi; i++) pthread_kill(polje_dretvi[i], SIGKILL);
        delete info_polje;
    }
    exit(0);
}

int main (int argc, char **argv) {
    system("clear");

    if (argc != 4)
    {
        cout << "Moraju se unijeti 3 argumenta\n";
        return 0;
    }

    info_polje = new slog_info_polje;
    info_polje->br_el = atoi(argv[1]);
    info_polje->blok = atoi(argv[2]);
    info_polje->m = atoi(argv[3]);
    info_polje->polje = new long double [info_polje->br_el];

    sigset(SIGINT, kraj);

    srand(time(0));
    printf("Eksponenti = \n");
    for (int i = 0; i < info_polje->br_el; i++)
    {
        info_polje->polje[i] = (long double) rand()/(RAND_MAX - 1) * 10;
        printf("%.11Lf\n", info_polje->polje[i]);
    }

    broj_dretvi = info_polje->br_el / info_polje->blok;
    if (info_polje->br_el % info_polje->blok) broj_dretvi++;

    polje_dretvi = new pthread_t [broj_dretvi];

    int *polje_i = new int [broj_dretvi];

    printf("\nEksponencijale =\n");

    for (int i = 0; i < broj_dretvi; i++)
    {
        polje_i[i] = i;
        pthread_create(&polje_dretvi[i], NULL, dretva, &polje_i[i]);

    }

    kraj(0);
}
