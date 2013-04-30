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
#include "NuiLib-Kinect-MS-API.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>

#undef NULL
#define NULL 0

const double DEPTH_SCALE_FACTOR = 255./65535.;

const int COLOUR = 0;
const int DEPTH = 1;
const int SKELETON = 2;

using namespace NuiLib;

KinectFactory _factory;

/*
INuiFactoryExtension *NuiLib::ExtensionFactory() { 
	return &_factory;
}

INuiFactory *NuiLib::NuiFactory() { 
	return &_factory;
}
*/

void NuiLib::InitKinectMS() {
	SetFactory(&_factory);
}

Vector makeJoint(const int index) {
	KinectJoint *joint = ExtensionFactory()->Make<KinectJoint>(KinectJoint::NameFromIndex((NUI_SKELETON_POSITION_INDEX)index));
	joint->SetIndex((NUI_SKELETON_POSITION_INDEX) index);
	return Vector(joint);
}

const NUI_IMAGE_RESOLUTION DEPTH_RES = NUI_IMAGE_RESOLUTION_640x480;
const NUI_IMAGE_RESOLUTION COLOUR_RES = NUI_IMAGE_RESOLUTION_640x480;


//Init
KinectFactory::KinectFactory() : 
INuiFactoryExtension("KinectFactory", "KinectFactory"),
	_pollOnly(true),
	_polling(false),
	_initialised(false),
	_numEvents(0),
	_skeletonListeners(), 
	_colourListeners(),
	_depthListeners(),
	_pNuiSensor(NULL),
	_currentSkeleton(0),
	_debugFrame(YRES, XRES, CV_8UC1, cv::Scalar(0.)),
	_colourFrame(YRES, XRES, CV_8UC4, cv::Scalar(0.)),
	_depthFrame(YRES, XRES, CV_16UC1)
{
	function<IVector *()> kinectJointCreator = [this]() -> IVector* { 
		KinectJoint *j = new KinectJoint();
		j->SetName(j->GetJoint());
		AddSkeletonListener(j);
		return j;
	};

	RegisterCreator(KinectJoint::GetTypeName(), kinectJointCreator);
	RegisterCreator(JointVector::GetTypeName(), kinectJointCreator);
}

void KinectFactory::Dispose() {
	//INuiFactoryExtension::Clear();
	_polling = false;
	if (_pNuiSensor != NULL) {
		_pNuiSensor->Release();
	}
}

bool KinectFactory::Init() {
	if (_initialised) {
		cout << "NuiFactory already initialised. Ignored Init request.\n";
		return false;
	}
	for (int i = 0; i < 3; i++) 
		_enabledEvents[i] = false;

	HRESULT hr = NuiCreateSensorByIndex(0, &_pNuiSensor);

	if (_pNuiSensor == NULL || hr) {
		cout << "Unable to get sensor.\n";
		return false;
	}

	_eventHandles[SKELETON] = CreateEvent( NULL, true, false, NULL );
	_eventHandles[COLOUR] = CreateEvent( NULL, true, false, NULL );
	_eventHandles[DEPTH] = CreateEvent( NULL, true, false, NULL );

	DWORD nuiFlags = NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_SKELETON | NUI_INITIALIZE_FLAG_USES_COLOR; 
	hr = _pNuiSensor->NuiInitialize(nuiFlags);    
	if (E_NUI_SKELETAL_ENGINE_BUSY == hr) {
		nuiFlags = NUI_INITIALIZE_FLAG_USES_DEPTH;
		hr = _pNuiSensor->NuiInitialize(nuiFlags);
	}

	if (!hr) {
		_initialised = true;
		cout << "Nui Initialised.\n";

		if (!_skeletonListeners.empty() > 0)
			EnableSkeleton(true);
		if (!_colourListeners.empty() > 0)
			EnableColour(true);
#ifndef VISUAL
		if (!_depthListeners.empty() > 0)
#endif
			EnableDepth(true);
	} else
		cout << "Unable to Initialise Sensor.\n";

#ifdef VISUAL
	cv::namedWindow("Depth", CV_WINDOW_NORMAL);
	cv::imshow("Depth", _debugFrame);
#endif

	return _initialised;

}

void KinectFactory::InitEvents() {
	delete _hEvents;
	_hEvents = new HANDLE[_numEvents];
	int event = 0;
	for (int eventType = 0; eventType < 3; eventType++) {
		if (_enabledEvents[eventType])
			_hEvents[event++] = _eventHandles[eventType];
		if (event == _numEvents)
			break;
	}
}

