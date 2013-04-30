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
#include <opencv\cv.h>
#include <functional>
#include <iostream>
#include <list>
#include <map>

#include <windows.h>
#include <pshpack8.h>
#include <poppack.h>
#include <sal.h>
#include <oaidl.h>
#include <ocidl.h>
#include <rpc.h>
#include <rpcndr.h>

#undef NULL
#define NULL 0
#include <NuiLib-Kinect-MS.h>
#include <NuiAPI.h>
#include <NuiLib-Vectors.h>
#include <NuiLib-Scalars.h>
#include <NuiLib-Conditions.h>

#ifndef KINECT_MS_API_H
#define KINECT_MS_API_H

#define NUILIB_KINECTMS_HIP_CENTER "HIP_CENTER"
#define NUILIB_KINECTMS_SPINE "SPINE"
#define NUILIB_KINECTMS_SHOULDER_CENTER "SHOULDER_CENTER"
#define NUILIB_KINECTMS_HEAD "HEAD"
#define NUILIB_KINECTMS_SHOULDER_LEFT "SHOULDER_LEFT"
#define NUILIB_KINECTMS_ELBOW_LEFT "ELBOW_LEFT"
#define NUILIB_KINECTMS_WRIST_LEFT "WRIST_LEFT"
#define NUILIB_KINECTMS_HAND_LEFT "HAND_LEFT"
#define NUILIB_KINECTMS_SHOULDER_RIGHT "SHOULDER_RIGHT"
#define NUILIB_KINECTMS_ELBOW_RIGHT "ELBOW_RIGHT"
#define NUILIB_KINECTMS_WRIST_RIGHT "WRIST_RIGHT"
#define NUILIB_KINECTMS_HAND_RIGHT "HAND_RIGHT"
#define NUILIB_KINECTMS_HIP_LEFT "HIP_LEFT"
#define NUILIB_KINECTMS_KNEE_LEFT "KNEE_LEFT"
#define NUILIB_KINECTMS_ANKLE_LEFT "ANKLE_LEFT"
#define NUILIB_KINECTMS_FOOT_LEFT "FOOT_LEFT"
#define NUILIB_KINECTMS_HIP_RIGHT "HIP_RIGHT"
#define NUILIB_KINECTMS_KNEE_RIGHT "KNEE_RIGHT"
#define NUILIB_KINECTMS_ANKLE_RIGHT "ANKLE_RIGHT"
#define NUILIB_KINECTMS_FOOT_RIGHT "FOOT_RIGHT"

using namespace NuiLib;

namespace NuiLib {
	class DLL KinectFactory;

	template <typename T> class IListener {
		public:
			///
			/// Process a frame.
			///
			virtual void Update(T *) = 0;
			///
			/// What to do when no frame is recieved.
			///
			virtual void Update() = 0;
	};

	class DLL KinectJoint : public IListener<NUI_SKELETON_DATA>, public JointVector {
		private:
			NUI_SKELETON_POSITION_INDEX _index;
			bool _updating;

		public:
			KinectJoint();
			KinectJoint(NUI_SKELETON_POSITION_INDEX index);

			void Update();
			void Update(NUI_SKELETON_DATA *skeleton);

			void SetJoint(string joint);
			void SetIndex(NUI_SKELETON_POSITION_INDEX);

			string ToXML();
			void LoadXML(string);

			static string NameFromIndex(NUI_SKELETON_POSITION_INDEX index);
			static NUI_SKELETON_POSITION_INDEX IndexFromName(string name);

			static string GetTypeName() { return "KinectJoint"; }

			friend class KinectFactory;
	};

	class DLL KinectFactory : public INuiFactoryExtension {
		private:
			bool _pollOnly;
			bool _polling;
			bool _initialised;

			int _numEvents;
			HANDLE *_hEvents;
			HANDLE _eventHandles[3];
			bool _enabledEvents[3];

			list<IListener<NUI_SKELETON_DATA>*> _skeletonListeners;
			list<IListener<NUI_IMAGE_FRAME>*> _colourListeners;
			list<IListener<NUI_IMAGE_FRAME>*> _depthListeners;

			list<INuiListener*> _nuiListeners;

			INuiSensor* _pNuiSensor;
			HANDLE _pDepthStreamHandle;
			HANDLE _pColourStreamHandle;

			DWORD _currentSkeleton;
			cv::Mat _debugFrame;
			cv::Mat _colourFrame;
			cv::Mat _depthFrame;

			NUI_IMAGE_VIEW_AREA _viewArea;

			void InitEvents();
			void Start();
			void Stop();
			void ProcessSkeletons(NUI_SKELETON_FRAME &);
			void ProcessSkeleton(NUI_SKELETON_DATA *);
			void ProcessLostSkeleton();
			void ProcessDepth(NUI_IMAGE_FRAME *);
			void ProcessColour(NUI_IMAGE_FRAME *);

			void EnableColour(bool);
			void EnableDepth(bool);
			void EnableSkeleton(bool);

			void Poll(bool fromExternal);

		protected:
			bool Triggers();

		public:
			KinectFactory();
			virtual ~KinectFactory();

			bool Init();
			bool GetAutoPoll();
			void SetAutoPoll(bool);
			void Poll();

			void AddSkeletonListener(IListener<NUI_SKELETON_DATA> *);
			void AddColourListener(IListener<NUI_IMAGE_FRAME> *);
			void AddDepthListener(IListener<NUI_IMAGE_FRAME> *);


			///
			/// Add a listener that will be notified of NUI events (Tick, SkeletonFound, SkeletonLost, SkeletonSwitched).
			///
			void AddNuiListener(INuiListener *listener);
			void RemoveNuiListener(INuiListener *listener);

			///
			/// Returns true if there is currently a skeleton being tracked. False otherwise.
			///
			bool TrackingSkeleton();

			static DWORD WINAPI Nui_ProcessThread (LPVOID p);
			DWORD WINAPI Nui_ProcessThread();

			///
			/// @return An array with of USHORTs (16bit unsigned integers). Any calls to mat.at<T> should use .at<USHORT>.
			///
			const cv::Mat GetDepth();
			const cv::Mat GetColour();
#ifdef VISUAL
			cv::Mat &GetDebugFrame();
#endif		

			bool HasColour();
			bool HasDepth();
			bool HasSkeleton();

			void Dispose();

			cv::Point SkeletonToDepth(Vector point);
			cv::Point SkeletonToColour(Vector point);

			cv::Point3f DepthToSkeleton(cv::Point point);
			cv::Point3f ColourToSkeleton(cv::Point point);

			string ToXML();
			void LoadXML(string);

			void **GetNuiDevices();

			void Init(void *item);

			Vector joint(const int joint);	
	};
}
#endif
