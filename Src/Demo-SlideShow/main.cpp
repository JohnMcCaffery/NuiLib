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
#include <opencv\highgui.h>
#include <NuiLib-API.h>
#include <stdio.h>

using namespace cv;
using namespace NuiLib;

Condition leftCheck;
Condition rightCheck;
bool cont;
char const **images;
int numImages;
int currentImage = 0;
Mat image;

void LoadImage();

int main(int argc, const char* argv[]) { 
	//Create the display window
	namedWindow("Display", CV_WINDOW_AUTOSIZE);
	if (argc == 1) {
		cout << "No images supplied via the command line. Press return to exit.\n";
		cv::waitKey();
		return 1;
	}

	//Store the variables
	numImages = argc - 1;
	currentImage = 0;
	images = argv;
	//Load the first image
	LoadImage();

	//Initialise the NuiLib factory
	NuiFactory()->Init();

	//Define left as being when the absolute value of the magnitude delta of the left hand is greater than .1 or the left hand is a fist.
	leftCheck = fist(false) || abs(delta(magnitude(joint(HAND_LEFT)))) > .1f;
	//Define right as being when the absolute value of the magnitude delta of the right hand is greater than .1 or the right hand is a fist.
	rightCheck = fist(true) || abs(delta(magnitude(joint(HAND_RIGHT)))) > .1f;

	//Add a listener to the right hand condition so that any time it becomes true the next image will be show.
	leftCheck.OnTrue([](IObservable *source) { 
		currentImage--;
		LoadImage();
	});
	//Add a listener to the left hand condition so that any time it becomes true the previous image will be show.
	rightCheck.OnTrue([](IObservable *source) { 
		currentImage++;
		LoadImage();
	});

	//Start a thread to regularly poll the device.
	NuiFactory()->SetAutoPoll(true);

	//Wait until the user presses a key on the display window
	waitKey();
	cont = false;

	return 0;
}

/*
Load an image into the "Display" frame. If the specified file name cannot be found a banner will be displayed with the file name.
*/
void LoadImage() {
	//Loop the image counter if all the images have been shown
	if (currentImage < 0) 
		currentImage = numImages-1;
	else 
		currentImage %= numImages;

	//Get the name of the image file
	const char *file = images[currentImage+1];
	//Read in the image
	image = imread(file);
	//If the read was not successful display just the name of the file
	if (!image.data) {
		image = Mat(50, 900, CV_8UC1, cv::Scalar(0, 0, 0));
		putText(image, file, Point(5,20), 0, .5f, cv::Scalar(255, 255, 255));
	}
	//Actually display the image
	cout << "Displaying " << file << " / " << currentImage << '\n';
	imshow("Display", image);
}
