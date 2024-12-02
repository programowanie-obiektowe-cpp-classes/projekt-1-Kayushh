#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <cmath>

class Pracownik {
public:
    virtual ~Pracownik() = default;
    virtual void wyswietlInformacje() const = 0;
    virtual int pobierzWynagrodzenie() const = 0;
};

class Inzynier : public Pracownik {
    std::string imie;
    int wynagrodzenie;
    std::string wydzial;

public:
    static constexpr int CI = 200;

    Inzynier(const std::string& imie, int wynagrodzenie, const std::string& wydzial)
        : imie(imie), wynagrodzenie(wynagrodzenie), wydzial(wydzial) {}

    void wyswietlInformacje() const override {
        std::cout << "Inzynier: " << imie << " wynagrodzenie: " << wynagrodzenie
                  << ", wydzial: " << wydzial << "\n";
    }

    int pobierzWynagrodzenie() const override {
        return wynagrodzenie;
    }
};

class Magazynier : public Pracownik {
    std::string imie;
    int wynagrodzenie;
    bool obslugaWozka;

public:
    static constexpr int CMag = 60;

    Magazynier(const std::string& imie, int wynagrodzenie, bool obslugaWozka)
        : imie(imie), wynagrodzenie(wynagrodzenie), obslugaWozka(obslugaWozka) {}

    void wyswietlInformacje() const override {
        std::cout << "Magazynier: " << imie << ", wynagrodzenie: " << wynagrodzenie
                  << ", obsluga wozka: " << (obslugaWozka ? "Tak" : "Nie") << "\n";
    }

    int pobierzWynagrodzenie() const override {
        return wynagrodzenie;
    }
};

class Marketer : public Pracownik {
    std::string imie;
    int wynagrodzenie;
    int obserwujacy;

public:
    static constexpr int CMkt = 30;

    Marketer(const std::string& imie, int wynagrodzenie, int obserwujacy)
        : imie(imie), wynagrodzenie(wynagrodzenie), obserwujacy(obserwujacy) {}

    void wyswietlInformacje() const override {
        std::cout << "Marketer: " << imie << " wynagrodzenie: " << wynagrodzenie
                  << ", obserwujacy: " << obserwujacy << "\n";
    }

    int pobierzWynagrodzenie() const override {
        return wynagrodzenie;
    }
};

class Robotnik : public Pracownik {
    std::string imie;
    int wynagrodzenie;
    double rozmiarButa;

public:
    static constexpr int CR = 20;

    Robotnik(const std::string& imie, int wynagrodzenie, double rozmiarButa)
        : imie(imie), wynagrodzenie(wynagrodzenie), rozmiarButa(rozmiarButa) {}

    void wyswietlInformacje() const override {
        std::cout << "Robotnik: " << imie << ", wynagrodzenie: " << wynagrodzenie
                  << ", rozmiar buta: " << rozmiarButa << "\n";
    }

    int pobierzWynagrodzenie() const override {
        return wynagrodzenie;
    }
};

class Kredyt {
    int kwota;
    int pozostalyCzas;
    double rata;

public:
    Kredyt(int kwota, int czas, double oprocentowanie)
        : kwota(kwota), pozostalyCzas(czas) {
        rata = kwota * (1.0 + oprocentowanie) / czas;
    }

    double pobierzRate() const {
        return rata;
    }

    bool splacMiesiac() {
        if (pozostalyCzas > 0) {
            pozostalyCzas--;
            return pozostalyCzas == 0;
        }
        return false;
    }

    int pobierzKwote() const {
        return kwota;
    }
};

class Firma {
    int stanKonta;
    int pojemnoscMagazynu;
    int produktyNaStanie;
    int wartoscDocelowa;
    double sumaPrzychodow;
    int liczbaMiesiecyPrzychodu;
    std::vector<std::unique_ptr<Pracownik>> pracownicy;
    std::vector<Kredyt> kredyty;
    static constexpr int N = 12; 
    static constexpr int X = 12; 
    static constexpr int M = 5;  

public:
    Firma(int stanPoczatkowy, int wartoscDocelowa)
        : stanKonta(stanPoczatkowy), pojemnoscMagazynu(50), produktyNaStanie(0),
          wartoscDocelowa(wartoscDocelowa), sumaPrzychodow(0), liczbaMiesiecyPrzychodu(0) {}

    void zatrudnijPracownika(std::unique_ptr<Pracownik> pracownik) {
        pracownicy.push_back(std::move(pracownik));
    }

    void wezKredyt(int kwota, int czas) {
        if (czas > X) {
            std::cout << "Nie mozna wziac kreddytu na wiecej niz " << X << " miesiecy.\n";
            return;
        }
        double wartoscFirmy = sumaPrzychodow / std::max(1, liczbaMiesiecyPrzychodu);
        double maksymalneZadluzenie = wartoscFirmy * M;
        int sumaZadluzenia = 0;
        for (const auto& kredyt : kredyty) {
            sumaZadluzenia += kredyt.pobierzKwote();
        }
        if (sumaZadluzenia + kwota > maksymalneZadluzenie) {
            std::cout << "Za duza kwota\n";
            return;
        }
        kredyty.emplace_back(kwota, czas, czas * 0.02);
        stanKonta += kwota;
    }

