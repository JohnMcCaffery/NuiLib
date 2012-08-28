<strong>&lt;NuiLib dir&gt;</strong> specifies the directory where NuiLib is installed to. It includes the sub directories "Include" and "Src" and has the solution, prebuild and makefile files in it.
<strong>&lt;flags&gt;</strong> can be d, v, dv or nothing. D specifies debug, V specifies visual. Visual mode means that, if the depth stream is enabled, the depth frames will be output, overlaid with debug information such as the position of joints and how the fist finder algorithm is operating.

<strong>Setting Up:</strong>
Currently NuiLib only supports the <a href="http://www.microsoft.com/en-us/kinectforwindows/">Microsoft Kinect SDK</a>. This will hopefully change soon. To work with NuiLib the first thing you have to do is <a href="http://www.microsoft.com/en-us/kinectforwindows/develop/developer-downloads.aspx">install the microsoft SDK</a>. This project also relies upon <a href="http://sourceforge.net/projects/opencvlibrary/">OpenCV</a>. To work you will need <a href="http://sourceforge.net/projects/opencvlibrary/">OpenCV</a> to be present on your computer. Before attempting to compile NuiLib or any project that links to NuiLib you will need to download and install <a href="http://sourceforge.net/projects/opencvlibrary/">OpenCV</a> which can be downloaded <a href="http://sourceforge.net/projects/opencvlibrary/files/latest/download">here</a>. Once installed you need to copy the relevant libraries to your Bin/ directory. If you want to use the pre-setup build tools such as the makefile and Visual Studio projects you also need to set up a global environment variable called "OPENCV_DIR" which points to the <a href="http://sourceforge.net/projects/opencvlibrary/">OpenCV</a> installation. To do both these things automatically run run prebuild.bat (Visual Studio) or prebuild.sh (MingW).

<strong>How to Build:</strong>
<em>Visual Studio 10</em>
1. Open NuiLib.sln
2. Select a profile
3. Build all (F6)
Libraries compile to "NuiLib-VC10&lt;flags&gt;.lib" and "NuiLib-Kinect-MS-VC10&lt;flags&gt;.lib" in Lib/.


<em>MingW + make + G++</em>
1. Run the make target "DLL".
Library compiles to "NuiLib.dll" in Lib/. "libNuiLib.dll.a" is also compiled to the same folder for linking. "NuiLib.dll" is copied to Bin/.

<strong>How to Include:</strong>
Add /Include/ to the include path.
<em>VS-10</em>: &lt;NuiLib dir&gt;Project-&gt;Properties-&gt;VC++ Directories-&gt;Include Directories.
<em>G++</em>: -I &lt;NuiLib dir&gt;/Include/
<strong>#include &lt;NuiLib-API.h&gt;</strong> at the top of any relevant source. This will give access to everything documented <a href="http://www.cs.st-andrews.ac.uk/~johnmcc/Documentation/">here</a>.

<strong>How to Link:</strong>
<em>Visual Studio 10</em>
Add the following to the library path (Project-&gt;Properties-&gt;VC++ Directories-&gt;Library Directories):
<em>&lt;NuiLib dir&gt;\Lib\<br>
	$(OPENCV_DIR)build\x86\vc10\lib\<br>
$(KINECTSDK10_DIR)lib\x86</em>

Add the following to the linker's additional dependencies (Project-&gt;Properties-&gt;Linker-&gt;Input-&gt;Additional Dependencies):<br>
<em>NuiLib-VC10&lt;flags&gt;.lib<br>
	NuiLib-Kinect-MS-VC10&lt;flags&gt;.lib<br>
opencv_core241[d].lib<br>
opencv_highgui241[d].lib<br>
opencv_imgproc241[d].lib<br>
opencv_objdetect241[d].lib<br>
kinect10.lib</em>

<em>G++</em>
Add the following to the library path (-L flag for each folder):<br>
<em>&lt;NuiLib dir&gt;/Lib/<br>
	$(OPENCV_DIR)build/x86/mingw/lib/<br>
$(KINECTSDK10_DIR)lib/x86/</em>

Add the following to the linker's inputs (-l flag for each file):<br>
<em>NuiLib.dll<br>
	opencv_core241.dll<br>
	opencv_highgui241.dll<br>
	opencv_imgproc241.dll<br>
	opencv_objdetect241.dll<br>
Kinect10.lib</em>

<strong>How to Add Components:</strong>
Add /Include/ to the include path.<br>
<em>VS-10</em>: &lt;NuiLib dir&gt;Project-&gt;Properties-&gt;VC++ Directories-&gt;Include Directories.<br>
<em>G++</em>: -I &lt;NuiLib dir&gt;/Include/<br>
<strong>#include &lt;NuiLib-Extendable.h&gt;</strong> at the top of any relevant source. This will give access to everything documented <a href="http://www.cs.st-andrews.ac.uk/~johnmcc/Documentation-Full/">here</a>. Specifically IComponent, IScalar, IVector and ICondition.
To be compatible new components must extend from IScalar, IVector or ICondition (or one of their pre-existing abstract sub-classes such as VectorWrappingScalar. These are found in 'NuiLib-Vectors.h', 'NuiLib-Conditions.h' or 'NuiLib-Scalars.h').
Generally all that is necessary is to implement a new component is to declare the class and one or more factory functions. The class must have a 0 argument constructor. It must also override the 'CalculateValue' function and implement 'static string GetName()' function. It may also need to provide mechanisms for setting any fields it needs to work with. If it has generic behaviour such as wrapping a Vector a pre-existing abstract super class may exist that does this for you.
The factory functions should return a Scalar, Vector or Condition with the _p field assigned to a new instance of the class that has been fully initialized.


<strong>How to Add Support for new Devices/Drivers:</strong>
Add /Include/ to the include path.
<em>VS-10</em>: &lt;NuiLib dir&gt;Project-&gt;Properties-&gt;VC++ Directories-&gt;Include Directories.
<em>G++</em>: -I &lt;NuiLib dir&gt;/Include/
<strong>#include &lt;NuiLib-Extendable.h&gt;</strong> at the top of any relevant source. This will give access to everything documented <a href="http://www.cs.st-andrews.ac.uk/~johnmcc/Documentation-Full/">here</a>. Specifically INuiFactoryExtension.
The main implementation is done via a factory class which extends INuiFactoryExtension. This should take care of polling the device and updating any components which need to be updated.
As well as extending the INuiFactoryExtension class the following functions must be implemented:<br>
INuiFactory()<br>
INuiFactoryExtension()<br>
GetHandProcessor()<br>
joint(int)<br>
The first two should return a pointer to a singleton instance of the new factory class. The last should return a Vector that will contain the coordinates of a skeleton joint.

<strong>More Info</strong>
<em>Prebuild:</em>
Prebuild does 3 things. Firstly it creates a global environment variable $(OPENCV_DIR) and asks the user to enter a value for it. This value should point to the main directory where Open CV has been installed and built. This variable will be used in the Visual Studio projects and the makefile to resolve dependencies. Secondly the prebuild creates a Bin/ directory. Thirdly it copies all the required libraries from the Open CV installation and the Microsoft Kinect SDK installation into Bin/.

<strong>Notes</strong>
This project has only been tested on Windows 7 using Visual Studio 10 and MingW.
