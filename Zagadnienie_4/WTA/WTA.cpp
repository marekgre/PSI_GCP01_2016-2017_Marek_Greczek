#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <vector>
#include <fstream>
#include <chrono> // pomiar czasu
#include "Functions.h"
using namespace std::chrono;
using namespace std;

void WTA::wypelnijWagi()
{
	srand(time(NULL));
	for (int i = 1; i < numberOfLayers; i++) // warstwa
	{
		for (int j = 0; j < n[i]; j++) // liczba neuronów w warstwie
		{
			for (int k = 0; k < n[i - 1]; k++) // ka¿dy neuron ma tyle wag, ile neuronów mia³a poprzednia warstwa
			{
				W[i][j][k] = (((rand() % 1000000L) / 1700.0) - 9.8)*0.0015;
				W1[i][j][k] = W[i][j][k];
				if (W[i][j][k] == 0.0)
				{
					W[i][j][k] = 0.01492;
					W1[i][j][k] = 0.01492;
				}
			}
		}
	}
}

void WTA::wprowadzWejsciaUczace()
{
	int liczba = rand() % 3;

	for (int i = 0; i < numberOfNeurons; i++)
		O[0][i] = WZOR[0][i][liczba]; // pomnó¿my przez wagê od razu
}

void WTA::wprowadzWejsciaTestowe()
{
	int liczba = rand() % 3;
	int kategoria = rand() % 2;

	for (int i = 0; i < numberOfNeurons; i++)
		O[0][i] = TEST[kategoria][i][liczba]; // pomnó¿my przez wagê od razu
}

void WTA::Epoki()
{
	ofstream WYJSCIAtxt;
	WYJSCIAtxt.open("WYJSCIA.txt", std::ios::app);
	WYJSCIAtxt.precision(4); // wyniki ERMS

	milliseconds start = duration_cast< milliseconds >( // pocz¹tek pomiaru czasu
		system_clock::now().time_since_epoch());

	cout << "Liczba epok to " << uczaceIteracje << ". Ponizej sa wyswietlone ostatnie wyjscia dla wprowadzonych sygnalow." << endl;

	for (int it = 0; it < uczaceIteracje; it++)
	{
		eta = 0.1 - it*0.01; // wspó³czynnik eta dla WTAa
		//eta = 0.5;

		if (!it % 4 == 0)
			wprowadzWejsciaUczace();
		else
			wprowadzWejsciaTestowe();
		
		for (int i = 1; i < numberOfLayers; i++) // zak³ada, ¿e outputy z pierwszej warstwy s¹ zawsze wprowadzane
			for (int j = 0; j < n[i]; j++)
			{
				I[i][j] = 0.0;
				for (int k = 0; k < n[i - 1]; k++) // il. synaps w neuronie równa liczbie neuronów poprzedniej warstwy
					I[i][j] += O[i - 1][k] * W[i][j][k]; // iloczyn sygna³ów wejœciowych przez wagi dla danych neuronów
				O[i][j] = I[i][j];
			}

		int jNaj2 = 0; // zapamiêtajmy numery najwiêkszych wyjœæ z warstwy 2
		for (int j = 0; j < n[1]-1; j++) // wybierzmy najwiêkszy neuron z warstwy drugiej
			if (!(O[1][jNaj2] > O[1][j + 1]))
				jNaj2 = j + 1;

		for (int j = 0; j < n[1]; j++) // po jego wybraniu dajmy mu wartoœæ jeden, a resztê wyzerujmy
		{
			if (j == jNaj2)
				O[1][j] = 1;
			else
				O[1][j] = 0;
		}

		// znormalizujmy wektory wejœciowe
		double Nor = 0;
		double tmp = 0;
		for (int j = 0; j < n[1]; j++) // wejœcia neuronów 2 i 3 warstwy
		{
			tmp = O[0][j] * O[0][j];
			tmp = sqrt(tmp);
			Nor += tmp;
		}

		// dostosowywanie wag
		for (int i = 1; i < numberOfLayers; i++)
			for (int k = 0; k < n[i - 1]; k++)
			{
					W[i][jNaj2][k] += eta*((O[0][k]/Nor) - W1[i][jNaj2][k]);
			}

		WYJSCIAtxt << "Epoka: " << it << "       ";
		WYJSCIAtxt << "Wyjscie 0: " << O[numberOfLayers - 1][0] << "       ";
		WYJSCIAtxt << "Wyjscie 1: " << O[numberOfLayers - 1][1] << "       ";
		WYJSCIAtxt << "Wyjscie 2: " << O[numberOfLayers - 1][2] << "       ";
		WYJSCIAtxt << endl;

		if (it) // nasze ostatnie kilka epok, jest to zestaw testowy
		{
			cout << endl << endl; // narysuje nam liczbê
			for (int i = 0; i < 7; i++)
			{
				for (int j = 0 + (i * 5); j < 5 + (i * 5); j++)
				{
					if (O[0][j] == 1)
						cout << "O";
					else
						cout << " ";
				}
				cout << endl;
			}

			cout << "Epoka: " << it << endl;
			cout << "Wyjscie 0: " << O[numberOfLayers - 1][0] << endl;
			cout << "Wyjscie 1: " << O[numberOfLayers - 1][1] << endl;
			cout << "Wyjscie 2: " << O[numberOfLayers - 1][2] << endl;
			/*cout << "Wyjscie 3: " << O[numberOfLayers - 1][3] << endl;
			cout << "Wyjscie 4: " << O[numberOfLayers - 1][4] << endl;
			cout << "Wyjscie 5: " << O[numberOfLayers - 1][5] << endl;
			cout << "Wyjscie 6: " << O[numberOfLayers - 1][6] << endl;
			cout << "Wyjscie 7: " << O[numberOfLayers - 1][7] << endl;
			cout << "Wyjscie 8: " << O[numberOfLayers - 1][8] << endl;
			cout << "Wyjscie 9: " << O[numberOfLayers - 1][9] << endl;*/
		}
	}
	milliseconds stop = duration_cast< milliseconds >(
		system_clock::now().time_since_epoch());

	cout << endl << "Czas nauczania dla naszych epok: " << stop.count() - start.count() << " ms" << endl;
}

