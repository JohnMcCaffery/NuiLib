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

#include <NuiLib-API.h>
//Needed for waitKey
#include <opencv\highgui.h>

int main (int argc, char **args) {
	//Initialise the factory.
	NuiLib::NuiFactory()->Init();

	//Get the shoulder position.
	NuiLib::Vector shoulder = NuiLib::joint(NuiLib::SHOULDER_RIGHT);
	//Get the hand position.
	NuiLib::Vector hand = NuiLib::joint(NuiLib::HAND_RIGHT);

	//Add a listener so whenever the arm vector changes its new values are output.
	NuiLib::NuiFactory()->AddListener([shoulder, hand](NuiLib::IObservable *s) {
		cv::Mat out;
		//Convert image to the correct scale. Involves scaling between 8 bit (maxval 255) and 16 bit (maxval 65535).
		NuiLib::NuiFactory()->GetDepth().convertTo(out, CV_8UC1, 255./65535.);
		//Get the depth frame and convert it to colour.
		cv::cvtColor(out, out, CV_GRAY2RGB);
		//Get the 2D coordinates of the hand and shoulder on the depth frame.
		cv::Point handP = NuiLib::NuiFactory()->SkeletonToDepth(hand);
		cv::Point shoulderP = NuiLib::NuiFactory()->SkeletonToDepth(shoulder);
		//Draw the hand and shoulder and a line between them.
		cv::circle(out, handP, 5, cv::Scalar(0, 0, 255), 5);
		cv::circle(out, shoulderP, 5, cv::Scalar(0, 0, 255), 5);
		cv::line(out, handP, shoulderP, cv::Scalar(0, 0, 255), 2);
		cv::imshow("Display", out);
	});

	//Create the window to display the depth map.
	cv::namedWindow("Display"); 

	//Start the factory polling.
	NuiLib::NuiFactory()->SetAutoPoll(true);

	//Wait for user input to stop the program.
	cv::waitKey();
}