    void przeprowadzMiesiac() {
        int popyt = liczMarketerow() * Marketer::CMkt;
        int produkcja = liczRobotnikow() * Robotnik::CR;
        produktyNaStanie += std::min(produkcja, pojemnoscMagazynu);

        int sprzedaz = std::min({popyt, produktyNaStanie, pojemnoscMagazynu});
        produktyNaStanie -= sprzedaz;
        int przychod = sprzedaz * liczInzynierow() * Inzynier::CI;
        sumaPrzychodow += przychod;
        liczbaMiesiecyPrzychodu = std::min(N, liczbaMiesiecyPrzychodu + 1);

        int wynagrodzenia = liczKosztyPracownikow();
        double raty = obliczRatyKredytowe();
        stanKonta += przychod - wynagrodzenia - raty;

        kredyty.erase(std::remove_if(kredyty.begin(), kredyty.end(), [](Kredyt& k) { return k.splacMiesiac(); }), kredyty.end());

        double sredniPrzychod = sumaPrzychodow / liczbaMiesiecyPrzychodu;

        std::cout << "Podsumowanie miesiaca:\n";
        std::cout << "Stan konta: " << stanKonta << "\n";
        std::cout << "Produkty na stanie: " << produktyNaStanie << "\n";
        std::cout << "Przychod: " << przychod << ", Koszty: " << wynagrodzenia
                  << ", Raty kredytowe: " << raty << "\n";
        std::cout << "Wartosc spółki: " << sredniPrzychod << "\n";

        if (stanKonta < 0) {
            std::cout << "Bankructwoo\n";
            exit(0);
        }

        if (sredniPrzychod >= wartoscDocelowa) {
            std::cout << "Osiagnieto wartosc docelowa spolki.\n";
            exit(0);
        }
    }

    void wyswietlPracownikow() const {
        if (pracownicy.empty()) {
            std::cout << "Nie masz żadnych pracownikow.\n";
        } else {
            for (const auto& pracownik : pracownicy) {
                pracownik->wyswietlInformacje();
            }
        }
    }

private:
    double obliczRatyKredytowe() const {
        double suma = 0.0;
        for (const auto& kredyt : kredyty) {
            suma += kredyt.pobierzRate();
        }
        return suma;
    }

    int liczMarketerow() const {
        return liczPracownikow<Marketer>();
    }

    int liczRobotnikow() const {
        return liczPracownikow<Robotnik>();
    }

    int liczInzynierow() const {
        return liczPracownikow<Inzynier>();
    }

    template <typename T>
    int liczPracownikow() const {
        return std::count_if(pracownicy.begin(), pracownicy.end(), [](const auto& pracownik) {
            return dynamic_cast<T*>(pracownik.get()) != nullptr;
        });
    }

    int liczKosztyPracownikow() const {
        int suma = 0;
        for (const auto& pracownik : pracownicy) {
            suma += pracownik->pobierzWynagrodzenie();
        }
        return suma;
    }
};



std::string generujImie() {
    static std::vector<std::string> imiona = {"Maciej", "Bartosz", "Ptolemeusz", "Mario", "Jacob"};
    static int indeks = 0;
    return imiona[indeks++ % imiona.size()];
}

int main() {
    Firma firma(50000, 100000);

    while (true) {
        std::cout << "\nDostepne akcje:\n"
                  << "1. Wyswietl pracownikow\n"
                  << "2. Zatrudnij inzyniera\n"
                  << "3. Zatrudnij magazyniera\n"
                  << "4. Zatrudnij marketera\n"
                  << "5. Zatrudnij robotnika\n"
                  << "6. Wez kredyt\n"
                  << "7. Przeprowadz miesiac\n"
                  << "8. Wyjdz\n";
        int wybor;
        std::cin >> wybor;

        switch (wybor) {
            case 1:
                firma.wyswietlPracownikow();
                break;
            case 2:
                firma.zatrudnijPracownika(std::make_unique<Inzynier>(generujImie(), 4000, "Lotnictwo"));
                break;
            case 3:
                firma.zatrudnijPracownika(std::make_unique<Magazynier>(generujImie(), 2000, true));
                break;
            case 4:
                firma.zatrudnijPracownika(std::make_unique<Marketer>(generujImie(), 3000, 100000));
                break;
            case 5:
                firma.zatrudnijPracownika(std::make_unique<Robotnik>(generujImie(), 3000, 44.5));
                break;
            case 6: {
                int kwota, czas;
                std::cout << "Podaj kwote kredytu: ";
                std::cin >> kwota;
                std::cout << "Podaj czas splaty (miesiace): ";
                std::cin >> czas;
                firma.wezKredyt(kwota, czas);
                break;
            }
            case 7:
                firma.przeprowadzMiesiac();
                break;
            case 8:
                std::cout << "Koniec gry\n";
                return 0;
            default:
                std::cout << "Error\n";
        }
    }
}
