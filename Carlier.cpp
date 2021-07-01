#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <functional>
#include <queue>
#include <vector>

#define INF 1000000

using namespace std;

class Zadanie {
public:
	int r = 0; //termin dostepnosci
	int p = 0; //czas wykonania
	int q = 0; //czas dostawy
	int C = 0; //moment zakoñczenia zadania

	Zadanie(int R, int P, int Q) { r = R, p = P, q = Q; };
	Zadanie() { r = 0; p = 0; q = 0; };
};

int Schrage(vector<Zadanie>& pi) {

	auto sort_q = [](const Zadanie& right, const Zadanie& left) {

		return left.q > right.q;
	};

	auto sort_r = [](const Zadanie& right, const Zadanie& left) {

		return left.r < right.r;
	};
	priority_queue<Zadanie, vector<Zadanie>, decltype(sort_r)> N(sort_r);
	priority_queue<Zadanie, vector<Zadanie>, decltype(sort_q)> G(sort_q);

	for (int i = 0; i < pi.size(); i++) {

		N.push(pi[i]);
	}

	pi.clear();

	int t = 0, Cmax = 0;
	int k = 0;

	while ((!G.empty()) || (!N.empty())) {

	flaga:
		while ((!N.empty()) && (N.top().r <= t)) {

			G.push(N.top());
			N.pop();
		}

		if (G.empty()) {

			t = N.top().r;
			goto flaga;
		}
		int tempp = G.top().p;
		int tempq = G.top().q;
		int tempr = G.top().r;
		pi.push_back(G.top());
		G.pop();
		t = t + tempp;
		pi[k].C = t;
		k++;
		Cmax = max(Cmax, (t + tempq));
	}
	return Cmax;
}

int Preschrage(vector<Zadanie>& pi) {

	auto sort_q = [](const Zadanie& right, const Zadanie& left) {

		return left.q > right.q;
	};

	auto sort_r = [](const Zadanie& right, const Zadanie& left) {

		return left.r < right.r;
	};

	int t = 0, Cmax = 0;
	int r = 0, p = 0, q = INF;
	Zadanie Potencjalne(r, p, q);

	priority_queue<Zadanie, vector<Zadanie>, decltype(sort_q)> G(sort_q);
	priority_queue<Zadanie, vector<Zadanie>, decltype(sort_r)> N(sort_r);

	for (int i = 0; i < pi.size(); i++) {

		N.push(pi[i]);
	}

	while (!(G.empty()) || !(N.empty())) {

	flaga:
		while (!(N.empty()) && (N.top().r <= t)) {

			int tempr, tempp, tempq;
			tempr = N.top().r;
			tempp = N.top().p;
			tempq = N.top().q;
			Zadanie pom(tempr, tempp, tempq);
			N.pop();
			G.push(pom);

			if (tempq > Potencjalne.q) {

				Potencjalne.p = t - tempr;
				t = tempr;

				if (Potencjalne.p > 0) {
					Zadanie pom1(tempr, Potencjalne.p, Potencjalne.q);
					G.push(pom1);
				}
			}
		}

		if (G.empty()) {

			t = N.top().r;
			goto flaga;
		}
		int tempp = G.top().p;
		int tempq = G.top().q;
		int tempr = G.top().r;
		G.pop();
		Potencjalne.r = tempr;
		Potencjalne.p = tempp;
		Potencjalne.q = tempq;
		t = t + tempp;
		Cmax = max(Cmax, (t + tempq));
	}
	return Cmax;
}

void Carlier(vector<Zadanie>& pi, int UB, vector<Zadanie>& pistar) {

	int output;
	int sum = 0, LB = 0;
	int U = Schrage(pi);

	cout << "WARTOSC U (Cmax SCHRAGE):" << U << endl;

	if (U < UB) {

		UB = U;
		pistar = pi;
	}


	int a = 0, b = 0;

	for (int i = pi.size() - 1; i >= 0; i--) {

		if (U == (pi[i].q + pi[i].C)) {

			b = i;
			break;
		}
	}

	cout << "PARAMETR B = " << b << endl;

	for (int i = 0; i < pi.size(); i++) {

		sum = 0;
		for (int j = i; j <= b; j++) {

			sum += pi[j].p;
		}
		if (U == (pi[i].r + sum + pi[b].q)) {

			a = i;
			break;
		}
	}

	cout << "PARAMETR A = " << a << endl;

	int c = -1;
	for (int i = b; i >= a; i--) {

		if (pi[i].q < pi[b].q) {

			c = i;
			break;
		}
	}
	cout << "PARAMETR C = " << c << endl;

	cout << "UB = " << UB << endl;

	//je¿eli c = -1 to otrzymane rozwiazanie jest optymalne.

	if (c == -1) {

		
		cout << "OPTYMALNA PERMUTACJA PI:" << endl;
		for (int i = 0; i < pistar.size(); i++) {
			cout << i << ". " << "r:" << pistar[i].r << " p:" << pistar[i].p << " q:" << pistar[i].q << endl;
		}
		
		cout << "OSTATECZNY WYNIK: " << UB << endl;
		exit(0);
	}

	int rprim = INF;
	int qprim = INF;
	int pprim = 0;

	for (int i = c + 1; i <= b; i++) {

		if (pi[i].r < rprim) {

			rprim = pi[i].r;
		}
	}

	for (int i = c + 1; i <= b; i++) {

		if (pi[i].q < qprim) {

			qprim = pi[i].q;
		}
	}

	for (int i = c + 1; i <= b; i++) {

		pprim += pi[i].p;
	}

	int pom = pi[c].r;
	pi[c].r = max(pi[c].r, (rprim + pprim));

	LB = Preschrage(pi);

	if (LB < UB) {

		Carlier(pi, UB, pistar);
	}

	pi[c].r = pom;
	int pom1 = pi[c].q;
	pi[c].q = max(pi[c].q, (qprim + pprim));



	LB = Preschrage(pi);

	if (LB < UB) {

		Carlier(pi, UB, pistar);
	}

	pi[c].q = pom1;
}

int main() {

	Zadanie zad;
	fstream plik;
	vector<Zadanie> pi;
	vector<Zadanie> pistar;

	plik.open("SCHRAGE7.DAT");

	int r, p, q;
	int Cmax;
	int ilosc;

	if (plik.good()) {

		plik >> ilosc;
		cout << "Ilosc zadan:" << ilosc << endl;

		for (int i = 0; i < ilosc; i++) {

			plik >> r;
			plik >> p;
			plik >> q;

			zad.r = r;
			zad.p = p;
			zad.q = q;

			pi.push_back(zad);
		}

		int UB = INF;
		Carlier(pi, UB, pistar);
	}
}
