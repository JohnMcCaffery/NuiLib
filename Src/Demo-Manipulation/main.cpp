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
#include <NuiLib-Extendable.h>
#include <opencv\highgui.h>
#include <Windows.h>

#ifndef M_PI
#define M_PI 3.14159
#endif

using namespace std;
using namespace NuiLib;                                

//--Manipulate
//True if translating using the right hand
NuiLib::Condition				mTranslateR;
//True if translating using the left hand
NuiLib::Condition				mTranslateL;
//True if rotating
NuiLib::Condition				mRotate;

//Pointing
//The X coordinate of the pointer
NuiLib::Scalar					mX;
//The Y coordinate of the pointer
NuiLib::Scalar					mY;

//Translate
//The translation delta for the right hand
NuiLib::Vector					mDeltaR;
//The translation delta for the left hand
NuiLib::Vector					mDeltaL;

//Rotate
//Rotation around the X axis
NuiLib::Scalar					mXRot;
//Rotation around the Y axis
NuiLib::Scalar					mYRot;
//Rotation around the Z axis
NuiLib::Scalar					mZRot;

class Listener : public IObserver {
	public:
		Listener(IObservable *source) { 
			AddAsListener(NULL, source);
		}

		void Changed(IObservable *root) {
			char buffer[999];
			if (*mRotate) 
				SPRINTF(buffer, 999, "ROTATE       --- %-40s %-40s %-40s\n", mXRot._p->ToString(15).c_str(), mYRot._p->ToString(15).c_str(), mZRot._p->ToString(15).c_str()); 
			else if (*mTranslateR)
				SPRINTF(buffer, 999, "TRANSLATE R  --- %s\n", mDeltaR._p->ToString(15).c_str()); 
			else if (*mTranslateL)
				SPRINTF(buffer, 999, "TRANSLATE L  --- %s\n", mDeltaL._p->ToString(15).c_str()); 
			else
				SPRINTF(buffer, 999, "");
			cout << buffer;
		}

};


string blah = "                    "; 

void RunExtensionFactory();
Listener *InitSecondary();

int main(int argc, const char* argv[]) { 
	RunExtensionFactory();
	return 0; 
}

void Pause() {
#ifdef VISUAL
	cv::waitKey();
#else
	getline(cin, blah);
#endif
}


