#include <iostream>
#include <iomanip>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

struct slog_info_polje {
    long double *polje;
    int br_el;
    int blok;
} *info_polje;

int main (int argc, char **argv) {
    system("clear");

    if (argc != 3)
    {
        cout << "1. argument = velicina polja\n2. argument = velicina bloka za obradu\n";
        return 0;
    }

    info_polje = new slog_info_polje;
    info_polje->br_el = atoi(argv[1]);
    info_polje->blok = atoi(argv[2]);
    info_polje->polje = new long double [info_polje->br_el];

    srand(time(0));
    for (int i = 0; i < info_polje->br_el; i++)
    {
        info_polje->polje[i] = (long double) rand()/(RAND_MAX - 1) * 10;
        cout << i << ". " << setprecision(12) << info_polje->polje[i] << endl;
    }

    int broj_procesa = info_polje->br_el / info_polje->blok;
    if (info_polje->br_el % info_polje->blok) broj_procesa++;
/*
    for (int i = 0; i < broj_procesa; i++)
    {
        switch (fork())
        {
        case 0:
        {

        }
        case -1:
        {

        }
        }
    }
*/
    delete info_polje;
    return 0;
}