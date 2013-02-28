// NuiLib-DotNet.h
#include <NuiLib-CLRSafe.h>

#pragma once

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace NuiLibSafe;

namespace NuiLibDotNet {
	ref class DotNetScalar;
	ref class DotNetVector;
	ref class DotNetCondition;

	public ref class NuiFactory {
	public:
		static bool Init() {
			return NuiLibSafe::Init();
		}
		static void Pause() {
			return NuiLibSafe::Pause();
		}
		static void SetAutoPoll(bool value) {
			NuiLibSafe::SetAutoPoll(value);
		}
	};

	public delegate void OnChangeEvt();
	public ref class Observable {
	private:
		OnChangeEvt^ _onChange;

	protected:
		void ChangeListener() {
			OnChange();
		}

	public:
		event OnChangeEvt ^OnChange {
			void add (OnChangeEvt ^listener) {
				_onChange += listener;
			}
			void remove (OnChangeEvt ^listener) {
				_onChange -= listener;
			}
			void raise() {
				OnChangeEvt^ tmp = _onChange;
				if (tmp)
					tmp->Invoke();
			}
		}
	};

	public ref class DotNetScalar : public Observable {
	private:
		DotNetScalar(SafeScalar *scalar) : _ps(scalar) { }

	public:
		SafeScalar *_ps;

		~DotNetScalar() { 
			delete _ps;
		}

		static DotNetScalar ^Create(SafeScalar *safe) {
			DotNetScalar^ scalar = gcnew DotNetScalar(safe);

			OnChangeEvt^ managedCallback = gcnew OnChangeEvt(scalar, &ChangeListener);
			IntPtr unmanagedCallback = Marshal::GetFunctionPointerForDelegate(managedCallback);
			GC::KeepAlive(managedCallback);

			safe->SetCallback((CallbackFunction) (void*) unmanagedCallback);

			return scalar;
		}

		static DotNetScalar ^Create(float value) {
			return Create(new SafeScalar(value));
		}

		property float Value {
			float get() { return _ps->Get(); }
			void set(float value) { _ps->Set(value); }
		}



		///
		/// Arithmetic sum of two scalars.
		///
		static DotNetScalar ^operator+(DotNetScalar ^addend1, DotNetScalar ^addend2);
		///
		/// Arithmetic difference of two scalars.
		///
		static DotNetScalar ^operator-(DotNetScalar ^minuend, DotNetScalar ^subtrahend);
		///
		/// Arithmetic product of two scalars.
		///
		static DotNetScalar ^operator*(DotNetScalar ^factor1, DotNetScalar ^factor2);
		///
		/// Arithmetic quotient of two scalars.
		///
		static DotNetScalar ^operator/(DotNetScalar ^dividend, DotNetScalar ^divisor);
		///
		/// Arithmetic sum of two scalars.
		///
		static DotNetScalar ^operator+=(DotNetScalar ^addend1, DotNetScalar ^addend);
		///
		/// Arithmetic difference of two scalars.
		/// minuend is assigned the new value.
		///
		static DotNetScalar ^operator-=(DotNetScalar ^minuend, DotNetScalar ^subtrahend);
		///
		/// Arithmetic product of two scalars.
		/// factor1 is assigned the new value.
		///
		static DotNetScalar ^operator*=(DotNetScalar ^factor1, DotNetScalar ^factor2);
		///
		/// Arithmetic quotient of two scalars.
		/// dividend is assigned the new value.
		///
		static DotNetScalar ^operator/=(DotNetScalar ^dividend, DotNetScalar ^divisor);

		///
		/// Arithmetic sum of two scalars.
		///
		static DotNetScalar ^operator+(DotNetScalar ^addend1, float addend2);
		///
		/// Arithmetic difference of two scalars.
		///
		static DotNetScalar ^operator-(DotNetScalar ^minuend, float subtrahend);
		///
		/// Arithmetic product of two scalars.
		///
		static DotNetScalar ^operator*(DotNetScalar ^factor1, float factor2);
		///
		/// Arithmetic quotient of two scalars.
		///
		static DotNetScalar ^operator/(DotNetScalar ^dividend, float divisor);
		///
		/// Arithmetic sum of two scalars.
		///
		static DotNetScalar ^operator+=(DotNetScalar ^addend1, float addend);
		///
		/// Arithmetic difference of two scalars.
		/// minuend is assigned the new value.
		///
		static DotNetScalar ^operator-=(DotNetScalar ^minuend, float subtrahend);
		///
		/// Arithmetic product of two scalars.
		/// factor1 is assigned the new value.
		///
		static DotNetScalar ^operator*=(DotNetScalar ^factor1, float factor2);
		///
		/// Arithmetic quotient of two scalars.
		///
		static DotNetScalar ^operator/=(DotNetScalar ^dividend, float divisor);

		///
		/// Arithmetic sum of two scalars.
		///
		static DotNetScalar ^operator+(float addend1, DotNetScalar ^addend2);
		///
		/// Arithmetic difference of two scalars.
		///
		static DotNetScalar ^operator-(float minuend, DotNetScalar ^subtrahend);
		///
		/// Arithmetic product of two scalars.
		///
		static DotNetScalar ^operator*(float factor1, DotNetScalar ^factor2);
		///
		/// Arithmetic quotient of two scalars.
		///
		static DotNetScalar ^operator/(float dividend, DotNetScalar ^divisor);



		//--------------------------------------------------------------------------------------------


		/// <summary>
		/// The x value for the given DotNetVector.
		/// </summary>
		/// <param name="vector"> The vector to take the x value of.</param></param>
		static DotNetScalar ^x(DotNetVector ^vector);
		/// <summary>
		/// The y value for the given DotNetVector.
		/// </summary>
		/// <param name="vector"> The vector to take the y value of.</param>
		static DotNetScalar ^y(DotNetVector ^vector);
		/// <summary>
		/// The z value for the given DotNetVector.
		/// </summary>
		/// <param name="vector"> The vector to take the z value of.</param>
		static DotNetScalar ^z(DotNetVector ^vector);
		/// <summary>
		/// The magnitude value of the given DotNetVector.
		/// </summary>
		static DotNetScalar ^magnitude(DotNetVector ^vector);
		/// <summary>
		/// The dot product of two vectors
		/// </summary>
		/// <param name="a"> The first vector.</param>
		/// <param name="b"> The second vector.</param>
		static DotNetScalar ^dot(DotNetVector ^a, DotNetVector ^b);
		/// <summary>
		/// Normalize the value of a scalar against the maximum value it has ever been.
		/// </summary>
		/// <param name="scalar"> The scalar to normalize.</param>
		static DotNetScalar ^normalize(DotNetScalar ^scalar);
		/// <summary>
		/// Normalize the value of a scalar against a specified maximum value.
		/// </summary>
		/// <param name="scalar"> The scalar to normalize.</param>
		/// <param name="max"> The maximum value to normalize against.</param>
		static DotNetScalar ^normalize(DotNetScalar ^scalar, float max);
		/// <summary>
		/// Normalize the value of a scalar against a specified maximum value.
		/// </summary>
		/// <param name="scalar"> The scalar to normalize.</param>
		/// <param name="max"> The maximum value to normalize against.</param>
		static DotNetScalar ^normalize(DotNetScalar ^scalar, DotNetScalar ^max);
		/// <summary>
		/// Constrain a scalar to a specific range.
		/// The final output is a value between 0.f and 1.f.
		/// If the input is < deadzone Value == 0.f
		/// If the input is < deadzone + range Value == (*input - deadzone) / range.
		/// If the input is < deadzone + range + grace Value == 1.f
		/// If the input is >= deadzone + range + grace Value == 0.f
		/// </summary>
		/// <param name="input"> The scalar to constrain.</param>
		/// <param name="deadzone"> The minimum threshold. If *input < deadzone Value == 0.f.</param>
		/// <param name="range"> The range of values accepted. If *input < deadzone + range Value == (*input - deadzone) / range.</param>
		/// <param name="grace"> The grace period past the end of the range where the value stays 1.f. If *input < deadzome + range + grace Value == 1.f.</param>
		/// <param name="mirror"> Whether to mirror the constraints. If true calculations are done against the absolute value if *input.</param>
		static DotNetScalar ^constrain(DotNetScalar ^input, float deadzone, float range, float grace, bool mirror);
		/// <summary>
		/// Constrain a scalar to a specific range.
		/// The final output is a value between 0.f and 1.f.
		/// If the input is < deadzone Value == 0.f
		/// If the input is < deadzone + range Value == (*input - deadzone) / range.
		/// If the input is < deadzone + range + grace Value == 1.f
		/// If the input is >= deadzone + range + grace Value == 0.f
		/// </summary>
		/// <param name="input"> The scalar to constrain.</param>
		/// <param name="deadzone"> The minimum threshold. If *input < deadzone Value == 0.f.</param>
		/// <param name="range"> The range of values accepted. If *input < deadzone + range Value == (*input - deadzone) / range.</param>
		/// <param name="grace"> The grace period past the end of the range where the value stays 1.f. If *input < deadzome + range + grace Value == 1.f.</param>
		/// <param name="mirror"> Whether to mirror the constraints. If true calculations are done against the absolute value if *input.</param>
		static DotNetScalar ^constrain(DotNetScalar ^input, float deadzone, float range, DotNetScalar ^grace, bool mirror);
		/// <summary>
		/// Constrain a scalar to a specific range.
		/// The final output is a value between 0.f and 1.f.
		/// If the input is < deadzone Value == 0.f
		/// If the input is < deadzone + range Value == (*input - deadzone) / range.
		/// If the input is < deadzone + range + grace Value == 1.f
		/// If the input is >= deadzone + range + grace Value == 0.f
		/// </summary>
		/// <param name="input"> The scalar to constrain.</param>
		/// <param name="deadzone"> The minimum threshold. If *input < deadzone Value == 0.f.</param>
		/// <param name="range"> The range of values accepted. If *input < deadzone + range Value == (*input - deadzone) / range.</param>
		/// <param name="grace"> The grace period past the end of the range where the value stays 1.f. If *input < deadzome + range + grace Value == 1.f.</param>
		/// <param name="mirror"> Whether to mirror the constraints. If true calculations are done against the absolute value if *input.</param>
		static DotNetScalar ^constrain(DotNetScalar ^input, float deadzone, DotNetScalar ^range, float grace, bool mirror);
		/// <summary>
		/// Constrain a scalar to a specific range.
		/// The final output is a value between 0.f and 1.f.
		/// If the input is < deadzone Value == 0.f
		/// If the input is < deadzone + range Value == (*input - deadzone) / range.
		/// If the input is < deadzone + range + grace Value == 1.f
		/// If the input is >= deadzone + range + grace Value == 0.f
		/// </summary>
		/// <param name="input"> The scalar to constrain.</param>
		/// <param name="deadzone"> The minimum threshold. If *input < deadzone Value == 0.f.</param>
		/// <param name="range"> The range of values accepted. If *input < deadzone + range Value == (*input - deadzone) / range.</param>
		/// <param name="grace"> The grace period past the end of the range where the value stays 1.f. If *input < deadzome + range + grace Value == 1.f.</param>
		/// <param name="mirror"> Whether to mirror the constraints. If true calculations are done against the absolute value if *input.</param>
		static DotNetScalar ^constrain(DotNetScalar ^input, float deadzone, DotNetScalar ^range, DotNetScalar^ grace, bool mirror);
		/// <summary>
		/// Constrain a scalar to a specific range.
		/// The final output is a value between 0.f and 1.f.
		/// If the input is < deadzone Value == 0.f
		/// If the input is < deadzone + range Value == (*input - deadzone) / range.
		/// If the input is < deadzone + range + grace Value == 1.f
		/// If the input is >= deadzone + range + grace Value == 0.f
		/// </summary>
		/// <param name="input"> The scalar to constrain.</param>
		/// <param name="deadzone"> The minimum threshold. If *input < deadzone Value == 0.f.</param>
		/// <param name="range"> The range of values accepted. If *input < deadzone + range Value == (*input - deadzone) / range.</param>
		/// <param name="grace"> The grace period past the end of the range where the value stays 1.f. If *input < deadzome + range + grace Value == 1.f.</param>
		/// <param name="mirror"> Whether to mirror the constraints. If true calculations are done against the absolute value if *input.</param>
		static DotNetScalar ^constrain(DotNetScalar ^input, DotNetScalar^ deadzone, float range, float grace, bool mirror);
		/// <summary>
		/// Constrain a scalar to a specific range.
		/// The final output is a value between 0.f and 1.f.
		/// If the input is < deadzone Value == 0.f
		/// If the input is < deadzone + range Value == (*input - deadzone) / range.
		/// If the input is < deadzone + range + grace Value == 1.f
		/// If the input is >= deadzone + range + grace Value == 0.f
		/// </summary>
		/// <param name="input"> The scalar to constrain.</param>
		/// <param name="deadzone"> The minimum threshold. If *input < deadzone Value == 0.f.</param>
		/// <param name="range"> The range of values accepted. If *input < deadzone + range Value == (*input - deadzone) / range.</param>
		/// <param name="grace"> The grace period past the end of the range where the value stays 1.f. If *input < deadzome + range + grace Value == 1.f.</param>
		/// <param name="mirror"> Whether to mirror the constraints. If true calculations are done against the absolute value if *input.</param>
		static DotNetScalar ^constrain(DotNetScalar ^input, DotNetScalar^ deadzone, float range, DotNetScalar^ grace, bool mirror);
		/// <summary>
		/// Constrain a scalar to a specific range.
		/// The final output is a value between 0.f and 1.f.
		/// If the input is < deadzone Value == 0.f
		/// If the input is < deadzone + range Value == (*input - deadzone) / range.
		/// If the input is < deadzone + range + grace Value == 1.f
		/// If the input is >= deadzone + range + grace Value == 0.f
		/// </summary>
		/// <param name="input"> The scalar to constrain.</param>
		/// <param name="deadzone"> The minimum threshold. If *input < deadzone Value == 0.f.</param>
		/// <param name="range"> The range of values accepted. If *input < deadzone + range Value == (*input - deadzone) / range.</param>
		/// <param name="grace"> The grace period past the end of the range where the value stays 1.f. If *input < deadzome + range + grace Value == 1.f.</param>
		/// <param name="mirror"> Whether to mirror the constraints. If true calculations are done against the absolute value if *input.</param>
		static DotNetScalar ^constrain(DotNetScalar ^input, DotNetScalar^ deadzone, DotNetScalar ^range, float grace, bool mirror);
		/// <summary>
		/// Constrain a scalar to a specific range.
		/// The final output is a value between 0.f and 1.f.
		/// If the input is < deadzone Value == 0.f
		/// If the input is < deadzone + range Value == (*input - deadzone) / range.
		/// If the input is < deadzone + range + grace Value == 1.f
		/// If the input is >= deadzone + range + grace Value == 0.f
		/// </summary>
		/// <param name="input"> The scalar to constrain.</param>
		/// <param name="deadzone"> The minimum threshold. If *input < deadzone Value == 0.f.</param>
		/// <param name="range"> The range of values accepted. If *input < deadzone + range Value == (*input - deadzone) / range.</param>
		/// <param name="grace"> The grace period past the end of the range where the value stays 1.f. If *input < deadzome + range + grace Value == 1.f.</param>
		/// <param name="mirror"> Whether to mirror the constraints. If true calculations are done against the absolute value if *input.</param>
		static DotNetScalar ^constrain(DotNetScalar ^input, DotNetScalar^ deadzone, DotNetScalar ^range, DotNetScalar^ grace, bool mirror);
		/// <summary>
		/// Take the absolute value of a scalar.
		/// </summary>
		/// <param name="input"> The scalar to take the absolute value of.</param>
		static DotNetScalar ^abs(DotNetScalar ^input);
		/// <summary>
		/// Take the arc cosine of a scalar.
		/// </summary>
		/// <param name="input"> The scalar to take the arc cosine of.</param>
		static DotNetScalar ^acos(DotNetScalar ^input);
		/// <summary>
		/// -1 or 1 depending on a condition.
		/// Value == *input ? -1.f : 1.f
		/// Used to invert other values depending on a condition.
		/// </summary>
		/// <param name="input"> The condition which dictates Value.</param>
		static DotNetScalar ^invert(DotNetCondition ^input);
		/// <summary>
		/// The change in a scalar since the last update.
		/// Will give the first derivitive of a scalar.
		/// </summary>
		/// <param name="input"> The scalar to track changes in.</param>
		static DotNetScalar ^delta(DotNetScalar ^input);
		/// <summary>
		/// The scalar projection of one vector onto another.
		/// a is projected onto b.
		/// </summary>
		/// <param name="a"> The projecting vector.</param>
		/// <param name="b"> The vector being projected on.</param>
		static DotNetScalar ^project(DotNetVector ^a, DotNetVector ^b);
		/*
		/// <summary>
		/// Value is one of two inputs depending on a condition.
		/// Value = *condition ? t : f);
		/// </summary>
		/// <param name="condition"> DotNetCondition up which the output is dependant.</param>
		/// <param name="t"> Value == t if condition is true.</param>
		/// <param name="f"> Value == f if condition is false.</param>
		static DotNetScalar ^ifScalar(DotNetCondition ^condition, float t, float f);
		/// <summary>
		/// Value is one of two inputs depending on a condition.
		/// Value = *condition ? t : f);
		/// </summary>
		/// <param name="condition"> DotNetCondition up which the output is dependant.</param>
		/// <param name="t"> Value == t if condition is true.</param>
		/// <param name="f"> Value == f if condition is false.</param>
		static DotNetScalar ^ifScalar(DotNetCondition ^condition, float t, DotNetScalar ^f);
		/// <summary>
		/// Value is one of two inputs depending on a condition.
		/// Value = *condition ? a : b);
		/// </summary>
		/// <param name="condition"> DotNetCondition up which the output is dependant.</param>
		/// <param name="a"> Value == a if condition is true.</param>
		/// <param name="b"> Value == b if condition is false.</param>
		static DotNetScalar ^ifScalar(DotNetCondition ^condition, DotNetScalar ^t, float f);
		/// <summary>
		/// Value is one of two inputs depending on a condition.
		/// Value = *condition ? a : b);
		/// </summary>
		/// <param name="condition"> DotNetCondition up which the output is dependant.</param>
		/// <param name="a"> Value == a if condition is true.</param>
		/// <param name="b"> Value == b if condition is false.</param>
		static DotNetScalar ^ifScalar(DotNetCondition ^condition, DotNetScalar ^t, DotNetScalar ^f);
		*/
		/// <summary>
		/// Value is set by an opencv track bar.
		/// Takes parameters to define how the integer value, starting at 0, that the tracker supplies is converted to a float.
		/// </summary>
		/// <param name="title"> The name of the tracker bar.</param>
		/// <param name="max"> the maximum value the track bar can have.</param>
		/// <param name="scale"> How the tracker input (between 0 and max) should be scaled to convert it to a float.</param>
		/// <param name="shift"> How the tracker input (between 0 and max) should be shifted to convert it to a float.</param>
		/// <param name="value"> The initial value of the track bar.</param>
		//static DotNetScalar ^tracker(string title, int max, float scale = 1.f, float shift = 0.f, int value = 0);

	};


