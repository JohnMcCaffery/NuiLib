#include <NuiLib-API-Safe.h>

#ifndef CLRSAFE_H
#define CLRSAFE_H

#ifdef BUILD
   #define DLL __declspec(dllexport)
#else
   #define DLL __declspec(dllimport)
#endif

namespace NuiLibSafe {
typedef DLL void (* CallbackFunction)();
typedef DLL void (* SkeletonCallbackFunction)();

DLL void RegisterCallbacks(
	CallbackFunction tickCallback,
	SkeletonCallbackFunction foundCallback,
	SkeletonCallbackFunction lostCallback,
	SkeletonCallbackFunction switchedCallback);

DLL bool Init();
DLL void SetAutoPoll(bool val);
DLL void Poll();
DLL void Pause();
DLL void Close();
DLL bool HasSkeleton();
DLL unsigned char *GetColourBytes();
DLL unsigned char *GetDepthBytes();
DLL int GetColourWidth();
DLL int GetColourHeight();
DLL int GetColourStride();
DLL int GetColourLength();
DLL int GetDepthWidth();
DLL int GetDepthHeight();
DLL int GetDepthStride();
DLL int GetDepthLength();
//DLL uchar *GetDebugBytes();

DLL struct Point {
	int X;
	int Y;
	Point(int x, int y) {
		X = x;
		Y = y;
	}
};

class SafeScalar;
class SafeVector;
class SafeCondition;

DLL Point SkeletonToColour(SafeVector *v);
DLL Point SkeletonToDepth(SafeVector *v);

	///
	/// Wrapper around a scalar value.
	/// The value is stored as a float.
	/// Easiest to use with the dereference operator.
	/// IScalar x(5.f);
	/// float value = *x;
	/// X will equal 5.
	///
	/// IScalars are intended to be extended. They form the root of an inheritance heirarchy.
	/// New behaviour is created by extending them and implementing the Changed method.
	/// IScalars are dynamically allocated by an INuiFactory. They cannot be created any other way.
	/// They will be de-allocated by the factory.
	/// 
	class DLL SafeScalar {
	private:
		void *_listener;
		static int _count;

	public: 
		void *_p;

		///
		/// Value = value
		///
		SafeScalar(float value);
		///
		/// Value = value
		///
		SafeScalar(const char *name, float value);
		///
		/// Value = value
		///
		SafeScalar(void *scalar);

		///
		/// Destructor
		///
		~SafeScalar();

		void SetCallback(CallbackFunction callback);

		///
		/// Get the value of the scalar. 
		/// The * operator calls this method.
		/// This method can be overrided if necessary to provide extra behaviour.
		///
		float Get();
		/// Set the value of this SafeScalar.
		///
		void Set(float);

		const char *GetName();
		void SetName(const char *name);
	};	


	//-------------------------------------------------------------------------



	class DLL SafeVector {
	private:
		void * _listener;
		static int _count;

	public: 
		void *_p;

		///
		/// Value = value
		///
		SafeVector(float value);
		///
		/// Value = value
		///
		SafeVector(const char *name, float value);
		///
		/// Value = value
		///
		SafeVector(float x, float y, float z);
		///
		/// Value = value
		///
		SafeVector(const char *name, float x, float y, float z);
		///
		/// Value = value
		///
		SafeVector(void *scalar);

		///
		/// Destructor
		///
		~SafeVector();

		void SetCallback(CallbackFunction callback);

		float X();
		float Y();
		float Z();
		void SetX(float);
		void SetY(float);
		void SetZ(float);
		void Set(float, float, float);

		const char *GetName();
		void SetName(const char *name);
	};


	//-------------------------------------------------------------------------


	///
	/// Wrapper around a scalar value.
	/// The value is stored as a float.
	/// Easiest to use with the dereference operator.
	/// IScalar x(5.f);
	/// float value = *x;
	/// X will equal 5.
	///
	/// IScalars are intended to be extended. They form the root of an inheritance heirarchy.
	/// New behaviour is created by extending them and implementing the Changed method.
	/// IScalars are dynamically allocated by an INuiFactory. They cannot be created any other way.
	/// They will be de-allocated by the factory.
	/// 
	class DLL SafeCondition {
	private:
		void * _listener;
		static int _count;

