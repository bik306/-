#include "Label.h"

Label::Label(string tekst, string shrift) {
	this->shrift.loadFromFile(shrift);

	this->tekst.setString(tekst);
	this->tekst.setFont(this->shrift);
	this->tekst.setCharacterSize(64);

	sf::FloatRect tekstPryamougolnik = this->tekst.getLocalBounds();
	this->tekst.setOrigin(tekstPryamougolnik.left + tekstPryamougolnik.width / 2.0f,
		tekstPryamougolnik.top + tekstPryamougolnik.height / 2.0f);
}

void Label::draw(RenderTarget& target, RenderStates sostoyanie) const {
	sostoyanie.transform.combine(getTransform());
	target.draw(tekst, sostoyanie);
}

void Label::ustanovitStroku(string tekst) {
	this->tekst.setString(tekst);

	sf::FloatRect tekstPryamougolnik = this->tekst.getLocalBounds();
	this->tekst.setOrigin(tekstPryamougolnik.left + tekstPryamougolnik.width / 2.0f,
		tekstPryamougolnik.top + tekstPryamougolnik.height / 2.0f);
}

void Label::tsentrirovatStroku(Vector2u okno) {
	setPosition(okno.x / 2.0f, okno.y / 2.0f);
}

Vector2f Label::poluchitRazmer() {
	return Vector2f(tekst.getGlobalBounds().width, tekst.getGlobalBounds().height);
}