	public ref class DotNetVector : public Observable {
	private:
		DotNetVector(SafeVector *vector) : _ps(vector) { }

	public:
		SafeVector *_ps;

		~DotNetVector() { 
			delete _ps;
		}

		static DotNetVector ^Create(SafeVector *safe) {
			DotNetVector^ vector = gcnew DotNetVector(safe);

			OnChangeEvt^ managedCallback = gcnew OnChangeEvt(vector, &ChangeListener);
			IntPtr unmanagedCallback = Marshal::GetFunctionPointerForDelegate(managedCallback);
			GC::KeepAlive(managedCallback);

			safe->SetCallback((CallbackFunction) (void*) unmanagedCallback);

			return vector;
		}

		static DotNetVector ^Create(float value) {
			return Create(new SafeVector(value));
		}

		static DotNetVector ^Create(float x, float y, float z) {
			return Create(new SafeVector(x, y, z));
		}

		property float X {
			float get() { return _ps->X(); }
			void set(float value) { _ps->SetX(value); }
		}
		property float Y {
			float get() { return _ps->Y(); }
			void set(float value) { _ps->SetY(value); }
		}	
		property float Z {
			float get() { return _ps->Z(); }
			void set(float value) { _ps->SetZ(value); }
		}
		void Set(float x, float y, float z) {
			_ps->Set(x, y, z);
		}


		/// <summary>
		/// Normalize the value of a vector so its magnitude is 1.
		/// </summary>
		/// <param name="vector"> The SafeVector to normalize.</param>
		/// <param name="max"> The maximum value to normalize against.</param>
		static DotNetVector ^normalize(DotNetVector ^vector);
		/// <summary>
		/// Get the cross product of two vectors.
		/// </summary>
		/// <param name="a"> The first vector.</param>
		/// <param name="b"> The second vector.</param>
		static DotNetVector ^cross(DotNetVector ^a, DotNetVector ^b);
		/// <summary>
		/// Take only specified axes from a vector.
		/// </summary>
		/// <param name="input"> The vector to limit.</param>
		/// <param name="x"> Whether to take the x axis.</param>
		/// <param name="y"> Whether to take the y axis.</param>
		/// <param name="z"> Whether to take the z axis.</param>
		static DotNetVector ^limit(DotNetVector ^input, bool x, bool y, bool z);
		/// <summary>
		/// The change in a vector since the last update.
		/// Will give the first derivitive of a vector in all three axes.
		/// </summary>
		/// <param name="input"> The vector to track changes in.</param>
		static DotNetVector ^delta(DotNetVector ^input);
		/// <summary>
		/// Take a snapshot of a vector whenever a condition becomes true.
		/// Every time *condition == true Value is set to vector.
		/// </summary>
		/// <param name="vector"> The vector to take snapshots of.</param>
		/// <param name="condition"> Every time this becomes true a snapshot is taken.</param>
		static DotNetVector ^snapshot(DotNetVector ^input, DotNetCondition ^);
		/// <summary>
		/// Take the momenum of an input vector.
		/// As long as condition is true Value == vector.
		/// As soon as condition becomes false Value is taken as the last position before the condition become false + 
		/// the delta between the penultimate true position and the final true position. 
		/// After this the same delta keeps on being applied and reduced until the magnitude of the delta approaches zero.
		/// </summary>
		/// <param name="input"> The vector to take the momentum of.</param>
		/// <param name="condition"> If true Value is input. Otherwise Value is taken from the momentum of the final position before condition became false.</param>
		static DotNetVector ^momentum(DotNetVector ^input, DotNetCondition ^condition);
		/// <summary>
		/// Take the momenum of an input vector.
		/// As long as condition is true Value == vector.
		/// As soon as condition becomes false Value is taken as the last position before the condition become false + 
		/// the delta between the penultimate true position and the final true position. 
		/// After this the same delta keeps on being applied and reduced by decrement until the magnitude of the delta is less than threshold.
		/// Momentum updates with every factory trigger.
		/// </summary>
		/// <param name="input"> The vector to take the momentum of.</param>
		/// <param name="condition"> If true Value is input. Otherwise Value is taken from the momentum of the final position before condition became false.</param>
		/// <param name="decrement"> Whilst applying momentum the delta is reduced by this much each iteration.</param>
		/// <param name="threshold"> Momentum stops being applied when the magnitude of the delta is less than this.</param>
		static DotNetVector ^momentum(DotNetVector ^input, DotNetCondition ^condition, float decrement, float threshold);
		/// <summary>
		/// Scale a vector so that its magnitude is scale.
		/// </summary>
		/// <param name="input"> the vector to scale.</param>
		/// <param name="scale"> The magnitude Value is to have in the direction of input.</param>
		static DotNetVector ^scale(DotNetVector ^input, DotNetScalar ^scale);
		/// <summary>
		/// Scale a vector so that its magnitude is scale.
		/// </summary>
		/// <param name="input"> the vector to scale.</param>
		/// <param name="scale"> The magnitude Value is to have in the direction of input.</param>
		static DotNetVector ^scale(DotNetVector ^input, float scale);
		/// <summary>
		/// Calculate the intersection of a line and a plane.
		/// The plane is specified as a point and a normal.
		/// </summary>
		/// The line is specified as a point and a direction.
		/// <param name="pPlane"> A point on the plane.</param>
		/// <param name="normalPlane"> The normal of the plane. Is expected to be normalized.</param>
		/// <param name="pLine"> A point on the line.</param>
		/// <param name="dirLine"> The direction of the line. Expected to be normalized.</param>
		/// @return The point in 3D space where the line intersects the plane.
		static DotNetVector ^intersect(DotNetVector ^pPlane, DotNetVector ^normalPlane, DotNetVector ^pLine, DotNetVector ^dirLine);
		/// <summary>
		/// The cartesian coordinates of a joint in the current user's skelton relative to the NUI device.
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
		/// </summary>
		/// <param name="joint"> Which joint to track.</param>
		static DotNetVector ^joint(const int joint);
	};


	public ref class DotNetCondition : public Observable {
	private:
		DotNetCondition(SafeCondition *condition) : _ps(condition) { }

	public:
		SafeCondition *_ps;

		~DotNetCondition() { 
			delete _ps;
		}

		static DotNetCondition ^Create(SafeCondition *safe) {
			DotNetCondition^ condition = gcnew DotNetCondition(safe);

			OnChangeEvt^ managedCallback = gcnew OnChangeEvt(condition, &ChangeListener);
			IntPtr unmanagedCallback = Marshal::GetFunctionPointerForDelegate(managedCallback);
			GC::KeepAlive(managedCallback);

			safe->SetCallback((CallbackFunction) (void*) unmanagedCallback);

			return condition;
		}

		static DotNetCondition ^Create(bool value) {
			return Create(new SafeCondition(value));
		}

		property bool Value {
			bool get() { return _ps->Get(); }
			void set(bool value) { _ps->Set(value); }
		}
	};
}
