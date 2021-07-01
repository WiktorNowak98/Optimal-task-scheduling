#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <cstdlib>

#define WIELKOSC 1000
#define INF 1000

using namespace std;

struct Lista
{
	Lista *next; // Struktura wczytanych danych
	int r, p, q;
};

class Kolejka // Klasa Kolejki Priotytetowej
{
private:
	Lista *glowa;
	Lista *ogon;

public:
	Kolejka();
	~Kolejka(); 

	bool pusty();
	void push(int,int,int);
	void push_sort_q(int, int, int);
	void pop();
	void wyswietl(Kolejka);
	int topr(Kolejka);
	int topp(Kolejka);
	int topq(Kolejka);
};

Kolejka::Kolejka() // Kontruktor Kolejki
{
	glowa = ogon = NULL;
}

Kolejka::~Kolejka() // Destruktor Kolejki
{
	while (glowa)
		pop();
}


bool Kolejka::pusty() 
{
	return !glowa;
}

void Kolejka::push(int r, int p, int q) 
{
	Lista *s, *t;
	s = new Lista;
	s->next = NULL;
	s->r = r;
	s->p = p;
	s->q = q;

	if (!glowa) 
		glowa = ogon = s;
	else if (glowa->r > r) 
	{
		s->next = glowa; 
		glowa = s; 
	}
	else
	{
		t = glowa;
		while ((t->next) && (t->next->r <= r))  
			t = t->next; 
		s->next = t->next; 
		t->next = s; 
		if (!s->next) 
			ogon = s;
	}
}

void Kolejka::push_sort_q(int r, int p, int q)
{
	Lista *s, *t;
	s = new Lista;
	s->next = NULL;
	s->r = r;
	s->p = p;
	s->q = q;

	if (!glowa)
		glowa = ogon = s;
	else if (glowa->q < q)
	{
		s->next = glowa;
		glowa = s;
	}
	else
	{
		t = glowa;
		while ((t->next) && (t->next->q >= q))
			t = t->next;
		s->next = t->next;
		t->next = s;
		if (!s->next)
			ogon = s;
	}
}


void Kolejka::pop() 
{
	if (glowa)
	{
		Lista * p = glowa;
		glowa = glowa->next;
		if (!glowa) 
			ogon = NULL;
	}
}

void Kolejka::wyswietl(Kolejka Q) 
{
	cout << Q.glowa->r << " " << Q.glowa->p << " " << Q.glowa->q << endl;
	
}

int Kolejka::topr(Kolejka Q) 
{
	if (glowa)
		return glowa->r;
}

int Kolejka::topp(Kolejka Q) 
{
	if (glowa)
		return glowa->p;
}

int Kolejka::topq(Kolejka Q) 
{
	if (glowa)
		return glowa->q;
}

int main()
{
	fstream plik;

	plik.open("SCHRAGE9.DAT");

	Kolejka N,G;
	int r, p, q;

	if (plik.good())
	{
		int ilosc;
		plik >> ilosc;
		ilosc = ilosc + 1;

		for (int i = 1; i < ilosc; i++)
		{
			plik >> r;
			plik >> p;
			plik >> q;

			N.push(r, p, q); 
		}
			
		int t = 0, k = 0, Cmax = 0;
	
		Lista Potencjalne;
		Potencjalne.r = 0;
		Potencjalne.p = 0; 
		Potencjalne.q = INF; 

		while (!(G.pusty()) || !(N.pusty()))
		{
			flaga:
			while (!(N.pusty()) && (N.topr(N) <= t))
			{
				int tempr, tempp, tempq;
				tempr = N.topr(N);
				tempp = N.topp(N); 
				tempq = N.topq(N); 
				N.pop();
				G.push_sort_q(tempr, tempp, tempq);

				if (tempq > Potencjalne.q)
				{
					Potencjalne.p = t - tempr;
					t = tempr;
					if (Potencjalne.p > 0)
					{
						G.push_sort_q(tempr, Potencjalne.p, Potencjalne.q);
					}
				}
			}
				if (G.pusty())
				{
					t = N.topr(N);
					goto flaga;
				}
			
			int tempp = G.topp(G);
			int tempq = G.topq(G); 
			int tempr = G.topr(G);
			G.pop();
			Potencjalne.r = tempr;
			Potencjalne.p = tempp; 
			Potencjalne.q = tempq;
		
			t = t + tempp;
			Cmax = max(Cmax, (t + tempq));
			
		}

		cout << Cmax << endl;
		
	}
	else std::cout << "Nie ma takiego pliku!" << std::endl;
	plik.close();

	system("pause");
	return 0;
}