	public: 
		void *_p;

		///
		/// Value = value
		///
		SafeCondition(bool value);
		///
		/// Value = value
		///
		SafeCondition(const char *name, bool value);
		///
		/// Value = value
		///
		SafeCondition(void *condition);

		///
		/// Destructor
		///
		~SafeCondition();

		void SetCallback(CallbackFunction callback);

		///
		/// Get the value of the scalar. 
		/// The * operator calls this method.
		/// This method can be overrided if necessary to provide extra behaviour.
		///
		bool Get();
		/// Set the value of this SafeCondition.
		///
		void Set(bool);

		const char *GetName();
		void SetName(const char *name);
	};	


	//-------------------------------------------------------------------------

	///
	/// Arithmetic sum of two scalars.
	///
	DLL SafeScalar *sum(SafeScalar *addend1, SafeScalar *addend2);
	///
	/// Arithmetic difference of two scalars.
	///
	DLL SafeScalar *difference(SafeScalar *minuend, SafeScalar *subtrahend);
	///
	/// Arithmetic product of two scalars.
	///
	DLL SafeScalar *product(SafeScalar *factor1, SafeScalar *factor2);
	///
	/// Arithmetic quotient of two scalars.
	///
	DLL SafeScalar *quotient(SafeScalar *dividend, SafeScalar *divisor);

	///
	/// Arithmetic sum of two scalars.
	///
	DLL SafeScalar *sum(SafeScalar *addend1, float addend2);
	///
	/// Arithmetic difference of two scalars.
	///
	DLL SafeScalar *difference(SafeScalar *minuend, float subtrahend);
	///
	/// Arithmetic product of two scalars.
	///
	DLL SafeScalar *product(SafeScalar *factor1, float factor2);
	///
	/// Arithmetic quotient of two scalars.
	///
	DLL SafeScalar *quotient(SafeScalar *dividend, float divisor);

	///
	/// Arithmetic sum of two scalars.
	///
	DLL SafeScalar *sum(float addend1, SafeScalar *addend2);
	///
	/// Arithmetic difference of two scalars.
	///
	DLL SafeScalar *difference(float minuend, SafeScalar *subtrahend);
	///
	/// Arithmetic product of two scalars.
	///
	DLL SafeScalar *product(float factor1, SafeScalar *factor2);
	///
	/// Arithmetic quotient of two scalars.
	///
	DLL SafeScalar *quotient(float dividend, SafeScalar *divisor);


