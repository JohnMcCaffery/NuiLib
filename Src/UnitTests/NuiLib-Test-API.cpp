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
#include "NuiLib-Test-API.h"
#include <NuiLib-Processing.h>


Listener::Listener(IObservable *target) {
	AddAsListener(NULL, target);
}

void Listener::Changed(IObservable *root) {
	triggered = true;
}

class DLL TestFactory : public INuiFactoryExtension {
private:
	cv::Mat _db;

public:
	TestFactory() : INuiFactoryExtension("TestFactory", "TestFactory"), _db() { }
	virtual bool Init() { return true; }

	void SetAutoPoll(bool) { }
	bool GetAutoPoll() { return false; }
	void Poll() { Trigger(); }

	bool Triggers() { return true; }

	const cv::Mat GetDepth() { return cv::Mat(); }
	const cv::Mat GetColour() { return cv::Mat(); }
#ifdef VISUAL
	cv::Mat &GetDebugFrame() { return _db; 
	}
#endif		
	bool HasDepth() { return false; } 
	bool HasColour() { return false; } 
	bool HasSkeleton() { return false; } 

	cv::Point SkeletonToDepth(Vector point) = 0;
	cv::Point SkeletonToColour(Vector point) = 0;

	cv::Point3f DepthToSkeleton(cv::Point point);
	cv::Point3f ColourToSkeleton(cv::Point point);
};

TestFactory _testFactory;
INuiFactory *NuiLib::NuiFactory() { return &_testFactory; }
INuiFactoryExtension *NuiLib::ExtensionFactory() { return &_testFactory; }
HandProcessor *NuiLib::GetHandProcessor(bool isRight) {
	return new HandProcessor(isRight);
}
