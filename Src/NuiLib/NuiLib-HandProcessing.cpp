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
//CODE ADAPTED FROM https://github.com/bmwesting/Kinect-Hand-Processing


#ifdef VISUAL
#include <opencv\highgui.h>
#endif

#include <iostream>
using namespace std;

#include <NuiLib-HandProcessing.h>
using namespace NuiLib;

// openCV
using namespace cv;

// defines the value about which thresholding occurs
const unsigned char BIN_THRESH_OFFSET = 5;

// defines the value about witch the region of interest is extracted
const unsigned int ROI_OFFSET = 70;

// median blur factor
const unsigned int MEDIAN_BLUR_K = 5;

// grasping threshold
const double GRASPING_THRESH = 0.9;

const int LENGTH = (ROI_OFFSET * 12) + 1;

const bool SIMPLE = true;

const double ROI_SCALE_H = 70. / .00011;
const double ROI_SCALE_V = 70. / .00011;

const int BODY_THRESHOLD = HAND_OFFSET_MM * 8;




// conversion from cvConvexityDefect
struct ConvexityDefect
{
	Point start;
	Point end;
	Point depth_point;
	float depth;
};

SkeletonPoint::SkeletonPoint(long X, long Y, ushort Z) : x(X), y(Y), z(Z) { }

bool hand = false;

HandProcessor::HandProcessor() : _initialised(false) { }

void HandProcessor::Init(bool isRight) {
	_found = false; 
	_isRight = isRight; 
	_vHand = joint(isRight ? HAND_RIGHT : HAND_LEFT); 
	_vHip = joint(isRight ? HIP_RIGHT : HIP_LEFT); 
	_initialised = true;
	_hand = isRight ? "R" : "L";
}

bool HandProcessor::IsInitialised() { return _initialised; }

Point HandProcessor::displayShift(int w, Point point) {
	//return Point(point.x + xShift, point.y + yShift);
	return Point(point.x + (XRES - w), point.y);
}

std::vector<Point> HandProcessor::adjustPoints(int w, std::vector<Point> points) {
	if (!_isRight)
		return points;
	std::vector<Point> newPoints;
	for (auto i = points.begin(); i != points.end(); i++)
		newPoints.push_back(displayShift(w, *i));
	return newPoints;
}

// returns true if the hand is near the edge of the sensor area
bool handApproachingDisplayPerimeter(long x, long y, int hROI, int vROI, cv::Scalar colour)
{
	bool ret = (x > (XRES - hROI)) || (x < (hROI)) || (y > (YRES - vROI)) || (y < (vROI));
	if (ret) {
#ifdef VISUAL
		if (x > (XRES - hROI))
			//Vertical Right (off r)
			line(ExtensionFactory()->GetDebugFrame(), Point(XRES - hROI, 0), Point(XRES - hROI, YRES), colour, 2);
		if (x < hROI)
			//Vertical Left (off l)
			line(ExtensionFactory()->GetDebugFrame(), Point(hROI, 0), Point(hROI, YRES), colour, 2);
		if (y > (YRES - vROI))
			//Horizontal Bottom (off bottom)
			line(ExtensionFactory()->GetDebugFrame(), Point(0, YRES - vROI), Point(XRES, YRES - vROI), colour, 2);
		if (y < vROI)
			//Horizontal Top (off top)
			line(ExtensionFactory()->GetDebugFrame(), Point(0, vROI), Point(XRES, vROI), colour, 2);
#endif    
	}
	return ret;
}
//TODO if head.y < hand.y + vROI we're fine
bool handNearBody(Point hand, ushort handz, Point hip, ushort hipz, Mat depth16, int hROI, int vROI, cv::Scalar colour) {
	if (handz  > hipz - (BODY_THRESHOLD << 3)) {
		int edgeR = -1, edgeL = -1;
		ushort hipOffset = (BODY_THRESHOLD << 3);
		ushort bodyMax = hipz + hipOffset;
		ushort bodyMin = hipz - hipOffset;
		for (int i = 0; edgeR == -1 || edgeL == -1; i++) {
			if (hip.x + i >= depth16.cols-1) 
				edgeR = depth16.cols - 1;
			else {
				ushort r = depth16.at<ushort>(Point(hip.x + i, hand.y));
				if (edgeR == -1 && (i > 2*hROI || r < bodyMin || r > bodyMax || r == 0)) 
					edgeR = hip.x + i;
			}

			if (hip.x - i < 0) 
				edgeL = 0;
			else {
				ushort l = depth16.at<ushort>(Point(hip.x - i, hand.y));
				if (edgeL == -1 && (i > 2*hROI || l < bodyMin || l > bodyMax || l == 0)) edgeL = hip.x - i;
			}
		}
		//If the hand is in front of the body in X/Y space and near the body in Z space, don't process it
		if(hand.x > edgeL - hROI && hand.x < edgeR + hROI) {
#ifdef VISUAL
			line(ExtensionFactory()->GetDebugFrame(), Point(edgeL, 0), Point(edgeL, YRES), colour, 2);
			line(ExtensionFactory()->GetDebugFrame(), Point(edgeR, 0), Point(edgeR, YRES), colour, 2);
			rectangle(ExtensionFactory()->GetDebugFrame(), Point(hand.x - hROI, hand.y - vROI), Point(hand.x + hROI, hand.y + vROI), colour);
#endif
			return true;	
		}
	}
	return false;
}

