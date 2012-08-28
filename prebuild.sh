# Copyright (c) 2012 John McCaffery 
# 
# This file is part of NuiLib.
# 
# NuiLib is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# NuiLib is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public License
# along with NuiLib.  If not, see <http://www.gnu.org/licenses/>.

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
