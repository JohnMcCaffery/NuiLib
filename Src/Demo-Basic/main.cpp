/*************************************************************************
Copyright (c) 2012 John McCaffery 

This file is part of NuiLib.

NuiLib is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

NuiLib is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with NuiLib.  If not, see <http://www.gnu.org/licenses/>.

**************************************************************************/

#include <NuiLib-API.h>
//Needed for waitKey
#include <opencv\highgui.h>

int main (int argc, char **args) {
	//Initialise the factory.
	NuiLib::NuiFactory()->Init();

	//Create the arm vector.
	NuiLib::Vector arm = NuiLib::joint(NuiLib::HAND_RIGHT) - NuiLib::joint(NuiLib::SHOULDER_RIGHT);
	//Add a listener so whenever the arm vector changes its new values are output.
	arm.AddListener([&arm](NuiLib::IObservable *s) { cout << "Right Arm: " << arm.X() << ',' << arm.Y() << ',' << arm.Z() << '\n'; });
	//Start the factory polling.
	NuiLib::NuiFactory()->SetAutoPoll(true);

	//Wait for user input to stop the program.
#ifdef VISUAL
	cv::waitKey();
#else
	string waitstr = " ";
	cin >> waitstr;
#endif
}
