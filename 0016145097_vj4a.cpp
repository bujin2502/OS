#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <csignal>

using namespace std;

sem_t semafor;
pthread_t *polje_dretvi;
int *polje_i;
int broj_dretvi;

void *dretva(void *z)
{
    sem_wait(&semafor);
    int broj;
    printf("Unesi broj: \n");
    cin >> broj;
    printf("Uneseni broj je: %d\n", broj);
    sem_post(&semafor);
    pthread_exit(z);
}

void prekid(int sig)
{
    sem_destroy(&semafor);
    for (int i = 0; i < broj_dretvi; i++)
        pthread_join(polje_dretvi[i], NULL);
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

    sem_init(&semafor, 0, 1);
    broj_dretvi = atoi(argv[1]);
    polje_dretvi = new pthread_t[broj_dretvi];
    polje_i = new int[broj_dretvi];

    sigset(SIGINT, prekid);

    for (int i = 0; i < broj_dretvi; i++)
    {
        polje_i[i] = i;
        pthread_create(&polje_dretvi[i], NULL, dretva, &polje_i[i]);
    }

    for (int i = 0; i < broj_dretvi; i++)
    {
        pthread_join(polje_dretvi[i], NULL);
    }

    sem_destroy(&semafor);
    delete[] polje_dretvi;
    delete[] polje_i;

    return 0;
}
