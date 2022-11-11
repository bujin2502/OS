#include <iostream>
#include <cmath>
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

    double a = double(ulaz->br_elem);
    double b = double(ulaz->br_dret);

    double broj = ceil(a / b);

    int prec = ulaz->m;
    int start = broj * i;
    int kraj = start + broj;

    if (kraj > ulaz->br_elem)
    {
        kraj = ulaz->br_elem;
    }

    srand(time(0));

    for (int l = start; l < kraj; l++)
    {
        ulaz->polje[l] = (long double)rand() / (RAND_MAX - 1) * 10;
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

    int *polje_i = new int[ulaz->br_dret];

    for (int i = 0; i < ulaz->br_dret; i++)
    {
        polje_i[i] = i;
        pthread_create(&polje_dretvi[i], NULL, dretva, &polje_i[i]);
    }

usleep(1000);

    printf("\nEksponenti = \n");
    for (int i = 0; i < ulaz->br_elem; i++)
    {
        printf("%17.11Lf\n", ulaz->polje[i]);
    }

    printf("\nEksponencijale =\n");
    for (int k = 0; k < ulaz->br_elem; k++)
    {
        printf("%17.11Lf\n", ulaz->izlaz[k]);
    }

    prekid(0);
}
