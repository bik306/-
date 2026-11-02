#include "Label.h"

using namespace sf;
using namespace std;

Label::Label(string noviyTekst, string putKShriftu)
{
    shrift.loadFromFile(putKShriftu);

    tekst.setString(noviyTekst);
    tekst.setFont(shrift);
    tekst.setCharacterSize(64);

    FloatRect granici = tekst.getLocalBounds();
    float centrX = granici.left + granici.width / 2.0f;
    float centrY = granici.top + granici.height / 2.0f;
    tekst.setOrigin(centrX, centrY);
}

void Label::draw(RenderTarget& target, RenderStates sostoyanie) const
{
    sostoyanie.transform.combine(getTransform());
    target.draw(tekst, sostoyanie);
}

void Label::ustanovitStroku(string novayaStroka)
{
    tekst.setString(novayaStroka);

    FloatRect granici = tekst.getLocalBounds();
    float centrX = granici.left + granici.width / 2.0f;
    float centrY = granici.top + granici.height / 2.0f;
    tekst.setOrigin(centrX, centrY);
}

void Label::tsentrirovatStroku(Vector2u razmerOkna)
{
    float newX = razmerOkna.x / 2.0f;
    float newY = razmerOkna.y / 2.0f;
    setPosition(newX, newY);
}

Vector2f Label::poluchitRazmer()
{
    float shir = tekst.getGlobalBounds().width;
    float vis = tekst.getGlobalBounds().height;
    return Vector2f(shir, vis);
}