WTA::WTA()
{
	// wstêpna charakterystyka sieci

	numberOfLayers = 2;
	n = new int[numberOfLayers]; // iloœæ neuronów w danej warstwie sieci
	n[0] = 35;
	n[1] = 3;

	numberOfNeurons = 35;
	numberOfSignals = 35;
	alfa = 0.6;

	// wpisywanie podstawowych wartoœci
	uczaceIteracje = 1000;
	testoweIteracje = 5;

	// wagi
	W = new double **[numberOfLayers];
	for (int i = 0; i< numberOfLayers; i++)
	{
		W[i] = new double *[numberOfNeurons];
		for (int j = 0; j<numberOfSignals; j++) // numberOfSignals
		{
			W[i][j] = new double[numberOfSignals]; // numberOfSignals
		}
	}

	// wagiPoprzednie
	W1 = new double **[numberOfLayers];
	for (int i = 0; i< numberOfLayers; i++)
	{
		W1[i] = new double *[numberOfNeurons];
		for (int j = 0; j<numberOfNeurons; j++)
		{
			W1[i][j] = new double[numberOfSignals];
		}
	}

	// wejœcia
	I = new double *[numberOfLayers];
	for (int i = 0; i < numberOfLayers; ++i)
	{
		I[i] = new double[numberOfNeurons];  // !
		for (int j = 0; j < numberOfNeurons; ++j)
			I[i][j] = 0;
	}

	// wejœcia
	Iprim = new double *[2];
	for (int i = 0; i < 2; ++i)
	{
		Iprim[i] = new double[numberOfNeurons];  // !
		for (int j = 0; j < numberOfNeurons; ++j)
			Iprim[i][j] = 0;
	}


	// wyjœcia
	O = new double *[numberOfLayers];
	for (int i = 0; i < numberOfLayers; ++i)
	{
		O[i] = new double[numberOfNeurons]; // !
		for (int j = 0; j < numberOfNeurons; ++j)
			O[i][j] = 0;
	}

	// b³¹d
	B = new double *[numberOfLayers];
	for (int i = 0; i < numberOfLayers; ++i)
	{
		B[i] = new double[numberOfNeurons];
		for (int j = 0; j < numberOfNeurons; ++j)
			B[i][j] = 0;
	}

	// oczekiwana wartoœæ
	Z = new double *[numberOfLayers];
	for (int i = 0; i < numberOfLayers; ++i)
	{
		Z[i] = new double[numberOfNeurons];
		for (int j = 0; j < numberOfNeurons; ++j)
			Z[i][j] = 0;
	}

	// b³¹d delta
	E = new double *[numberOfLayers];
	for (int i = 0; i < numberOfLayers; ++i)
	{
		E[i] = new double[numberOfNeurons];
		for (int j = 0; j < numberOfNeurons; ++j)
			E[i][j] = 0;
	}

	stworzWzorce();
	stworzTesty();
	wypelnijWagi();
	Epoki();
}

WTA::~WTA()
{
	delete[] n;
	n = nullptr;

	for (int i(0); i < numberOfLayers; ++i)
	{
		delete[] I[i];
		delete[] O[i];
		delete[] B[i];
		delete[] Z[i];
		delete[] E[i];
	}
	delete[] I, O, B, Z;
	I = nullptr;
	O = nullptr;
	B = nullptr;
	Z = nullptr;
	E = nullptr;

	for (int i(0); i < 2; ++i)
		delete[] Iprim[i];
	delete[] Iprim;
	Iprim = nullptr;

	for (int i = 0; i< numberOfLayers; i++)
	{
		for (int j = 0; j<numberOfNeurons; j++)
		{
			delete[] W[i][j];
		}
		delete[] W[i];
	}
	delete[]W;
	W = nullptr;

	for (int i = 0; i< numberOfLayers; i++)
	{
		for (int j = 0; j<numberOfNeurons; j++)
		{
			delete[] W1[i][j];
		}
		delete[] W1[i];
	}
	delete[]W1;
	W1 = nullptr;
}