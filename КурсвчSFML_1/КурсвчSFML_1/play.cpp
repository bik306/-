#include "play.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>

using namespace sf;
using namespace std;

Play::Play(RenderWindow& win, int razmerPolya) : okno(win)
{
    // Запоминаем всякие начальные значения
    igraZakonchena = false;
    kartinkiZagruzheny = false;
    nomerPodskazki = 0;
    oknoPodskazok = nullptr;
    oknoPodskazokOtkrito = false;
    oknoPobedyOtkrito = false;
    oknoPobedy = nullptr;

    // Настраиваем рандом
    unsigned int vremya = (unsigned int)time(nullptr);
    srand(vremya);

    // Выбираем размер поля
    if (razmerPolya == 3)
    {
        razmer = 3;
        kolPerekluch = 1;
    }
    else
    {
        razmer = 5;
        kolPerekluch = 2;
    }

    // Считаем размеры ячеек
    float shirinaOkna = (float)okno.getSize().x;
    float visotaOkna = (float)okno.getSize().y;

    if (razmer == 3)
    {
        razmerYacheyki = 120;
        otstup = 2;
    }
    else
    {
        razmerYacheyki = 80;
        otstup = 2;
    }

    // Считаем отступы чтобы поле было по центру
    float obshayaShir = razmer * razmerYacheyki + (razmer - 1) * otstup;
    float obshayaVis = razmer * razmerYacheyki + (razmer - 1) * otstup;
    smX = (shirinaOkna - obshayaShir) / 2.0f;
    smY = (visotaOkna - obshayaVis) / 2.0f - 50;

    // Загружаем картинки
    zagruzitKartinki();

    // Загружаем шрифт
    bool shriftOk = shrift.loadFromFile("C:/Windows/Fonts/arial.ttf");
    if (shriftOk == false)
    {
        shriftOk = shrift.loadFromFile("arial.ttf");
        if (shriftOk == false)
        {
            cout << "Шрифт не загрузился :(" << endl;
        }
    }

    // Создаем кнопки на поле
    sozdatKnopki();

    // Загружаем подсказки из файла
    zagruzitPodskazki();

    cout << "Генерирую уровень..." << endl;

    // Генерируем поле пока оно не станет решаемым
    int reshaemost = 0;
    int popytka = 0;

    while (reshaemost == 0)
    {
        popytka = popytka + 1;

        // Заполняем поле случайными символами
        for (int i = 0; i < razmer; i++)
        {
            for (int j = 0; j < razmer; j++)
            {
                int randChislo = rand() % 2;
                if (randChislo == 0)
                {
                    pole[i][j] = '-';
                }
                else
                {
                    pole[i][j] = '|';
                }
            }
        }

        // Для поля 3x3 всегда решаемо, для 5x5 проверяем
        if (razmer == 3)
        {
            reshaemost = 1;
        }
        else
        {
            reshaemost = proverkaReshimosti();
            if (reshaemost == 0)
            {
                cout << "Попытка " << popytka << ": поле нерешаемое, генерирую заново..." << endl;
            }
        }
    }

    if (razmer == 5)
    {
        cout << "Ура! Сгенерировал решаемое поле! (попыток: " << popytka << ")" << endl;
    }

    // Обновляем отображение
    obnovit();

    // Настраиваем текст для подсказок
    tekstPodskazki.setFont(shrift);
    tekstPodskazki.setCharacterSize(18);
    tekstPodskazki.setFillColor(Color::Black);

    // Настраиваем кнопку закрытия
    knopkaZakrit.setSize(Vector2f(100, 30));
    knopkaZakrit.setFillColor(Color(200, 100, 100));
    knopkaZakrit.setOutlineThickness(2);
    knopkaZakrit.setOutlineColor(Color(150, 50, 50));

    tekstKnopkiZakrit.setFont(shrift);
    tekstKnopkiZakrit.setString(L"Закрыть");
    tekstKnopkiZakrit.setCharacterSize(18);
    tekstKnopkiZakrit.setFillColor(Color::White);

    FloatRect tr = tekstKnopkiZakrit.getLocalBounds();
    tekstKnopkiZakrit.setOrigin(tr.left + tr.width / 2.0f, tr.top + tr.height / 2.0f);

    // Настраиваем кнопку "Далее" для окна победы
    knopkaDalye.setSize(Vector2f(200, 50));
    knopkaDalye.setFillColor(Color(100, 150, 200));
    knopkaDalye.setOutlineThickness(2);
    knopkaDalye.setOutlineColor(Color(50, 100, 150));

    tekstKnopkiDalye.setFont(shrift);
    tekstKnopkiDalye.setString(L"Далее →");
    tekstKnopkiDalye.setCharacterSize(24);
    tekstKnopkiDalye.setFillColor(Color::White);

    FloatRect nr = tekstKnopkiDalye.getLocalBounds();
    tekstKnopkiDalye.setOrigin(nr.left + nr.width / 2.0f, nr.top + nr.height / 2.0f);
}

