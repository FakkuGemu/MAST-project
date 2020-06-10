#include <iostream>
#include <string>

using namespace std;

struct Lisc {
    int wartosc = 0;
    int pozycja = 0;
    int ilosc_dzieci = 0;
    Lisc* rodzic = NULL;
    Lisc* brat = NULL;
    Lisc* syn = NULL;
};

struct Drzewo {
    Lisc* head = NULL;
    int ilosc_lisci = 0;
    int ilosc_wezlow = 0;
    int* tablica;
    int** tablica_hash;
    Lisc** tablica_wskaznikow;
};



Drzewo wczytaj_drzewo() {
    int pomocnicza = 0;
    int pozycja = 0;
    int ilosc_wezlow = 0;
    int ilosc_lisci = 0;

    string linia;

    Drzewo* nowe_drzewo = new Drzewo;
    Lisc* pierwszy_lisc = new Lisc;

    nowe_drzewo->head = pierwszy_lisc;

    Lisc* kopia_liscia = pierwszy_lisc;

    cin >> linia;
    for (int i = 0; i < linia.length() - 1; i++) {
        if (linia[i] > '0' && linia[i] <= '9') {
            if (linia[i] == '1' && linia[i + 1] == '0') {
                i++;
            }
            ilosc_lisci++;
        }

    }
    int* tablica_lisci = new int[ilosc_lisci];


    for (int i = 0; i < linia.length() - 1; i++) {
        if (linia[i] == '(') {
            Lisc* syn = new Lisc;
            kopia_liscia->ilosc_dzieci++;
            kopia_liscia->syn = syn;
            syn->rodzic = kopia_liscia;
            kopia_liscia->wartosc = pomocnicza--;
            ilosc_wezlow++;
            kopia_liscia = syn;
        }
        else if (linia[i] == ',') {
            Lisc* brat = new Lisc;
            kopia_liscia->rodzic->ilosc_dzieci++;
            kopia_liscia->brat = brat;
            brat->rodzic = kopia_liscia->rodzic;
            kopia_liscia = brat;
        }
        else if (linia[i] == ')') {
            kopia_liscia = kopia_liscia->rodzic;
        }
        else {
            kopia_liscia->wartosc = linia[i] - '0';

            if (linia[i] == '1' && linia[i + 1] == '0') {
                kopia_liscia->wartosc = 10;
                i++;
            }
            kopia_liscia->pozycja = pozycja;
            tablica_lisci[pozycja] = kopia_liscia->wartosc;
            pozycja++;

        }
    }




    Lisc** tablica_wskaznikow = new Lisc * [ilosc_wezlow];

    nowe_drzewo->ilosc_lisci = ilosc_lisci;
    nowe_drzewo->tablica = tablica_lisci;
    nowe_drzewo->tablica_wskaznikow = tablica_wskaznikow;
    nowe_drzewo->ilosc_wezlow = ilosc_wezlow;
    return*nowe_drzewo;
}


int szukaj_indeksu(Drzewo* drzewo, int wartosc) {
    for (int i = 0; i < drzewo->ilosc_lisci; i++) {
        if (drzewo->tablica[i] == wartosc)
            return i;
    }
    return 0;
}

void wartosci_dzieci(Drzewo* drzewo1, Drzewo* drzewo2, Lisc* lisc, int** tablica) {


    if (lisc == NULL)
        return;


    if (lisc->wartosc > 0) {
        tablica[szukaj_indeksu(drzewo2, lisc->wartosc)][lisc->rodzic->wartosc * (-1)] = 1;
    }
    else if (lisc->syn != NULL) {
        wartosci_dzieci(drzewo1, drzewo2, lisc->syn, tablica);
    }
    if (lisc->brat != NULL) {
        wartosci_dzieci(drzewo1, drzewo2, lisc->brat, tablica);
    }
    if (lisc->wartosc <= 0) {
        Lisc* kopia = lisc->syn;

        Lisc* wskaznik_na_wierzcholek = lisc;


        drzewo1->tablica_wskaznikow[lisc->wartosc * (-1)] = wskaznik_na_wierzcholek;

        while (kopia != NULL) {
            if (kopia->wartosc <= 0) {
                for (int i = 0; i < drzewo1->ilosc_lisci; i++) {
                    tablica[i][lisc->wartosc * (-1)] += tablica[i][kopia->wartosc * (-1)];
                }
            }
            kopia = kopia->brat;
        }
    }


}


