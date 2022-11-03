#include <iostream>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <csignal>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

pthread_t *polje_dretvi;

struct slog_ulaz
{
    int br_elem;
    int m;
    int br_dret;
    long double *polje;
    long double *izlaz;
};
slog_ulaz *ulaz;

void prekid(int sig)
{
    for (int i = 0; i < ulaz->br_dret; i++)
        pthread_join(polje_dretvi[i], NULL);
    delete ulaz;
    exit(0);
}

long double faktorijel(int m)
{
    long double rezultat = 1;
    for (int i = m; i > 0; i--)
    {
        rezultat *= i;
    }
    return rezultat;
}

void *dretva(void *arg)
{
    int i = *((int *)arg);
    int broj = ulaz->br_elem / ulaz->br_dret;
    int mod = ulaz->br_elem % ulaz->br_dret;
    int prec = ulaz->m;
    int start = broj * i;
    int kraj = start + broj;
    if (start == ulaz->br_elem - broj - mod)
    {
        kraj = ulaz->br_elem;
    }

    for (int l = start; l < kraj; l++)
    {
        long double rez = 0;
        for (int z = 0; z < prec; z++)
        {
            rez += pow(ulaz->polje[l], z) / faktorijel(z);
        }
        ulaz->izlaz[l] = rez;
    }
    pthread_exit(arg);
}

int main(int argc, char **argv)
{
    system("clear");

    if (argc != 4)
    {
        cout << "Moraju se unijeti 3 argumenta\n";
        return 0;
    }

    ulaz = new slog_ulaz;
    ulaz->br_elem = atoi(argv[1]);
    ulaz->m = atoi(argv[2]);
    ulaz->br_dret = atoi(argv[3]);
    ulaz->polje = new long double[ulaz->br_elem];
    ulaz->izlaz = new long double[ulaz->br_elem];

    polje_dretvi = new pthread_t[ulaz->br_dret];

    sigset(SIGINT, prekid);

    srand(time(0));
    printf("Eksponenti = \n");
    for (int i = 0; i < ulaz->br_elem; i++)
    {
        ulaz->polje[i] = (long double)rand() / (RAND_MAX - 1) * 10;
        printf("%17.11Lf\n", ulaz->polje[i]);
    }

    int *polje_i = new int[ulaz->br_dret];

    for (int i = 0; i < ulaz->br_dret; i++)
    {
        polje_i[i] = i;
        pthread_create(&polje_dretvi[i], NULL, dretva, &polje_i[i]);
    }

    usleep(1000);

    printf("\nEksponencijale =\n");

    int brojac = ulaz->br_elem;
    for (int k = 0; k < brojac; k++)
    {
        printf("%17.11Lf\n", ulaz->izlaz[k]);
    }

    prekid(0);
}
