#include<NuiLib-API.h>
#include<NuiLib-CLRSafe.h>
#include<NuiLib-Extendable.h>
#include<opencv\highgui.h>
#include<iostream>

using namespace std;
using namespace NuiLib;

int main(int argc, char** argv) {

	NuiLibSafe::SafeVector *s = new NuiLibSafe::SafeVector(0.f, 0.f, 0.f);
	const char *cname = s->GetName();

	Vector planePoint = Vector("PlanePoint", 1.f, 1.f, 0.f);
	Vector planeNormal = Vector("PlaneNormal", 0.f, 0.f, -1.f);

	NuiFactory()->Init();
	NuiFactory()->SetAutoPoll(true);

	fist(true);

	cv::namedWindow("TestWindow");
	cv::namedWindow("TestDepth");

	Vector scaledV = smooth(joint(HAND_RIGHT), 5);
	Scalar scaledS = smooth(x(joint(HAND_RIGHT)), 5);

	Vector pointEnd = joint(HAND_RIGHT);
	Vector mPointStart = joint(SHOULDER_RIGHT);
	Vector mPointDir = mPointStart - pointEnd;
	Scalar mW = Scalar("W", 2.f);
	Scalar mH = Scalar("H", 2.f);

	Vector vertical = Vector(0.f, 1.f, 0.f); // Vertical
	//Calculate the intersection of the plane defined by the point mPlaneTopLeft and the normal mPlaneNormal and the line defined by the point mPointStart and the direction mPointDir.
	Vector mIntersection = intersect(planePoint, planeNormal, mPointStart, mPointDir); 
	//Calculate a vector that represents the orientation of the top of the window.
	Vector mTop = scale(cross(vertical, planeNormal), mW);
	//Calculate a vector that represents the orientation of the side of the window.
	Vector mSide = scale(cross(planeNormal, mTop), mH);

	//Calculate the vector (running along the plane) between the top left corner and the point of intersection.
	Vector diff = mIntersection - planePoint;

	//Project the diff line onto the top and side vectors to get x and y values.
	Scalar mX = project(diff, mTop);
	Scalar mY = project(diff, mSide);
	
	mW.Set(2.1f);


	NuiFactory()->AddListener([&mX, &mY] (IObservable *source) { 
		cv::imshow("TestWindow", NuiFactory()->GetColour());
		cv::imshow("TestDepth", NuiFactory()->GetDepth());
	});


	//Wait for user input to stop the program.
#ifdef VISUAL
	cv::waitKey();
#else
	string waitstr = " ";
	cin >> waitstr;
#endif

	return 0;
}