void KinectFactory::EnableSkeleton(bool enable) {
	if (!_initialised)
		return;
	if (enable && !_enabledEvents[SKELETON]) {
		HRESULT hr = _pNuiSensor->NuiSkeletonTrackingEnable(_eventHandles[SKELETON], 0);
		if(!hr) {
			_enabledEvents[SKELETON] = true;
			_numEvents++;
			InitEvents();

			cout << "Nui tracking skeletons\n";
		} else
			cout << "Nui not tracking skeletons\n";
	} else if (_enabledEvents[SKELETON]) {
		HRESULT hr = _pNuiSensor->NuiSkeletonTrackingDisable();
		_numEvents--;
		if (!hr) 
			cout << "Nui disabled tracking skeletons\n";
		else
			cout << "Nui failed to disable tracking skeletons\n";
	}
}

void KinectFactory::EnableColour(bool enable) {
	if (!_initialised)
		return;

	if (enable && !_enabledEvents[COLOUR]) {
		HRESULT hr = _pNuiSensor->NuiImageStreamOpen(
			NUI_IMAGE_TYPE_COLOR,
			COLOUR_RES,
			0,
			2,
			_eventHandles[COLOUR],
			&_pColourStreamHandle );		
		if(!hr) {
			_enabledEvents[COLOUR] = true;
			_numEvents++;
			InitEvents();

			cout << "Nui tracking colour\n";
		} else
			cout << "Nui unable to track colour\n";
	} else if (_enabledEvents[COLOUR]) {
		_numEvents--;
		cout << "Nui failed to disable tracking colour\n";
	}
}

void KinectFactory::EnableDepth(bool enable) {
	if (!_initialised)
		return;

	if (enable && !_enabledEvents[DEPTH]) {
		HRESULT hr = _pNuiSensor->NuiImageStreamOpen(
			NUI_IMAGE_TYPE_DEPTH,
			DEPTH_RES,
			0,
			2,
			_eventHandles[DEPTH],
			&_pDepthStreamHandle );		
		if(!hr) {
			_enabledEvents[DEPTH] = true;
			_numEvents++;
			InitEvents();

			cout << "Nui tracking depth\n";
		} else
			cout << "Nui unable to track depth\n";
	} else if (_enabledEvents[DEPTH]) {
		_numEvents--;
		cout << "Nui disabled tracking depth\n";
	}
}

void KinectFactory::Start() {
	if (_initialised)
		CreateThread(NULL, 0,Nui_ProcessThread, this, 0, NULL);
}

//Teardown
KinectFactory::~KinectFactory() {
	Stop();

	if (_pNuiSensor) {
		_pNuiSensor->Release();
		_pNuiSensor = NULL;
	}

	delete _hEvents;

	cout << "Factory destroyed\n";

}

void KinectFactory::Stop() {
	_polling = false;
}

//Inherited

bool KinectFactory::Triggers() { return true; }

void KinectFactory::AddSkeletonListener(IListener<NUI_SKELETON_DATA> *listener) {
	if (_skeletonListeners.empty())
		EnableSkeleton(true);

	_skeletonListeners.push_back(listener);
}

void KinectFactory::AddDepthListener(IListener<NUI_IMAGE_FRAME> *listener) {
	if (_depthListeners.empty())
		EnableDepth(true);

	_depthListeners.push_back(listener);
}

const cv::Mat KinectFactory::GetColour() { 
	if (!_enabledEvents[COLOUR])
		EnableColour(true);
	return _colourFrame; 
}
const cv::Mat KinectFactory::GetDepth() { 
	if (!_enabledEvents[DEPTH])
		EnableDepth(true);
	return _depthFrame; 
}
#ifdef VISUAL
cv::Mat &KinectFactory::GetDebugFrame() { 
	if (!_enabledEvents[DEPTH])
		EnableDepth(true);
	return _debugFrame; 
}
#endif

void KinectFactory::AddNuiListener(INuiListener *listener) {
	auto togo = _nuiListeners.end();
	for (auto i = _nuiListeners.begin(); i != _nuiListeners.end(); i++) {
		if ((*i) == listener) {
			togo = i;
			break;
		}
	}

	if (togo == _nuiListeners.end())
		_nuiListeners.push_back(listener);
}
void KinectFactory::RemoveNuiListener(INuiListener *listener) {
	auto togo = _nuiListeners.end();
	for (auto i = _nuiListeners.begin(); i != _nuiListeners.end(); i++) {
		if ((*i) == listener) {
			togo = i;
			break;
		}
	}

	if (togo != _nuiListeners.end())
		_nuiListeners.erase(togo);
}