	///
	/// The x value for the given SafeVector.
	/// @param vector The vector to take the x value of.
	///
	DLL SafeScalar *x(SafeVector *vector);
	///
	/// The y value for the given SafeVector.
	/// @param vector The vector to take the y value of.
	///
	DLL SafeScalar *y(SafeVector *vector);
	///
	/// The z value for the given SafeVector.
	/// @param vector The vector to take the z value of.
	///
	DLL SafeScalar *z(SafeVector *vector);
	///
	/// The magnitude value of the given SafeVector.
	///
	DLL SafeScalar *magnitude(SafeVector *vector);
	///
	/// The dot product of two vectors
	/// @param a The first vector.
	/// @param b The second vector.
	///
	DLL SafeScalar *dot(SafeVector *a, SafeVector *b);
	///
	/// Normalize the value of a scalar against the maximum value it has ever been.
	/// @param scalar The scalar to normalize.
	///
	DLL SafeScalar *normalizeScalar(SafeScalar *);
	///
	/// Normalize the value of a scalar against a specified maximum value.
	/// @param scalar The scalar to normalize.
	/// @param max The maximum value to normalize against.
	///
	DLL SafeScalar *normalize(SafeScalar *, float);
	///
	/// Normalize the value of a scalar against a specified maximum value.
	/// @param scalar The scalar to normalize.
	/// @param max The maximum value to normalize against.
	///
	DLL SafeScalar *normalize(SafeScalar *, SafeScalar *);
	///
	/// Constrain a scalar to a specific range.
	/// The final output is a value between 0.f and 1.f.
	/// If the input is < deadzone Value == 0.f
	/// If the input is < deadzone + range Value == (*input - deadzone) / range.
	/// If the input is < deadzone + range + grace Value == 1.f
	/// If the input is >= deadzone + range + grace Value == 0.f
	/// @param input The scalar to constrain.
	/// @param deadzone The minimum threshold. If *input < deadzone Value == 0.f.
	/// @param range The range of values accepted. If *input < deadzone + range Value == (*input - deadzone) / range.
	/// @param grace The grace period past the end of the range where the value stays 1.f. If *input < deadzome + range + grace Value == 1.f.
	/// @param mirror Whether to mirror the constraints. If true calculations are done against the absolute value if *input.
	///
	DLL SafeScalar *constrain(SafeScalar *input, float deadzone, float range, float grace, bool mirror);
	///
	/// Constrain a scalar to a specific range.
	/// The final output is a value between 0.f and 1.f.
	/// If the input is < deadzone Value == 0.f
	/// If the input is < deadzone + range Value == (*input - deadzone) / range.
	/// If the input is < deadzone + range + grace Value == 1.f
	/// If the input is >= deadzone + range + grace Value == 0.f
	/// @param input The scalar to constrain.
	/// @param deadzone The minimum threshold. If *input < deadzone Value == 0.f.
	/// @param range The range of values accepted. If *input < deadzone + range Value == (*input - deadzone) / range.
	/// @param grace The grace period past the end of the range where the value stays 1.f. If *input < deadzome + range + grace Value == 1.f.
	/// @param mirror Whether to mirror the constraints. If true calculations are done against the absolute value if *input.
	///
	DLL SafeScalar *constrain(SafeScalar *input, float deadzone, float range, SafeScalar *grace, bool mirror);
	///
	/// Constrain a scalar to a specific range.
	/// The final output is a value between 0.f and 1.f.
	/// If the input is < deadzone Value == 0.f
	/// If the input is < deadzone + range Value == (*input - deadzone) / range.
	/// If the input is < deadzone + range + grace Value == 1.f
	/// If the input is >= deadzone + range + grace Value == 0.f
	/// @param input The scalar to constrain.
	/// @param deadzone The minimum threshold. If *input < deadzone Value == 0.f.
	/// @param range The range of values accepted. If *input < deadzone + range Value == (*input - deadzone) / range.
	/// @param grace The grace period past the end of the range where the value stays 1.f. If *input < deadzome + range + grace Value == 1.f.
	/// @param mirror Whether to mirror the constraints. If true calculations are done against the absolute value if *input.
	///
	DLL SafeScalar *constrain(SafeScalar *input, float deadzone, SafeScalar *range, float grace, bool mirror);
	///
	/// Constrain a scalar to a specific range.
	/// The final output is a value between 0.f and 1.f.
	/// If the input is < deadzone Value == 0.f
	/// If the input is < deadzone + range Value == (*input - deadzone) / range.
	/// If the input is < deadzone + range + grace Value == 1.f
	/// If the input is >= deadzone + range + grace Value == 0.f
	/// @param input The scalar to constrain.
	/// @param deadzone The minimum threshold. If *input < deadzone Value == 0.f.
	/// @param range The range of values accepted. If *input < deadzone + range Value == (*input - deadzone) / range.
	/// @param grace The grace period past the end of the range where the value stays 1.f. If *input < deadzome + range + grace Value == 1.f.
	/// @param mirror Whether to mirror the constraints. If true calculations are done against the absolute value if *input.
	///
	DLL SafeScalar *constrain(SafeScalar *input, float deadzone, SafeScalar *range, SafeScalar *grace, bool mirror);
	///
	/// Constrain a scalar to a specific range.
	/// The final output is a value between 0.f and 1.f.
	/// If the input is < deadzone Value == 0.f
	/// If the input is < deadzone + range Value == (*input - deadzone) / range.
	/// If the input is < deadzone + range + grace Value == 1.f
	/// If the input is >= deadzone + range + grace Value == 0.f
	/// @param input The scalar to constrain.
	/// @param deadzone The minimum threshold. If *input < deadzone Value == 0.f.
	/// @param range The range of values accepted. If *input < deadzone + range Value == (*input - deadzone) / range.
	/// @param grace The grace period past the end of the range where the value stays 1.f. If *input < deadzome + range + grace Value == 1.f.
	/// @param mirror Whether to mirror the constraints. If true calculations are done against the absolute value if *input.
	///
	DLL SafeScalar *constrain(SafeScalar *input, SafeScalar *deadzone, float range, float grace, bool mirror);
	///
	/// Constrain a scalar to a specific range.
	/// The final output is a value between 0.f and 1.f.
	/// If the input is < deadzone Value == 0.f
	/// If the input is < deadzone + range Value == (*input - deadzone) / range.
	/// If the input is < deadzone + range + grace Value == 1.f
	/// If the input is >= deadzone + range + grace Value == 0.f
	/// @param input The scalar to constrain.
	/// @param deadzone The minimum threshold. If *input < deadzone Value == 0.f.
	/// @param range The range of values accepted. If *input < deadzone + range Value == (*input - deadzone) / range.
	/// @param grace The grace period past the end of the range where the value stays 1.f. If *input < deadzome + range + grace Value == 1.f.
	/// @param mirror Whether to mirror the constraints. If true calculations are done against the absolute value if *input.
	///
	DLL SafeScalar *constrain(SafeScalar *input, SafeScalar *deadzone, float range, SafeScalar *grace, bool mirror);
	///
	/// Constrain a scalar to a specific range.
	/// The final output is a value between 0.f and 1.f.
	/// If the input is < deadzone Value == 0.f
	/// If the input is < deadzone + range Value == (*input - deadzone) / range.
	/// If the input is < deadzone + range + grace Value == 1.f
	/// If the input is >= deadzone + range + grace Value == 0.f
	/// @param input The scalar to constrain.
	/// @param deadzone The minimum threshold. If *input < deadzone Value == 0.f.
	/// @param range The range of values accepted. If *input < deadzone + range Value == (*input - deadzone) / range.
	/// @param grace The grace period past the end of the range where the value stays 1.f. If *input < deadzome + range + grace Value == 1.f.
	/// @param mirror Whether to mirror the constraints. If true calculations are done against the absolute value if *input.
	///
	DLL SafeScalar *constrain(SafeScalar *input, SafeScalar *deadzone, SafeScalar *range, float grace, bool mirror);
	///
	/// Constrain a scalar to a specific range.
	/// The final output is a value between 0.f and 1.f.
	/// If the input is < deadzone Value == 0.f
	/// If the input is < deadzone + range Value == (*input - deadzone) / range.
	/// If the input is < deadzone + range + grace Value == 1.f
	/// If the input is >= deadzone + range + grace Value == 0.f
	/// @param input The scalar to constrain.
	/// @param deadzone The minimum threshold. If *input < deadzone Value == 0.f.
	/// @param range The range of values accepted. If *input < deadzone + range Value == (*input - deadzone) / range.
	/// @param grace The grace period past the end of the range where the value stays 1.f. If *input < deadzome + range + grace Value == 1.f.
	/// @param mirror Whether to mirror the constraints. If true calculations are done against the absolute value if *input.
	///
	DLL SafeScalar *constrain(SafeScalar *input, SafeScalar *deadzone, SafeScalar *range, SafeScalar *grace, bool mirror);
	///
	/// Take the absolute value of a scalar.
	/// @param input The scalar to take the absolute value of.
	///
	DLL SafeScalar *abs(SafeScalar *input);
	///
	/// Take the arc cosine of a scalar.
	/// @param input The scalar to take the arc cosine of.
	///
	DLL SafeScalar *acos(SafeScalar *input);
	///
	/// -1 or 1 depending on a condition.
	/// Value == *input ? -1.f : 1.f
	/// Used to invert other values depending on a condition.
	/// @param input The condition which dictates Value.
	///
	DLL SafeScalar *invert(SafeCondition *input);
	///
	/// The change in a scalar since the last update.
	/// Will give the first derivitive of a scalar.
	/// @param input The scalar to track changes in.
	///
	DLL SafeScalar *deltaScalar(SafeScalar *input);
	///
	/// The scalar projection of one vector onto another.
	/// a is projected onto b.
	/// @param a The projecting vector.
	/// @param b The vector being projected on.
	///
	DLL SafeScalar *project(SafeVector *a, SafeVector *b);
	///
	/// Value is one of two inputs depending on a condition.
	/// Value = *condition ? t : f;
	/// @param condition SafeCondition up which the output is dependant.
	/// @param t Value == t if condition is true.
	/// @param f Value == f if condition is false.
	///
	DLL SafeScalar *ifScalar(SafeCondition *condition, float t, float f);
	///
	/// Value is one of two inputs depending on a condition.
	/// Value = *condition ? t : f;
	/// @param condition SafeCondition *up which the output is dependant.
	/// @param t Value == t if condition is true.
	/// @param f Value == f if condition is false.
	///
	DLL SafeScalar *ifScalar(SafeCondition *condition, float t, SafeScalar *f);
	///
	/// Value is one of two inputs depending on a condition.
	/// Value = *condition ? a : b;
	/// @param condition SafeCondition *up which the output is dependant.
	/// @param a Value == a if condition is true.
	/// @param b Value == b if condition is false.
	///
	DLL SafeScalar *ifScalar(SafeCondition *condition, SafeScalar *t, float f);
	///
	/// Value is one of two inputs depending on a condition.
	/// Value = *condition ? a : b;
	/// @param condition SafeCondition up which the output is dependant.
	/// @param a Value == a if condition is true.
	/// @param b Value == b if condition is false.
	///
	DLL SafeScalar *ifScalar(SafeCondition *condition, SafeScalar *t, SafeScalar *f);
	///
	/// Value is set by an opencv track bar.
	/// Takes parameters to define how the integer value, starting at 0, that the tracker supplies is converted to a float.
	/// @param title The name of the tracker bar.
	/// @param max the maximum value the track bar can have.
	/// @param scale How the tracker input (between 0 and max) should be scaled to convert it to a float.
	/// @param shift How the tracker input (between 0 and max) should be shifted to convert it to a float.
	/// @param value The initial value of the track bar.
	///
	//DLL SafeScalar *tracker(char* title, int max, float scale = 1.f, float shift = 0.f, int value = 0);
	///
	/// Take a scalar and smooth it. Smoothing is done by averaging the values of the last x frames.
	/// x is the current value of numFrames.
	///
	DLL SafeScalar *smooth(SafeScalar *toSmooth, SafeScalar *numFrames);
	///
	/// Take a scalar and smooth it. Smoothing is done by averaging the values of the last x frames.
	/// x is numFrames.
	///
	DLL SafeScalar *smooth(SafeScalar *toSmooth, int numFrames);



