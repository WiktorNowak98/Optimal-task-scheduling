#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <functional>
#include <queue>
#include <vector>

class Operacja {

public:
	int m; //numer maszyny
	int p; //czas wykonania zadania
	int Cn; //moment zakoñczenia zadania na maszynie
	int Sn; //moment rozpoczecia zadania na maszynie

	Operacja() { m = 0; p = 0; Cn = 0; Sn = 0; };
	Operacja(int maszyna, int czas) { m = maszyna; p = czas; Cn = 0; Sn = 0; };
};

class Zadanie {

public:
	std::vector<Operacja> op; //wektor operacji
	int Sop; //suma czasów wykonania operacji

	Zadanie() { Sop = 0; };
};

void Policz_Cmn_Smn(std::vector<Zadanie>& pi) {

	for (int i = 0; i < pi[0].op.size(); i++) { 
		for (int j = 0; j < pi.size(); j++) { 

			if (i == 0 && j == 0) { 

				pi[j].op[i].Sn = 0;	
				pi[j].op[i].Cn = pi[i].op[j].p; 
			}
			else {

				if (j == 0) {	

					pi[j].op[i].Sn = std::max(0, pi[j].op[i - 1].Cn); 
					pi[j].op[i].Cn = pi[j].op[i].Sn + pi[j].op[i].p;	
				}
				else if (i == 0) {	

					pi[j].op[i].Sn = std::max(pi[j - 1].op[i].Cn, 0); 
					pi[j].op[i].Cn = pi[j].op[i].Sn + pi[j].op[i].p;	
				}
				else {	

					pi[j].op[i].Sn = std::max(pi[j - 1].op[i].Cn, pi[j].op[i - 1].Cn); 
					pi[j].op[i].Cn = pi[j].op[i].Sn + pi[j].op[i].p;	
				}
			}
		}
	}
}

 void Policz_Sop(std::vector<Zadanie>& pi) {

	for (int i = 0; i < pi.size(); i++) {
		
		int sumap = 0;
		for (int j = 0; j < pi[0].op.size(); j++) {

			sumap += pi[i].op[j].p; 
		}
		pi[i].Sop = sumap;
	}
}

int Policz_Cmax(std::vector<Zadanie>& pi) {

	Policz_Cmn_Smn(pi);
	return pi.back().op.back().Cn;
}

std::vector<Zadanie> NEH(std::vector<Zadanie>& N) {

	auto op_sort_p = [](const Zadanie& lewy, const Zadanie& prawy) {

		return lewy.Sop < prawy.Sop;
	};

	std::priority_queue<Zadanie, std::vector<Zadanie>, decltype(op_sort_p)> W(op_sort_p);  
	std::vector<Zadanie> pi; 
	Policz_Sop(N);

	for (int i = 0; i < N.size(); i++) {

		W.push(N[i]);	
	}
	int k = 0;
	while (!W.empty()) { 

		if (k == 0) { 

			pi.push_back(W.top());	
			W.pop();
			k++;
		}
		else { 

			auto ref_pi = pi; 
			std::vector<int> pom_cmax; 
			for (int i = 0; i <= k; i++) { 

				pi.insert(pi.begin() + i, W.top()); 
				pom_cmax.push_back(Policz_Cmax(pi)); 
				pi = ref_pi; 
			}

			auto min_index = std::min_element(pom_cmax.begin(), pom_cmax.end()); 
			int pom = std::distance(pom_cmax.begin(), min_index); 
			pi.insert(pi.begin() + pom, W.top()); 
			W.pop(); 
			k++;
		}	
	} 
	return pi;
}

int main() {

	std::fstream plik;
	std::vector<Zadanie> pi;

	plik.open("NEH5.DAT");

	int ilosc_zadan, ilosc_maszyn, czas;

	if (plik.good()) {

		plik >> ilosc_zadan;
		plik >> ilosc_maszyn;

		for (int i = 0; i < ilosc_zadan; i++) {

			pi.push_back(Zadanie());

			for (int j = 0; j < ilosc_maszyn; j++) {

				plik >> czas;
				pi[i].op.push_back(Operacja(j, czas));
			}
		}

		std::vector<Zadanie> wynik = NEH(pi);
		std::cout << Policz_Cmax(wynik) << std::endl;
	}
	else { std::cout << "Plik do wymiany" << std::endl; }

	return 0;
}