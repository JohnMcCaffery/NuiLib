echo "Enter the root directory for your OpenCV installation: "
read CV

if [[ "${CV:(-1)}" = "\\" || "${CV:(-1)}" = "/" ]]; then
	export OPENCV_DIR=$CV
	cvDir=$CV
else
	export OPENCV_DIR=$CV/
	cvDir=$CV/
fi

echo $cvDir

if [ ! -d Bin ] 
then 
	mkdir Bin 
fi

echo Moving mingw OpenCV libraries to Bin
cp "${cvDir}build/x86/mingw/bin/libopencv_core241.dll" Bin/
cp "${cvDir}build/x86/mingw/bin/libopencv_highgui241.dll" Bin/
cp "${cvDir}build/x86/mingw/bin/libopencv_imgproc241.dll" Bin/
cp "${cvDir}build/x86/mingw/bin/libopencv_objdetect241.dll" Bin/
cp "${cvDir}build/common/tbb/ia32/mingw/tbb.dll" Bin/
cp "${cvDir}build/common/tbb/ia32/mingw/tbb_debug.dll" Bin/

if [ -n "$KINECTSDK10_DIR" ]; then
	echo Moving Microsoft Kinect libraries to Bin
	cp "${KINECTSDK10_DIR}Assemblies/Microsoft.Kinect.dll" Bin/Microsoft.Kinect.dll
fi