	//-------------------------------------------------------------------------




	///
	/// Arithmetic sum of two scalars.
	///
	DLL SafeVector *sum(SafeVector *addend1, SafeVector *addend2);
	///
	/// Arithmetic difference of two scalars.
	///
	DLL SafeVector *difference(SafeVector *minuend, SafeVector *subtrahend);
	///
	/// Arithmetic product of two scalars.
	///
	DLL SafeVector *product(SafeVector *factor1, SafeVector *factor2);
	///
	/// Arithmetic quotient of two scalars.
	///
	DLL SafeVector *quotient(SafeVector *dividend, SafeVector *divisor);

	///
	/// Arithmetic sum of two scalars.
	///
	DLL SafeVector *sum(SafeVector *addend1, float addend2);
	///
	/// Arithmetic difference of two scalars.
	///
	DLL SafeVector *difference(SafeVector *minuend, float subtrahend);
	///
	/// Arithmetic product of two scalars.
	///
	DLL SafeVector *product(SafeVector *factor1, float factor2);
	///
	/// Arithmetic quotient of two scalars.
	///
	DLL SafeVector *quotient(SafeVector *dividend, float divisor);

	///
	/// Arithmetic sum of two scalars.
	///
	DLL SafeVector *sum(float addend1, SafeVector *addend2);
	///
	/// Arithmetic difference of two scalars.
	///
	DLL SafeVector *difference(float minuend, SafeVector *subtrahend);
	///
	/// Arithmetic product of two scalars.
	///
	DLL SafeVector *product(float factor1, SafeVector *factor2);
	///
	/// Arithmetic quotient of two scalars.
	///
	DLL SafeVector *quotient(float dividend, SafeVector *divisor);

