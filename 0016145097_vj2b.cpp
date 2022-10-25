#include <iostream>
#include <iomanip>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <csignal>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <cmath>

using namespace std;

pthread_t *polje_dretvi;

struct slog_info_polje {
    long double *polje;
    int br_el;
    int m;
    int threadnum;
} *info_polje;

void prekid (int sig) {
    if (!sig) {
        for (int i = 0; i < info_polje->threadnum; i++) pthread_join(polje_dretvi[i], NULL);
    }
    else {
        for (int i = 0; i < info_polje->threadnum; i++) pthread_kill(polje_dretvi[i], SIGINT);
        delete info_polje;
    }
    exit(0);
}

double faktorijel (double m) {
    double rezultat = 1;
    for (double i = 1; i <= m; i++) {
        rezultat *= rezultat*i;
    }
    return rezultat;
}

void *dretva (void *arg) {
    int i = *((int*)arg);
    int blok = info_polje->br_el / info_polje->threadnum;
    int ostatak = info_polje->br_el % info_polje->threadnum;
    int start = blok * i;
    int kraj = start + blok;
    if (kraj < info_polje->br_el) {
        kraj = info_polje->br_el;
    }
    
    long double rezultat = exp(info_polje->polje[i]);

    printf("%17.11Lf\n", rezultat);
    sleep(30);
    return 0;
}

int main (int argc, char **argv) {
    system("clear");

    if (argc != 4)
    {
        cout << "Moraju se unijeti 3 argumenta\n";
        return 0;
    }

    info_polje = new slog_info_polje;
    info_polje->br_el = atoi(argv[1]); // broj elemenata polja - l
    info_polje->m = atoi(argv[2]); // broj iteracija - m
    info_polje->threadnum = atoi(argv[3]); // broj dretvi - n
    info_polje->polje = new long double [info_polje->br_el];

    polje_dretvi = new pthread_t [info_polje->threadnum];

    sigset(SIGINT, prekid);

    srand(time(0));
    printf("Eksponenti = \n");
    for (int i = 0; i < info_polje->br_el; i++)
    {
        info_polje->polje[i] = (long double) rand()/(RAND_MAX - 1) * 10;
        printf("%.11Lf\n", info_polje->polje[i]);
    }

    int *polje_i = new int [info_polje->threadnum];

    printf("\nEksponencijale =\n");

    for (int i = 0; i < info_polje->threadnum; i++)
    {
        polje_i[i] = i;
        pthread_create(&polje_dretvi[i], NULL, dretva, &polje_i[i]);

    }
//    return 0;
    prekid(0);
}