bool KinectFactory::HasColour() { return false; }
bool KinectFactory::HasDepth() { return false; }
bool KinectFactory::HasSkeleton() { return _currentSkeleton > 0; }

cv::Point KinectFactory::SkeletonToDepth(Vector point) {
	//TODO - test this! / Error handling
	LONG x, y;
	USHORT z;
	Vector4 v;
	v.x = point.X();
	v.y = point.Y();
	v.z = point.Z();
	v.w = 1.f;
	NuiTransformSkeletonToDepthImage(v, &x, &y, &z, DEPTH_RES);
	return cv::Point(x, y);
}
cv::Point KinectFactory::SkeletonToColour(Vector point) { 
	//TODO - test this! / Error handling
	cv::Point p = SkeletonToDepth(point);
	LONG x, y;
	USHORT z = (USHORT) point.Z();
	NuiImageGetColorPixelCoordinatesFromDepthPixelAtResolution(COLOUR_RES, DEPTH_RES, &_viewArea, p.x, p.y, z, &x, &y);
	return cv::Point(x, y);
}

cv::Point3f KinectFactory::DepthToSkeleton(cv::Point point) { 
	//TODO - test this! / Error handling
	USHORT z = _depthFrame.at<USHORT>(point);
	Vector4 v = NuiTransformDepthImageToSkeleton( point.x,  point.y, z, DEPTH_RES);
	return cv::Point3f(v.x, v.y, v.z);
}
cv::Point3f KinectFactory::ColourToSkeleton(cv::Point point) { 
	return DepthToSkeleton(point);
}

bool KinectFactory::GetAutoPoll() { return _pollOnly; }
void KinectFactory::SetAutoPoll(bool value){
	_pollOnly = !value; 
	if (value && !_polling)
		Start();
	else if (!value && _polling)
		Stop();
}

bool newDebug = true;
bool skeletonLost = true;

DWORD skeletonFrame;
DWORD colourFrame;
DWORD depthFrame;

void KinectFactory::Poll() {
	Poll(true);
}
void KinectFactory::Poll(bool fromExternal) {
	if (fromExternal && _polling)
		return;
	WaitForMultipleObjects(_numEvents, _hEvents, false, 1000);

	bool depthUpdated = false;
	if (_enabledEvents[COLOUR] && _pNuiSensor) {
		//New Colour Frame
		NUI_IMAGE_FRAME frame;
		HRESULT hr = _pNuiSensor->NuiImageStreamGetNextFrame(_pColourStreamHandle, 0, &frame);
		if (hr >= 0 && frame.dwFrameNumber > colourFrame) {
			colourFrame = frame.dwFrameNumber;
			ProcessColour(&frame);
			if (_pNuiSensor)
				_pNuiSensor->NuiImageStreamReleaseFrame(_pColourStreamHandle, &frame);
		}
	} 
	if (_enabledEvents[DEPTH] && _pNuiSensor) {
		//New Depth Frame
		NUI_IMAGE_FRAME frame;
		HRESULT hr = _pNuiSensor->NuiImageStreamGetNextFrame(_pDepthStreamHandle, 0, &frame);
		if (hr >= 0 && frame.dwFrameNumber > depthFrame) {
			depthUpdated = true;
			depthFrame = frame.dwFrameNumber;
			ProcessDepth(&frame);
			if (_pNuiSensor)
				_pNuiSensor->NuiImageStreamReleaseFrame(_pDepthStreamHandle, &frame);
		}
	}
	if (_enabledEvents[SKELETON] && _pNuiSensor) {
		//New Skeleton Frame
		NUI_SKELETON_FRAME frame;
		HRESULT hr = _pNuiSensor->NuiSkeletonGetNextFrame(0, &frame);
		if (hr >= 0  && (frame.dwFrameNumber > skeletonFrame || depthUpdated)) {
			if (!depthUpdated)
				_debugFrame = _depthFrame.clone();
			skeletonFrame = frame.dwFrameNumber;
			ProcessSkeletons(frame);
		}
	} 

	if (fromExternal || (!fromExternal && _polling)) {
		Trigger();
		for (auto i = _nuiListeners.begin(); i != _nuiListeners.end(); i++)
			(*i)->Tick();
	}

#ifdef VISUAL
	if (_enabledEvents[DEPTH]) {
		cv::imshow("Depth", _debugFrame);
		newDebug = true;
	}
#endif		
}