	//-------------------------------------------------------------------------


	///
	/// Normalize the value of a vector so its magnitude is 1.
	/// @param vector The SafeVector to normalize.
	/// @param max The maximum value to normalize against.
	///
	DLL SafeVector *normalize(SafeVector *vector);
	///
	/// Get the cross product of two vectors.
	/// @param a The first vector.
	/// @param b The second vector.
	///
	DLL SafeVector *cross(SafeVector *a, SafeVector *b);
	///
	/// Take only specified axes from a vector.
	/// @param input The vector to limit.
	/// @param x Whether to take the x axis.
	/// @param y Whether to take the y axis.
	/// @param z Whether to take the z axis.
	///
	DLL SafeVector *limit(SafeVector *input, bool x, bool y, bool z);
	///
	/// The change in a vector since the last update.
	/// Will give the first derivitive of a vector in all three axes.
	/// @param input The vector to track changes in.
	///
	DLL SafeVector *delta(SafeVector *input);
	///
	/// Take a snapshot of a vector whenever a condition becomes true.
	/// Every time *condition == true Value is set to vector.
	/// @param vector The vector to take snapshots of.
	/// @param condition Every time this becomes true a snapshot is taken.
	DLL SafeVector *snapshot(SafeVector *input, SafeCondition *);
	///
	/// Take the momenum of an input vector.
	/// As long as condition is true Value == vector.
	/// As soon as condition becomes false Value is taken as the last position before the condition become false + 
	/// the delta between the penultimate true position and the final true position. 
	/// After this the same delta keeps on being applied and reduced until the magnitude of the delta approaches zero.
	/// @param input The vector to take the momentum of.
	/// @param condition If true Value is input. Otherwise Value is taken from the momentum of the final position before condition became false.
	///
	DLL SafeVector *momentum(SafeVector *input, SafeCondition *condition);
	///
	/// Take the momenum of an input vector.
	/// As long as condition is true Value == vector.
	/// As soon as condition becomes false Value is taken as the last position before the condition become false + 
	/// the delta between the penultimate true position and the final true position. 
	/// After this the same delta keeps on being applied and reduced by decrement until the magnitude of the delta is less than threshold.
	/// Momentum updates with every factory trigger.
	/// @param input The vector to take the momentum of.
	/// @param condition If true Value is input. Otherwise Value is taken from the momentum of the final position before condition became false.
	/// @param decrement Whilst applying momentum the delta is reduced by this much each iteration.
	/// @param threshold Momentum stops being applied when the magnitude of the delta is less than this.
	///
	DLL SafeVector *momentum(SafeVector *input, SafeCondition *condition, float decrement, float threshold);
	///
	/// Scale a vector so that its magnitude is scale.
	/// @param input the vector to scale.
	/// @param scale The magnitude Value is to have in the direction of input.
	/// 
	DLL SafeVector *scale(SafeVector *input, SafeScalar *scale);
	///
	/// Scale a vector so that its magnitude is scale.
	/// @param input the vector to scale.
	/// @param scale The magnitude Value is to have in the direction of input.
	/// 
	DLL SafeVector *scale(SafeVector *input, float scale);
	///
	/// Calculate the intersection of a line and a plane.
	/// The plane is specified as a point and a normal.
	/// The line is specified as a point and a direction.
	/// @param pPlane A point on the plane.
	/// @param normalPlane The normal of the plane. Is expected to be normalized.
	/// @param pLine A point on the line.
	/// @param dirLine The direction of the line. Expected to be normalized.
	/// @return The point in 3D space where the line intersects the plane.
	///
	DLL SafeVector *intersect(SafeVector *pPlane, SafeVector *normalPlane, SafeVector *pLine, SafeVector *dirLine);
	///
	/// The cartesian coordinates of a joint in the current user's skelton relative to the NUI device.
	/// @param joint Which joint to track.
	/// Hip Centre: 0
	/// Spine: 1
	/// Shoulder Centre: 2
	/// Head: 3
	/// Shoulder Left: 4
	/// Elbow Left: 5
	/// Wrist Left: 6
	/// Hand Left: 7
	/// Shoulder Right: 8
	/// Elbow Right: 9
	/// Wrist Right: 10
	/// Hand Right: 11
	/// Hip Left: 12
	/// Knee Left: 13
	/// Ankle Left: 14
	/// Foot Left: 15
	/// Hip Right: 16
	/// Knee Right: 17
	/// Ankle Right: 18
	/// Foot Right: 19
	///
	DLL SafeVector *joint(int joint);	
	///
	/// Take a scalar and smooth it. Smoothing is done by averaging the values of the last x frames.
	/// x is the current value of numFrames.
	///
	DLL SafeVector *smooth(SafeVector *toSmooth, SafeScalar *numFrames);
	///
	/// Take a scalar and smooth it. Smoothing is done by averaging the values of the last x frames.
	/// x is numFrames.
	///
	DLL SafeVector *smooth(SafeVector *toSmooth, int numFrames);