Play::~Play()
{
    if (oknoPodskazok != nullptr)
    {
        oknoPodskazok->close();
        delete oknoPodskazok;
        oknoPodskazok = nullptr;
    }
    if (oknoPobedy != nullptr)
    {
        oknoPobedy->close();
        delete oknoPobedy;
        oknoPobedy = nullptr;
    }
}

void Play::zagruzitKartinki()
{
    bool viklZagruz = teksturaVikl.loadFromFile("vikl.png");
    if (viklZagruz == false)
    {
        cout << "Не могу загрузить vykl.png, делаю сам" << endl;
        int w = (unsigned int)razmerYacheyki;
        int h = (unsigned int)razmerYacheyki;
        teksturaVikl.create(w, h);
        Image kartinka;
        kartinka.create(w, h, Color(100, 100, 150));
        teksturaVikl.update(kartinka);
    }

    bool vklZagruz = teksturaVkl.loadFromFile("vkl.png");
    if (vklZagruz == false)
    {
        cout << "Не могу загрузить vkl.png, делаю сам" << endl;
        int w = (unsigned int)razmerYacheyki;
        int h = (unsigned int)razmerYacheyki;
        teksturaVkl.create(w, h);
        Image kartinka;
        kartinka.create(w, h, Color(150, 150, 200));
        teksturaVkl.update(kartinka);
    }

    kartinkiZagruzheny = true;
}

void Play::sozdatKnopki()
{
    yacheyki.resize(razmer);

    for (int i = 0; i < razmer; i++)
    {
        yacheyki[i].resize(razmer);

        for (int j = 0; j < razmer; j++)
        {
            float x = smX + j * (razmerYacheyki + otstup);
            float y = smY + i * (razmerYacheyki + otstup);

            yacheyki[i][j].setTexture(teksturaVikl);
            yacheyki[i][j].setPosition(x, y);

            Vector2u razmerTekst = teksturaVikl.getSize();
            float masshtabX = razmerYacheyki / (float)razmerTekst.x;
            float masshtabY = razmerYacheyki / (float)razmerTekst.y;
            yacheyki[i][j].setScale(masshtabX, masshtabY);
        }
    }

    float shirKnopki = 180;
    float visKnopki = 50;

    knopkaNazad.setSize(Vector2f(shirKnopki, visKnopki));
    knopkaNazad.setPosition(10, 10);
    knopkaNazad.setFillColor(Color(200, 100, 100));
    knopkaNazad.setOutlineThickness(2);
    knopkaNazad.setOutlineColor(Color(150, 50, 50));

    tekstKnopkiNazad.setFont(shrift);
    tekstKnopkiNazad.setString(L"Меню");
    tekstKnopkiNazad.setCharacterSize(24);
    tekstKnopkiNazad.setFillColor(Color::White);

    FloatRect tr = tekstKnopkiNazad.getLocalBounds();
    tekstKnopkiNazad.setOrigin(tr.left + tr.width / 2.0f, tr.top + tr.height / 2.0f);
    tekstKnopkiNazad.setPosition(10 + shirKnopki / 2.0f, 10 + visKnopki / 2.0f);

    float shirPodskazki = 180;
    float visPodskazki = 50;

    knopkaPodskazka.setSize(Vector2f(shirPodskazki, visPodskazki));
    knopkaPodskazka.setPosition(10, 70);
    knopkaPodskazka.setFillColor(Color(100, 150, 200));
    knopkaPodskazka.setOutlineThickness(2);
    knopkaPodskazka.setOutlineColor(Color(50, 100, 150));

    tekstKnopkiPodskazka.setFont(shrift);
    tekstKnopkiPodskazka.setString(L"Подсказка");
    tekstKnopkiPodskazka.setCharacterSize(24);
    tekstKnopkiPodskazka.setFillColor(Color::White);

    FloatRect hr = tekstKnopkiPodskazka.getLocalBounds();
    tekstKnopkiPodskazka.setOrigin(hr.left + hr.width / 2.0f, hr.top + hr.height / 2.0f);
    tekstKnopkiPodskazka.setPosition(10 + shirPodskazki / 2.0f, 70 + visPodskazki / 2.0f);
}

