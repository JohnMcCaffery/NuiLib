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
#include <NuiAPI.h>
#include <NuiLib-Vectors.h>
#include <NuiLib-Scalars.h>
#include <NuiLib-Conditions.h>

#ifndef KINECT_MS_H
#define KINECT_MS_H

#define HIP_CENTER "HIP_CENTER"
#define SPINE "SPINE"
#define SHOULDER_CENTER "SHOULDER_CENTER"
#define HEAD "HEAD"
#define SHOULDER_LEFT "SHOULDER_LEFT"
#define ELBOW_LEFT "ELBOW_LEFT"
#define WRIST_LEFT "WRIST_LEFT"
#define HAND_LEFT "HAND_LEFT"
#define SHOULDER_RIGHT "SHOULDER_RIGHT"
#define ELBOW_RIGHT "ELBOW_RIGHT"
#define WRIST_RIGHT "WRIST_RIGHT"
#define HAND_RIGHT "HAND_RIGHT"
#define HIP_LEFT "HIP_LEFT"
#define KNEE_LEFT "KNEE_LEFT"
#define ANKLE_LEFT "ANKLE_LEFT"
#define FOOT_LEFT "FOOT_LEFT"
#define HIP_RIGHT "HIP_RIGHT"
#define KNEE_RIGHT "KNEE_RIGHT"
#define ANKLE_RIGHT "ANKLE_RIGHT"
#define FOOT_RIGHT "FOOT_RIGHT"

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

	class KinectFactory : public INuiFactoryExtension {
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

			cv::Point SkeletonToDepth(Vector point);
			cv::Point SkeletonToColour(Vector point);

			cv::Point3f DepthToSkeleton(cv::Point point);
			cv::Point3f ColourToSkeleton(cv::Point point);

			string ToXML();
			void LoadXML(string);

			void **GetNuiDevices();

			void Init(void *item);
	};
}
#endif
