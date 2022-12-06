#include <iostream>
#include <pthread.h>
#include <csignal>
#include <unistd.h>

using namespace std;

pthread_t *polje_dretvi_nakupac;
pthread_t *dretva_veletrgovac;
int *polje_dr_nakupac;
int broj_sudionika = 3;

int slucajni()
{
    int r = rand() % 3;
    return r;
}

void *veletrgovac(void *z)
{
    // VELETRGOVAC
    while (true)
    {
        int slucajni1 = slucajni();
        switch (slucajni1)
        {
        case 0:
            printf("Veletrgovac stavio monitor i racunalo\n");
            sleep(1);
            break;
        case 1:
            printf("Veletrgovac stavio tipkovnicu i racunalo\n");
            sleep(1);
            break;
        case 2:
            printf("Veletrgovac stavio monitor i tipkovnicu\n");
            sleep(1);
            break;
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
    delete[] polje_dretvi_nakupac;
    delete[] dretva_veletrgovac;
    delete[] polje_dr_nakupac;
    exit(0);
}

int main()
{
    system("clear");
    srand(time(NULL));

    polje_dretvi_nakupac = new pthread_t[broj_sudionika];
    dretva_veletrgovac = new pthread_t[1];
    polje_dr_nakupac = new int[broj_sudionika];

    for (int i = 0; i < broj_sudionika; i++)
    {
        polje_dr_nakupac[i] = i;
        pthread_create(&polje_dretvi_nakupac[i], NULL, nakupac, &polje_dr_nakupac[i]);
    }
    pthread_create(&dretva_veletrgovac[0], NULL, veletrgovac, NULL);

    for (int i = 0; i < broj_sudionika; i++)
    {
        pthread_join(polje_dretvi_nakupac[i], NULL);
    }
    pthread_join(dretva_veletrgovac[0], NULL);

    sigset(SIGINT, prekid);

    prekid(0);
}
