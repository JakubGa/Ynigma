#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <locale.h>
#include "obrazki.h"
#include "struktura.h"


void menu(int poziom)
//funkcja wyœwietla komunikaty w menu g³ównym
{
	switch (poziom)
	{
	case 0:
		printf("Co chcesz zrobiæ?\n1. Zaszyfruj wiadomoœæ\n2. Odszyfruj wiadomoœæ\n");
		break;
	case 1:
		printf("1. Szyfrowanie tekstowe\n2. Szyfrowanie obrazem\n");
		break;
	}
}
int wez_cyfre()
//zwraca cyfrê potrzebn¹ do sterowania menu. Je¿eli nie jest to 1 lub 2, wyœwietla komunikat o b³êdzie, a menu wraca na pocz¹tek
{
	int i, j;
	char* strg;
	j = scanf("%d", &i);
	strg = (char*)malloc(15 * sizeof(char));
	if (j == 0)
	{
		gets(strg);
		printf("Nie ma takiej mo¿liwoœci\n");
		i = 0;
	}
	else if (!((i == 1) || (i == 2)))
	{
		printf("Nie ma takiej mo¿liwoœci\n");
	}
	free(strg);
	return i;
}
char* pobierz_plik(char* tablica)
//pobiera plik .txt o nazwie podanej przez u¿ytkownika i zwraca wskaŸnik na ³añcuch z za³adowanym tekstem
{
	char* nazwa;
	FILE* fp;
	int i = 0;
	char* c;
	c = malloc(sizeof(char));
	nazwa = (char*)malloc(21 * sizeof(char));
	printf("Podaj nazwê szyfrowanego pliku: ");
	scanf_s("%s", nazwa, 21); //teoretycznie przy scanf_s nie powinno dojœæ do przepe³nienia bufora, ale... plik o zbyt d³ugiej nazwie siê nie otworzy
	fp = fopen(nazwa, "r");
	free(nazwa);
	if (fp == NULL)
	{
		perror("B³¹d otwarcia pliku!\n");
		free(tablica);	//je¿eli nie otworzy siê plik, to najpierw zwalnia pamiêæ zaalokowan¹ pod tablicê, dopiero potem zwraca NULL
		return NULL;
	}
	else
	{
		while (fscanf(fp, "%c", c) != EOF)	//odczytuje kolejny znak
		{
			tablica[i] = *c;	//je¿eli nie natrafiono na koniec pliku, to wartoœæ odczytanego znaku zapisywana w tablicy
			i++;
			if (i % 10 == 0)
			{
				tablica = (char*)realloc(tablica, sizeof(char)*(i + 10));	//je¿eli zape³nienie tablicy dojdzie do koñca, zwiêksza rozmiar o kolejne 10
			}
		}
		fclose(fp);
		*c = '\0';	//koñczy ³añcuch znaków
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
//funkcja otrzymuje ³añcuch odczytany z pliku i wylosowan¹ liczbê, stosuje szyfr cezara
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
			oper = ((int)tablica[i] + przesuniecie - 255);	//jeœli wynik przekroczy³by wartoœæ 255 "zawija" go
			tablica[i] = (char)oper;
		}
	}
	return tablica;
}
void zapis(char* tablica)
//zapisuje wczeœniej zaszyfrowany ³añcuch znaków
{
	FILE* fp;
	fp = fopen("zaszyfrowany.txt", "w");
	if (fp == NULL)
	{
		perror("B³¹d zapisu!/n");
	}
	else
	{
		fprintf(fp, "%s", tablica);
		printf("Zapis do pliku ukoñczony\n");
		fclose(fp);
	}
}
void zaszyfruj_tekst()
//funkcja wywo³ywana z menu g³ównego w celu zaszyfrowania pliku
{
	char* tekst;
	int przesuniecie;
	tekst = (char*)malloc(10 * sizeof(char));	//pocz¹tkowy bufor to 10 znaków
	tekst = pobierz_plik(tekst);	//odczytuje tekst z pliku
	if (tekst != NULL)	//a je¿eli siê to uda, wykonuje operacj w celu zaszfrowania go i zapisania
	{
		przesuniecie = losuj();
		tekst = cezar(tekst, przesuniecie);
		zapis(tekst);
		przesuniecie = przesuniecie * 100 + losuj();
		printf("Kod do odczytu twojego pliku to: %d\n", przesuniecie);	//przekazuje u¿ytkownikowi kod do odszyfrowania
		free(tekst);
	}
}
char* pobierz_plik2(char* tablica)
//do tablicy znaków wczytuje zaszyfrowany tekst z pliku
//nie ró¿ni siê zbyt od pobierz_plik, z wyj¹tkiem tego, ¿e nazwa jest na sztywno ustalona
//ale jako ¿e w C nie mogê przeci¹¿yæ funkcji, to muszê dublowaæ nazwy
{
	FILE* fp;
	int i = 0;
	char* c;
	c = (char*)malloc(sizeof(char));
	fp = fopen("zaszyfrowany.txt", "r");
	if (fp == NULL)
	{
		perror("B³¹d otwarcia pliku!\n");
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
//pobiera kod od u¿ytkownika
{
	int haslo;
	printf("Podaj kod do odczytu pliku: ");
	scanf("%d", &haslo); //je¿eli nie poda siê liczby, program przestanie dzia³aæ, ale... nie moja wina ¿e ktoœ nie zna has³a
	haslo /= 100;
	return haslo;
}
char* decesar(char* tablica, int przesuniecie)
//funkcja odwrotna do szyfrowania, sposób dzia³ania ten sam
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
//zaspisuje odszsyfrowany ³añcuch, ale nowa nazwa - nowa funkcja
{
	FILE* fp;
	fp = fopen("odszyfrowany.txt", "w");
	if (fp == NULL)
	{
		perror("B³¹d zapisu!/n");
	}
	else
	{
		fprintf(fp, "%s", tablica);
		printf("Zapis do pliku ukonczony\n");
		fclose(fp);
	}
}
void odszyfruj_tekst()
//funkcja wywo³ywana w menu w celu odszyfrowania tekstu
{
	char *tekst;
	int przesuniecie;
	tekst = (char*)malloc(10 * sizeof(char));
	tekst = pobierz_plik2(tekst);
	if (tekst != NULL)	//je¿eli uda siê za³adowaæ tekst z pliku, to po kolei go oszyfrowujemy, wyœwietlamy i zapisujemy
	{
		przesuniecie = zaladuj_kod();
		tekst = decesar(tekst, przesuniecie);
		printf("%s\n", tekst);
		zapis2(tekst);
		free(tekst);
	}
}
int* na_bitowy(int* tablica, int wartosc)
//funkcja rozbija podan¹ wartoœæ na zapis binarny
{
	int i;
	if (wartosc < 0)	//bo 0-255 to zbyt prosto, lepiej traktowaæ 8 bit jako minus... (potrzebne do odczytu polskich znaków)
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
//funkcja otrzymuje strukturê z obrazem i ³añcuch z tekstem z pliku
//modyfikuje obraz tak, ¿eby zawiera³ w sobie tekst
{
	int* bity;
	int i, j, k=0,l=0,m,a;	//tutaj zrobi³em chaos, ale: i-d³.³añcucha j-wskazuje.kolejny.el.³añcucha l-wysokoœæ.obrazka k-szer.obrazka m-kojelne.cyfry.w.bicie a-zmienna.pomocnicza,¿eby.sprawdziæ.jaki.jest.ostatni.bit.w.pikselu
	i = strlen(tablica)+1;
	bity = (int*)malloc(8*sizeof(int));
	while (i > ((obrazek->szer*obrazek->wys)/8))	//je¿eli obrazek jest za ma³y, powiêksza go tak d³ugo jak trzeba
	{
		skaluj(obrazek);	//jednokrotne powiêkszenie pozwala zapisaæ (prawie) 4 razy wiêcej tekstu, ale wp³yw na jakoœæ grafiki jest przera¿aj¹cy
	}
	for (j = 0; j < i; j++)	//powtarzamy przez ca³y ³añcuch
	{
		bity = na_bitowy(bity, (int)tablica[j]);	//kolejne znaki z ³añcucha s¹ rozbijane na zapis bitowy
		for (m = 0; m < 8; m++)	//i dla ka¿dego kolejnego bitu
		{
			if (k == obrazek->szer)	//je¿eli doszliœmy do koñca wierssza, idziemy do nastêpnego (takie dzyñ-szur w maszynie do pisania)
			{
				k = 0;
				l++;
			}
			a = obrazek->piksele[l][k];	//zmiennej pomocniczej przypisujemy wartoœæ obecnie rozwa¿anego piksela
			if (a % 2 == 1)	//zerujemy najmniej znacz¹cy bit
			{
				a -= 1;
			}
			a += bity[m];	//ustawiamy na najmniej znacz¹cym bicie piksela wartoœæ odczytan¹ ze znaku ³añcucha
			obrazek->piksele[l][k] = a;
			k++;
		}
	}
	free(bity);
}
void zaszyfruj_obraz()
//wywo³ywane z menu g³., zapisuje tekst w obrazie
{
	struct obraz* pobrany;
	char* tekst;
	tekst = (char*)malloc(10 * sizeof(char));
	tekst = pobierz_plik(tekst);	//odczytuje tekst do zaszyfrowania
	pobrany = (struct obraz*)malloc(sizeof(struct obraz));
	pobrany = kreator(pobrany);	//z obrazki.c, otwiera obraz angel.pgm
	if (pobrany != NULL && tekst != NULL)
	{
		modyfikuj(pobrany, tekst);	//je¿eli otwar³o oba elementy modyfikuje obrazek
		zapis_obrazu(*pobrany);	//i zapisuje go jako zaszyfrowany.pgm
	}
	if (pobrany != NULL)	//zwalnia co otworzy³
	{
		uwolnij(pobrany);
	}
	if (tekst != NULL)
	{
		free(tekst);
	}
}
int zdebituj(int* tablica)
//na podstawie ci¹gu zer i jedynek odtwarza liczbê
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
//odczytuje najmniej znacz¹ce bity z kolejnych pikseli obrazka, zbiera po 8 i odczytuje znak
{
	char* c;
	int i = 0, k = 0, l, a,b=0;
	int* bity;
	c = (char*)malloc(sizeof(char));
	*c = '0';
	bity = (int*)malloc(8 * sizeof(int));
	while (*c != '\0')	//pêtla dzia³a dopóki nie natrafi na koniec ³añcucha znaków. Je¿eli otworzymy plik który nie zosta³ zapisany przez ten program, to... obrazek siê skoñczy, a pêtla nadal bêdzie próbowa³a dzia³aæ
	{
		for (l = 0; l < 8; l++)	//kolejne 8 bitów
		{
			bity[l] = obrazek.piksele[i][k] % 2;	//sczytaj najmniej znacz¹cy bit z piksela
			k++;
			if (k == obrazek.szer)	//dzyñ-szur
			{
				i++;
				k = 0;
			}
		}
		a = zdebituj(bity);	//a przyjmuje wartoœæ odczytan¹ z 8 kolejnych bitów
		*c = (char)a;
		tablica[b] = *c;	//a kolejne miejsce w tablicy jest zajmowane przez odczytany znak
		b++;
		if (b % 10 == 0)	//zapobiegamy przepe³nieniu tablicy
		{
			tablica = (char*)realloc(tablica, (b + 10) * sizeof(char));
		}
	}
	free(c);
	free(bity);
	return tablica;
}
void odszyfruj_obraz()
//wywo³ywany z menu g³, odczytuje tekst o wczeœniej zapisanego obrazu
{
	struct obraz* pobrany;
	char* tekst;
	tekst = (char*)malloc(10 * sizeof(char));
	pobrany = (struct obraz*)malloc(sizeof(struct obraz));
	pobrany = kreator2(pobrany);	//otwiera obraz zaszyfrowany.pgm
	if (pobrany != NULL)
	{
		tekst = odczytaj(*pobrany, tekst);	//odczytuje tekst
		printf("%s\n", tekst);	//wyœwietla
		zapis2(tekst);	//zapisuje
		uwolnij(pobrany);	//i zwalnia miejsce zajmowane przez obraz
	}
	free(tekst);
}
int main()
{
	srand((unsigned int)time(NULL));	//ustawia punkt do losowania
	setlocale(LC_ALL, "POLISH");	//pozwala wyœwietliæ w konsoli polskie znaki
	while (1)	//a program wy³¹cza siê czerwonym krzy¿ykiem
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