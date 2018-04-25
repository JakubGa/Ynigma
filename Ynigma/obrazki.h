#ifndef OBRAZKI_H
#define OBRAZKI_H


struct obraz* kreator(struct obraz* lista);
struct obraz* kreator2(struct obraz* lista);
void zapis_obrazu(struct obraz lista);
void skaluj(struct obraz* lista);
void uwolnij(struct obraz* lista);

#endif //OBRAZKI_H