void stworz_tabele(Drzewo* drzewo1, Drzewo* drzewo2, int** tablica) {

    wartosci_dzieci(drzewo1, drzewo2, drzewo1->head, tablica);

    drzewo1->tablica_hash = tablica;
}



int stworzenie_malej_tablicy(Drzewo* drzewo1, Drzewo* drzewo2, int i, int j, int** tablica_porownawcza, int** mala_tablica) {
    int wynik = 0;
    if (mala_tablica[i][0] > 0 && mala_tablica[0][j] > 0) {
        if (mala_tablica[i][0] == mala_tablica[0][j])
            return 1;
    }
    else if (mala_tablica[i][0] > 0 && mala_tablica[0][j] <= 0) {
        if (drzewo2->tablica_hash[szukaj_indeksu(drzewo1, mala_tablica[i][0])][mala_tablica[0][j] * (-1)] == 1)
            return 1;
    }
    else if (mala_tablica[i][0] <= 0 && mala_tablica[0][j] > 0) {
        if (drzewo1->tablica_hash[szukaj_indeksu(drzewo2, mala_tablica[0][j])][mala_tablica[i][0] * (-1)] == 1)
            return 1;
    }
    else if (mala_tablica[i][0] <= 0 && mala_tablica[0][j] <= 0) {
        return tablica_porownawcza[mala_tablica[i][0] * (-1)][mala_tablica[0][j] * (-1)];
    }

    return wynik;
}

int suma_permutacji(int* tablica, int rozmiar)
{
    int suma = 0;
    for (int i = 0; i < rozmiar; i++)
        suma += tablica[i];
    return suma;
}

bool czy_sie_powtarza(int* tablica, int rozmiar)
{
    for (int i = 0; i < rozmiar - 1; i++)
    {
        for (int j = i + 1; j < rozmiar; j++)
        {
            if (tablica[i] == tablica[j]) return true;
        }
    }
    return false;
}


void najwieksza_permutacja(int** mala_tablica, int** tablica_permutacji, int rozmiar1, int rozmiar2, int rozmiar, int tymczasowy_rozmiar, int& najwieksza) {
    if (rozmiar2 > rozmiar1)
    {
        for (int i = 1; i < rozmiar2; i++)
        {


            tablica_permutacji[0][rozmiar - tymczasowy_rozmiar] = mala_tablica[rozmiar - tymczasowy_rozmiar + 1][i];
            tablica_permutacji[1][rozmiar - tymczasowy_rozmiar] = i;
            if (!czy_sie_powtarza(tablica_permutacji[1], rozmiar))
            {
                int tymczasowa_najwieksza = suma_permutacji(tablica_permutacji[0], rozmiar);
                if (tymczasowa_najwieksza > najwieksza) najwieksza = tymczasowa_najwieksza;
            }
            if (tymczasowy_rozmiar > 1) najwieksza_permutacja(mala_tablica, tablica_permutacji, rozmiar1, rozmiar2, rozmiar, tymczasowy_rozmiar - 1, najwieksza);
        }
    }
    else
    {
        for (int i = 1; i < rozmiar1; i++)
        {
            tablica_permutacji[0][rozmiar - tymczasowy_rozmiar] = mala_tablica[i][rozmiar - tymczasowy_rozmiar + 1];
            tablica_permutacji[1][rozmiar - tymczasowy_rozmiar] = i;
            if (!czy_sie_powtarza(tablica_permutacji[1], rozmiar))
            {
                int tymczasowa_najwieksza = suma_permutacji(tablica_permutacji[0], rozmiar);
                if (tymczasowa_najwieksza > najwieksza) najwieksza = tymczasowa_najwieksza;
            }
            if (tymczasowy_rozmiar > 1) najwieksza_permutacja(mala_tablica, tablica_permutacji, rozmiar1, rozmiar2, rozmiar, tymczasowy_rozmiar - 1, najwieksza);
        }
    }
}