	//-------------------------------------------------------------------------

	/*----------------- Condition Operators --------------*/

	///
	/// True if operand1 is greater than operand2.
	///
	DLL SafeCondition *Greater(SafeScalar *operand1, SafeScalar *operand2);
	///
	/// True if operand1 is less than operand2.
	///
	DLL SafeCondition *Less(SafeScalar *operand1, SafeScalar *operand2);
	///
	/// True if operand1 is greater than or equ.al to operand2
	///
	DLL SafeCondition *GreaterEqual(SafeScalar *operand1, SafeScalar *operand2);
	///
	/// True if operand1 is less than or equal. to operand2
	///
	DLL SafeCondition *LessEqual(SafeScalar *operand1, SafeScalar *operand2);
	///
	/// True if operand1 is equal to operand2.
	///
	DLL SafeCondition *Equal(SafeScalar *operand1, SafeScalar *operand2);
	///
	/// True if operand1 is equal to operand2.
	///
	//DLL SafeCondition *Equal(SafeVector *operand1, SafeVector *operand2);
	///
	/// True if operand1 is not equal to operand2.
	///
	DLL SafeCondition *NotEqual(SafeScalar *operand1, SafeScalar *operand2);
	///
	/// True if operand1 is not equal to operand2.
	///
	DLL SafeCondition *NotEqual(SafeVector *operand1, SafeVector *operand2);
	///
	/// True if both operand1 and operand2 are true.
	///
	DLL SafeCondition *And(SafeCondition *operand1, SafeCondition *operand2);
	///
	/// True if either operand1 or operand2 or both are true.
	///
	DLL SafeCondition *Or(SafeCondition *operand1, SafeCondition *operand2);
	///
	/// True if operand is false.
	///
	DLL SafeCondition *Not(SafeCondition *operand);