int Play::proverkaReshimosti()
{
    int n = razmer * razmer;

    vector<vector<int>> A(n, vector<int>(n + 1, 0));

    for (int i = 0; i < razmer; i++)
    {
        for (int j = 0; j < razmer; j++)
        {
            int ii = i * razmer + j;

            A[ii][ii] = 1;

            if (i > 0)
            {
                A[ii][(i - 1) * razmer + j] = 1;
            }
            if (i < razmer - 1)
            {
                A[ii][(i + 1) * razmer + j] = 1;
            }
            if (j > 0)
            {
                A[ii][i * razmer + (j - 1)] = 1;
            }
            if (j < razmer - 1)
            {
                A[ii][i * razmer + (j + 1)] = 1;
            }

            if (pole[i][j] == '-')
            {
                A[ii][n] = 1;
            }
            else
            {
                A[ii][n] = 0;
            }
        }
    }

    int stroka = 0;
    vector<int> gde(n, -1);

    for (int stolbec = 0; stolbec < n && stroka < n; stolbec++)
    {
        int vybor = stroka;
        for (int i = stroka; i < n; i++)
        {
            if (A[i][stolbec] == 1)
            {
                vybor = i;
                break;
            }
        }

        if (A[vybor][stolbec] == 0)
        {
            continue;
        }

        swap(A[stroka], A[vybor]);
        gde[stolbec] = stroka;

        for (int i = 0; i < n; i++)
        {
            if (i != stroka && A[i][stolbec] == 1)
            {
                for (int j = stolbec; j <= n; j++)
                {
                    A[i][j] = A[i][j] ^ A[stroka][j];
                }
            }
        }
        stroka = stroka + 1;
    }

    for (int i = 0; i < n; i++)
    {
        bool vseNuli = true;
        for (int j = 0; j < n; j++)
        {
            if (A[i][j] != 0)
            {
                vseNuli = false;
                break;
            }
        }
        if (vseNuli == true && A[i][n] == 1)
        {
            return 0;
        }
    }

    return 1;
}

void Play::perekluchit(int stroka, int stolbec)
{
    // Меняем центральную ячейку
    if (pole[stroka][stolbec] == '-')
    {
        pole[stroka][stolbec] = '|';
    }
    else
    {
        pole[stroka][stolbec] = '-';
    }

    // Верхняя ячейка
    if (stroka > 0)
    {
        if (pole[stroka - 1][stolbec] == '-')
        {
            pole[stroka - 1][stolbec] = '|';
        }
        else
        {
            pole[stroka - 1][stolbec] = '-';
        }
    }

    // Нижняя ячейка
    if (stroka < razmer - 1)
    {
        if (pole[stroka + 1][stolbec] == '-')
        {
            pole[stroka + 1][stolbec] = '|';
        }
        else
        {
            pole[stroka + 1][stolbec] = '-';
        }
    }

    // Левая ячейка
    if (stolbec > 0)
    {
        if (pole[stroka][stolbec - 1] == '-')
        {
            pole[stroka][stolbec - 1] = '|';
        }
        else
        {
            pole[stroka][stolbec - 1] = '-';
        }
    }

    // Правая ячейка
    if (stolbec < razmer - 1)
    {
        if (pole[stroka][stolbec + 1] == '-')
        {
            pole[stroka][stolbec + 1] = '|';
        }
        else
        {
            pole[stroka][stolbec + 1] = '-';
        }
    }
}