int wybierz_najwiekszy(int** mala_tablica, int rozmiar1, int rozmiar2) {
    int maksymalna = 0;
    int rozmiar;
    rozmiar1 > rozmiar2 ? rozmiar = rozmiar2 - 1 : rozmiar = rozmiar1 - 1;
    int** tablica_permutacji = new int* [2];
    for (int i = 0; i < 2; i++)
        tablica_permutacji[i] = new int[rozmiar];
    if (rozmiar1 >= rozmiar2)
    {
        for (int i = 0; i < rozmiar; i++)
        {
            tablica_permutacji[1][i] = 1;
            tablica_permutacji[0][i] = mala_tablica[1][i + 1];
        }
    }
    else
    {
        for (int i = 0; i < rozmiar; i++)
        {
            tablica_permutacji[1][i] = 1;
            tablica_permutacji[0][i] = mala_tablica[i + 1][1];
        }
    }
    najwieksza_permutacja(mala_tablica, tablica_permutacji, rozmiar1, rozmiar2, rozmiar, rozmiar, maksymalna);
    for (int i = 0; i < 2; i++)
        delete[] tablica_permutacji[i];
    delete[] tablica_permutacji;
    return maksymalna;
}


int sumowanie(Drzewo* drzewo1, Drzewo* drzewo2, Lisc* wierzcholek1, Lisc* wierzcholek2, int** tablica_porownawcza, int** mala_tablica) {
    int wynik_ostateczny = 0, tymczasowa = 0;
    Lisc* kopia_w1 = wierzcholek1;
    Lisc* kopia_w2 = wierzcholek2;
    kopia_w2 = kopia_w2->syn;


    for (int j = 1; j < wierzcholek2->ilosc_dzieci + 1; j++) {
        mala_tablica[0][j] = kopia_w2->wartosc;
        if (kopia_w2->brat != NULL) {
            kopia_w2 = kopia_w2->brat;
        }
    }

    kopia_w1 = kopia_w1->syn;

    for (int j = 1; j < wierzcholek1->ilosc_dzieci + 1; j++) {
        mala_tablica[j][0] = kopia_w1->wartosc;
        if (kopia_w1->brat != NULL) {
            kopia_w1 = kopia_w1->brat;
        }
    }



    for (int i = 1; i < wierzcholek1->ilosc_dzieci + 1; i++) {
        for (int j = 1; j < wierzcholek2->ilosc_dzieci + 1; j++) {
            mala_tablica[i][j] = stworzenie_malej_tablicy(drzewo1, drzewo2, i, j, tablica_porownawcza, mala_tablica);
        }
    }


    wynik_ostateczny = wybierz_najwiekszy(mala_tablica, wierzcholek1->ilosc_dzieci + 1, wierzcholek2->ilosc_dzieci + 1);

    for (int i = 1; i < wierzcholek2->ilosc_dzieci + 1; i++) {

        if (mala_tablica[0][i] <= 0) {
            tymczasowa = tablica_porownawcza[wierzcholek1->wartosc * (-1)][mala_tablica[0][i] * (-1)];
        }

        if (tymczasowa > wynik_ostateczny) {
            wynik_ostateczny = tymczasowa;
        }
    }


    for (int i = 1; i < wierzcholek1->ilosc_dzieci + 1; i++) {

        if (mala_tablica[i][0] <= 0) {
            tymczasowa = tablica_porownawcza[mala_tablica[i][0] * (-1)][wierzcholek2->wartosc * (-1)];
        }

        if (tymczasowa > wynik_ostateczny) {
            wynik_ostateczny = tymczasowa;
        }
    }


    return wynik_ostateczny;
}

