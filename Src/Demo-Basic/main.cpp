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
#include<NuiLib-CLRSafe.h>
//Needed for waitKey
#include <opencv\highgui.h>

int main (int argc, char **args) {
	//Initialise the factory.
	NuiLib::NuiFactory()->Init();

	//Create the arm vector as the difference between the shoulder and the hand.
	//NuiLib::Vector arm = NuiLib::joint(NuiLib::HAND_RIGHT) - NuiLib::joint(NuiLib::SHOULDER_RIGHT);
	//Add a listener so whenever the arm vector changes its new values are output.
	//arm.AddListener([&arm](NuiLib::IObservable *s) { cout << "Right Arm: " << arm.X() << ',' << arm.Y() << ',' << arm.Z() << '\n'; });
	//Start the factory polling.
	//NuiLib::NuiFactory()->SetAutoPoll(true);

	NuiLibSafe::SafeVector *s = new NuiLibSafe::SafeVector(0.f, 0.f, 0.f);
	const char *cname = s->GetName();

	NuiLib::Vector planePoint = NuiLib::Vector("PlanePoint", 1.f, 1.f, 0.f);
	NuiLib::Vector planeNormal = NuiLib::Vector("PlaneNormal", 0.f, 0.f, -1.f);
	NuiLib::Vector linePoint = NuiLib::joint(NuiLib::SHOULDER_RIGHT);
	NuiLib::Vector lineDir = NuiLib::joint(NuiLib::HAND_RIGHT);
	NuiLib::Scalar mW = NuiLib::Scalar("W", 0.f);
	NuiLib::Scalar mH = NuiLib::Scalar("H", 0.f);

	NuiLib::Vector intersect = NuiLib::intersect(planePoint, planeNormal, linePoint, lineDir);

	NuiLib::Vector vertical = NuiLib::Vector(0.f, 1.f, 0.f); // Vertical
	//Calculate the intersection of the plane defined by the point mPlaneTopLeft and the normal mPlaneNormal and the line defined by the point mPointStart and the direction mPointDir.
	NuiLib::Vector intersection = NuiLib::intersect(planePoint, planeNormal, linePoint, lineDir); 
	//Calculate a vector that represents the orientation of the top of the window.
	NuiLib::Vector top = NuiLib::scale(NuiLib::cross(vertical, planeNormal), mW);
	//Calculate a vector that represents the orientation of the side of the window.
	NuiLib::Vector side = NuiLib::scale(NuiLib::cross(planeNormal, top), mH);

	//Calculate the vector (running along the plane) between the top left corner and the point of intersection.
	NuiLib::Vector diff = intersection - planePoint;

	//Project the diff line onto the top and side vectors to get x and y values.
	NuiLib::Scalar mX = NuiLib::project(diff, top);
	NuiLib::Scalar mY = NuiLib::project(diff, side);

	NuiLib::NuiFactory()->AddListener([&mX, &mY] (NuiLib::IObservable *source) { cout << "X: " << (*mX) << ", Y: " << (*mY) << "\n"; });

	mX.AddListener([&top, &mH] (NuiLib::IObservable *source) { 
		cout << "Test\n"; 
	});
	float x = mX.Get();
	float y = mY.Get();
	mW.Set(20.f);
	x = mW.Get();
	y = mH.Get();
	mH.Set(0.f);
	x = mW.Get();
	y = mH.Get();

	NuiLib::NuiFactory()->Poll();

	//Wait for user input to stop the program.
#ifdef VISUAL
	cv::waitKey();
#else
	string waitstr = " ";
	cin >> waitstr;
#endif
}