bool Play::proveritPobedu()
{
    for (int i = 0; i < razmer; i++)
    {
        for (int j = 0; j < razmer; j++)
        {
            if (pole[i][j] != '|')
            {
                return false;
            }
        }
    }

    cout << "ПОБЕДА!!! Ура!!! Решил головоломку!" << endl;
    return true;
}

void Play::pokazatOknoPobedy()
{
    if (oknoPobedy != nullptr)
    {
        oknoPobedy->close();
        delete oknoPobedy;
        oknoPobedy = nullptr;
    }

    oknoPobedy = new RenderWindow(VideoMode(400, 250), "ПОБЕДА!");
    oknoPobedyOtkrito = true;

    Text tekstPobeda;
    tekstPobeda.setFont(shrift);
    tekstPobeda.setCharacterSize(28);
    tekstPobeda.setFillColor(Color::Black);
    tekstPobeda.setString(L"ПОБЕДА!\n\nВы решили\nголоволомку!");

    FloatRect gran = tekstPobeda.getLocalBounds();
    tekstPobeda.setOrigin(gran.left + gran.width / 2.0f, gran.top + gran.height / 2.0f);
    tekstPobeda.setPosition(200, 100);

    float xKnop = (400 - 200) / 2.0f;
    float yKnop = 180;
    knopkaDalye.setPosition(xKnop, yKnop);
    tekstKnopkiDalye.setPosition(xKnop + 100, yKnop + 25);

    oknoPobedy->clear(Color(255, 255, 200));
    oknoPobedy->draw(tekstPobeda);
    oknoPobedy->draw(knopkaDalye);
    oknoPobedy->draw(tekstKnopkiDalye);
    oknoPobedy->display();

    while (oknoPobedyOtkrito == true && oknoPobedy != nullptr && oknoPobedy->isOpen() == true)
    {
        Event sobitie;
        while (oknoPobedy->pollEvent(sobitie))
        {
            if (sobitie.type == Event::Closed)
            {
                oknoPobedy->close();
                oknoPobedyOtkrito = false;
                delete oknoPobedy;
                oknoPobedy = nullptr;
                igraZakonchena = true;
                return;
            }
            if (sobitie.type == Event::MouseButtonPressed)
            {
                Vector2i m = Mouse::getPosition(*oknoPobedy);
                float mx = (float)m.x;
                float my = (float)m.y;

                if (knopkaDalye.getGlobalBounds().contains(mx, my))
                {
                    oknoPobedy->close();
                    oknoPobedyOtkrito = false;
                    delete oknoPobedy;
                    oknoPobedy = nullptr;
                    igraZakonchena = true;
                    return;
                }
            }
        }
    }
}

string Play::poslednyayaStroka()
{
    string rez = "";
    for (int j = 0; j < razmer; j++)
    {
        rez = rez + pole[razmer - 1][j];
    }
    return rez;
}

