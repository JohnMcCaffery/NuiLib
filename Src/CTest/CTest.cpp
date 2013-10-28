#include<NuiLib-API.h>
#include<NuiLib-Kinect-MS.h>
#include<NuiLib-CLRSafe.h>
#include<NuiLib-Extendable.h>
#include<NuiLib-Scalars.h>
#include<opencv\highgui.h>
#include<iostream>

using namespace std;
using namespace NuiLib;

void init(bool autoenable);
Vector armR(-1);
Condition walkActiveR;
Scalar yawLean;
Scalar yawTwist;
Scalar yawArm;
Scalar yawArmL;
Scalar yawArmR;
Scalar mYaw;
Scalar mYawTwist;
Scalar yawTwistDot;
Scalar mYawArm;
Scalar walkDiffR;
Vector shoulderR (1);
Vector shoulderL (1);
Vector diff(1);
Vector Forward(1);
Condition mFlyActive;
Condition mFlyUp;
Scalar flyAngleR;
Scalar flyAngleL;
Scalar flyAngleL_c;
Condition zombieActiveR;
Condition zombieActiveL;
Scalar zombieAngleR;
Scalar zombieAngleL;


int main(int argc, char** argv) {
	//Register the Microsoft Kinect factory as the factory to use
	/*NuiLib::RegisterFactory();

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
	});*/

	init(true);

	//Wait for user input to stop the program.
#ifdef VISUAL
	cv::waitKey();
#else
	string waitstr = " ";
	cin >> waitstr;
#endif

	return 0;
}

