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

@echo Moving VC10 OpenCV libraries to Bin
copy "%cvDir:/=\%build\x86\vc10\bin\opencv_core241.dll" Bin\
copy "%cvDir:/=\%build\x86\vc10\bin\opencv_core241d.dll" Bin\
copy "%cvDir:/=\%build\x86\vc10\bin\opencv_highgui241.dll" Bin\
copy "%cvDir:/=\%build\x86\vc10\bin\opencv_highgui241d.dll" Bin\
copy "%cvDir:/=\%build\x86\vc10\bin\opencv_imgproc241.dll" Bin\
copy "%cvDir:/=\%build\x86\vc10\bin\opencv_imgproc241d.dll" Bin\
copy "%cvDir:/=\%build\x86\vc10\bin\opencv_objdetect241.dll" Bin\
copy "%cvDir:/=\%build\x86\vc10\bin\opencv_objdetect241d.dll" Bin\
copy "%cvDir:/=\%build\common\tbb\ia32\vc10\tbb.dll" Bin\
copy "%cvDir:/=\%build\common\tbb\ia32\vc10\tbb_debug.dll" Bin\

if defined KINECTSDK10_DIR (
	@echo Moving Microsoft Kinect libraries to Bin
	copy "%KINECTSDK10_DIR:/=\%Assemblies\Microsoft.Kinect.dll" Bin\Microsoft.Kinect.dll
)