void RunExtensionFactory() {
	ExtensionFactory()->Init();

	InitSecondary();

	cout << "Press return to start auto polling.\n";
	Pause();
	ExtensionFactory()->SetAutoPoll(true);

	cout << "Press return to stop auto polling.\n";
	Pause();
	ExtensionFactory()->SetAutoPoll(false);

	cout << "Press return to kill factory.\n";
	Pause();

	//delete ExtensionFactory();
	Sleep(500);

	cout << "Press return to exit.\n";
	Pause();
}
Listener *InitSecondary() {
#ifndef Hardwire
	Vector shoulderR = joint(SHOULDER_RIGHT);
	Vector shoulderC = joint(SHOULDER_CENTER);
	Vector shoulderL = joint(SHOULDER_LEFT);
	Vector elbowR = joint(ELBOW_RIGHT);
	Vector handR = joint(HAND_RIGHT);
	Vector handL = joint(HAND_LEFT);
	Vector hipC = joint(HIP_CENTER);
	Condition fistR = fist(true);
	Condition fistL = fist(false);
#else
	IVector *shoulderR = new NuiLib::Vector(k, "ShoulderR", -1, 0, 0);
	IVector *shoulderC = new NuiLib::Vector(k, "ShoulderC", 0, 0, 0);
	IVector *shoulderL = new NuiLib::Vector(k, "ShoulderL", 1, 0, 0);
	IVector *hipC = new NuiLib::Vector(k, "CentreHip", 0, -1, 0);
	IVector *elbowR = new NuiLib::Vector(k, "ElbowR", 0, 0, 0);
	IVector *elbowL = new NuiLib::Vector(k, "ElbowR", 0, 0, 0);
	IVector *wristR = new NuiLib::Vector(k, "WristR", 0, 0, 0);
	IVector *wristL = new NuiLib::Vector(k, "WristR", 0, 0, 0);
	IVector *handR = new NuiLib::Vector(k, "HandR", -1, -1, 1);
	IVector *handL = new NuiLib::Vector(k, "HandL", 0, 0, 0);
	ICondition *fistL = new Condition(k, "FistL", false);
	ICondition *fistR = new Condition(k, "FistR", false);
#endif
	Vector yAxis = NuiLib::Vector("Y", 0.f, 1.f, 0.f);
	Vector normal = Vector("Normal", 0, 0, 1);
	
	//Rotate
	mRotate = fistL && fistR;
	NuiLib::Vector relativeCurrent = handR - handL;
	NuiLib::Vector relativeSnapshot = snapshot(relativeCurrent, fistR && fistL);
	NuiLib::Vector xCurrent = limit(relativeCurrent, false, true, true);
	NuiLib::Vector yCurrent = limit(relativeCurrent, true, false, true);
	NuiLib::Vector zCurrent = limit(relativeCurrent, true, true, false);
	NuiLib::Vector xRelative = limit(relativeSnapshot, false, true, true);
	NuiLib::Vector yRelative = limit(relativeSnapshot, true, false, true);
	NuiLib::Vector zRelative = limit(relativeSnapshot, true, true, false);
	mXRot = dot(normalize(xCurrent), normalize(xRelative)) * invert(x(cross(xCurrent, xRelative)) >= 0);
	mYRot = dot(normalize(yCurrent), normalize(yRelative)) * invert(y(cross(yCurrent, yRelative)) >= 0);
	mZRot = dot(normalize(zCurrent), normalize(zRelative)) * invert(z(cross(zCurrent, zRelative)) >= 0);

	//Translate
	mTranslateL = fistL && !fistR;
	mTranslateR = fistR && !fistL;
	Vector deltaL = shoulderL - handL;
	Scalar xDelta = constrain(x(deltaL), tracker("X-D", 10, .1f, 0.f), tracker("X-R", 10, .1f, 0.f), tracker("X-G", 10, .1f, 0.f), true);
	Scalar yDelta = constrain(y(deltaL), tracker("Y-D", 10, .1f, 0.f), tracker("Y-R", 10, .1f, 0.f), tracker("Y-G", 10, .1f, 0.f), true);
	Scalar zDelta = constrain(z(deltaL) - tracker("Z-O", 10, .1f, 0.f), tracker("Z-D", 10, .1f, 0.f), tracker("Z-R", 10, .1f, 0.f), tracker("Z-G", 10, .1f, 0.f), true);
	//Scalar xDelta = constrain(x(deltaL), .5f, .5f, .5f, true);
	//Scalar yDelta = constrain(y(deltaL), .5f, .5f, .5f, true);
	//Scalar zDelta = constrain(z(deltaL) - .5f, .5f, .5f, .5f, true);
	mDeltaL = Vector("DeltaL", xDelta, yDelta, zDelta);
	//mDeltaL = delta(snapshot(handL, mTranslateL)) * NuiLib::Vector(tracker("Left Scale", 100, 1.f, 50));
	mDeltaR = delta(snapshot(handR, mTranslateR)) * NuiLib::Vector(tracker("Right Scale", 100, 1.f, 50));

	//Point
	float w = 10;
	float h = 10;
	float d = 5;
	Vector xShift = scale(shoulderR - shoulderL, w / 2);
	Vector yShift = scale(shoulderC - hipC, h / 2) + scale(yAxis, 1.0f);
	Vector zShift = scale(normal, d);
	Vector shift = xShift + (yShift + zShift);
	Vector p0 = shoulderC + shift;
	Vector l = normalize(handR - elbowR);
	Vector i = intersect(p0, normal, elbowR, l);
	Vector top = scale(cross(yAxis, normal), w);
	Vector side = scale(cross(normal, top), h);
	Vector diff = i - p0;
	mX = constrain(project(diff, top), 0.f, w, 0.f, false);
	mY = constrain(project(diff, side), 0.f, h, 0.f, false);

	return new Listener(NuiFactory());
}
