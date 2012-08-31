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

/*
The Hull method for fist detection is described in this paper: http://iss.bu.edu/data/jkonrad/reports/HDTT11-04buece.pdf and implemented in this example https://github.com/bmwesting/Kinect-Hand-Processing. The example is demonstrated in this youtube video. http://www.youtube.com/watch?v=NeHX5jzHFM4.
The source code implemented here is based on the algorithm described in the paper and used the source code from the example as a starting point.

The paper was written by Heng Du and TszHang To. The example is provided by Brandt Westing.
*/
#include <opencv/cv.h>
#include <NuiLib-API.h>
#include <NuiLib-Extendable.h>

#ifndef PROCESSING_H
#define PROCESSING_H

const int XRES = 640;
const int YRES = 480;

const USHORT HAND_OFFSET_MM = 20;
const USHORT HIP_OFFSET_MM = 250;

namespace NuiLib {
	enum Hand { RIGHT, LEFT };

	struct DLL SkeletonPoint {
		LONG x, y;
		USHORT z;
		SkeletonPoint(LONG X, LONG Y, USHORT Z);
	};

	class DLL HandProcessor : public IObservable, public IObserver {
	private:
		string _hand;

		int _numPoints;

		// area of hull and curve
		double _hullArea;
		double _curveArea;
		double _handRatio;
		bool _found;
		bool _initialised;

		bool _isRight;

		inline cv::Point displayShift(int w, cv::Point point);
		std::vector<cv::Point> adjustPoints(int w, std::vector<cv::Point> points);
		cv::Mat thresholdHand(cv::Mat depth16, cv::Point hand, USHORT handz, int hROI, int vROI);

		Vector _vHand;
		Vector _vHip;

	public:
		HandProcessor();
		void Init(bool isRight);

		void HandleNoInput();
		void HandleInput(cv::Mat &depth16, SkeletonPoint hand, SkeletonPoint hip);

		int GetNumFingers();
		
		double GetHullArea();
		double GetCurveArea();
		double GetHandRatio(); 	
		bool WasFound(); 	

		void Changed(IObservable *source);
		bool IsInitialised();
	};

	HandProcessor *GetHandProcessor(bool rightHand);

	//---------------------------- TrackerScalar ---------------------------

	void OnTracker(int, void*);

	class DLL TrackerScalar : public IScalar {
	private:
		int _value;
		float _scale;
		float _shift;
		void Update();

	public:
		TrackerScalar(string title, int max, float scale = 1.f, float shift = 0.f, int value = 0);
		float CalculateValue();
		friend void OnTracker(int, void*);
	};

	DLL TrackerScalar *trackerP(string title, int max, float scale = 1.f, float shift = 0.f, int value = 0);
}


#endif
