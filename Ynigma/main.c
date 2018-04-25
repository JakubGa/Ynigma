#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <locale.h>
#include "obrazki.h"
#include "struktura.h"


void menu(int poziom)
//funkcja wy�wietla komunikaty w menu g��wnym
{
	switch (poziom)
	{
	case 0:
		printf("Co chcesz zrobi�?\n1. Zaszyfruj wiadomo��\n2. Odszyfruj wiadomo��\n");
		break;
	case 1:
		printf("1. Szyfrowanie tekstowe\n2. Szyfrowanie obrazem\n");
		break;
	}
}
int wez_cyfre()
//zwraca cyfr� potrzebn� do sterowania menu. Je�eli nie jest to 1 lub 2, wy�wietla komunikat o b��dzie, a menu wraca na pocz�tek
{
	int i, j;
	char* strg;
	j = scanf("%d", &i);
	strg = (char*)malloc(15 * sizeof(char));
	if (j == 0)
	{
		gets(strg);
		printf("Nie ma takiej mo�liwo�ci\n");
		i = 0;
	}
	else if (!((i == 1) || (i == 2)))
	{
		printf("Nie ma takiej mo�liwo�ci\n");
	}
	free(strg);
	return i;
}
char* pobierz_plik(char* tablica)
//pobiera plik .txt o nazwie podanej przez u�ytkownika i zwraca wska�nik na �a�cuch z za�adowanym tekstem
{
	char* nazwa;
	FILE* fp;
	int i = 0;
	char* c;
	c = malloc(sizeof(char));
	nazwa = (char*)malloc(21 * sizeof(char));
	printf("Podaj nazw� szyfrowanego pliku: ");
	scanf_s("%s", nazwa, 21); //teoretycznie przy scanf_s nie powinno doj�� do przepe�nienia bufora, ale... plik o zbyt d�ugiej nazwie si� nie otworzy
	fp = fopen(nazwa, "r");
	free(nazwa);
	if (fp == NULL)
	{
		perror("B��d otwarcia pliku!\n");
		free(tablica);	//je�eli nie otworzy si� plik, to najpierw zwalnia pami�� zaalokowan� pod tablic�, dopiero potem zwraca NULL
		return NULL;
	}
	else
	{
		while (fscanf(fp, "%c", c) != EOF)	//odczytuje kolejny znak
		{
			tablica[i] = *c;	//je�eli nie natrafiono na koniec pliku, to warto�� odczytanego znaku zapisywana w tablicy
			i++;
			if (i % 10 == 0)
			{
				tablica = (char*)realloc(tablica, sizeof(char)*(i + 10));	//je�eli zape�nienie tablicy dojdzie do ko�ca, zwi�ksza rozmiar o kolejne 10
			}
		}
		fclose(fp);
		*c = '\0';	//ko�czy �a�cuch znak�w
		tablica[i] = *c;
	}
	free(c);
	return tablica;
}
int losuj()
{
	int i;
	i = rand() % 90;	//losuje 0-89
	i += 10;
	return i;	//zwraca 10-99
}
char* cezar(char* tablica, int przesuniecie)
//funkcja otrzymuje �a�cuch odczytany z pliku i wylosowan� liczb�, stosuje szyfr cezara
{
	int i, j, oper;
	j = strlen(tablica);
	for (i = 0; i < j; i++)
	{
		if (((int)tablica[i] + przesuniecie) < 255)
		{
			oper = (int)tablica[i] + przesuniecie;
			tablica[i] = (char)oper;
		}
		else
		{
			oper = ((int)tablica[i] + przesuniecie - 255);	//je�li wynik przekroczy�by warto�� 255 "zawija" go
			tablica[i] = (char)oper;
		}
	}
	return tablica;
}
void zapis(char* tablica)
//zapisuje wcze�niej zaszyfrowany �a�cuch znak�w
{
	FILE* fp;
	fp = fopen("zaszyfrowany.txt", "w");
	if (fp == NULL)
	{
		perror("B��d zapisu!/n");
	}
	else
	{
		fprintf(fp, "%s", tablica);
		printf("Zapis do pliku uko�czony\n");
		fclose(fp);
	}
}
void zaszyfruj_tekst()
//funkcja wywo�ywana z menu g��wnego w celu zaszyfrowania pliku
{
	char* tekst;
	int przesuniecie;
	tekst = (char*)malloc(10 * sizeof(char));	//pocz�tkowy bufor to 10 znak�w
	tekst = pobierz_plik(tekst);	//odczytuje tekst z pliku
	if (tekst != NULL)	//a je�eli si� to uda, wykonuje operacj w celu zaszfrowania go i zapisania
	{
		przesuniecie = losuj();
		tekst = cezar(tekst, przesuniecie);
		zapis(tekst);
		przesuniecie = przesuniecie * 100 + losuj();
		printf("Kod do odczytu twojego pliku to: %d\n", przesuniecie);	//przekazuje u�ytkownikowi kod do odszyfrowania
		free(tekst);
	}
}
char* pobierz_plik2(char* tablica)
//do tablicy znak�w wczytuje zaszyfrowany tekst z pliku
//nie r�ni si� zbyt od pobierz_plik, z wyj�tkiem tego, �e nazwa jest na sztywno ustalona
//ale jako �e w C nie mog� przeci��y� funkcji, to musz� dublowa� nazwy
{
	FILE* fp;
	int i = 0;
	char* c;
	c = (char*)malloc(sizeof(char));
	fp = fopen("zaszyfrowany.txt", "r");
	if (fp == NULL)
	{
		perror("B��d otwarcia pliku!\n");
		free(tablica);
		return NULL;
	}
	else
	{
		while (fscanf(fp, "%c", c) != EOF)
		{
			tablica[i] = *c;
			i++;
			if (i % 10 == 0)
			{
				tablica = (char*)realloc(tablica, sizeof(char)*(i + 10));
			}
		}
		fclose(fp);
		*c = '\0';
		tablica[i] = *c;
	}
	free(c);
	return tablica;
}
int zaladuj_kod()
//pobiera kod od u�ytkownika
{
	int haslo;
	printf("Podaj kod do odczytu pliku: ");
	scanf("%d", &haslo); //je�eli nie poda si� liczby, program przestanie dzia�a�, ale... nie moja wina �e kto� nie zna has�a
	haslo /= 100;
	return haslo;
}
char* decesar(char* tablica, int przesuniecie)
//funkcja odwrotna do szyfrowania, spos�b dzia�ania ten sam
{
	int i, j, oper;
	j = strlen(tablica);
	for (i = 0; i < j; i++)
	{
		if (((int)tablica[i] - przesuniecie)>0)
		{
			oper = (int)tablica[i] - przesuniecie;
			tablica[i] = (char)oper;
		}
		else
		{
			oper = (int)tablica[i] - przesuniecie + 256;
			tablica[i] = (char)oper;
		}
	}
	return tablica;
}
void zapis2(char* tablica)
//zaspisuje odszsyfrowany �a�cuch, ale nowa nazwa - nowa funkcja
{
	FILE* fp;
	fp = fopen("odszyfrowany.txt", "w");
	if (fp == NULL)
	{
		perror("B��d zapisu!/n");
	}
	else
	{
		fprintf(fp, "%s", tablica);
		printf("Zapis do pliku ukonczony\n");
		fclose(fp);
	}
}
void odszyfruj_tekst()
//funkcja wywo�ywana w menu w celu odszyfrowania tekstu
{
	char *tekst;
	int przesuniecie;
	tekst = (char*)malloc(10 * sizeof(char));
	tekst = pobierz_plik2(tekst);
	if (tekst != NULL)	//je�eli uda si� za�adowa� tekst z pliku, to po kolei go oszyfrowujemy, wy�wietlamy i zapisujemy
	{
		przesuniecie = zaladuj_kod();
		tekst = decesar(tekst, przesuniecie);
		printf("%s\n", tekst);
		zapis2(tekst);
		free(tekst);
	}
}
int* na_bitowy(int* tablica, int wartosc)
//funkcja rozbija podan� warto�� na zapis binarny
{
	int i;
	if (wartosc < 0)	//bo 0-255 to zbyt prosto, lepiej traktowa� 8 bit jako minus... (potrzebne do odczytu polskich znak�w)
	{
		tablica[7] = 1;
		wartosc *= -1;
	}
	else {
		tablica[7] = 0;
	}
	for (i = 6; i >= 0; i--)
	{
		if (wartosc >= (int)pow(2, i))
		{
			tablica[i] = 1;
			wartosc -= (int)pow(2, i);
		}
		else
		{
			tablica[i] = 0;
		}
	}
	return tablica;	//i zwracamy z trudem skompletowany zestaw 8 zer lub jedynek
}
void modyfikuj(struct obraz* obrazek, char* tablica)
//funkcja otrzymuje struktur� z obrazem i �a�cuch z tekstem z pliku
//modyfikuje obraz tak, �eby zawiera� w sobie tekst
{
	int* bity;
	int i, j, k=0,l=0,m,a;	//tutaj zrobi�em chaos, ale: i-d�.�a�cucha j-wskazuje.kolejny.el.�a�cucha l-wysoko��.obrazka k-szer.obrazka m-kojelne.cyfry.w.bicie a-zmienna.pomocnicza,�eby.sprawdzi�.jaki.jest.ostatni.bit.w.pikselu
	i = strlen(tablica)+1;
	bity = (int*)malloc(8*sizeof(int));
	while (i > ((obrazek->szer*obrazek->wys)/8))	//je�eli obrazek jest za ma�y, powi�ksza go tak d�ugo jak trzeba
	{
		skaluj(obrazek);	//jednokrotne powi�kszenie pozwala zapisa� (prawie) 4 razy wi�cej tekstu, ale wp�yw na jako�� grafiki jest przera�aj�cy
	}
	for (j = 0; j < i; j++)	//powtarzamy przez ca�y �a�cuch
	{
		bity = na_bitowy(bity, (int)tablica[j]);	//kolejne znaki z �a�cucha s� rozbijane na zapis bitowy
		for (m = 0; m < 8; m++)	//i dla ka�dego kolejnego bitu
		{
			if (k == obrazek->szer)	//je�eli doszli�my do ko�ca wierssza, idziemy do nast�pnego (takie dzy�-szur w maszynie do pisania)
			{
				k = 0;
				l++;
			}
			a = obrazek->piksele[l][k];	//zmiennej pomocniczej przypisujemy warto�� obecnie rozwa�anego piksela
			if (a % 2 == 1)	//zerujemy najmniej znacz�cy bit
			{
				a -= 1;
			}
			a += bity[m];	//ustawiamy na najmniej znacz�cym bicie piksela warto�� odczytan� ze znaku �a�cucha
			obrazek->piksele[l][k] = a;
			k++;
		}
	}
	free(bity);
}
void zaszyfruj_obraz()
//wywo�ywane z menu g�., zapisuje tekst w obrazie
{
	struct obraz* pobrany;
	char* tekst;
	tekst = (char*)malloc(10 * sizeof(char));
	tekst = pobierz_plik(tekst);	//odczytuje tekst do zaszyfrowania
	pobrany = (struct obraz*)malloc(sizeof(struct obraz));
	pobrany = kreator(pobrany);	//z obrazki.c, otwiera obraz angel.pgm
	if (pobrany != NULL && tekst != NULL)
	{
		modyfikuj(pobrany, tekst);	//je�eli otwar�o oba elementy modyfikuje obrazek
		zapis_obrazu(*pobrany);	//i zapisuje go jako zaszyfrowany.pgm
	}
	if (pobrany != NULL)	//zwalnia co otworzy�
	{
		uwolnij(pobrany);
	}
	if (tekst != NULL)
	{
		free(tekst);
	}
}
int zdebituj(int* tablica)
//na podstawie ci�gu zer i jedynek odtwarza liczb�
{
	int war = 0, i;
	for (i = 6; i >= 0; i--)
	{
		war += (int)pow(2, i)*tablica[i];
	}
	if (tablica[7] == 1)	//tak, polskie znaki...
	{
		war *= -1;
	}
	return war;
}
char* odczytaj(struct obraz obrazek, char* tablica)
//odczytuje najmniej znacz�ce bity z kolejnych pikseli obrazka, zbiera po 8 i odczytuje znak
{
	char* c;
	int i = 0, k = 0, l, a,b=0;
	int* bity;
	c = (char*)malloc(sizeof(char));
	*c = '0';
	bity = (int*)malloc(8 * sizeof(int));
	while (*c != '\0')	//p�tla dzia�a dop�ki nie natrafi na koniec �a�cucha znak�w. Je�eli otworzymy plik kt�ry nie zosta� zapisany przez ten program, to... obrazek si� sko�czy, a p�tla nadal b�dzie pr�bowa�a dzia�a�
	{
		for (l = 0; l < 8; l++)	//kolejne 8 bit�w
		{
			bity[l] = obrazek.piksele[i][k] % 2;	//sczytaj najmniej znacz�cy bit z piksela
			k++;
			if (k == obrazek.szer)	//dzy�-szur
			{
				i++;
				k = 0;
			}
		}
		a = zdebituj(bity);	//a przyjmuje warto�� odczytan� z 8 kolejnych bit�w
		*c = (char)a;
		tablica[b] = *c;	//a kolejne miejsce w tablicy jest zajmowane przez odczytany znak
		b++;
		if (b % 10 == 0)	//zapobiegamy przepe�nieniu tablicy
		{
			tablica = (char*)realloc(tablica, (b + 10) * sizeof(char));
		}
	}
	free(c);
	free(bity);
	return tablica;
}
void odszyfruj_obraz()
//wywo�ywany z menu g�, odczytuje tekst o wcze�niej zapisanego obrazu
{
	struct obraz* pobrany;
	char* tekst;
	tekst = (char*)malloc(10 * sizeof(char));
	pobrany = (struct obraz*)malloc(sizeof(struct obraz));
	pobrany = kreator2(pobrany);	//otwiera obraz zaszyfrowany.pgm
	if (pobrany != NULL)
	{
		tekst = odczytaj(*pobrany, tekst);	//odczytuje tekst
		printf("%s\n", tekst);	//wy�wietla
		zapis2(tekst);	//zapisuje
		uwolnij(pobrany);	//i zwalnia miejsce zajmowane przez obraz
	}
	free(tekst);
}
int main()
{
	srand((unsigned int)time(NULL));	//ustawia punkt do losowania
	setlocale(LC_ALL, "POLISH");	//pozwala wy�wietli� w konsoli polskie znaki
	while (1)	//a program wy��cza si� czerwonym krzy�ykiem
	{
		menu(0);
		switch (wez_cyfre())
		{
		case 1:
			menu(1);
			switch (wez_cyfre())
			{
			case 1:
				zaszyfruj_tekst();
				break;
			case 2:
				zaszyfruj_obraz();
				break;
			}
			break;
		case 2:
			menu(1);
			switch (wez_cyfre())
			{
			case 1:
				odszyfruj_tekst();
				break;
			case 2:
				odszyfruj_obraz();
				break;
			}
			break;
		}
	}
}