void Play::zagruzitPodskazki()
{
    ifstream fail("podskazki.txt");
    if (fail.is_open() == false)
    {
        cout << "Ошибка! Файл podskazki.txt не найден!" << endl;
        return;
    }

    string stroka;
    Podskazka tekushaya;
    bool chitayuHodi = false;

    while (getline(fail, stroka))
    {
        // Убираем пробелы в начале
        while (stroka.empty() == false && stroka[0] == ' ')
        {
            stroka.erase(0, 1);
        }

        // Убираем пробелы в конце
        while (stroka.empty() == false && stroka[stroka.length() - 1] == ' ')
        {
            stroka.erase(stroka.length() - 1, 1);
        }

        // Убираем символ возврата каретки
        if (stroka.empty() == false && stroka[stroka.length() - 1] == '\r')
        {
            stroka.pop_back();
        }

        // Если пустая строка - разделитель
        if (stroka.empty() == true)
        {
            if (chitayuHodi == true && tekushaya.hody.empty() == false)
            {
                vsePodskazki.push_back(tekushaya);
            }
            chitayuHodi = false;
            continue;
        }

        // Убираем все пробелы для проверки
        string bezProbelov = "";
        for (size_t k = 0; k < stroka.length(); k++)
        {
            if (stroka[k] != ' ')
            {
                bezProbelov = bezProbelov + stroka[k];
            }
        }

        // Проверка - это паттерн из - и | ?
        bool etoPattern = true;
        for (size_t k = 0; k < bezProbelov.length(); k++)
        {
            if (bezProbelov[k] != '-' && bezProbelov[k] != '|')
            {
                etoPattern = false;
                break;
            }
        }

        // Проверка - это числа?
        bool etoChisla = false;
        if (bezProbelov.empty() == false)
        {
            if (isdigit(bezProbelov[0]) != 0)
            {
                etoChisla = true;
            }
        }

        if (etoPattern == true && etoChisla == false)
        {
            // Начало новой записи
            if (chitayuHodi == true && tekushaya.hody.empty() == false)
            {
                vsePodskazki.push_back(tekushaya);
            }
            tekushaya.poslednyayaStroka = bezProbelov;
            tekushaya.hody.clear();
            chitayuHodi = true;
            cout << "Загружен паттерн: [" << tekushaya.poslednyayaStroka << "] для поля "
                << tekushaya.poslednyayaStroka.length() << "x"
                << tekushaya.poslednyayaStroka.length() << endl;
        }
        else if (etoChisla == true && chitayuHodi == true && bezProbelov.length() >= 2)
        {
            int strokaH = bezProbelov[0] - '0';
            int stolbecH = bezProbelov[1] - '0';
            tekushaya.hody.push_back({ strokaH, stolbecH });
            cout << "  Подсказка: строка=" << strokaH << ", столбец=" << stolbecH << endl;
        }
    }

    if (chitayuHodi == true && tekushaya.hody.empty() == false)
    {
        vsePodskazki.push_back(tekushaya);
    }

    fail.close();
    cout << "Всего загружено " << vsePodskazki.size() << " блоков подсказок" << endl;
}

void Play::obnovitOknoPodskazok()
{
    if (oknoPodskazokOtkrito == false || oknoPodskazok == nullptr)
    {
        return;
    }

    if (oknoPodskazok->isOpen() == false)
    {
        oknoPodskazokOtkrito = false;
        delete oknoPodskazok;
        oknoPodskazok = nullptr;
        return;
    }

    oknoPodskazok->clear(Color(255, 255, 200));

    RectangleShape fon;
    fon.setSize(Vector2f((float)oknoPodskazok->getSize().x, (float)oknoPodskazok->getSize().y));
    fon.setFillColor(Color(255, 255, 200));
    oknoPodskazok->draw(fon);

    float tekY = 20;
    for (size_t idx = 0; idx < tekstPodskazok.size(); idx++)
    {
        Text stroka;
        stroka.setFont(shrift);
        stroka.setCharacterSize(18);
        stroka.setFillColor(Color::Black);
        stroka.setString(tekstPodskazok[idx]);

        FloatRect gran = stroka.getLocalBounds();
        stroka.setPosition(((float)oknoPodskazok->getSize().x - gran.width) / 2.0f, tekY);
        oknoPodskazok->draw(stroka);
        tekY = tekY + gran.height + 10;
    }

    if (oknoPodskazok != nullptr && oknoPodskazok->isOpen() == true)
    {
        float xKn = ((float)oknoPodskazok->getSize().x - 100) / 2.0f;
        float yKn = (float)oknoPodskazok->getSize().y - 50;
        knopkaZakrit.setPosition(xKn, yKn);
        tekstKnopkiZakrit.setPosition(xKn + 50, yKn + 15);

        oknoPodskazok->draw(knopkaZakrit);
        oknoPodskazok->draw(tekstKnopkiZakrit);
    }

    oknoPodskazok->display();
}