//Threading

DWORD WINAPI KinectFactory::Nui_ProcessThread (LPVOID p) {
	if (p == NULL)
		return 0;
	KinectFactory* pKinect = (KinectFactory*) p;
	return pKinect->Nui_ProcessThread();
}

DWORD WINAPI KinectFactory::Nui_ProcessThread() {
	_polling = true;
	cout << "\nThread running \n";

	while (_polling) {
		Poll(false);
		Sleep(5);
	}
	cout << "Thread stopped\n";
	return 0;
}

void KinectFactory::ProcessSkeletons(NUI_SKELETON_FRAME &frame) {
	//http://msdn.microsoft.com/en-us/library/jj131024.aspx
	//_pNuiSensor->NuiTransformSmooth(&frame, NULL);
	//const NUI_TRANSFORM_SMOOTH_PARAMETERS smooth = {
		//.8, .2, .2, .02, .05 
	//};
	//_pNuiSensor->NuiTransformSmooth(&frame, &smooth);
	int index = -1;
	//Iterate through every skeleton
	for (int i = 0; i < NUI_SKELETON_COUNT; i++) {
		//Check if the current skeleton is tracked
		if (frame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED) {
			//If this is the first tracked skeleton record it in index
			if (index == -1)
				index =  i;
			DWORD id = frame.SkeletonData[i].dwTrackingID;
			//If this skeleton is the currently tracked skeleton update it and return
			if (id != 0 && id == _currentSkeleton) {
				ProcessSkeleton(&frame.SkeletonData[i]);
				return;
			}
		}
	}
	//If this point is reached either the previously tracked skeleton has been lost
	//Or there wasn't a previously tracked skeleton
	if (index != -1) {
		//If this point is reached a new skeleton has been found
		if (skeletonLost) {
			//If there previously hadn't been a skeleton
			cout << "Skeleton found.\n";
			for (auto i = _nuiListeners.begin(); i != _nuiListeners.end(); i++)
				(*i)->SkeletonFound(index);
		} else {
			//If there was a skeleton before but that has been lost
			cout << "Skeleton switched.\n";
			for (auto i = _nuiListeners.begin(); i != _nuiListeners.end(); i++)
				(*i)->SkeletonSwitched(index);
		}
		skeletonLost = false;

		NUI_SKELETON_DATA *skeleton = &frame.SkeletonData[index];
		_currentSkeleton = skeleton->dwTrackingID;
		ProcessSkeleton(skeleton);
	} else if(!skeletonLost) {
		skeletonLost = true;
		//No skeleton was found and there had previously been a skeleton
		cout << "Skeleton lost.\n";
		for (auto i = _nuiListeners.begin(); i != _nuiListeners.end(); i++)
			(*i)->SkeletonLost(_currentSkeleton);
		ProcessLostSkeleton();
		_currentSkeleton = 0;
	} else {
		//No skeleton found, justl like the previous frame
		//Do nothing
	}
}

void KinectFactory::ProcessSkeleton(NUI_SKELETON_DATA *skeleton) {
	for (auto it = _skeletonListeners.begin(); it != _skeletonListeners.end(); it++) 
		(*it)->Update(skeleton);
}

void KinectFactory::ProcessLostSkeleton() {
	for (auto it = _skeletonListeners.begin(); it != _skeletonListeners.end(); it++) 
		(*it)->Update();
}

void KinectFactory::ProcessDepth(NUI_IMAGE_FRAME *frame) {
	NUI_LOCKED_RECT _lockedRect;
	HRESULT hr = frame->pFrameTexture->LockRect(0, &_lockedRect, NULL, 0);

	if (hr >= 0) {
		memcpy(_depthFrame.data, _lockedRect.pBits, _lockedRect.size);	
		frame->pFrameTexture->UnlockRect(0);
	}

#ifdef VISUAL
	if (newDebug) {
		newDebug = false;
		_depthFrame.convertTo(_debugFrame, CV_8UC1, DEPTH_SCALE_FACTOR);
		cv::cvtColor(_debugFrame, _debugFrame, CV_GRAY2RGB);
		//cv::imshow("Depth", _debugFrame);
	}
#endif

	for (auto it = _depthListeners.begin(); it != _depthListeners.end(); it++) 
		(*it)->Update(frame);
}

