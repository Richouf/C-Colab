// PID.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "pidcontrol.h"

int main()
{
	bool IOn = true;
	int timer = 0;
	double PV = 0;
	double CV = 0;
	double SP= 75;
	pidcontrol PID1 = pidcontrol(PV, SP, CV);
	PID1.setGain(5, .1, .05);
	std::cout << SP << '\t' << PID1.getfbReal() << '\t' << CV << '\t' << '\t' << '\t';
	std::cout << '\t' << PID1.getP() << '\t' << PID1.getI() << '\t' << PID1.getD() << '\n';
	while (timer < 10)
	{
		PID1.runPID();
		std::cout << SP << '\t' << PID1.getfbReal() << '\t' << CV << '\t' << '\t' << '\t';
		std::cout << '\t' << PID1.getP() << '\t' << PID1.getI() << '\t' << PID1.getD() << '\n';
		if (PID1.getfbReal() > SP - .5 && PID1.getfbReal() < SP + .5)
			++timer;
		double tempPV = PV + ( 1028 * CV / 2048 ) / 10 - 15;
		PV = tempPV <= 0 ? 0 : tempPV >= 1024 ? 1024 : tempPV;
		PID1.changeSetting(2, IOn);
		IOn = !IOn;
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
