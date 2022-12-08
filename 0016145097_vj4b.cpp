#include <iostream>
#include <pthread.h>
#include <csignal>
#include <unistd.h>

using namespace std;

pthread_t *polje_dretvi_nakupac;
int *polje_dr_nakupac;
int broj_nakupaca = 3;

pthread_t *dretva_veletrgovac;

pthread_mutex_t mutex1;
pthread_mutex_t mutex2;

pthread_cond_t uvjet[3];
pthread_cond_t uvjet_veletrgovac;

bool moze;
int komplet[2];
int broj;

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
        pthread_mutex_lock(&mutex1);
        while (moze)
        {
            pthread_cond_wait(&uvjet_veletrgovac, &mutex1);
        };
        stvori_ponudu();
        if ((komplet[0] == 0 && komplet[1] == 1) || (komplet[0] == 1 && komplet[1] == 0))
        {
            broj = 0;
            printf("Veletrgovac stavio monitor i tipkovnicu\n");
            sleep(1);
        }
        else if ((komplet[0] == 0 && komplet[1] == 2) || (komplet[0] == 2 && komplet[1] == 0))
        {
            broj = 1;
            printf("Veletrgovac stavio tipkovnicu i racunalo\n");
            sleep(1);
        }
        else if ((komplet[0] == 1 && komplet[1] == 2) || (komplet[0] == 2 && komplet[1] == 1))
        {
            broj = 2;
            printf("Veletrgovac stavio racunalo i monitor\n");
            sleep(1);
        }
        moze = true;
        pthread_cond_signal(&uvjet[broj]);
        pthread_mutex_unlock(&mutex1);
    }
    pthread_exit(z);
}

void *nakupac(void *z)
{
    int i = *(int *)z;
    while (true)
    {
        pthread_mutex_lock(&mutex2);
        while (!moze)
        {
            pthread_cond_wait(&uvjet[i], &mutex2);
        };
        switch (i)
        {
        case 0:
            printf("Nakupac s racunalima uzeo monitor i tipkovnicu\n");
            sleep(1);
            break;
        case 1:
            printf("Nakupac s monitorima uzeo tipkovnicu i racunalo\n");
            sleep(1);
            break;
        case 2:
            printf("Nakupac s tipkovnicama uzeo racunalo i monitor\n");
            sleep(1);
            break;
        }
        sleep(1);
        moze = false;
        pthread_cond_signal(&uvjet_veletrgovac);
        pthread_mutex_unlock(&mutex2);
    };
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
