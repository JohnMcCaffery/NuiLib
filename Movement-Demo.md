Demonstrates how the various components can be combined to provide a basic movement model. The model allows camera pitch and yaw along with vertical movement (positive and negative) and forward movement.

To control the various axes different gestures are used. To go forward, push forward (directly towards the kinect) with the right hand whilst keeping it level with the shoulder. To fly up raise the right hand above the arm "superman style". To do mouse look raise the right arm so that elbow is in line with the shoulders. Once in mouse look position raising or lowering the hand (whilst keeping the elbow steady) will control pitch. Moving the hand horizontally (again whilst the shoulder is steady) will control yaw. Yaw can also be controlled by leaning right or left or by twisting the shoulders. All 3 yaw movements are cumulative so twisting right whilst leaning right and doing mouselook right will result in a very large yaw value.

The demo will output a graphic displaying what movement is being triggered. This is represented by pitch and yaw values shown as numbers and ASCII bars. 'FORWARD' is printed if the forward state is entered. Vertical movement is represented by 'UP' and 'down' being printed. The ASCII bar output is normalized against largest value that field has ever contained. This means that if the largest yaw value so far has been .2 the bar will go all the way to the right. If the yaw value then raises to .4 the bar will stay all the way right.

Some example output:
<code>
	1. &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Yaw:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Pitch:&nbsp;-------|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-0.048&nbsp;&nbsp;&nbsp;&nbsp;Fly:<br>
	2. &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Yaw:&nbsp;-------|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-0.048&nbsp;&nbsp;&nbsp;&nbsp;Pitch:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0.000&nbsp;&nbsp;&nbsp;&nbsp;Fly:<br>
	3. &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Yaw:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|------&nbsp;&nbsp;&nbsp;0.048&nbsp;&nbsp;&nbsp;&nbsp;Pitch:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|-------&nbsp;&nbsp;0.048&nbsp;&nbsp;&nbsp;&nbsp;Fly:<br>
	4. FORWARD&nbsp;&nbsp;&nbsp;&nbsp;Yaw:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-0.000&nbsp;&nbsp;&nbsp;&nbsp;Pitch:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Fly:<br>
	5. FORWARD&nbsp;&nbsp;&nbsp;&nbsp;Yaw:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;--|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-0.016&nbsp;&nbsp;&nbsp;&nbsp;Pitch:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Fly:<br>
	6. &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Yaw:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Pitch:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Fly:&nbsp;down<br>
	7. &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Yaw:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Pitch:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Fly:&nbsp;UP<br>
</code>
1. Pitching down.  2. Yawing left.  3. Yawing right and pitching up.  4. Moving forward.  5. Moving forward and pitching left.  6. Flying down.  7. Flying up.

The demo can be compiled and run using the 'Demo-Movement' solution in Visual Studio or with the make targets "DEMO_MOVEMENT" and "DEMO_MOVEMENT_RUN". The source is available <a href="http://www.cs.st-andrews.ac.uk/~johnmcc/Documentation-Full/_movement_demo_2main_8cpp_source.html">here</a>.

If run in visual mode this demo also provides a series of sliders to control various parameters about how the mapping is done. These sliders are:
<table>
	<tr>
		<td>PushD</td>
		<td>Push Deadzone</td>
		<td>How far extended the right arm has to be in order to push forward. Smaller values mean you don't have to push as far forward to start going forward.</td>
	</tr>
	<tr>
		<td>PushActive</td>
		<td>Push Ratio scale</td>
		<td>Controls how strictly the right arm must be facing toward the kinect in order for push to be recognised. Smaller values mean the arm can be more to the side or above and still be picked up as pushing.</td>
	</tr>
	<tr>
		<td>PitchArmD:</td>
		<td>Pitch Deadzone</td>
		<td>Controls the cut off that stops tiny pitch movements in the centre constantly moving the view. Smaller values mean that you have to raise/lower your hand less to look up/down.</td>
	</tr>
	<tr>
		<td>PitchArmR:</td>
		<td>Pitch Range</td>
		<td>Small values mean that you will reach the maximum rate of pitching by raising your arm less.</td>
	</tr>
	<tr>
		<td>PitchArmG:</td>
		<td>Pitch Grace</td>
		<td>Controls how far past the maximum pitch angle your arm can go before the pitch value resets to zero.</td>
	</tr>
	<tr>
		<td>PitchAS:</td>
		<td>Pitch Scaling Factor</td>
		<td>Whatever value (between 0 and 1) comes out of the constraints will be divided by this value.</td>
	</tr>
	<tr>
		<td>YawArmD:</td>
		<td>Mouselook Yaw Deadzone</td>
		<td>Controls the cut off that stops tiny yaw movements in the centre constantly moving the view. Smaller values mean that you have to move your hand less to move right or left.</td>
	</tr>
	<tr>
		<td>YawArmR:</td>
		<td>Mouselook Yaw Range</td>
		<td>Small values mean that you will reach the maximum rate of yaw by moving your arm less.</td>
	</tr>
	<tr>
		<td>YawArmG:</td>
		<td>Mouselook Yaw </td>
		<td>Controls how far past the maximum yaw angle your arm can go before the value yaw resets to zero.</td>
	</tr>
	<tr>
		<td>YawLeanD:</td>
		<td>Lean Yaw Deadzone</td>
		<td>Controls the cutoff that stops tiny yaw movements in the centre constantly moving the view. Smaller values mean that you have to lean less to move right or left.</td>
	</tr>
	<tr>
		<td>YawLeanR:</td>
		<td>Lean Yaw Range</td>
		<td>Small values mean that you will reach the maximum rate of yaw by leaning less.</td>
	</tr>
	<tr>
		<td>YawLeanG:</td>
		<td>Lean Yaw Grace</td>
		<td>Controls how far past the maximum lean angle your head can go before the yaw value resets to zero.</td>
	</tr>
	<tr>
		<td>YawTwistD:</td>
		<td>Twist Yaw Deadzone</td>
		<td>Controls the cut off that stops tiny yaw movements in the centre constantly moving the view. Smaller values mean that you have to twist less to move right or left.</td>
	</tr>
	<tr>
		<td>YawTwistR:</td>
		<td>Twist Yaw Range</td>
		<td>Small values mean that you will reach the maximum rate of yaw by twisting less.</td>
	</tr>
	
	<tr>
		<td>YawTwistG:</td>
		<td>Twist Yaw Grace</td>
		<td>Controls how far past the maximum twist angle your shoulders can go before the yaw value resets to zero.</td>
	</tr>
	<tr>
		<td>YawAS:</td>
		<td>Mouselook Yaw Scaling Factor</td>
		<td>Whatever value (between 0 and 1) comes out of the constraints will be divided by this value.</td>
	</tr>
	<tr>
		<td>YawLS:</td>
		<td>Lean Yaw Scaling Factor</td>
		<td>Whatever value (between 0 and 1) comes out of the constraints will be divided by this value.</td>
	</tr>
	<tr>
		<td>YawTS:</td>
		<td>Twist Yaw Scaling Factor</td>
		<td>Whatever value (between 0 and 1) comes out of the constraints will be divided by this value.</td>
	</tr>
</table>
