void madaline()
{
	
	ofstream q;
	bool test = false;
	q.open("TEST6.txt", ios::out);
	q << "test 6, cycles: " << getNumberOfCycles() << endl;
	float* tmp = new float[getN(1)];
	milliseconds start = duration_cast< milliseconds >( // pocz¹tek pomiaru czasu
		system_clock::now().time_since_epoch());

	for (int i = 0; i < getNumberOfCycles(); i++)
	{
		if (getNumberOfCycles() - 40 > i)
			writePatterns(false, false);
		else
		{
			test = true;
			writePatterns(true, false);
		}

		if ((getNumberOfCycles() - 40) == i)
			cout << endl << endl << "TESTY______________________________________________________" << endl << endl;

		// liczenie wyjœcia na drugiej (koñcowej) warstwie
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
			O[1][j] = 1.0 / (1.0 + exp(getBeta()*(-tmp[j])));
		}

		// dostosowywanie wag
		if (test == false)
		{
			for (int i = 0; i < getN(1); i++)
			{
				for (int j = 0; j < getN(0); j++)
				{
					setW(i, j, getW(i, j) + getEta()*(getT(i) - getO(1, i))*getO(0, j));
				}
			}
		}

		//ustalanie b³êdu
		setQ(0);
		for (int j = 0; j < getN(1); j++)
			setQ(getQ() + 0.5*pow((getT(j) - getO(1, j)), 2));


		if (test == false)
		{
			if (getNumberOfCycles() - 100 < i)
			{
				paintPattern();
				for (int i = 0; i < getN(1); i++)
				{
					if (i == 0)
						cout << endl;
					cout << "Wyjscie " << i << ": " << getO(1, i) << endl;
				}
				cout << "Iteracja: " << i + 1 << endl;
			}
		}

		else
		{
			if (getNumberOfCycles() - 100 < i)
			{
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
			}
		}
	}

	milliseconds stop = duration_cast< milliseconds >(
		system_clock::now().time_since_epoch());

	cout << endl << "Time: " << stop.count() - start.count() << " ms" << endl;
	q << "time: " << stop.count() - start.count() << " ms" << endl; // zapis czasu

	system("pause");
}