///
/// Create a reduced size matrix that is just the region of interest around the hand and has been thresholded so the hand is white and everything else is black.
///
Mat HandProcessor::thresholdHand(Mat depth16, Point hand, ushort handz, int hROI, int vROI) {
	//Extract and threshold hand
	ushort thresh = handz + (HAND_OFFSET_MM << 3);	

	int w = hROI * 2;	
	int h = vROI * 2;	
	int xShift = hand.x - hROI;
	int yShift = hand.y - vROI;

#ifdef VISUAL
	int displayShift = _isRight ? XRES - w : 0;
#endif
	Mat handThresh(w, w, CV_8UC1);
	ushort *raw = (ushort *) depth16.data;
	for (int row = 0; row < w; row++) {
		for (int col = 0; col < h; col++) {
			int rawIndex = ((row+yShift) * XRES) + (col+xShift);
			handThresh.data[(row*w)+col] = raw[rawIndex] == 0 || raw[rawIndex] > thresh ? 0 : 255;
#ifdef VISUAL
			int displayIndex = ((row * XRES) + (col+displayShift)) * 3;
			ExtensionFactory()->GetDebugFrame().data[displayIndex] = handThresh.data[(row*w) + col];
			ExtensionFactory()->GetDebugFrame().data[displayIndex + 1] = handThresh.data[(row*w) + col];
			ExtensionFactory()->GetDebugFrame().data[displayIndex + 2] = handThresh.data[(row*w) + col];
#endif
		}
	}
	return handThresh;
}

std::vector<Point> largestContour(std::vector<std::vector<Point>> contours) {
	std::vector<Point> contourPoints;
	double testArea = 0;
	for (unsigned int i = 0; i < contours.size(); i++) {
		double cArea = contourArea(contours[i]);
		if (cArea > testArea) {
			testArea = cArea;
			contourPoints = contours[i];
		}
	}
	return contourPoints;
}

void HandProcessor::HandleNoInput() {
	_hullArea = -1;
	_curveArea = -1;
	_handRatio= -1;
	_found = false;
	Trigger();
}