	///
	/// True if operand1 is greater than operand2.
	///
	DLL SafeCondition *Greater(SafeScalar *operand1, float operand2);
	///
	/// True if operand1 is less than operand2.
	///
	DLL SafeCondition *Less(SafeScalar *operand1, float operand2);
	///
	/// True if operand1 is greater than or equ.al to operand2
	///
	DLL SafeCondition *GreaterEqual(SafeScalar *operand1, float operand2);
	///
	/// True if operand1 is less than or equal. to operand2
	///
	DLL SafeCondition *LessEqual(SafeScalar *operand1, float operand2);
	///
	/// True if operand1 is equal to operand2.
	/// All components of operand1 (x, y and z) must be equal of to operand1.
	///
	DLL SafeCondition *Equal(SafeScalar *operand1, float operand2);
	///
	/// True if operand1 is equal to operand2.
	///	
	//DLL SafeCondition *Equal(SafeVector *operand1, float operand2);
	///
	/// True if operand1 is equal to operand2.
	///	
	//DLL SafeCondition *Equal(SafeVector *operand1, cv::Point3f);
	///
	/// True if operand1 is not equal to operand2.
	///
	DLL SafeCondition *NotEqual(SafeScalar *operand1, float operand2);
	///
	/// False if all components of operand1 (x, y and z) are equal to operand2.
	///
	DLL SafeCondition *NotEqual(SafeVector *operand1, float operand2);
	///
	/// True if both operand1 and operand2 are true.
	///
	DLL SafeCondition *And(SafeCondition *operand1, bool);
	///
	/// True if either operand1 or operand2 or both are true.
	///
	DLL SafeCondition *Or(SafeCondition *operand1, bool);

