NuiLib is a utility library intended to ease integration of NUI (Natural User Input) Devices (such as the Microsoft Kinect) into applications. It provides an abstraction layer which hides the device being input from and provides easy support for common operations. Applications built using NUI lib benefit from the ability to make the logic of how the device is used clear and easy to understand. They also gain the ability to switch to different driver sets or even different devices with no code changes.

<b>Target Users</b>
The library is focused on three users groups. Firstly, developers creating / modifying applications to provide NUI support. Secondly, developers interested in optimising NUI device use. Thirdly, developers experimenting with new algorithms for processing NUI data. Each group can work on or with the library in different ways and their innovations automatically feed back in to the other groups. Any optimisations done on the core library will speed up any application or algorithm based around the library. Any new algorithm developed through the library becomes another tool in the application developer's arsenal. Lastly any feedback given by application developers can focus the efforts of the other two groups.

<b>Ease of Use</b>
Making it easy to integrate NUI input is a primary concern for NuiLib. This is what should make it attractive to application developers. The following code snippet is an example of the simplicity of accessing NUI input through NuiLib. It demonstrates initialising the device, gaining access to the location information for two skeleton joints and then computing the vector between the two joints. Last it outputs this vector whenever the value changes.
<code>
	<a name="example"></a>
	#include <a href="http://www.cs.st-andrews.ac.uk/~johnmcc/Documentation/_nui_lib-_a_p_i_8h_source.html">&lt;NuiLib-API.h&gt;</a><br>
	using namespace NuiLib;<br>
	...<br>
	NuiFactory()-&gt;Init();<br>
	Vector arm = joint(HAND_RIGHT) - joint(SHOULDER_RIGHT);<br>
	arm.AddListener([&arm](IObservable *s) { <br>
	&nbsp;&nbsp;&nbsp;&nbsp;cout &lt;&lt; "Right Arm: " &lt;&lt; arm.X() &lt;&lt; ',' &lt;&lt; arm.Y() &lt;&lt; ',' &lt;&lt; arm.Z() &lt;&lt; '\n';<br>
	});<br>
	NuiFactory()-&gt;SetAutoPoll(true);<br>
	...<br>
</code>
The full source code for this example is available <a href="Demos/Basic/">here</a>.

<b>Extensibility</b>
As well as ease of use the library focuses on extensibility. This makes it an attractive platform for experimenting with new algorithms. Developers can easily see the results of their work and make them available to other users. The library includes an extension mechanism so any physical device capable of providing cartesian coordinates for skeleton joints could be integrated. This support can be added to the main trunk or built as a seperate linkable unit. The core functionality of the system can also be extended. New algorithms can be written and linked into the system. 

To aid in developing computer vision based algorithms NuiLib is built on top of <a href="http://opencv.willowgarage.com/wiki/">OpenCV</a>. This means depth and colour frames are available as <a href="http://opencv.willowgarage.com/wiki/">OpenCV</a> matrices for easy processing.

<b>Structure</b>
The library is accessed through three concepts. Components, Component Factory Functions and the <a href="http://www.cs.st-andrews.ac.uk/~johnmcc/Documentation-Full/class_nui_lib_1_1_i_nui_factory.html">NuiFactory</a>. 

Components actually do the work, the three current component types are <a href="http://www.cs.st-andrews.ac.uk/~johnmcc/Documentation-Full/struct_nui_lib_1_1_vector.html">Vectors</a>, <a href="http://www.cs.st-andrews.ac.uk/~johnmcc/Documentation-Full/struct_nui_lib_1_1_scalar.html">Scalars</a> and <a href="http://www.cs.st-andrews.ac.uk/~johnmcc/Documentation-Full/struct_nui_lib_1_1_condition.html">Conditions</a>. If you access a joint in a skeleton it will be as a <a href="http://www.cs.st-andrews.ac.uk/~johnmcc/Documentation-Full/struct_nui_lib_1_1_vector.html">Vector</a>. The mathematical and logical operators are overloaded to work with Components. The main feature of components is that they chain and automatically updated. Once the relationship between them is initialised then whenever the components at the top of the chain updated the update propogates all the way down the chain. In the basic <a href="#example">example</a> given above the arm <a href="http://www.cs.st-andrews.ac.uk/~johnmcc/Documentation-Full/struct_nui_lib_1_1_vector.html">Vector</a> is chained to two joint <a href="http://www.cs.st-andrews.ac.uk/~johnmcc/Documentation-Full/struct_nui_lib_1_1_vector.html">Vectors</a>. Whenever they update it will update. This is the same all the way through, even for much more complicated examples. This means that configuration can be done during initialisation. Once the relationships are established a main program loop can just read off the values at every iteration and input them directly to where they are needed. The <a href="Component-List">Component List</a> lists all components currently implemented.

Component Factory Functions create the components. The <a href="http://www.cs.st-andrews.ac.uk/~johnmcc/Documentation/namespace_nui_lib.html#a7f966136dfd3ba98ba461a1859f95bfb"><i>joint</i></a> function is an example. By calling <a href="http://www.cs.st-andrews.ac.uk/~johnmcc/Documentation/namespace_nui_lib.html#a7f966136dfd3ba98ba461a1859f95bfb"><i>joint</i></a> and specifying which joint the user gets a <a href="http://www.cs.st-andrews.ac.uk/~johnmcc/Documentation-Full/struct_nui_lib_1_1_vector.html">Vector</a> which will automatically track the position of a joint. Using joint factories means that code can be written as regular, arithmetic, code. For instance taking the dot product of two Vectors is as simple as <code>Scalar dot = dot(vector1, vector2)</code>. The <a href="Component-List">Component List</a> shows the names and parameters for all the Component Factory Functions.

The <a href="http://www.cs.st-andrews.ac.uk/~johnmcc/Documentation-Full/class_nui_lib_1_1_i_nui_factory.html">NuiFactory</a> is what allows direct interaction with the device. It can be used to control general behaviour such as initalising and polling the device. It can also be used to load and save XML files. This way a configuration can be created in one application, saved as XML and then loaded into another. Alternatively multiple different configurations can be created and swapped in as necessary.
