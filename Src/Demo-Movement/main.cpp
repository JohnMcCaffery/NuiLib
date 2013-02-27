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

#ifndef M_PI
#define M_PI 3.14159
#endif

using namespace std;
using namespace NuiLib;                                

//True if any of the movement conditions are met.
NuiLib::Condition				mCanMove;

//True if the user should be pushed forward.
NuiLib::Condition				mPush;

//True if yawing right or left.
NuiLib::Condition				mCanYaw;
//How far left or right to yaw (- = yaw left).
NuiLib::Scalar					mYaw;

//True if pitching up or down.
NuiLib::Condition				mCanPitch;
//How far up or down to pitch.
NuiLib::Scalar					mPitch;

//True if flying up or down
NuiLib::Condition				mCanFly;
//True if flying up, false if flying down.
NuiLib::Condition				mFly;

string blah = "                    "; 

void Init();

void Pause();

int main(int argc, const char* argv[]) { 
	NuiFactory()->Init();

	Init();

	NuiFactory()->AddListener([](IObservable *source) {
		char buffer[9999];
		// If any movement is to be done.
		if (*mCanMove) {
			// Print out FORWARD if that is enabled.
			SPRINTF(buffer, 9999, "%-10s", (*mPush ? "FORWARD" :  ""));
			// If yaw is enabled print out the yaw value and give a visual representation.
			SPRINTF(buffer, 9999, "%s %s %-25s", buffer, "Yaw:", (*mCanYaw ? mYaw._p->ToString(7).c_str() : ""));
			// If yaw is enabled print out the pitch value and give a visual representation.
			SPRINTF(buffer, 9999, "%s %s %-25s", buffer, "Pitch:", (*mCanPitch ? mPitch._p->ToString(7).c_str() : ""));
			// Print out UP if flying up, down if flying down. Otherwise, nothing.
			SPRINTF(buffer, 9999, "%s %s %-6s\n", buffer, "Fly:", (*mCanFly ? (*mFly ? "UP" : "down") : ""));
		} else 
			SPRINTF(buffer, 9999, "");
		cout << buffer;
	});

	cout << "Press return to start auto polling.\n";
	Pause();
	ExtensionFactory()->SetAutoPoll(true);

	cout << "Press return to stop auto polling.\n";
	Pause();
	ExtensionFactory()->SetAutoPoll(false);

	cout << "Press return to kill factory.\n";
	Pause();

	Sleep(500);

	cout << "Press return to exit.\n";
	Pause();
	return 0; 
}

void Pause() {
#ifdef VISUAL
	cv::waitKey();
#else
	getline(cin, blah);
#endif
}

const float R2DEG = (180 / (float) M_PI);