void HandProcessor::Changed(IObservable *source) {
	const Mat depth16 = ExtensionFactory()->GetDepth();
	_hullArea = -1;
	_curveArea = -1;
	_handRatio = 0;
	_found = false;

	Point hand = ExtensionFactory()->SkeletonToDepth(_vHand);
	Point hip = ExtensionFactory()->SkeletonToDepth(_vHip);
	
	if (hand.x == 0 && hand.y == 0 && hip.x == 0 && hip.y == 0)
		return;

	cv::Scalar colour = _isRight ? cv::Scalar(0, 255, 0) : cv::Scalar(0, 0, 255);
#ifdef VISUAL
	circle(ExtensionFactory()->GetDebugFrame(), hand, 2, colour, 2);
#endif

	if (hand.x < 0 || hand.y < 0 || hip.x < 0 || hip.y < 0 || hand.x >= XRES || hand.y >= YRES || hip.x >= XRES || hip.y >= YRES)
		return;
	ushort handz = depth16.at<ushort>(hand);
	ushort hipz = depth16.at<ushort>(hip);

	int hROI = (int) ((1./handz) * ROI_SCALE_H);
	int vROI = (int) ((1./handz) * ROI_SCALE_V);
	//If the hand is near the edge of the screen or near the body, don't process it
	if (handApproachingDisplayPerimeter(hand.x, hand.y, hROI, vROI, colour) || 
		handNearBody(hand, handz, hip, hipz, depth16, hROI, vROI, colour)) {
			Trigger();
			return;
	}

	// last pre-filtering step, apply median blur
	Mat handBlur;
	medianBlur(thresholdHand(depth16, hand, handz, hROI, vROI), handBlur, MEDIAN_BLUR_K);

	std::vector< std::vector<Point> > contours;
	findContours(handBlur, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	if (contours.size()) {
		std::vector<Point> contourPoints = largestContour(contours);

		std::vector<Point> approxPoints;
		std::vector<int> hull;
		std::vector<Point> hullPoints;

		if (contourPoints.size())
			// approximate the contour by a simple curve
			approxPolyDP(contourPoints, approxPoints, 5, true);

		if (approxPoints.size())
			// calculate a convex hull from the aproximate points
			convexHull(approxPoints, hull, false, false);

		// assemble point set of convex hull
		for(unsigned int k = 0; k < hull.size(); k++) {
			int curveIndex = hull[k];
			Point p = approxPoints[curveIndex];
			hullPoints.push_back(p);
		}

		// area of hull and curve
		_hullArea  = hullPoints.size() ? contourArea(hullPoints) : -1.;
		_curveArea = approxPoints.size() ? contourArea(approxPoints) : -1.;
		_handRatio = _hullArea > 0. ? _curveArea/_hullArea : -1.;
		_found = true;

#ifdef VISUAL
		if (contourPoints.size()) 
			polylines(ExtensionFactory()->GetDebugFrame(), adjustPoints(hROI * 2, contourPoints), true, cv::Scalar(255, 0, 0), 1);
		if (approxPoints.size())
			polylines(ExtensionFactory()->GetDebugFrame(), adjustPoints(hROI * 2, approxPoints), true, cv::Scalar(0, 0, 255), 1);
		if (hullPoints.size())
			polylines(ExtensionFactory()->GetDebugFrame(), adjustPoints(hROI * 2, hullPoints), true, cv::Scalar(0, 255, 0), 2);
		circle(ExtensionFactory()->GetDebugFrame(), hand, 2, colour, 2);
#endif			
	} 

	Trigger();
}

inline double HandProcessor::GetHullArea() { return _hullArea; }
inline double HandProcessor::GetCurveArea() { return _curveArea; }
double HandProcessor::GetHandRatio() { return _handRatio; }
bool HandProcessor::WasFound() { return _found; }

HandProcessor gRightHand;
HandProcessor gLeftHand;

HandProcessor *NuiLib::GetHandProcessor(bool rightHand) {
	if (rightHand && !gRightHand.IsInitialised()) {
		gRightHand.Init(true);
		gRightHand.AddAsListener(NULL, ExtensionFactory());
	}
	if (!rightHand && !gLeftHand.IsInitialised()) {
		gLeftHand.Init(false);
		gLeftHand.AddAsListener(NULL, ExtensionFactory());
	}

	return rightHand ? &gRightHand : &gLeftHand;
}

//------------------------- Tracker Scalar ------------------------

void NuiLib::OnTracker(int val, void *userData) {
	TrackerScalar *t = (TrackerScalar*) userData;
	if (t)
		t->Update();
}
bool windowCreated = false;

TrackerScalar::TrackerScalar(string name, float max, float min, float value) : 
IScalar(name, value), 
	_value(value), 
	_scale(_ticks / (max - min)),
	_shift(-min) {
#ifdef VISUAL
		if (!windowCreated) {
			windowCreated = true;
			namedWindow("TrackerWindow", CV_WINDOW_NORMAL);
		}
		createTrackbar(name, "TrackerWindow", &_value, _ticks, NuiLib::OnTracker, this);
#endif
}

void TrackerScalar::Update() {
	Set(CalculateValue());
}

float TrackerScalar::CalculateValue() {
	return (_value * _scale) + _shift;
}

TrackerScalar *NuiLib::trackerP(string title, float max, float min, float value) {
	TrackerScalar *scalar = new TrackerScalar(title, max, min, value);
	ExtensionFactory()->Add(scalar);
	return scalar;
}

NuiLib::Scalar NuiLib::tracker(string title, float max, float min, float value) {
	return NuiLib::Scalar(trackerP(title, max, min, value));
}
