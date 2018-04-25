Funkcje:
1. Szyfrowanie tekstem.
Prosty szyfr cezara, losuje liczbê z zakresu 10-99 i "przesuwa" ka¿dy kolejny znak o tyle, za odniesienie bior¹c 255-znakowy kod ASCII. W przypadku gdyby wartoœæ znaku mia³a przekroczyæ 255 "zawija" j¹. W odpowiedzi podaje kod w formacie xxyy, gdzie xx to liczba potrzebna od odwrócenia procesu, a yy losowe cyfry.
2 . Szyfrowanie obrazem.
W zasadzie g³ówna zabawa. Jako ¿e ten projekt nie mia³ polegaæ na edycji obrazów pozwoli³em sobie wykorzystaæ lekko zmodyfikowane funkcje z poprzedniego projektu (obrazki.c). W zwi¹zku z tym operujê na obrazach pgn z 225 bitow¹ g³êbi¹ szaroœci.
Zdajê sobie sprawê, ¿e pliki typu RGB i RGBA by³yby 3-4 krotnie wydajniejsze, ale jak na razie nie narzekam na brak miejca w obrazie.Do³¹czony plik angel.pgm pozwala na zaszyfrowanie ponad 20 tys znaków, a wbudowane funkcje powiêkszaj¹ go w razie potrzeby.
Pokrótce jak to dzia³a - idea jest taka, ¿e ka¿dy znak (ASCII) rozbija siê na zapis bitowy. Nastêpnie w ka¿dym kolejnym pikselu obrazu podmienia siê najmniej znacz¹cy bit, tak ¿eby 8 pikseli zawiera³o ca³y znak. Ró¿nice, nawet przy odcieniu szaroœci, s¹ niezauwa¿alne dla ludzkiego oka. 
U¿ycie RGBA pozwoli³oby zapisaæ znak w 2 pikselach, a ró¿nice by³yby niemo¿liwe do wykrycia.
W formie odwrotnej pobiera siê zaszyfrowany obraz i "czyta" kolejne znaki po 8 pikseli, a¿ do koñca tablicy.

Mam nadziejê, ¿e uzna Pan ten projekt za interesuj¹cy, a wykonanie za wystarczaj¹ce.