/*
Initialise the global variables.
*/
void Init() {
	//Get the primary vectors.
	Vector shoulderR = joint(SHOULDER_RIGHT);
	Vector shoulderL = joint(SHOULDER_LEFT);
	Vector elbowR = joint(ELBOW_RIGHT);
	Vector elbowL = joint(ELBOW_LEFT);
	Vector wristR = joint(WRIST_RIGHT);
	Vector wristL = joint(WRIST_LEFT);
	Vector handR = joint(HAND_RIGHT);
	Vector handL = joint(HAND_LEFT);
	Vector hipC = joint(HIP_CENTER);
	Vector head = joint(HEAD);

	Vector yAxis = NuiLib::Vector("Y", 0.f, 1.f, 0.f);
	// Normal is the direction the camera is facing.
	Vector normal = Vector("Normal", 0, 0, 1);

	//Camera - If the right elbow is raised to be in line with the shoulders the camera is active.
	NuiLib::Vector upperArmCameraR = elbowR - shoulderR;
	NuiLib::Vector lowerArmCameraR = elbowR - wristR;
	Condition cameraActiveR = abs(x(upperArmCameraR)) > (abs(y(upperArmCameraR)) + abs(z(upperArmCameraR))) * 2.f;

	//Camera - If the right elbow is raised to be in line with the shoulders the camera is active.
	NuiLib::Vector upperArmCameraL = shoulderL - elbowL;
	NuiLib::Vector lowerArmCameraL = elbowL - wristL;
	Condition cameraActiveL = abs(x(upperArmCameraL)) > (abs(y(upperArmCameraL)) + abs(z(upperArmCameraL))) * 2.f;

	Condition cameraActive = cameraActiveL || cameraActiveR;
	//Condition cameraActive = cameraActiveR;
	
	// Normalize the distance between the shoulder and the right hand against the total length of the arm (armMax).
	// Once normalized constrain the value between the input from tracker PushD (starting at .8) and 1. 
	// So if the normalized value is .9 the constrained value is .5. Alternatively if the normalized value is .8 the constrained value is 0.
	//Scalar push = constrain(normalize(magnitude(armR), armMax), tracker("PushD", 25, .04f, 0.f, 20), 1.f, 0.f, false);
	// If the camera is inactive and the push value is > 0 and the forward component is significantly larger than the horizontal and vertical components combined.
	//mPush = !cameraActive && push > 0 && z(armR) > ((abs(x(armR) + abs(y(armR)))) * tracker("PushActive", 9, .5f, .5f, 5));

	//Pitch
	Scalar pitchArmD = tracker("PitchArmD", 20, 1.f, 0.f, 10);
	Scalar pitchArmR = tracker("PitchArmR", 40, 2.f, 10.f, 17);
	Scalar pitchArmG = tracker("PitchArmG", 30, 1.f, 0.f, 15);
	Scalar pitchAS = tracker("PitchAS", 29, 1.f, 1.f, 20);

	Vector vPlaneCameraR = NuiLib::limit(lowerArmCameraR, false, true, true);
	Vector vPlaneCameraL = NuiLib::limit(lowerArmCameraL, false, true, true);
	// Pitch is the angle between normal and the vertical component of the vector between right shoulder and right hand.
	Scalar pitchR = NuiLib::acos(dot(normalize(vPlaneCameraR), normal)) * invert(x(cross(normal, vPlaneCameraR)) >= 0);
	Scalar pitchL = NuiLib::acos(dot(normalize(vPlaneCameraL), normal)) * invert(x(cross(normal, vPlaneCameraL)) >= 0);
	// Constrain the pitch value by 3 values input by 3 trackers.
	pitchR = constrain(pitchR * R2DEG, pitchArmD, pitchArmR, pitchArmG, true) / pitchAS;
	pitchL = constrain(pitchL * R2DEG, pitchArmD, pitchArmR, pitchArmG, true) / pitchAS;
	mPitch = ifScalar(cameraActiveR, pitchR, .0f) + ifScalar(cameraActiveL, pitchL, .0f);

	mCanPitch = cameraActive && mPitch != 0.f;

	//Yaw - Yaw has 3 components. The camera arm. The horizontal lean (head vs hip centre) and the twist of the shoulders.
	Scalar yawArmD = tracker("YawArmD", 20, 1.f, 0.f, 10);
	Scalar yawArmR = tracker("YawArmR", 40, 2.f, 10.f, 15);
	Scalar yawArmG = tracker("YawArmG", 20, 1.f, 0.f, 10);
	Scalar yawLeanD = tracker("YawLeanD", 20, .5f, 0.f, 10);
	Scalar yawLeanR = tracker("YawLeanR", 20, 1.f, 0.f, 15);
	Scalar yawLeanG = tracker("YawLeanG", 50, 1.f, 0.f, 30);
	Scalar yawTwistD = tracker("YawTwistD", 10, .025f, 0.f, 6);
	Scalar yawTwistR = tracker("YawTwistR", 20, .05f, 0.f, 9);
	Scalar yawTwistG = tracker("YawTwistG", 20, 1.f, 0.f, 10);
	
	Vector hPlaneCameraR = NuiLib::limit(lowerArmCameraR, true, false, true);
	Vector hPlaneCameraL = NuiLib::limit(lowerArmCameraL, true, false, true);
	// Yaw component 1 is the angle between normal and the horizontal component of the vector between right shoulder and right hand.
	Scalar yawCameraR = NuiLib::acos(dot(normalize(hPlaneCameraR), normal)) * invert(y(cross(normal, hPlaneCameraR)) >= 0);
	Scalar yawCameraL = NuiLib::acos(dot(normalize(hPlaneCameraL), normal)) * invert(y(cross(normal, hPlaneCameraL)) >= 0);
	// Constrain the component value by 3 values input by 3 trackers.
	yawCameraR = constrain(yawCameraR * R2DEG, yawArmD, yawArmR, yawArmG, true) / tracker("YawAS", 29, 1.f, 1.f, 20);
	yawCameraL = constrain(yawCameraL * R2DEG, yawArmD, yawArmR, yawArmG, true) / tracker("YawAS", 29, 1.f, 1.f, 20);
	//Only take the value if camera is active
	yawCameraR = ifScalar(cameraActiveR, yawCameraR, .0f);
	yawCameraL = ifScalar(cameraActiveL, yawCameraL, .0f);

	Vector yawCore = limit(head - hipC, true, true, false);
	// Yaw component 2 is how far the user is leaning horizontally. This is calculated the angle between vertical and the vector between the hip centre and the head.
	Scalar yawLean = NuiLib::acos(dot(normalize(yawCore), yAxis)) * invert(z(cross(yawCore, yAxis)) >= 0);
	// Constrain the component value by 3 values input by 3 trackers.
	yawLean = constrain(yawLean * R2DEG, yawLeanD, yawLeanR, yawLeanG, true) / tracker("YawLS", 29, 1.f, 1.f, 20);
	
	Vector shoulderDiff = shoulderR - shoulderL;
	// Yaw component 3 is the twist of the shoulders. This is calculated as the difference between the two z values.
	Scalar yawTwist = z(shoulderDiff) / magnitude(shoulderDiff);
	// Constrain the component value by 3 values input by 3 trackers.
	yawTwist = constrain(yawTwist, yawTwistD, yawTwistR, yawTwistG, true) / tracker("YawTS", 29, 1.f, 1.f, 20);

	// Combine all 3 components into the final yaw value.
	mYaw = yawCameraR + yawCameraL + yawLean + yawTwist;
	mCanYaw = (cameraActive && (yawCameraR + yawCameraL) != 0.f) || yawLean != 0.f || yawTwist != 0.f;

	Scalar flyUpD = tracker("FlyUpD", 120, 1.f, 0.f, 65);
	Scalar flyUpR = tracker("FlyUpR", 120, 1.f, 0.f, 50);
	Scalar flyDownD = tracker("FlyDownD", 120, 1.f, 0.f, 45);
	Scalar flyDownR = tracker("FlyDownR", 120, 1.f, 0.f, 15);

	//Fly
	NuiLib::Vector armR = shoulderR - handR;
	NuiLib::Vector vPlaneR = limit(armR, false, true, true);
	// The angle between normal and the vector between the shoulder and the hand.
	Scalar flyR = NuiLib::acos(NuiLib::dot(normalize(vPlaneR), normal));
	// Constrain the positive angle to go up past vertical.
	Scalar upR = constrain(flyR * R2DEG, flyUpD, flyUpR, 0.f, true); //Constraints if R is raised
	// Constrain the negative angle to stop before vertical so that hands lying by the side doesn't trigger flying down.
	Scalar downR = constrain(flyR * R2DEG, flyDownD, flyDownR, 0.f, true); //Constraints if R is lowered
	// Whether the arm is raised or lowered.
	Condition dirR = NuiLib::x(cross(normal, vPlaneR)) >= 0; 
	// Whether R is in range to fly
	Condition flyCondR = (magnitude(vPlaneR) > 0.f) && ((dirR && (upR > 0)) || ((!dirR) && (downR > 0))); 

	NuiLib::Vector armL = shoulderL - handL;
	NuiLib::Vector vPlaneL = limit(armL, false, true, true);
	// The angle between normal and the vector between the shoulder and the hand.
	Scalar flyL = NuiLib::acos(NuiLib::dot(normalize(vPlaneL), normal)) * R2DEG;
	// Constrain the positive angle to go up past vertical.
	Scalar upL = constrain(flyL, flyUpD, flyUpR, 0.f, true); //Constraints if L is raised
	// Constrain the negative angle to stop before vertical so that hands lying by the side doesn't trigger flying down.
	Scalar downL = constrain(flyL, flyDownD, flyDownR, 0.f, true); //Constraints if L is lowered
	// Whether the arm is raised or lowered.
	Condition dirL = NuiLib::x(cross(normal, vPlaneL)) >= 0; 
	// Whether L is in range to fly
	Condition flyCondL = magnitude(vPlaneL) > 0.f && (dirL && upL > 0.f) || (!dirL && downL > 0.f); 

	//Up trumps down
	mFly = (dirR && flyCondR) || (dirL && flyCondL);
	// Fly if camera is inactive and flying with right or left arm
	mCanFly = (flyCondR && !cameraActiveR) || (flyCondL && !cameraActiveL);

	Scalar pushThresh = tracker("PushThreshold", 30, .05f, .0f, 9);
	Condition pushR = z(shoulderR) - z(handR) > pushThresh; 
	Condition pushL = z(shoulderL) - z(handL) > pushThresh; 
	mPush = (pushR && !cameraActiveR) || (pushL && !cameraActiveL);

	mCanMove = mPush || mCanYaw || mCanPitch || mCanFly;
}
