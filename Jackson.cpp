#include <iostream>
#include <fstream>
#include <algorithm>

//ri - termin, pi - czas wykonania zadania, Si - moment rozpoczecia, Ci - moment zakonczenia.
//Si >= ri , Si >=C(i-1) ===> Si=max{ri,C(i-1)}
//Ci=Si+pi
//Ci=max(ri,(Ci-1))+pi
#define WIELKOSC 1000

int main() {

	std::fstream plik;

	plik.open("JACK7.DAT");

	if (plik.good())
	{
		int ilosc;
		plik >> ilosc;
		ilosc = ilosc + 1;
		int r[WIELKOSC];
		int p[WIELKOSC];

		for (int i = 1; i < ilosc; i++)
		{
			plik >> r[i];
			plik >> p[i];
		}


		for (int i = 1; i < ilosc; i++)
		{
			std::cout << r[i] << " " << p[i] << std::endl;
		}
		std::cout<<std::endl;

		for (int i = 1; i < ilosc; i++)
		{
			for (int j = 2; j < ilosc; j++)
			{
				if (r[j - 1] > r[j])
				{
					std::swap(r[j], r[j - 1]);
					std::swap(p[j], p[j - 1]);
				}
			}
		}

		int C[WIELKOSC];
		C[0] = 0;
		for (int i = 1; i < ilosc; i++)
		{
			C[i] = std::max(r[i], C[i - 1]) + p[i];
		}

		for (int i = 1; i < ilosc; i++)
		{
			std::cout << C[i] << std::endl;
		}


	}
	else std::cout << "Nie ma takiego pliku!" << std::endl;
	plik.close();
	return 0;
}






