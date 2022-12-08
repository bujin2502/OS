#include <iostream>
#include <pthread.h>
#include <csignal>

using namespace std;

pthread_mutex_t mutex;
pthread_t *polje_dretvi;
pthread_cond_t uvjet;
int *polje_i;
int broj_dretvi;
int broj_unesenih = 0;

void *dretva(void *z)
{
    int i = *(int *)z;
    pthread_mutex_lock(&mutex);
    int broj;
    printf("Dretva broj: %d. Unesi broj: ", i + 1);
    cin >> broj;
    broj_unesenih++;
    if (broj_unesenih < broj_dretvi)
    {
        pthread_cond_wait(&uvjet, &mutex);
    }
    else
    {
        pthread_cond_broadcast(&uvjet);
    }
    printf("Unesen broj: %d u dretvu: %d\n", broj, i + 1);
    pthread_mutex_unlock(&mutex);
    pthread_exit(z);
}

void prekid(int sig)
{
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&uvjet);
    delete[] polje_dretvi;
    delete[] polje_i;
    exit(0);
}

int main(int argc, char **argv)
{
    system("clear");

    if (argc != 2)
    {
        cout << "Morate unijeti 1 argument (broj dretvi)\n";
        return 0;
    }

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&uvjet, NULL);
    broj_dretvi = atoi(argv[1]);
    polje_dretvi = new pthread_t[broj_dretvi];
    polje_i = new int[broj_dretvi];

    for (int i = 0; i < broj_dretvi; i++)
    {
        polje_i[i] = i;
        pthread_create(&polje_dretvi[i], NULL, dretva, &polje_i[i]);
    }

    for (int i = 0; i < broj_dretvi; i++)
    {
        pthread_join(polje_dretvi[i], NULL);
    }

    sigset(SIGINT, prekid);

    prekid(0);
}
