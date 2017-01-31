void kohonen()
{
	// biblioteka zapisuj¹ca do pliku
	ofstream q;
	bool test = false;
	q.open("NAUKA10.txt", ios::out);
	q << "learning 10, cycles: " << getNumberOfCycles() << ", eta: " << getEta() << endl;
	float* tmp = new float[getN(1)];
	milliseconds start = duration_cast< milliseconds >( // pocz¹tek pomiaru czasu
		system_clock::now().time_since_epoch());

	for (int i = 0; i < getNumberOfCycles(); i++)
	{
		if (getNumberOfCycles() - 40 > i)
			writePatterns(false, true);
		else
		{
			test = true;
			writePatterns(true, true);
		}

		if ((getNumberOfCycles() - 40) == i)
			cout << endl << endl << "TESTY______________________________________________________" << endl << endl;

		// liczymy wyjœcia na drugiej (koñcowej) warstwie
		for (int i = 0; i < getN(1); i++)
		{
			tmp[i] = 0;
			for (int j = 0; j < getN(0); j++)
			{
				tmp[i] += getO(0, j) * getW(i, j);
			}
		}

		// funkcja aktywacji
		for (int j = 0; j < getN(1); j++)
		{
			//O[1][j] = 1.0 / (1.0 + exp(getBeta()*(-tmp[j])));
			setO(1, j, tmp[j]);
		}

		// wybierzanie najmocniejszego wektorów
		int jNaj = 0;
		for (int i = 1; i < getN(1); i++)
		{
			if (tmp[i] > tmp[jNaj])
			{
				jNaj = i;
			}
		}
		cout << "Najlepszy neuron: " << jNaj;
		cout << ", Iteracja: " << i + 1 << endl;

		// normalizacja wektorów wejœciowych
		double sumaWektorow = 1;
		for (int i = 0; i < getN(0); i++)
			//sumaWektorow += getO(0, i);

			//przypiszmy wagi tymczasowe
			for (int i = 0; i < getN(1); i++)
			{
				for (int j = 0; j < getN(0); j++)
				{
					setWtmp(i, j, getW(i, j));
				}
			}

		// dostosowywanie wag
		/*	if (test == false)
		{*/
		//for (int i = 0; i < getN(1); i++)
		//{
		for (int j = 0; j < getN(0); j++)
		{
			setW(jNaj, j, getW(jNaj, j) + getEta()*((getO(0, j) / sumaWektorow) - getWtmp(jNaj, j)));
		}
		//}
		/*}*/

		// porównajmy wagi tymczasowe z jedn¹ zmienion¹
		double suma = 0;
		int zmiana = 0;
		for (int i = 0; i < getN(1); i++)
		{
			suma = 0;
			for (int j = 0; j < getN(0); j++)
			{
				suma += abs(getWtmp(i, j)) - abs(getW(i, j));
			}

			if (suma != 0)
			{
				zmiana = i;
			}
			//cout << "SUMA: " << suma << "dla i: " << i << endl;
		}
		//cout << "Zmiana: " << zmiana << endl;

		for (int i = 0; i < getN(1); i++)
		{
			if (i == zmiana)
				setO(1, i, 1);
			else
				setO(1, i, 0);
		}

		/*if (test == false)
		{*/
		/*if (getNumberOfCycles() - 100 < i)
		{*/
		paintPattern();
		for (int i = 0; i < getN(1); i++)
		{
			if (i == 0)
				cout << endl;
			cout << "Wyjscie " << i << ": " << getO(1, i) << endl;
		}
		cout << "Iteracja: " << i + 1 << endl;
		/*}*/
		/*}

		else
		{*/
		/*if (getNumberOfCycles() - 100 < i)
		{*/
		paintPattern();

		for (int i = 0; i < getN(0); i++)
		{
			if (i % 5 == 0)
				q << endl;
			if (getO(0, i) == 1)
				q << "O";
			else
				q << " ";
		}

		for (int i = 0; i < getN(1); i++)
		{
			if (i == 0)
				q << endl;
			q << "Wyjscie " << i << ": " << getO(1, i) << endl;
		}
		q << "Iteracja: " << i + 1 << endl;

		for (int i = 0; i < getN(1); i++)
		{
			if (i == 0)
				cout << endl;
			cout << "Wyjscie " << i << ": " << getO(1, i) << endl;
		}
		cout << "Iteracja: " << i + 1 << endl;
		/*}*/
		/*}*/
	}

	milliseconds stop = duration_cast< milliseconds >(
		system_clock::now().time_since_epoch());

	cout << endl << "Time: " << stop.count() - start.count() << " ms" << endl;
	q << "time: " << stop.count() - start.count() << " ms" << endl; // wrzuæmy czas do notatnika

	system("pause");
}