void KinectFactory::ProcessColour(NUI_IMAGE_FRAME *frame) {
	_viewArea = frame->ViewArea;
	NUI_LOCKED_RECT _lockedRect;
	HRESULT hr = frame->pFrameTexture->LockRect(0, &_lockedRect, NULL, 0);

	if (hr >= 0) {
		memcpy(_colourFrame.data, _lockedRect.pBits, _lockedRect.size);	
		frame->pFrameTexture->UnlockRect(0);
	}

	for (auto it = _colourListeners.begin(); it != _colourListeners.end(); it++) 
		(*it)->Update(frame);
}

string KinectFactory::ToXML() { return ""; }
void KinectFactory::LoadXML(string xml) { }

void **KinectFactory::GetNuiDevices() { return (void **) (&_pNuiSensor); }

Vector KinectFactory::joint(const int joint) {
	return makeJoint(joint);
}

//----------------------- KinectJoint -------------------------------

KinectJoint::KinectJoint() : JointVector(GetTypeName(), NUILIB_KINECTMS_HIP_CENTER), _index(NUI_SKELETON_POSITION_HIP_CENTER) { }
KinectJoint::KinectJoint(NUI_SKELETON_POSITION_INDEX index) : JointVector(GetTypeName(), NameFromIndex(index)), _index(index) { }

string KinectJoint::NameFromIndex(NUI_SKELETON_POSITION_INDEX index) {
	if (NUI_SKELETON_POSITION_HIP_CENTER == index)
		return NUILIB_KINECTMS_HIP_CENTER;
	else if (NUI_SKELETON_POSITION_SPINE == index)
		return NUILIB_KINECTMS_SPINE;
	else if (NUI_SKELETON_POSITION_SHOULDER_CENTER == index)
		return NUILIB_KINECTMS_SHOULDER_CENTER;
	else if (NUI_SKELETON_POSITION_HEAD == index)
		return NUILIB_KINECTMS_HEAD;
	else if (NUI_SKELETON_POSITION_SHOULDER_LEFT == index)
		return NUILIB_KINECTMS_SHOULDER_LEFT;
	else if (NUI_SKELETON_POSITION_ELBOW_LEFT == index)
		return NUILIB_KINECTMS_ELBOW_LEFT;
	else if (NUI_SKELETON_POSITION_WRIST_LEFT == index)
		return NUILIB_KINECTMS_WRIST_LEFT;
	else if (NUI_SKELETON_POSITION_HAND_LEFT == index)
		return NUILIB_KINECTMS_HAND_LEFT;
	else if (NUI_SKELETON_POSITION_SHOULDER_RIGHT == index)
		return NUILIB_KINECTMS_SHOULDER_RIGHT;
	else if (NUI_SKELETON_POSITION_ELBOW_RIGHT == index)
		return NUILIB_KINECTMS_ELBOW_RIGHT;
	else if (NUI_SKELETON_POSITION_WRIST_RIGHT == index)
		return NUILIB_KINECTMS_WRIST_RIGHT;
	else if (NUI_SKELETON_POSITION_HAND_RIGHT == index)
		return NUILIB_KINECTMS_HAND_RIGHT;
	else if (NUI_SKELETON_POSITION_HIP_LEFT == index)
		return NUILIB_KINECTMS_HIP_LEFT;
	else if (NUI_SKELETON_POSITION_KNEE_LEFT == index)
		return NUILIB_KINECTMS_KNEE_LEFT;
	else if (NUI_SKELETON_POSITION_ANKLE_LEFT == index)
		return NUILIB_KINECTMS_ANKLE_LEFT;
	else if (NUI_SKELETON_POSITION_FOOT_LEFT == index)
		return NUILIB_KINECTMS_FOOT_LEFT;
	else if (NUI_SKELETON_POSITION_HIP_RIGHT == index)
		return NUILIB_KINECTMS_HIP_RIGHT;
	else if (NUI_SKELETON_POSITION_KNEE_RIGHT == index)
		return NUILIB_KINECTMS_KNEE_RIGHT;
	else if (NUI_SKELETON_POSITION_ANKLE_RIGHT == index)
		return NUILIB_KINECTMS_ANKLE_RIGHT;
	else if (NUI_SKELETON_POSITION_FOOT_RIGHT == index)
		return NUILIB_KINECTMS_FOOT_RIGHT;
	return "";
}

