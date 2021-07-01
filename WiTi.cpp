#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <functional>
#include <vector>


class Zadanie {

public:

	int P = 0; //czas realizacji zadania
	int W = 0; //naliczana kara za spóŸnienie
	int D = 0; //¿¹dany termin zakoñczenia zadania

	Zadanie() { P = 0, W = 0, D = 0; };

	int wyznacz_kare(Zadanie zad, int Ci);

	int WiTi(std::vector<Zadanie>& pi);
};

int Zadanie::wyznacz_kare(Zadanie zad, int Ci) {

	int pom = (Ci - zad.D) * zad.W;

	if (pom > 0) { return pom; }
	else { return 0; }
}

int Zadanie::WiTi(std::vector<Zadanie>& pi) {

	unsigned long Z = 1; 
	int ilosc_podzbiorow_z = (Z << pi.size()); 

	int* tablica_kar = new int[ilosc_podzbiorow_z]; 
	tablica_kar[0] = 0; 

	for (int i = 1; i < ilosc_podzbiorow_z; i++) { 

		int Ci = 0;
		tablica_kar[i] = std::numeric_limits<int>::max();
		for (int j = 0; j < pi.size(); j++) { 

			if (i & (1 << j)) { Ci += pi[j].P; } 
		}

		for (int j = 0; j < pi.size(); j++) {

			if (i & (1 << j)) {	

				int OPTz = tablica_kar[i & (~(1 << j))] + wyznacz_kare(pi[j], Ci);

				if (tablica_kar[i] > OPTz) { tablica_kar[i] = OPTz; }
			}
		}
	}

	return tablica_kar[ilosc_podzbiorow_z - 1];
}

int main() {

	std::fstream plik;
	Zadanie zad;
	std::vector<Zadanie> pi;

	plik.open("WITI8.txt");
	
	int P, W, D, n;

	if (plik.good()) {

		plik >> n;
		std::cout << "Ilosc zadan: " << n << std::endl;

		for (int i = 0; i < n; i++) {

			plik >> P;
			plik >> W;
			plik >> D;

			zad.P = P;
			zad.W = W;
			zad.D = D;

			pi.push_back(zad);
		}

		std::cout << "Najmniejsza mozliwa kara: " << zad.WiTi(pi) << std::endl;
	}
	else {

		std::cout << "Niepoprawny plik" << std::endl;
	}
}