void Play::pokazatPodskazku()
{
    // Сначала ищем выключенные переключатели не в последней строке
    for (int i = 0; i < razmer - 1; i++)
    {
        for (int j = 0; j < razmer; j++)
        {
            if (pole[i][j] == '-')
            {
                if (oknoPodskazok != nullptr)
                {
                    oknoPodskazok->close();
                    delete oknoPodskazok;
                    oknoPodskazok = nullptr;
                }

                oknoPodskazok = new RenderWindow(VideoMode(450, 220), "Подсказка");
                oknoPodskazokOtkrito = true;

                tekstPodskazok.clear();
                tekstPodskazok.push_back(L"ПОДСКАЗКА:");
                tekstPodskazok.push_back(L"");
                tekstPodskazok.push_back(L"Нажмите на строку: " + to_wstring(i + 2));
                tekstPodskazok.push_back(L"Нажмите на столбец: " + to_wstring(j + 1));
                tekstPodskazok.push_back(L"");
                obnovitOknoPodskazok();
                return;
            }
        }
    }

    // Смотрим последнюю строку
    string poslStroka = poslednyayaStroka();
    cout << "Последняя строка поля " << razmer << "x" << razmer << ": [" << poslStroka << "]" << endl;

    // Ищем подсказку в загруженных
    tekushiePodskazki.clear();
    for (size_t idx = 0; idx < vsePodskazki.size(); idx++)
    {
        if (vsePodskazki[idx].poslednyayaStroka.length() == razmer &&
            vsePodskazki[idx].poslednyayaStroka == poslStroka)
        {
            tekushiePodskazki = vsePodskazki[idx].hody;
            cout << "Нашел паттерн! Подсказок: " << tekushiePodskazki.size() << endl;
            break;
        }
    }

    if (tekushiePodskazki.empty() == false)
    {
        if (oknoPodskazok != nullptr)
        {
            oknoPodskazok->close();
            delete oknoPodskazok;
            oknoPodskazok = nullptr;
        }

        int visOkna = 150 + (int)tekushiePodskazki.size() * 25;
        if (visOkna > 600)
        {
            visOkna = 600;
        }

        oknoPodskazok = new RenderWindow(VideoMode(500, visOkna), "Подсказки");
        oknoPodskazokOtkrito = true;

        tekstPodskazok.clear();
        tekstPodskazok.push_back(L"ПОДСКАЗКИ ДЛЯ ЭТОЙ КОМБИНАЦИИ:");
        tekstPodskazok.push_back(L"");

        for (size_t i = 0; i < tekushiePodskazki.size(); i++)
        {
            wstringstream ss;
            ss << (i + 1) << L") Строка: " << tekushiePodskazki[i].first
                << L", Столбец: " << tekushiePodskazki[i].second;
            tekstPodskazok.push_back(ss.str());
        }
        obnovitOknoPodskazok();
        return;
    }

    // Если нет готовых подсказок, смотрим последнюю строку
    for (int j = 0; j < razmer; j++)
    {
        if (pole[razmer - 1][j] == '-')
        {
            if (oknoPodskazok != nullptr)
            {
                oknoPodskazok->close();
                delete oknoPodskazok;
                oknoPodskazok = nullptr;
            }

            oknoPodskazok = new RenderWindow(VideoMode(450, 220), "Подсказка");
            oknoPodskazokOtkrito = true;

            tekstPodskazok.clear();
            tekstPodskazok.push_back(L"ПОДСКАЗКА:");
            tekstPodskazok.push_back(L"");
            tekstPodskazok.push_back(L"Нажмите на строку: " + to_wstring(razmer));
            tekstPodskazok.push_back(L"Нажмите на столбец: " + to_wstring(j + 1));
            tekstPodskazok.push_back(L"");
            obnovitOknoPodskazok();
            return;
        }
    }

    // Если всё включено - победа!
    if (oknoPodskazok != nullptr)
    {
        oknoPodskazok->close();
        delete oknoPodskazok;
        oknoPodskazok = nullptr;
    }

    oknoPodskazok = new RenderWindow(VideoMode(400, 200), "Победа!");
    oknoPodskazokOtkrito = true;

    tekstPodskazok.clear();
    tekstPodskazok.push_back(L"ПОЗДРАВЛЯЕМ!");
    tekstPodskazok.push_back(L"");
    tekstPodskazok.push_back(L"Все переключатели уже включены!");
    tekstPodskazok.push_back(L"Вы решили головоломку!");
    obnovitOknoPodskazok();
}