NUI_SKELETON_POSITION_INDEX KinectJoint::IndexFromName(string name) {
	if (name.compare(NUILIB_KINECTMS_HIP_CENTER) == 0)
		return NUI_SKELETON_POSITION_HIP_CENTER;
	else if (name.compare(NUILIB_KINECTMS_SPINE) == 0)
		return NUI_SKELETON_POSITION_SPINE;
	else if (name.compare(NUILIB_KINECTMS_SHOULDER_CENTER) == 0)
		return NUI_SKELETON_POSITION_SHOULDER_CENTER;
	else if (name.compare(NUILIB_KINECTMS_HEAD) == 0)
		return NUI_SKELETON_POSITION_HEAD;
	else if (name.compare(NUILIB_KINECTMS_SHOULDER_LEFT) == 0)
		return NUI_SKELETON_POSITION_SHOULDER_LEFT;
	else if (name.compare(NUILIB_KINECTMS_ELBOW_LEFT) == 0)
		return NUI_SKELETON_POSITION_ELBOW_LEFT;
	else if (name.compare(NUILIB_KINECTMS_WRIST_LEFT) == 0)
		return NUI_SKELETON_POSITION_WRIST_LEFT;
	else if (name.compare(NUILIB_KINECTMS_HAND_LEFT) == 0)
		return NUI_SKELETON_POSITION_HAND_LEFT;
	else if (name.compare(NUILIB_KINECTMS_SHOULDER_RIGHT) == 0)
		return NUI_SKELETON_POSITION_SHOULDER_RIGHT;
	else if (name.compare(NUILIB_KINECTMS_ELBOW_RIGHT) == 0)
		return NUI_SKELETON_POSITION_ELBOW_RIGHT;
	else if (name.compare(NUILIB_KINECTMS_WRIST_RIGHT) == 0)
		return NUI_SKELETON_POSITION_WRIST_RIGHT;
	else if (name.compare(NUILIB_KINECTMS_HAND_RIGHT) == 0)
		return NUI_SKELETON_POSITION_HAND_RIGHT;
	else if (name.compare(NUILIB_KINECTMS_HIP_LEFT) == 0)
		return NUI_SKELETON_POSITION_HIP_LEFT;
	else if (name.compare(NUILIB_KINECTMS_KNEE_LEFT) == 0)
		return NUI_SKELETON_POSITION_KNEE_LEFT;
	else if (name.compare(NUILIB_KINECTMS_ANKLE_LEFT) == 0)
		return NUI_SKELETON_POSITION_ANKLE_LEFT;
	else if (name.compare(NUILIB_KINECTMS_FOOT_LEFT) == 0)
		return NUI_SKELETON_POSITION_FOOT_LEFT;
	else if (name.compare(NUILIB_KINECTMS_HIP_RIGHT) == 0)
		return NUI_SKELETON_POSITION_HIP_RIGHT;
	else if (name.compare(NUILIB_KINECTMS_KNEE_RIGHT) == 0)
		return NUI_SKELETON_POSITION_KNEE_RIGHT;
	else if (name.compare(NUILIB_KINECTMS_ANKLE_RIGHT) == 0)
		return NUI_SKELETON_POSITION_ANKLE_RIGHT;
	else if (name.compare(NUILIB_KINECTMS_FOOT_RIGHT) == 0)
		return NUI_SKELETON_POSITION_FOOT_RIGHT;
	return NUI_SKELETON_POSITION_COUNT;
}

void KinectJoint::Update() {
	if (_updating) {
		Set(0.f, 0.f, 0.f);
		_updating = false;
	}
}

void KinectJoint::Update(NUI_SKELETON_DATA *skeleton) {
	_updating = true;
	Vector4 vector = skeleton->SkeletonPositions[_index];
	Set(vector.x, vector.y, vector.z);
	float x, y;
	NuiTransformSkeletonToDepthImage(vector, &x, &y, DEPTH_RES);
#ifdef VISUAL
	cv::Scalar colour = skeleton->eSkeletonPositionTrackingState[_index] == NUI_SKELETON_POSITION_INFERRED ? cv::Scalar(255, 255, 0) : cv::Scalar(0, 0, 255);
	cv::circle(_factory.GetDebugFrame(), cv::Point((int)x, (int)y), 5, colour, 3);
#endif
}

void KinectJoint::SetJoint(string joint) {
	SetIndex(IndexFromName(joint));
}
void KinectJoint::SetIndex(NUI_SKELETON_POSITION_INDEX index) {
	_index = index;
}

string KinectJoint::ToXML() {
	return "";
}

void KinectJoint::LoadXML(string) {
}