	///
	/// True if operand1 is greater than operand2.
	///
	DLL SafeCondition *Greater(float operand1, SafeScalar *operand2);
	///
	/// True if operand1 is less than operand2.
	///
	DLL SafeCondition *Less(float operand1, SafeScalar *operand2);
	///
	/// True if operand1 is greater than or equ.al to operand2
	///
	DLL SafeCondition *GreaterEqual(float operand1, SafeScalar *operand2);
	///
	/// True if operand1 is less than or equal. to operand2
	///
	DLL SafeCondition *LessEqual(float operand1, SafeScalar *operand2);
	///
	/// True if operand1 is equal to operand2.
	/// Operand1 must be qual to all components of operand2 (x, y and z).
	///
	DLL SafeCondition *Equal(float operand1, SafeScalar *operand2);
	///
	/// True if operand1 is equal to operand2.
	///
	//DLL SafeCondition *Equal(float operand1, SafeVector *operand2);
	///
	/// True if operand1 is equal to operand2.
	///
	//DLL SafeCondition *Equal(cv::Point3f operand1, SafeVector *operand2);
	///
	/// True if operand1 is not equal to operand2.
	///
	DLL SafeCondition *NotEqual(float operand1, SafeScalar *operand2);
	///
	/// False if operand1 is equal to all components of operand2 (x, y and z).
	///
	DLL SafeCondition *NotEqual(float operand1, SafeVector *operand2);
	///
	/// True if both operand1 and operand2 are true.
	///
	DLL SafeCondition *And(bool operand1, SafeCondition *operand2);
	///
	/// True if either operand1 or operand2 or both are true.
	///
	DLL SafeCondition *Or(bool operand1, SafeCondition *operand2);


	///
	/// True if all points are reasonable spread out.
	/// Guards against incorrect input when all the points cluster together.
	/// Reasonably spread out is defined as the area contained in the circle head->handR->footR->footL->handL->head.
	///
	DLL SafeCondition *closeGuard();
	///
	/// True if all points are reasonable spread out.
	/// Guards against incorrect input when all the points cluster together.
	/// Reasonably spread out is defined as the area contained in the circle head->handR->footR->footL->handL->head.
	/// @param scale The threshold for the area of the points within the circle.
	///
	DLL SafeCondition *closeGuard(float scale);
	///
	/// True if all points are reasonable spread out.
	/// Guards against incorrect input when all the points cluster together.
	/// Reasonably spread out is defined as the area contained in the circle head->handR->footR->footL->handL->head.
	/// @param scale The threshold for the area of the points within the circle.
	///
	DLL SafeCondition *closeGuard(SafeScalar *scale);
	///
	/// True if the specified hand is in a fist. False otherwise
	/// @param rightHand If true, track the right hand. False, left hand.
	/// 
	DLL SafeCondition *fist(bool rightHand);
	///
	/// True once when condition becomes true.
	/// Once the true value has been accessed it returns to false.
	/// @param condition The condition which is being checked for clicking.
	///
	DLL SafeCondition *click(SafeCondition *condition);
	///
	/// True if condition has been true for longer than a set threshold.
	/// @param condition The condition which is being checked for holding.
	///
	DLL SafeCondition *hold(SafeCondition *condition);	
}

#endif //CLRFSAFE
