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
        delete info_polje;
    }
    else {
        for (int i = 0; i < info_polje->threadnum; i++) pthread_kill(polje_dretvi[i], SIGKILL);
        delete info_polje;
    }
    exit(0);
}

int faktorijel (int m) {
    int rezultat = 1;
    for (int i = 1; i <= m; i++) {
        rezultat *= i;
    }
    return rezultat;
}

void *dretva (void *arg) {
    int i = *((int*)arg);
    int blok = info_polje->br_el / info_polje->threadnum;
    int start = blok * i;
    int kraj = start + blok;
    if (kraj > info_polje->br_el) {
        kraj = info_polje->br_el;
    }
    
    int kursor = start;

    while (kursor <= kraj)
    {
       printf("Dretva broj %d \t %d \t %d \t %d\n", i, blok, start, kraj);
       for (int j=start; j<kraj; j++) {
        printf("Broj u polju %d; %Lf\n", j, info_polje->polje[j]);
       }
       kursor++;
       sleep(1);
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
        printf("%17.11Lf\n", info_polje->polje[i]);
    }

    int *polje_i = new int [info_polje->threadnum];

    printf("\nEksponencijale =\n");

    for (int i = 0; i < info_polje->threadnum; i++)
    {
        polje_i[i] = i;
        pthread_create(&polje_dretvi[i], NULL, dretva, &polje_i[i]);

    }
    prekid(0);
}
