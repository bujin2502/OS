#include <iostream>
#include <pthread.h>
#include <csignal>
#include <unistd.h>

using namespace std;

pthread_t *polje_dretvi_nakupac;
pthread_t *dretva_veletrgovac;

pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
pthread_cond_t uvjet[3];
pthread_cond_t uvjet_veletrgovac;
int komplet[2];
int *polje_dr_nakupac;
int broj_nakupaca = 3;
bool kompletirano;

void stvori_ponudu()
{
    do
    {
        komplet[0] = rand() % 3;
        komplet[1] = rand() % 3;
    } while (komplet[0] == komplet[1]);
}

void *veletrgovac(void *z)
{
    while (true)
    {
        stvori_ponudu();
        if ((komplet[0] == 0 && komplet[1] == 1) || (komplet[0] == 1 && komplet[1] == 0))
        {
            printf("tipkovnica i monitor\n");
            sleep(1);
        }
        else if ((komplet[0] == 0 && komplet[1] == 2) || (komplet[0] == 2 && komplet[1] == 0))
        {
            printf("racunalo i tipkovnica\n");
            sleep(1);
        }
        else if ((komplet[0] == 1 && komplet[1] == 2) || (komplet[0] == 2 && komplet[1] == 1))
        {
            printf("monitor i racunalo\n");
            sleep(1);
        }
    }
    pthread_exit(z);
}

void *nakupac(void *z)
{
    int i = *(int *)z;
    while (true)
    {
        switch (i)
        {
        case 0: // NAKUPAC - TIPKOVNICA
            printf("Nakupac s tipkovnicama uzeo monitor i racunalo\n");
            sleep(1);
            break;
        case 1: // NAKUPAC - MONITOR
            printf("Nakupac s monitorima uzeo tipkovnicu i racunalo\n");
            sleep(1);
            break;
        case 2: // NAKUPAC - RACUNALO
            printf("Nakupac s racunalima uzeo monitor i tipkovnicu\n");
            sleep(1);
            break;
        }
    }
    pthread_exit(z);
}

void prekid(int sig)
{
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);
    pthread_cond_destroy(&uvjet_veletrgovac);
    for (int x = 0; x < 3; x++)
    {
        pthread_cond_destroy(&uvjet[x]);
    }
    delete[] polje_dretvi_nakupac;
    delete[] dretva_veletrgovac;
    delete[] polje_dr_nakupac;
    exit(0);
}

int main()
{
    system("clear");
    srand(time(NULL));

    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);
    for (int x = 0; x < 3; x++)
    {
        pthread_cond_init(&uvjet[x], NULL);
    }
    pthread_cond_init(&uvjet_veletrgovac, NULL);

    polje_dretvi_nakupac = new pthread_t[broj_nakupaca];
    dretva_veletrgovac = new pthread_t[1];
    polje_dr_nakupac = new int[broj_nakupaca];

    for (int i = 0; i < broj_nakupaca; i++)
    {
        polje_dr_nakupac[i] = i;
        pthread_create(&polje_dretvi_nakupac[i], NULL, nakupac, &polje_dr_nakupac[i]);
    }
    pthread_create(&dretva_veletrgovac[0], NULL, veletrgovac, NULL);

    for (int i = 0; i < broj_nakupaca; i++)
    {
        pthread_join(polje_dretvi_nakupac[i], NULL);
    }
    pthread_join(dretva_veletrgovac[0], NULL);

    sigset(SIGINT, prekid);

    prekid(0);
}
