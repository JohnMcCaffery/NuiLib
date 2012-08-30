: Copyright (c) 2012 John McCaffery 
: 
: This file is part of NuiLib.
: 
: NuiLib is free software: you can redistribute it and/or modify
: it under the terms of the GNU Lesser General Public License as published by
: the Free Software Foundation, either version 3 of the License, or
: (at your option) any later version.
: 
: NuiLib is distributed in the hope that it will be useful,
: but WITHOUT ANY WARRANTY; without even the implied warranty of
: MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
: GNU Lesser General Public License for more details.
: 
: You should have received a copy of the GNU Lesser General Public License
: along with NuiLib.  If not, see <http://www.gnu.org/licenses/>.

@echo off
set /P cv="Enter the root directory for your OpenCV installation: "
set fslash=/
set bslash=\
set cvDir=CVDIR

if not defined OPENCV_DIR (
	if %cv:~-1% == %fslash% (
		@echo on 
		setx OPENCV_DIR %cv%
		set cvDir=%cv%
	) else (
		if %cv:~-1% == %bslash% (
			setx OPENCV_DIR %cv%
			set cvDir=%cv%
		) else (
			setx OPENCV_DIR %cv%/
			set dvDir=%cv%/
	       )
       )
) else set cvDir=%OPENCV_DIR%

if not exist Bin md Bin
if not exist Bin\x86 md Bin\x86
if not exist Bin\x86\vc10 md Bin\x86\vc10

@echo Moving VC10 OpenCV libraries to Bin
copy "%cvDir:/=\%build\x86\vc10\bin\opencv_core241.dll" Bin\x86\vc10\
copy "%cvDir:/=\%build\x86\vc10\bin\opencv_core241d.dll" Bin\x86\vc10\
copy "%cvDir:/=\%build\x86\vc10\bin\opencv_highgui241.dll" Bin\x86\vc10\
copy "%cvDir:/=\%build\x86\vc10\bin\opencv_highgui241d.dll" Bin\x86\vc10\
copy "%cvDir:/=\%build\x86\vc10\bin\opencv_imgproc241.dll" Bin\x86\vc10\
copy "%cvDir:/=\%build\x86\vc10\bin\opencv_imgproc241d.dll" Bin\x86\vc10\
copy "%cvDir:/=\%build\x86\vc10\bin\opencv_objdetect241.dll" Bin\x86\vc10\
copy "%cvDir:/=\%build\x86\vc10\bin\opencv_objdetect241d.dll" Bin\x86\vc10\
copy "%cvDir:/=\%build\common\tbb\ia32\vc10\tbb.dll" Bin\x86\vc10\
copy "%cvDir:/=\%build\common\tbb\ia32\vc10\tbb_debug.dll" Bin\x86\vc10\

if defined KINECTSDK10_DIR (
	@echo Moving Microsoft Kinect libraries to Bin
	copy "%KINECTSDK10_DIR:/=\%Assemblies\Microsoft.Kinect.dll" Bin\x86\vc10\
)
