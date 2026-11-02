#ifndef PLAY_HPP
#define PLAY_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

using namespace sf;

class Play {
public:
    Play(RenderWindow& okno, int razmerPolya);
    ~Play();

    void obrabotatSobytie(Event sobytie);
    void risovat();
    bool zakonchena();
    void obnovit();

private:
    int proverkaReshimosti();
    void sozdatKnopki();
    void zagruzitKartinki();
    void perekluchit(int stroka, int stolbec);
    bool proveritPobedu();
    void pokazatPodskazku();
    void obnovitOknoPodskazok();
    void obrabotatOknoPodskazok();
    void pokazatOknoPobedy();
    std::string poslednyayaStroka();
    void zagruzitPodskazki();

    RenderWindow& okno;
    int razmer;
    int kolPerekluch;
    char pole[5][5];

    std::vector<std::vector<Sprite>> yacheyki;
    Texture teksturaVikl;
    Texture teksturaVkl;

    RectangleShape knopkaNazad;
    Text tekstKnopkiNazad;

    RectangleShape knopkaPodskazka;
    Text tekstKnopkiPodskazka;

    Font shrift;

    bool igraZakonchena;
    bool kartinkiZagruzheny;
    bool oknoPobedyOtkrito;

    float razmerYacheyki;
    float otstup;
    float smX;
    float smY;

    struct Podskazka {
        std::string poslednyayaStroka;
        std::vector<std::pair<int, int>> hody;
    };
    std::vector<Podskazka> vsePodskazki;
    std::vector<std::pair<int, int>> tekushiePodskazki;
    int nomerPodskazki;

    RenderWindow* oknoPodskazok;
    bool oknoPodskazokOtkrito;
    std::vector<String> tekstPodskazok;
    Text tekstPodskazki;
    RectangleShape knopkaZakrit;
    Text tekstKnopkiZakrit;

    RenderWindow* oknoPobedy;
    RectangleShape knopkaDalye;
    Text tekstKnopkiDalye;
};

#endif