#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "struktura.h"

/*Poni¿sze funkcje to mniej lub bardziej zmodyfikowane fragmenty programu do przetwarzania obrazów
W zwi¹zku z tym nie bêdê ich komentowa³, dzia³aj¹ jak poprzednio*/

void olej_komentarze(FILE* uchwyt)
{
	int znak;
	char linia[50];
	while (((znak = fgetc(uchwyt)) != EOF) && isspace(znak))
		;
	if (znak == '#')
	{
		fgets(linia, 50, uchwyt);
	}
	else
	{
		fseek(uchwyt, -1, SEEK_CUR);
	}
}
struct obraz* kreator(struct obraz* lista)
{
	char c;
	int i, j;
	FILE* fp;
	fp = fopen("angel.pgm", "r");
	if (fp == NULL)
	{
		perror("Nie znaleziono pliku");
		system("PAUSE");
		free(lista);
		return NULL;
	}
	else
	{
		olej_komentarze(fp);
		fseek(fp, 3, SEEK_CUR);
		olej_komentarze(fp);
		fscanf(fp, "%d", &lista->szer);
		fscanf(fp, "%d", &lista->wys);
		lista->piksele = (int**)malloc(lista->wys*sizeof(int*));
		for (i = 0; i < lista->wys; i++)
		{
			lista->piksele[i] = (int*)malloc(lista->szer*sizeof(int));
		}
		olej_komentarze(fp);
		fscanf(fp, "%d", &lista->glebia);
		while (fscanf(fp, "%c", &c) != EOF)
		{
			for (i = 0; i < lista->wys; i++)
			{
				olej_komentarze(fp);
				for (j = 0; j < lista->szer; j++)
				{
					fscanf(fp, "%d", &lista->piksele[i][j]);
				}

			}

		}
		fclose(fp);
	}
	return lista;


}
struct obraz* kreator2(struct obraz* lista)
{
	char c;
	int i, j;
	FILE* fp;
	fp = fopen("zaszyfrowany.pgm", "r");
	if (fp == NULL)
	{
		perror("Nie znaleziono pliku");
		system("PAUSE");
		free(&lista);
		return NULL;
	}
	else
	{
		olej_komentarze(fp);
		fseek(fp, 3, SEEK_CUR);
		olej_komentarze(fp);
		fscanf(fp, "%d", &lista->szer);
		fscanf(fp, "%d", &lista->wys);
		lista->piksele = (int**)malloc(lista->wys*sizeof(int*));
		for (i = 0; i < lista->wys; i++)
		{
			lista->piksele[i] = (int*)malloc(lista->szer*sizeof(int));
		}
		olej_komentarze(fp);
		fscanf(fp, "%d", &lista->glebia);
		while (fscanf(fp, "%c", &c) != EOF)
		{
			for (i = 0; i < lista->wys; i++)
			{
				olej_komentarze(fp);
				for (j = 0; j < lista->szer; j++)
				{
					fscanf(fp, "%d", &lista->piksele[i][j]);
				}

			}

		}
		fclose(fp);
	}
	return lista;


}
void zapis_obrazu(struct obraz lista)
{
	int i, j;
	FILE* fp;
	fp = fopen("zaszyfrowany.pgm", "w");
	fprintf(fp, "P2\n%d %d\n%d\n", lista.szer, lista.wys, lista.glebia);
	for (i = 0; i<lista.wys; i++)
	{
		for (j = 0; j<lista.szer; j++)
		{
			fprintf(fp, "%d ", lista.piksele[i][j]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
	printf("Zapisany jako \"zaszyfrowany.pgm\"\n");
}
void filtr_Gaussa(struct obraz* lista)
{
	int** robocza;
	int i, j;
	robocza = (int**)malloc((lista->wys - 2)*sizeof(int*));
	for (i = 0; i<(lista->wys - 2); i++)
	{
		robocza[i] = (int*)malloc((lista->szer - 2)*sizeof(int));
	}
	for (i = 1; i<(lista->wys - 1); i++)
	{
		for (j = 1; j<(lista->szer - 1); j++)
		{
			robocza[i - 1][j - 1] = (((lista->piksele[i - 1][j - 1] + lista->piksele[i - 1][j + 1] + lista->piksele[i + 1][j - 1] + lista->piksele[i + 1][j + 1]) +
				(lista->piksele[i][j - 1] + lista->piksele[i][j + 1] + lista->piksele[i + 1][j] + lista->piksele[i + 1][j]) * 2 +
				(lista->piksele[i][j] * 4)) / 16);
		}
	}
	lista->wys -= 2;
	lista->szer -= 2;
	lista->piksele = (int**)realloc(lista->piksele, lista->wys*sizeof(int*));
	for (i = 0; i < lista->wys; i++)
	{
		lista->piksele[i] = (int*)realloc(lista->piksele[i], lista->szer*sizeof(int));
	}
	for (i = 0; i < lista->wys; i++)
	{
		for (j = 0; j < lista->szer; j++)
		{
			lista->piksele[i][j] = robocza[i][j];
		}

	}
	for (i = 0; i<lista->wys; i++)
	{
		free(robocza[i]);
	}
	free(robocza);
}
void wyostrzenie(struct obraz* lista)
{
	int** robocza;
	int i, j;
	robocza = (int**)malloc((lista->wys - 2)*sizeof(int*));
	for (i = 0; i<(lista->wys - 2); i++)
	{
		robocza[i] = (int*)malloc((lista->szer - 2)*sizeof(int));
	}
	for (i = 1; i<(lista->wys - 1); i++)
	{
		for (j = 1; j<(lista->szer - 1); j++)
		{
			robocza[i - 1][j - 1] = (((lista->piksele[i][j - 1] + lista->piksele[i][j + 1] + lista->piksele[i + 1][j] + lista->piksele[i - 1][j])*(-1) +
				(lista->piksele[i][j] * 5)));
			if (robocza[i - 1][j - 1]<0)
			{
				robocza[i - 1][j - 1] = 0;
			}
		}
	}
	lista->wys -= 2;
	lista->szer -= 2;
	lista->piksele = (int**)realloc(lista->piksele, lista->wys*sizeof(int*));
	for (i = 0; i < lista->wys; i++)
	{
		lista->piksele[i] = (int*)realloc(lista->piksele[i], lista->szer*sizeof(int));
	}
	for (i = 0; i < lista->wys; i++)
	{
		for (j = 0; j < lista->szer; j++)
		{
			lista->piksele[i][j] = robocza[i][j];
		}

	}
	for (i = 0; i<lista->wys; i++)
	{
		free(robocza[i]);
	}
	free(robocza);
}
void uwolnij(struct obraz* lista)
{
	int i;
	for (i = 0; i < lista->wys; i++)
	{
		free(lista->piksele[i]);
	}
	free(lista->piksele);
}
void skaluj(struct obraz* lista)
{
	wyostrzenie(lista);
	int x, y, n, i, j;
	int** robocza;
	float skala;
	n = 200;
	skala = (float)n / 100.F;
	x = (int)((float)lista->wys*skala) + 1;
	y = (int)((float)lista->szer*skala) + 1;
	robocza = (int**)calloc((unsigned)x, sizeof(int*));
	for (i = 0; i<x; i++)
	{
		robocza[i] = (int*)calloc((unsigned)y, sizeof(int));
	}
	for (i = 0; i<lista->wys; i++)
	{
		for (j = 0; j<lista->szer; j++)
		{
			robocza[(int)(i*skala)][(int)(j*skala)] = lista->piksele[i][j] + 1;
		}
	}
	if (n>100)
	{
		for (i = 1; i<x - 1; i++)
		{
			for (j = 1; j<y - 1; j++)
			{
				if (robocza[i][j] == 0)
				{
					robocza[i][j] = (robocza[i - 1][j - 1] + robocza[i + 1][j + 1]) / 2;
				}
			}
		}
		for (i = 1; i<y - 1; i++)
		{
			for (j = 1; j<x - 1; j++)
			{
				if (robocza[j][i] == 0)
				{
					robocza[j][i] = (robocza[j - 1][i - 1] + robocza[j + 1][i + 1]) / 2;
				}
			}
		}
		for (i = 1; i<x - 1; i++)
		{
			for (j = 1; j<y - 1; j++)
			{
				if (robocza[i][j] == 0)
				{
					robocza[i][j] = (robocza[i - 1][j - 1] + robocza[i + 1][j + 1] + robocza[i - 1][j + 1] + robocza[i + 1][j - 1]) / 4;
				}
			}
		}
	}
	uwolnij(lista);

	lista->wys = x;
	lista->szer = y;
	lista->piksele = (int**)malloc(lista->wys*sizeof(int*));
	for (i = 0; i < lista->wys; i++)
	{
		lista->piksele[i] = (int*)malloc(lista->szer*sizeof(int));
	}
	for (i = 0; i < lista->wys; i++)
	{
		for (j = 0; j < lista->szer; j++)
		{
			lista->piksele[i][j] = robocza[i][j] - 1;
		}

	}
	for (i = 0; i<lista->wys; i++)
	{
		free(robocza[i]);
	}
	free(robocza);
	filtr_Gaussa(lista);

}
