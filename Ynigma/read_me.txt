Funkcje:
1. Szyfrowanie tekstem.
Prosty szyfr cezara, losuje liczb� z zakresu 10-99 i "przesuwa" ka�dy kolejny znak o tyle, za odniesienie bior�c 255-znakowy kod ASCII. W przypadku gdyby warto�� znaku mia�a przekroczy� 255 "zawija" j�. W odpowiedzi podaje kod w formacie xxyy, gdzie xx to liczba potrzebna od odwr�cenia procesu, a yy losowe cyfry.
2 . Szyfrowanie obrazem.
W zasadzie g��wna zabawa. Jako �e ten projekt nie mia� polega� na edycji obraz�w pozwoli�em sobie wykorzysta� lekko zmodyfikowane funkcje z poprzedniego projektu (obrazki.c). W zwi�zku z tym operuj� na obrazach pgn z 225 bitow� g��bi� szaro�ci.
Zdaj� sobie spraw�, �e pliki typu RGB i RGBA by�yby 3-4 krotnie wydajniejsze, ale jak na razie nie narzekam na brak miejca w obrazie.Do��czony plik angel.pgm pozwala na zaszyfrowanie ponad 20 tys znak�w, a wbudowane funkcje powi�kszaj� go w razie potrzeby.
Pokr�tce jak to dzia�a - idea jest taka, �e ka�dy znak (ASCII) rozbija si� na zapis bitowy. Nast�pnie w ka�dym kolejnym pikselu obrazu podmienia si� najmniej znacz�cy bit, tak �eby 8 pikseli zawiera�o ca�y znak. R�nice, nawet przy odcieniu szaro�ci, s� niezauwa�alne dla ludzkiego oka. 
U�ycie RGBA pozwoli�oby zapisa� znak w 2 pikselach, a r�nice by�yby niemo�liwe do wykrycia.
W formie odwrotnej pobiera si� zaszyfrowany obraz i "czyta" kolejne znaki po 8 pikseli, a� do ko�ca tablicy.

Mam nadziej�, �e uzna Pan ten projekt za interesuj�cy, a wykonanie za wystarczaj�ce.