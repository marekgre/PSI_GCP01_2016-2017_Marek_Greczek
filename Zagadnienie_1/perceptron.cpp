void perceptron()
{
	ofstream plik;
	plik.open("WYNIK5.txt", ios::out);

	milliseconds start = duration_cast< milliseconds >( // pocz¹tek pomiaru czasu
		system_clock::now().time_since_epoch());
	for (int i = 0; i < getNumberOfCycles(); i++)
	{
		writePatterns(false, false); // wprowadzamy x na wyjœcie z nauczycielem

									 // liczymy wyjœcia na drugiej (koñcowej) warstwie
		float tmp1 = 0;
		float tmp2 = 0;
		for (int j = 0; j < getN(0); j++)
		{
			tmp1 += getO(0, j) * getW(0, j); // wyjœcie pierwsze
			tmp2 += getO(0, j) * getW(1, j); // wyjœcie drugie
		}
		setO(1, 0, tmp1);
		setO(1, 1, tmp2);

		// funkcja aktywacji
		if (O[1][0] >= 0)
			setO(1, 0, 1);
		else
			setO(1, 0, 0);

		if (O[1][1] >= 0)
			setO(1, 1, 1);
		else
			setO(1, 1, 0);

		// dostosowywanie wag
		for (int i = 0; i < getN(1); i++)
		{
			for (int j = 0; j < getN(0); j++)
			{
				setW(i, j, getW(i, j) + getEta()*(getT(i) - getO(1, i))*getO(0, j));
			}
		}

		if (getNumberOfCycles() - 32 < i)
		{
			paintPattern();
			cout << endl << "Wyjscie wstepne 0: " << tmp1 << endl;
			cout << "Wyjscie wstepne 1: " << tmp2 << endl;
			cout << endl << "Wyjscie 0: " << getO(1, 0) << endl;
			cout << "Wyjscie 1: " << getO(1, 1) << endl;
			cout << "Iteracja: " << i + 1 << endl;

			for (int i = 0; i < getN(0); i++)
			{
				if (i % 5 == 0)
					plik << endl;
				if (getO(0, i) == 1)
					plik << "O";
				else
					plik << " ";
			}

			plik << endl << "Iteracja: " << i + 1 << endl;
			plik << "Wyjscie wstepne 0: " << tmp1 << endl;
			plik << "Wyjscie wstepne 1: " << tmp2 << endl;
			plik << "Wyjscie 0: " << getO(1, 0) << endl;
			plik << "Wyjscie 1: " << getO(1, 1) << endl;
		}
	}

	milliseconds stop = duration_cast< milliseconds >(
		system_clock::now().time_since_epoch());

	cout << endl << "Time: " << stop.count() - start.count() << " ms" << endl;
	plik << "time: " << stop.count() - start.count() << " ms" << endl; 

	system("pause");
}