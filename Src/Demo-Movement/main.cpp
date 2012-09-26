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
		char buffer[999];
		// If any movement is to be done.
		if (*mCanMove) {
			// Print out FORWARD if that is enabled.
			SPRINTF(buffer, 999, "%-10s", (*mPush ? "FORWARD" :  ""));
			// If yaw is enabled print out the yaw value and give a visual representation.
			SPRINTF(buffer, 999, "%s %s %-25s", buffer, "Yaw:", (*mCanYaw ? mYaw._p->ToString(7).c_str() : ""));
			// If yaw is enabled print out the pitch value and give a visual representation.
			SPRINTF(buffer, 999, "%s %s %-25s", buffer, "Pitch:", (*mCanPitch ? mPitch._p->ToString(7).c_str() : ""));
			// Print out UP if flying up, down if flying down. Otherwise, nothing.
			SPRINTF(buffer, 999, "%s %s %-6s\n", buffer, "Fly:", (*mCanFly ? (*mFly ? "UP" : "down") : ""));
		} else 
			SPRINTF(buffer, 999, "");
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
	Vector wristR = joint(WRIST_RIGHT);
	Vector handR = joint(HAND_RIGHT);
	Vector hipC = joint(HIP_CENTER);
	Vector head = joint(HEAD);

	Vector yAxis = NuiLib::Vector("Y", 0.f, 1.f, 0.f);
	// Normal is the direction the camera is facing.
	Vector normal = Vector("Normal", 0, 0, 1);

	//Camera - If the right elbow is raised to be in line with the shoulders the camera is active.
	NuiLib::Vector upperArmCamera = elbowR - shoulderR;
	NuiLib::Vector lowerArmCamera = elbowR - wristR;
	Condition cameraActive = abs(x(upperArmCamera)) > (abs(y(upperArmCamera)) + abs(z(upperArmCamera))) * 2.f;

	//Push - If the right arm is extended forward push is true.
	NuiLib::Vector armRight = shoulderR - handR;
	NuiLib::Vector upperArmMove = elbowR - shoulderR;
	NuiLib::Vector lowerArmMove = wristR - elbowR;
	NuiLib::Vector palm = handR - wristR;
	
	Scalar armMax = magnitude(upperArmMove) + magnitude(lowerArmMove) + magnitude(palm);
	
	// Normalize the distance between the shoulder and the right hand against the total length of the arm (armMax).
	// Once normalized constrain the value between the input from tracker PushD (starting at .8) and 1. 
	// So if the normalized value is .9 the constrained value is .5. Alternatively if the normalized value is .8 the constrained value is 0.
	Scalar push = constrain(normalize(magnitude(armRight), armMax), tracker("PushD", 25, .04f, 0.f, 20), 1.f, 0.f, false);
	// If the camera is inactive and the push value is > 0 and the forward component is significantly larger than the horizontal and vertical components combined.
	mPush = !cameraActive && push > 0 && z(armRight) > ((abs(x(armRight) + abs(y(armRight)))) * tracker("PushActive", 9, .5f, .5f, 5));

	//Pitch
	Scalar pitchArmD = tracker("PitchArmD", 20, 1.f, 0.f, 10);
	Scalar pitchArmR = tracker("PitchArmR", 40, 2.f, 10.f, 17);
	Scalar pitchArmG = tracker("PitchArmG", 30, 1.f, 0.f, 15);
	
	Vector vPlaneCamera = NuiLib::limit(lowerArmCamera, false, true, true);
	// Pitch is the angle between normal and the vertical component of the vector between right shoulder and right hand.
	Scalar pitchArm = NuiLib::acos(dot(normalize(vPlaneCamera), normal)) * invert(x(cross(normal, vPlaneCamera)) >= 0);
	// Constrain the pitch value by 3 values input by 3 trackers.
	mPitch = constrain(pitchArm * R2DEG, pitchArmD, pitchArmR, pitchArmG, true) / tracker("PitchAS", 29, 1.f, 1.f, 20);

	mCanPitch = cameraActive && pitchArm != 0.f;

	//Yaw - Yaw has 3 components. The camera arm. The horizontal lean (head vs hip centre) and the twist of the shoulders.
	Scalar yawArmD = tracker("YawArmD", 20, 1.f, 0.f, 10);
	Scalar yawArmR = tracker("YawArmR", 40, 2.f, 10.f, 15);
	Scalar yawArmG = tracker("YawArmG", 20, 1.f, 0.f, 10);
	Scalar yawLeanD = tracker("YawLeanD", 20, .5f, 0.f, 10);
	Scalar yawLeanR = tracker("YawLeanR", 20, 1.f, 0.f, 15);
	Scalar yawLeanG = tracker("YawLeanG", 50, 1.f, 0.f, 30);
	Scalar yawTwistD = tracker("YawTwistD", 20, .05f, 0.f, 3);
	Scalar yawTwistR = tracker("YawTwistR", 20, 1.f, 0.f, 9);
	Scalar yawTwistG = tracker("YawTwistG", 20, 1.f, 0.f, 10);
	
	Vector hPlaneCamera = NuiLib::limit(lowerArmCamera, true, false, true);
	// Yaw component 1 is the angle between normal and the horizontal component of the vector between right shoulder and right hand.
	Scalar yawArm = NuiLib::acos(dot(normalize(hPlaneCamera), normal)) * invert(y(cross(normal, hPlaneCamera)) >= 0);
	// Constrain the component value by 3 values input by 3 trackers.
	yawArm = constrain(yawArm * R2DEG, yawArmD, yawArmR, yawArmG, true) / tracker("YawAS", 29, 1.f, 1.f, 20);

	Vector yawCore = limit(head - hipC, true, true, false);
	// Yaw component 2 is how far the user is leaning horizontally. This is calculated the angle between vertical and the vector between the hip centre and the head.
	Scalar yawLean = NuiLib::acos(dot(normalize(yawCore), yAxis)) * invert(z(cross(yawCore, yAxis)) >= 0);
	// Constrain the component value by 3 values input by 3 trackers.
	yawLean = constrain(yawLean * R2DEG, yawLeanD, yawLeanR, yawLeanG, true) / tracker("YawLS", 29, 1.f, 1.f, 20);
	
	Vector shoulderDiff = shoulderR - shoulderL;
	// Yaw component 3 is the twist of the shoulders. This is calculated as the angle between across the normal and the vector between the two shoulders.
	Scalar yawTwist = normalize(z(shoulderDiff), magnitude(shoulderDiff));
	// Constrain the component value by 3 values input by 3 trackers.
	yawTwist = constrain(yawTwist, yawTwistD, yawTwistR, yawTwistG, true) / tracker("YawTS", 29, 1.f, 1.f, 20);

	// Combine all 3 components into the final yaw value.
	mYaw = yawArm + yawLean + yawTwist;
	mCanYaw = (cameraActive && yawArm != 0.f) || yawLean != 0.f || yawTwist != 0.f;

	//Fly
	NuiLib::Vector vPlaneR = limit(armRight, false, true, true);
	// The angle between normal and the vector between the shoulder and the hand.
	Scalar flyR = NuiLib::acos(NuiLib::dot(normalize(vPlaneR), normal));
	// Constrain the positive angle to go up past vertical.
	Scalar upR = constrain(flyR * R2DEG, 65.f, 50.f, 0.f, true); //Constraints if R is raised
	// Constrain the negative angle to stop before vertical so that hands lying by the side doesn't trigger flying down.
	Scalar downR = constrain(flyR * R2DEG, 45.f, 15.f, 0.f, true); //Constraints if R is lowered
	// Whether the arm is raised or lowered.
	Condition dirR = NuiLib::x(cross(normal, vPlaneR)) >= 0; 
	// Whether R is in range to fly
	Condition flyCondR = (magnitude(vPlaneR) > 0.f) && ((dirR && (upR > 0)) || ((!dirR) && (downR > 0))); 
	mFly = dirR;
	// Fly if camera is inactive and 
	mCanFly = flyCondR && !cameraActive;

	mCanMove = mPush || mCanYaw || mCanPitch || mCanFly;
}