void porownanie_drzew(Drzewo* drzewo1, Drzewo* drzewo2, int** tablica_porownawcza) {
    for (int i = drzewo1->ilosc_wezlow - 1; i >= 0; i--)
        for (int j = drzewo2->ilosc_wezlow - 1; j >= 0; j--) {

            int** mala_tablica = new int* [drzewo1->tablica_wskaznikow[i]->ilosc_dzieci + 1];
            for (int k = 0; k < drzewo1->tablica_wskaznikow[i]->ilosc_dzieci + 1; k++)
                mala_tablica[k] = new int[drzewo2->tablica_wskaznikow[j]->ilosc_dzieci + 1];
            for (int k = 0; k < drzewo1->tablica_wskaznikow[i]->ilosc_dzieci + 1; k++)
                for (int l = 0; l < drzewo2->tablica_wskaznikow[j]->ilosc_dzieci + 1; l++)
                    mala_tablica[k][l] = 0;

            tablica_porownawcza[i][j] = sumowanie(drzewo1, drzewo2, drzewo1->tablica_wskaznikow[i], drzewo2->tablica_wskaznikow[j], tablica_porownawcza, mala_tablica);



            for (int k = 0; k < drzewo1->tablica_wskaznikow[i]->ilosc_dzieci + 1; k++)
                delete mala_tablica[k];
            delete[] mala_tablica;
        }
}

int main()
{
    int ilosc_drzew, i, j;
    cin >> ilosc_drzew;

    Drzewo* tablica_drzew = new Drzewo[ilosc_drzew];

    for (i = 0; i < ilosc_drzew; i++) {
        tablica_drzew[i] = wczytaj_drzewo();
    }


    for (i = 0; i < ilosc_drzew; i++) {
        for (j = i + 1; j < ilosc_drzew; j++) {

            int** tablica1 = new int* [tablica_drzew[i].ilosc_lisci];
            for (int k = 0; k < tablica_drzew[i].ilosc_lisci; k++)
                tablica1[k] = new int[tablica_drzew[i].ilosc_wezlow];
            for (int k = 0; k < tablica_drzew[i].ilosc_lisci; k++)
                for (int l = 0; l < tablica_drzew[i].ilosc_wezlow; l++)
                    tablica1[k][l] = 0;

            int** tablica2 = new int* [tablica_drzew[j].ilosc_lisci];
            for (int k = 0; k < tablica_drzew[j].ilosc_lisci; k++)
                tablica2[k] = new int[tablica_drzew[j].ilosc_wezlow];
            for (int k = 0; k < tablica_drzew[j].ilosc_lisci; k++)
                for (int l = 0; l < tablica_drzew[j].ilosc_wezlow; l++)
                    tablica2[k][l] = 0;

            int** tablica_porownawcza = new int* [tablica_drzew[i].ilosc_wezlow];

            for (int k = 0; k < tablica_drzew[i].ilosc_wezlow; k++)
                tablica_porownawcza[k] = new int[tablica_drzew[j].ilosc_wezlow];
            for (int k = 0; k < tablica_drzew[i].ilosc_wezlow; k++)
                for (int l = 0; l < tablica_drzew[j].ilosc_wezlow; l++)
                    tablica_porownawcza[k][l] = 0;



            stworz_tabele(&tablica_drzew[i], &tablica_drzew[j], tablica1);

            stworz_tabele(&tablica_drzew[j], &tablica_drzew[i], tablica2);


            porownanie_drzew(&tablica_drzew[i], &tablica_drzew[j], tablica_porownawcza);


            printf("%i \n", tablica_drzew[i].ilosc_lisci - tablica_porownawcza[0][0]);


            delete[] tablica1;
            delete[] tablica2;
            delete[] tablica_porownawcza;
        }

    }

    delete[] tablica_drzew;

    return 0;
}