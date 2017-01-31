void hebb()
{
	// biblioteka zapisuj¹ca do pliku
	ofstream hebb;
	bool test = false;
	hebb.open("TEST5.txt", ios::out);
	hebb << "test 5, eta=0.1-it*0.001, cycles: " << getNumberOfCycles() << endl;
	float* tmp = new float[getN(1)];
	milliseconds start = duration_cast< milliseconds >( // pocz¹tek pomiaru czasu
		system_clock::now().time_since_epoch());

	for (it = 0; it < getNumberOfCycles(); it++)
	{
		if (getNumberOfCycles() - 40 > it)
			writePatterns(false, true);
		else
		{
			test = true;
			writePatterns(true, true);
		}

		if (test)
			if ((getNumberOfCycles() - 40) == it)
				cout << endl << endl << "TESTY______________________________________________________" << endl << endl;

		// liczymy wyjœcia na drugiej (koñcowej) warstwie
		for (int i = 0; i < getN(1); i++)
		{
			tmp[i] = 0;
			for (int j = 0; j < getN(0); j++)
				tmp[i] += getO(0, j) * getW(i, j);
		}

		// funkcja aktywacji
		for (int j = 0; j < getN(1); j++)
		{
			setO(1, j, tmp[j]);
			//setO(1, j, 1.0 / (1.0 + exp(getBeta()*(-tmp[j]))));
		}

		// dostosowywanie wag
		if (test == false)
		{
			for (int i = 0; i < getN(1); i++)
				for (int j = 0; j < getN(0); j++)
					//setW(i, j, getW(i, j) + getEta()*getO(0, i)*(getO(0, j) - (getO(1,i)*getW(i, j)))); // regu³a Oja
					//setW(i, j, getW(i, j) + getEta()*(getO(1, i) - getW(i, j))); // zwi¹zany z wyjœciami
					setW(i, j, getW(i, j) + getEta()*(getO(0, j) - getW(i, j))); // zwi¹zany z wejœciami
																				 //setW(i, j, getW(i, j) + getEta()*getO(1,i)*getO(0, j)); // tradycyjny
		}

		if (test == false)
		{
			if (getNumberOfCycles() - 40 < it)
			{
				paintPattern();
				for (int i = 0; i < getN(1); i++)
				{
					if (i == 0)
						cout << endl;
					cout << "Wyjscie " << i << ": " << getO(1, i) << endl;
				}
				cout << "Iteracja: " << it + 1 << endl;
			}
		}

		else
		{
			if (getNumberOfCycles() - 100 < it)
			{
				paintPattern();

				for (int i = 0; i < getN(0); i++)
				{
					if (i % 5 == 0)
						hebb << endl;
					if (getO(0, i) == 1)
						hebb << "O";
					else
						hebb << " ";
				}

				for (int i = 0; i < getN(1); i++)
				{
					if (i == 0)
						hebb << endl;
					hebb << "Wyjscie " << i << ": " << getO(1, i) << endl;
				}
				hebb << "Iteracja: " << it + 1 << endl;

				for (int i = 0; i < getN(1); i++)
				{
					if (i == 0)
						cout << endl;
					cout << "Wyjscie " << i << ": " << getO(1, i) << endl;
				}
				cout << "Iteracja: " << it + 1 << endl;
			}
		}
	}

	milliseconds stop = duration_cast< milliseconds >(
		system_clock::now().time_since_epoch());

	cout << endl << "Time: " << stop.count() - start.count() << " ms" << endl;
	hebb << "time: " << stop.count() - start.count() << " ms" << endl; // wrzuæmy czas do notatnika

	system("pause");
}