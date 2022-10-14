#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <unistd.h>

using namespace std;

void(*sigset(int sig, void (*disp)(int)))(int);

int N;
int n;

void prekid(int sig)
{
    if (SIGTSTP)
        cout << "Izlazak iz igre!" << endl;
    exit(0);
}

void odabir(int sig)
{
    switch (sig)
    {
    case SIGINT:
    {
        sighold(sig);
        cout << "Zigica igrac A uzeo sa stola:\n";
        do
        {
            cin >> n;
            if (n < 1 || n > 3 || n > N)
            {
                cout << "Broj mora biti izmedu 1 i 3 i ne moze biti veci od broja zigica na stolu!\n";
            }
        } while (n < 1 || n > 3 || n > N);
        N = N - n;
        sigrelse(sig);
        break;
    }
    case SIGQUIT:
    {
        sighold(sig);
        cout << "Zigica igrac B uzeo sa stola:\n";
        do
        {
            cin >> n;
            if (n < 1 || n > 3 || n > N)
            {
                cout << "Broj mora biti izmedu 1 i 3 i ne moze biti veci od broja zigica na stolu!\n";
            }
        } while (n < 1 || n > 3 || n > N);
        N = N - n;
        sigrelse(sig);
        break;
    }
    }
}

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        cout << "Ovaj program mora primiti jedan argument!\n";
        cout << "Pri unosu programa u komandnoj liniji na kraju dodajte jedan broj veci od broj 3.\n";
        return 0;
    }
    N = atoi(argv[1]);

    if (N < 4)
    {
        cout << "Unesen je broj manji od broja 4! Morate unjeti broj veci od broja 3.\n";
        return 0;
    }

    cout << "Parametri su ispravno uneseni.\n";
    cout << "Unesen je broj zigica i isti iznosi: " << N << ".\n";
    cout << "Tijekom igre mozete povlaciti jednu, dvije ili tri zigice.\n";

    sigset(SIGTSTP, prekid);

    bool igrac_a = true;

    do
    {
        if (igrac_a)
        {
            cout << "Zigica na stolu: " << N << ". Na redu igrac A. Unesi Ctrl-C.\n";
            sigset(SIGINT, odabir);
            pause();
        }
        else
        {
            cout << "Zigica na stolu: " << N << ". Na redu igrac B. Unesi Ctrl-\\\n";
            sigset(SIGQUIT, odabir);
            pause();
        }
        igrac_a = !igrac_a;
    } while (N > 0);

    if (N < 1)
    {
        cout << "Ostalo je " << N << " zigica na stolu.\n";
        if (igrac_a)
        {
            cout << "Pobjednik je igrac A! Cestitam!\n";
        }
        else
        {
            cout << "Pobjednik je igrac B! Cestitam!\n";
        }
    }
    return 0;
}