void init(bool autoenable)
{
	RegisterFactory();

	if (!  NuiFactory()->Init()) {
		//mDriverState = NUI_UNINITIALIZED;
		return;
	}
	//mDriverState = NUI_INITIALIZED;


	
	//Get the primary vectors.
	Vector shoulderC = joint(SHOULDER_CENTER);
	shoulderR = joint(SHOULDER_RIGHT);
	shoulderL = joint(SHOULDER_LEFT);
	Vector elbowR = joint(ELBOW_RIGHT);
	Vector elbowL = joint(ELBOW_LEFT);
	Vector wristR = joint(WRIST_RIGHT);
	Vector wristL = joint(WRIST_LEFT);
	Vector handR = joint(HAND_RIGHT);
	Vector handL = joint(HAND_LEFT);
	Vector hipC = joint(HIP_CENTER);
	Vector hipL = joint(HIP_LEFT);
	Vector hipR = joint(HIP_RIGHT);
	Vector head = joint(HEAD);
	//Condition guard = closeGuard();

	armR = handR - shoulderR;
	Vector armL = handL - shoulderL;

	Condition activeR = y(handR) > y(hipR);
	Condition activeL = y(handL) > y(hipL);

	//----------- Walk----------- 
	Scalar walkThreshold = tracker("WalkStart", 1.f, 0.f, .5f);
	//Left and right
	walkDiffR = z(handR - hipC);
	/*walkDiffR.AddListener([](NuiLib::IObservable *s) { 
		cout << "armR: " << walkDiffR.Get() << '\n';
	});*/
	Scalar walkDiffL = z(handL - hipC);
	//Active
	walkActiveR = (abs(walkDiffR) > walkThreshold && walkDiffR < 0.f) || (walkDiffR > walkThreshold / 5.f);
	Condition walkActiveL = (abs(walkDiffL) > walkThreshold && walkDiffL < 0.f) || (walkDiffL > walkThreshold / 5.f);
	Condition mWalkActive = walkActiveR || walkActiveL;
	//Direction
	Condition mForward = (walkActiveR && walkDiffR < 0.f) || (walkActiveL && walkDiffL < 0.f);

	/*armR.AddListener([](NuiLib::IObservable *s) { 
		cout << "armR: " << armR.Z() << "--" << walkActiveR.Get() << '\n';
	});*/

	//----------- Fly----------- 
	Scalar flyThreshold =  tracker("FlyStart", 180.0, 0.f, 25.f);
	Scalar flyMax =  tracker("FlyMax", 180.0, 0.f, 90.f);
	Scalar flyGrace = tracker("FlyGrace", 180.0, 0.f, 0.f);
	Vector Y = Vector(0.f, 1.f, 0.f);
	flyAngleR = constrain(signedAngle(Vector(1.f, 0.f, 0.f), armR, 2), flyThreshold * M_PI/180, flyMax * M_PI/180, flyGrace * M_PI/180, true);
	flyAngleL = constrain(signedAngle(Vector(-1.f, 0.f, 0.f), armL, 2) * -1, flyThreshold * M_PI/180, flyMax * M_PI/180, flyGrace * M_PI/180, true);

	Condition flyActiveR = activeR && flyAngleR != 0.f && magnitude(armR) - magnitude(limit(armR, true, true, false)) < .1f;
	Condition flyActiveL = activeL && flyAngleL != 0.f && magnitude(armL) - magnitude(limit(armL, true, true, false)) < .1f;
	mFlyActive = (flyActiveR || flyActiveL) /*&& z(armR) < 0.f && z(armL) < 0.f*/;
	mFlyUp = (flyActiveR && flyAngleR > 0.f) || (flyActiveL && flyAngleL > 0.f);
	//mFlyActive = flyActiveR;
	//mFlyUp = (flyActiveR && flyAngleR > 0.f);
	/*mFlyActive.AddListener([](NuiLib::IObservable *s) { 
		cout << "mFlyActive: " << mFlyActive.Get()  << " mFlyUp: " << mFlyUp.Get() << '\n';
	});
	mFlyUp.AddListener([](NuiLib::IObservable *s) { 
		cout << "mFlyActive: " << mFlyActive.Get()  << " mFlyUp: " << mFlyUp.Get() << '\n';
	});*/


	//----------- Yaw----------- 
	Scalar yawLeanD = tracker("YawLeanStart", 180.f, 0.f, 5.f);
	Scalar yawLeanSpeed = tracker("YawSpeed", 180.f, 0.f, 0.01f);
	Scalar yawLeanRange = tracker("YawLeanRange", 180.f, 0.f, 23.0f);
	Scalar yawLeanGrace = tracker("YawLeanGrace", 180.f, 0.f, 90.0f);
	Vector yawCore = limit(head - hipC, true, true, false);
	// Yaw is how far the user is leaning horizontally. This is calculated the angle between vertical and the vector between the hip centre and the head.
	yawLean = signedAngle(yawCore, Vector(0.f, 1.f, 0.f), 2);
	/*yawLean.AddListener([](NuiLib::IObservable *s) { 
		cout << "yawLean: " << yawLean.Get() << " mYaw: " << mYaw.Get() << '\n';
	});*/
	// Constrain the value, deadzone is provided by a slider.
	mYaw = constrain(yawLean,  yawLeanD * M_PI/180, yawLeanRange * M_PI/180, yawLeanGrace * M_PI/180, true)* yawLeanSpeed;


	Scalar yawTwistD =  tracker("YawTwistD", 180.f, 0.f, 20.f);
	Scalar yawTwistSpeed =  tracker("YawTwistSpeed", 100.f, 1.f, 1.f);
	Vector X = Vector("X", 0.f, 0.f, 1.f);
	diff = normalize(limit(shoulderL, true, false, true) - limit(shoulderR, true, false, true));
	//diff = Vector(-0.1f, 0.f, 1.2f) - Vector(0.1f, 0.f, 1.2f);
	yawTwist = signedAngle(Vector(-1.f, 0.f, 0.f), diff, 1);
	mYawTwist = constrain(yawTwist, yawTwistD * M_DEGTORAD, .5f, .1f, true) / yawTwistSpeed;
	/*yawLean.AddListener([](NuiLib::IObservable *s) { 
		cout << "yawTwist: " << yawTwist.Get() << " -- " << mYawTwist.Get() << '\n';

	});*/


	
	Vector hip_L = normalize(shoulderC - hipL);
	Vector hip_R = normalize(shoulderC - hipR);
	Vector armRL = limit(armR, true, false, true);
	Vector armLL = limit(armL, true, false, true);
	Forward = normalize(limit(cross(hip_R, hip_L), true, false, true));
	yawArmL = signedAngle(Forward,  armLL, 1);
	yawArmR = signedAngle(Forward, armRL, 1);

	/*yawArmL.AddListener([](NuiLib::IObservable *s) { 
		cout << "yawArmL: " << yawArmL.Get() << " yamArmR: " << yawArmR.Get()
			<< '\n';
	});*/
	mYawArm = yawArmL + yawArmR;
	yawArm = ifScalar(walkActiveL, yawArmL, 0.f) + ifScalar(walkActiveR, yawArmR, 0.f);
	/*yawArm.AddListener([](NuiLib::IObservable *s) { 
		cout << "WalkActiveR: " << *walkActiveR << " - yawArm: " << yawArm.Get() << '\n';
	});*/

	Scalar zombierange = tracker("YawTwistD", 180.f, 0.f, 20.f);
	zombieAngleR = constrain(signedAngle(Vector(0.f, 0.f, -1.f), armR, 0), 0.f, zombierange * M_PI/180, 0.f, true);
	zombieAngleL = /*constrain(*/signedAngle(Vector(0.f, 0.f, -1.f), armL, 0)/*, 0.f, zombierange * M_PI/180, 0.f, true)*/;

	zombieActiveR = abs(zombieAngleR) < 30 * M_PI/180;
	zombieActiveL = abs(zombieAngleL) < 30 * M_PI/180;
	zombieActiveL.AddListener([](NuiLib::IObservable *s) { 
		cout << "zombieActiveL: " << zombieActiveL.Get() << " - zombieAngleL: " << zombieAngleL.Get() << '\n';
	});
	/*zombieAngleL.AddListener([](NuiLib::IObservable *s) { 
		cout << "zombieAngleL: " << zombieAngleL.Get() << '\n';
	});*/


	Condition  mYawActive = mYaw != 0.f;

	Condition  mMoveActive = mWalkActive || mFlyActive || mYawActive;



	/*mLClick = fist(true);
	mLClick.OnTrue([](IObservable *s) {
		//LLMouseHandler* mouse_captor = gFocusMgr.getMouseCapture();
		//if( mouse_captor ) {
			//S32 x, y;
			//LLUI::getMousePositionScreen(&x, &y);
			//S32 local_x, local_y;
			//mouse_captor->screenPointToLocal( x, y, &local_x, &local_y );
			////if (LLView::sDebugMouseHandling) {
				//llinfos << "left" << " Kinect " << "click" << " handled by captor " << mouse_captor->getName() << llendl;
			//}
			//return mouse_captor->handleAnyMouseClick(local_x, local_y, gKeyboard->currentMask(TRUE), LLMouseHandler::CLICK_LEFT, false);
		//}
	});*/

	NuiFactory()->SetAutoPoll(true);
}