void Play::obrabotatOknoPodskazok()
{
    if (oknoPodskazokOtkrito == false || oknoPodskazok == nullptr)
    {
        return;
    }

    Event sobitie;
    while (oknoPodskazok->pollEvent(sobitie))
    {
        if (sobitie.type == Event::Closed)
        {
            oknoPodskazok->close();
            delete oknoPodskazok;
            oknoPodskazok = nullptr;
            oknoPodskazokOtkrito = false;
            return;
        }
        if (sobitie.type == Event::MouseButtonPressed)
        {
            if (oknoPodskazok == nullptr)
            {
                return;
            }

            Vector2i m = Mouse::getPosition(*oknoPodskazok);
            float mx = (float)m.x;
            float my = (float)m.y;

            if (knopkaZakrit.getGlobalBounds().contains(mx, my))
            {
                oknoPodskazok->close();
                delete oknoPodskazok;
                oknoPodskazok = nullptr;
                oknoPodskazokOtkrito = false;
                return;
            }
        }
    }

    if (oknoPodskazokOtkrito == true && oknoPodskazok != nullptr)
    {
        obnovitOknoPodskazok();
    }
}

void Play::obnovit()
{
    if (kartinkiZagruzheny == false)
    {
        return;
    }

    for (int i = 0; i < razmer; i++)
    {
        for (int j = 0; j < razmer; j++)
        {
            if (pole[i][j] == '|')
            {
                yacheyki[i][j].setTexture(teksturaVkl);
            }
            else
            {
                yacheyki[i][j].setTexture(teksturaVikl);
            }
        }
    }

    if (igraZakonchena == false && proveritPobedu() == true)
    {
        igraZakonchena = true;
        pokazatOknoPobedy();
    }
}

void Play::obrabotatSobytie(Event sobytie)
{
    obrabotatOknoPodskazok();

    if (igraZakonchena == true)
    {
        return;
    }

    if (sobytie.type == Event::MouseButtonPressed)
    {
        Vector2i m = Mouse::getPosition(okno);
        float mx = (float)m.x;
        float my = (float)m.y;

        // Кнопка "Меню"
        if (knopkaNazad.getGlobalBounds().contains(mx, my))
        {
            igraZakonchena = true;
            return;
        }

        // Кнопка "Подсказка"
        if (knopkaPodskazka.getGlobalBounds().contains(mx, my))
        {
            pokazatPodskazku();
            return;
        }

        // Клик по игровому полю
        for (int i = 0; i < razmer; i++)
        {
            for (int j = 0; j < razmer; j++)
            {
                if (yacheyki[i][j].getGlobalBounds().contains(mx, my))
                {
                    perekluchit(i, j);
                    obnovit();
                    return;
                }
            }
        }
    }
}

void Play::risovat()
{
    // Рисуем фон для поля
    RectangleShape fon;
    float shirFona = razmer * razmerYacheyki + (razmer - 1) * otstup;
    float visFona = razmer * razmerYacheyki + (razmer - 1) * otstup;
    fon.setSize(Vector2f(shirFona, visFona));
    fon.setPosition(smX - 2, smY - 2);
    fon.setFillColor(Color::Black);
    okno.draw(fon);

    // Рисуем все ячейки
    for (int i = 0; i < razmer; i++)
    {
        for (int j = 0; j < razmer; j++)
        {
            okno.draw(yacheyki[i][j]);
        }
    }

    // Рисуем кнопки
    okno.draw(knopkaNazad);
    okno.draw(tekstKnopkiNazad);
    okno.draw(knopkaPodskazka);
    okno.draw(tekstKnopkiPodskazka);
}

bool Play::zakonchena()
{
    return igraZakonchena;
}