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
#include <functional>
#include <string>
#include <cstdio>
#include <map>
#include <list>
#include <set>

using namespace std;

#ifndef API_H
#define API_H

#if !defined GPLUSPLUS
#if defined BUILD
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif
#define SPRINTF sprintf_s
#elif defined BUILD
#define DLL __declspec(dllexport)
#define SPRINTF snprintf
#else
#define DLL __declspec(dllimport)
#define SPRINTF snprintf
#endif

#undef NULL
#define NULL 0

namespace NuiLib {
	const int HIP_CENTER = 0;
	const int SPINE = 1;
	const int SHOULDER_CENTER = 2;
	const int HEAD = 3;
	const int SHOULDER_LEFT = 4;
	const int ELBOW_LEFT = 5;
	const int WRIST_LEFT = 6;
	const int HAND_LEFT = 7;
	const int SHOULDER_RIGHT = 8;
	const int ELBOW_RIGHT = 9;
	const int WRIST_RIGHT = 10;
	const int HAND_RIGHT = 11;
	const int HIP_LEFT = 12;
	const int KNEE_LEFT = 13;
	const int ANKLE_LEFT = 14;
	const int FOOT_LEFT = 15;
	const int HIP_RIGHT = 16;
	const int KNEE_RIGHT = 17;
	const int ANKLE_RIGHT = 18;
	const int FOOT_RIGHT = 19;

	class DLL INuiFactory;

	class DLL IObservable;

	///
	/// Abstract Interface for a class which can be notified when a change has happened.
	/// Includes functionality to specify that this observer is a factory observer.
	/// A factory observer will only ever be activated when the singleton NuiFactory triggers.
	/// Factory observers are useful if the observer wants to wait until all its dependant observables have been updated before recalculating its value.
	///
	class DLL IObserver {
		private:
			IObservable *_singleObservable;
			list<IObservable*> _observed;
			void StopObserving();

		protected:
			IObserver();
			IObserver(IObservable*);

		public:
			virtual ~IObserver();

			///
			/// Add this object as a listener to a target observable.
			/// Will register this object as a listener to pNew. If pOld is not null this object will be removed as a listener from it.
			/// @param pOld The observable this object was listening to. This object will be removed as a listener from pOld if it is not null.
			/// @param pNew The observeable this object is to listen to.
			///
			void AddAsListener(IObservable *pOld, IObservable *pNew);
			///
			/// Remove this object as a listener for the target observable.
			/// @param The target to remove this as a listener from.
			///
			void RemoveAsListener(IObservable*target);
			///
			/// Set whether this observer functions as a factory observer.
			/// A factory observer will only ever be activated when the singleton NuiFactory triggers.
			/// @param value If true this observer will function as a factory observer.
			void SetFactoryObserver(bool value);

			///
			/// Called when any observed object triggers.
			/// @param source The observable that triggered.
			virtual void Changed(IObservable *source) =0;
	};

	///
	/// Interface for observable objects. Contains a protected method "Trigger" which can be used to notify all observers.
	/// To add a listener use the AddAsListener method in the IObserver interface.
	///
	class IObservable {
		private:
			set<IObserver*> _observers;
			list<function<void (IObservable*)>> _listeners;

			void AddListener(IObserver *);
			void RemoveListener(IObserver *);

		protected: 
			virtual ~IObservable();

			///
			/// Trigger this observable.
			/// Any listeneing IObservers will have their Changed methods called with 'this' as the source.
			///
			void Trigger();

		public: 
			friend class IObserver;
			///
			/// Add a listener that will called whenever the observabled changes.
			///
			void AddListener(function<void(IObservable*)> listener);
	};

	enum AXIS { X_AXIS, Y_AXIS, Z_AXIS };

	class IScalar;
	class IVector;
	class ICondition;

	///
	/// Scalar value.
	/// To access the value in a Scalar use the * operator or the Get method.
	/// Mathematical operators (+,-,*,/,+=,-=,*=,/=) are all overriden. Operands can be Scalars or floats.
	/// If the Scalar's value is based on another scalar then when the source Scalar is updated the listening scalars will also be updated.
	/// Scalar x(.5f);
	/// Scalar y(.5f);
	/// Scalar z = x + y; // *z == 1.f;
	/// x._p->Set(1.f); // *z == 1.5f;
	///
	struct DLL Scalar {
		///
		/// Pointer to an object which is actually implementing the scalar.
		///
		IScalar *_p;
		///
		/// Value = 0
		Scalar();
		///
		/// Value = value
		///
		Scalar(float value);		
		///
		/// Value = value
		///
		Scalar(string name, float value);
		///
		/// Value = p->Value();
		/// @param name The name to identify this Scalar by.
		///
		Scalar(IScalar *p);

		///
		/// Get the value of the scalar.
		/// Calls Get();
		/// @return The value of the scalar.
		///
		float operator*();
		///
		/// Get the value of the scalar.
		/// @return The value of the scalar.
		///
		float Get();
		
		///
		/// Add a listener that will called whenever the Scalar updates.
		/// Behaviour for this listener is undefined if the Scalar is re-allocated or the pointer it wraps is changed.
		///
		void AddListener(function<void(IObservable*)> listener);
	};

	///
	/// Boolean value.
	/// To access the value in a Condition use the * operator or the Get method.
	/// Logical operators (&&,||,!) are all overriden. Operands can be Conditions or bools.
	/// Comparison operators (>,<,>=,<=,==,!=) are all overriden. Operands can be Scalars or floats.
	/// If the Condition's value is based on another condition then when the source condition is updated the listening condition will also be updated.
	/// Condition x(true);
	/// Condition y(false);
	/// Condition z = x || y; // *z == true;
	/// x._p->Set(false); // *z == false;
	///
	struct DLL Condition {
		///
		/// Pointer to an object which is actually implementing the condition.
		///
		ICondition *_p;
		///
		/// Value = 0
		Condition();
		///
		/// Value = value
		/// @param name The name to identify this Condition by.
		///
		Condition(bool value);		
		///
		/// Value = value
		/// @param name The name to identify this Condition by.
		///
		Condition(string name, bool value);
		///
		/// Value = p->Value();
		///
		Condition(ICondition *p);

		///
		/// Get the value of the condition.
		/// Calls Get();
		/// @return The value of the condition.
		///
		bool operator*();
		///
		/// Get the value of the condition.
		/// @return The value of the condition.
		///
		bool Get();
		
		///
		/// Add a listener that will called whenever the Condition changes.
		/// Behaviour for this listener is undefined if the Condition is re-allocated or the pointer it wraps is changed.
		///
		void AddListener(function<void(IObservable*)> listener);
		///
		/// Add a listener that will called whenever the Condition is true.
		/// Behaviour for this listener is undefined if the Condition is re-allocated or the pointer it wraps is changed.
		///
		void OnTrue(function<void(IObservable*)> listener);	
		///
		/// Add a listener that will called whenever the Condition updates.
		/// Behaviour for this listener is undefined if the Condition is re-allocated or the pointer it wraps is changed.
		///
		void OnFalse(function<void(IObservable*)> listener);	
	};

	///
	/// Mathematical 3D Cartesian Coordinate value.
	/// To access the value in a Vector use the [] operator or the X, Y or Z methods.
	/// Arithmetic operators (+,-,*,/,+=,-=,*=,/=) are all overriden. Operands can be Vectors or floats.
	/// If the Vector's value is based on another Vector then when the source Vector is updated the listening Vector will also be updated.
	/// Vector x(1.f, 2.f, 3.f);
	/// Vector y(3.f, 2.f, 1.f);
	/// Vector z = x + y; // z[X_AXIS] == 3.f, z[Y_AXIS] == 3.f, z[Z_AXIS] == 3.f
	/// x._p->Set(3.f, 2.f, 1.f); // z[X_AXIS] == 6.f, z[Y_AXIS] == 4.f, z[Z_AXIS] == 2.f
	///
	struct DLL Vector {
		///
		/// Pointer to an object which is actually implementing the vector.
		///
		IVector *_p;
		///
		/// Value = 0
		Vector();
		///
		/// X = value
		/// Y = value
		/// Z = value
		///
		Vector(float value);		
		///
		/// X = value.x
		/// Y = value.y
		/// Z = value.z
		///
		Vector(cv::Point3f value);		
		///
		/// X = *value
		/// Y = *value
		/// Z = *value
		///
		Vector(const Scalar &value);		
		///
		/// X = x
		/// Y = y
		/// Z = z
		///
		Vector(float x, float y, float z);
		///
		/// X = x
		/// Y = y
		/// Z = z
		///
		Vector(float x, float y, const Scalar &z);				
		///
		/// X = x
		/// Y = y
		/// Z = z
		///
		Vector(float x, const Scalar &y, float z);				
		///
		/// X = x
		/// Y = y
		/// Z = z
		///
		Vector(float x, const Scalar &y, const Scalar &z);				
		///
		/// X = x
		/// Y = y
		/// Z = z
		///
		Vector(const Scalar &x, float y, float z);				
		///
		/// X = x
		/// Y = y
		/// Z = z
		///
		Vector(const Scalar &x, float y, const Scalar &z);				
		///
		/// X = x
		/// Y = y
		/// Z = z
		///		
		Vector(const Scalar &x, const Scalar &y, float z);				
		///
		/// X = x
		/// Y = y
		/// Z = z
		///	
		Vector(const Scalar &x, const Scalar &y, const Scalar &z);						
		///
		/// X = value
		/// Y = value
		/// Z = value
		/// @param name The name to identify this Vector by.
		///
		Vector(string, float value);
		///
		/// X = value.x
		/// Y = value.y
		/// Z = value.z
		/// @param name The name to identify this Vector by.
		///
		Vector(string, cv::Point3f value);		
		///
		/// X = *value
		/// Y = *value
		/// Z = *value
		/// @param name The name to identify this Vector by.
		///
		Vector(string, const Scalar &value);
		///
		/// X = x
		/// Y = y
		/// Z = z
		/// @param name The name to identify this Vector by.
		///
		Vector(string name, float x, float y, float z);
		///
		/// X = x
		/// Y = y
		/// Z = z
		/// @param name The name to identify this Vector by.
		///
		Vector(string name, float x, float y, const Scalar &z);				
		///
		/// X = x
		/// Y = y
		/// Z = z
		/// @param name The name to identify this Vector by.
		///
		Vector(string name, float x, const Scalar &y, float z);				
		///
		/// X = x
		/// Y = y
		/// Z = z
		/// @param name The name to identify this Vector by.
		///
		Vector(string name, float x, const Scalar &y, const Scalar &z);				
		///
		/// X = x
		/// Y = y
		/// Z = z
		///
		Vector(string name, const Scalar &x, float y, float z);				
		///
		/// X = x
		/// Y = y
		/// Z = z
		/// @param name The name to identify this Vector by.
		///
		Vector(string name, const Scalar &x, float y, const Scalar &z);				
		///
		/// X = x
		/// Y = y
		/// Z = z
		/// @param name The name to identify this Vector by.
		///		
		Vector(string name, const Scalar &x, const Scalar &y, float z);				
		///
		/// X = x
		/// Y = y
		/// Z = z
		/// @param name The name to identify this Vector by.
		///	
		Vector(string name, const Scalar &x, const Scalar &y, const Scalar &z);				
		///
		/// Value = p->Value();
		/// @param name The name to identify this Vector by.
		///
		Vector(IVector *p);

		///
		/// Get the value of the vector.
		/// @return The value of the vector.
		///
		cv::Point3f operator*();
		///
		/// Get one of the axes of the vector.
		/// Calls X, Y or Z.
		///
		const float operator[](AXIS axis);

		///
		/// Get the x value of the vector.
		/// @return The x value of the vector.
		///
		float X();
		///
		/// Get the y value of the vector.
		/// @return The y value of the vector.
		///
		float Y();
		///
		/// Get the z value of the vector.
		/// @return The z value of the vector.
		///
		float Z();
		
		///
		/// Add a listener that will called whenever the Vector updates.
		/// Behaviour for this listener is undefined if the Vector is re-allocated or the pointer it wraps is changed.
		///
		void AddListener(function<void(IObservable*)> listener);
	};


	/*----------------- Scalar Operators --------------*/


	///
	/// Arithmetic sum of two scalars.
	///
	DLL Scalar operator+(const Scalar &addend1, const Scalar &addend2);
	///
	/// Arithmetic difference of two scalars.
	///
	DLL Scalar operator-(const Scalar &minuend, const Scalar &subtrahend);
	///
	/// Arithmetic product of two scalars.
	///
	DLL Scalar operator*(const Scalar &factor1, const Scalar &factor2);
	///
	/// Arithmetic quotient of two scalars.
	///
	DLL Scalar operator/(const Scalar &dividend, const Scalar &divisor);
	///
	/// Arithmetic sum of two scalars.
	///
	DLL Scalar operator+=(Scalar &addend1, const Scalar &addend);
	///
	/// Arithmetic difference of two scalars.
	/// minuend is assigned the new value.
	///
	DLL Scalar operator-=(Scalar &minuend, const Scalar &subtrahend);
	///
	/// Arithmetic product of two scalars.
	/// factor1 is assigned the new value.
	///
	DLL Scalar operator*=(Scalar &factor1, const Scalar &factor2);
	///
	/// Arithmetic quotient of two scalars.
	/// dividend is assigned the new value.
	///
	DLL Scalar operator/=(Scalar &dividend, const Scalar &divisor);

	///
	/// Arithmetic sum of two scalars.
	///
	DLL Scalar operator+(const Scalar &addend1, float addend2);
	///
	/// Arithmetic difference of two scalars.
	///
	DLL Scalar operator-(const Scalar &minuend, float subtrahend);
	///
	/// Arithmetic product of two scalars.
	///
	DLL Scalar operator*(const Scalar &factor1, float factor2);
	///
	/// Arithmetic quotient of two scalars.
	///
	DLL Scalar operator/(const Scalar &dividend, float divisor);
	///
	/// Arithmetic sum of two scalars.
	///
	DLL Scalar operator+=(Scalar &addend1, float addend);
	///
	/// Arithmetic difference of two scalars.
	/// minuend is assigned the new value.
	///
	DLL Scalar operator-=(Scalar &minuend, float subtrahend);
	///
	/// Arithmetic product of two scalars.
	/// factor1 is assigned the new value.
	///
	DLL Scalar operator*=(Scalar &factor1, float factor2);
	///
	/// Arithmetic quotient of two scalars.
	///
	DLL Scalar operator/=(Scalar &dividend, float divisor);
	
	///
	/// Arithmetic sum of two scalars.
	///
	DLL Scalar operator+(float addend1, const Scalar &addend2);
	///
	/// Arithmetic difference of two scalars.
	///
	DLL Scalar operator-(float minuend, const Scalar &subtrahend);
	///
	/// Arithmetic product of two scalars.
	///
	DLL Scalar operator*(float factor1, const Scalar &factor2);
	///
	/// Arithmetic quotient of two scalars.
	///
	DLL Scalar operator/(float dividend, const Scalar &divisor);


	/*----------------- Vector Operators --------------*/

	///
	/// Arithmetic sum of two vectors.
	/// @return x = addend1.X + addend2.X
	///			y = addend1.Y + addend2.Y
	///			z = addend1.Z + addend2.Z
	///
	DLL Vector operator+(const Vector &addend1, const Vector &addend2);
	///
	/// Arithmetic difference of two vectors.
	/// @return x = minuend.X - subtrahend.X
	///			y = minuend.Y - subtrahend.Y
	///			z = minuend.Z - subtrahend.Z	
	///
	DLL Vector operator-(const Vector &minuend, const Vector &subtrahend);
	///
	/// Arithmetic product of two vectors.
	/// @return x = factor1.X * factor2.X
	///			y = factor1.Y * factor2.Y
	///			z = factor1.Z * factor2.Z	
	///
	DLL Vector operator*(const Vector &factor1, const Vector &factor2);
	///
	/// Arithmetic quotient of two vectors.
	/// @return x = dividend.X / divisor.X
	///			y = dividend.Y / divisor.Y
	///			z = dividend.Z / divisor.Z	
	///
	DLL Vector operator/(const Vector &dividend, const Vector &divisor);
	///
	/// Arithmetic sum of two vectors.
	/// @return x = addend1.X + addend2.X
	///			y = addend1.Y + addend2.Y
	///			z = addend1.Z + addend2.Z	
	///
	DLL Vector operator+=(Vector &addend1, const Vector &addend);
	///
	/// Arithmetic difference of two vectors.
	/// minuend is assigned the new value.
	/// @return x = minuend.X - subtrahend.X
	///			y = minuend.Y - subtrahend.Y
	///			z = minuend.Z - subtrahend.Z	
	///
	DLL Vector operator-=(Vector &minuend, const Vector &subtrahend);
	///
	/// Arithmetic product of two vectors.
	/// factor1 is assigned the new value.
	/// @return x = factor1.X * factor2.X
	///			y = factor1.Y * factor2.Y
	///			z = factor1.Z * factor2.Z	
	///
	DLL Vector operator*=(Vector &factor1, const Vector &factor2);
	///
	/// dividend is assigned the new value.
	/// Arithmetic quotient of two vectors.
	/// @return x = dividend.X / divisor.X
	///			y = dividend.Y / divisor.Y
	///			z = dividend.Z / divisor.Z	
	///
	DLL Vector operator/=(Vector &dividend, const Vector &divisor);

	///
	/// Arithmetic sum of a vector and a float.
	/// @return x = addend1.X + addend2
	///			y = addend1.Y + addend2
	///			z = addend1.Z + addend2	
	///
	DLL Vector operator+(const Vector &addend1, float addend2);
	///
	/// Arithmetic difference of a vector and a float.
	/// @return x = minuend.X - subtrahend
	///			y = minuend.Y - subtrahend
	///			z = minuend.Z - subtrahend	
	///
	DLL Vector operator-(const Vector &minuend, float subtrahend);
	///
	/// Arithmetic product of a vector and a float.
	/// @return x = factor1.X * factor2
	///			y = factor1.Y * factor2
	///			z = factor1.Z * factor2	
	///
	DLL Vector operator*(const Vector &factor1, float factor2);
	///
	/// Arithmetic quotient of a vector and a float.
	/// @return x = dividend.X / divisor
	///			y = dividend.Y / divisor
	///			z = dividend.Z / divisor	
	///
	DLL Vector operator/(const Vector &dividend, float divisor);
	///
	/// Arithmetic sum of a vector and a float.
	/// @return x = addend1.X + addend2
	///			y = addend1.Y + addend2
	///			z = addend1.Z + addend2	
	///
	DLL Vector operator+=(Vector &addend1, float addend);
	///
	/// Arithmetic difference of a vector and a float.
	/// minuend is assigned the new value.
	/// @return x = minuend.X - subtrahend
	///			y = minuend.Y - subtrahend
	///			z = minuend.Z - subtrahend	
	///
	DLL Vector operator-=(Vector &minuend, float subtrahend);
	///
	/// Arithmetic product of a vector and a float.
	/// factor1 is assigned the new value.
	/// @return x = factor1.X * factor2
	///			y = factor1.Y * factor2
	///			z = factor1.Z * factor2	
	///
	DLL Vector operator*=(Vector &factor1, float factor2);
	///
	/// Arithmetic quotient of a vector and a float.
	/// dividend is assigned the new value.
	/// @return x = dividend.X / divisor
	///			y = dividend.Y / divisor
	///			z = dividend.Z / divisor	
	///
	DLL Vector operator/=(Vector &dividend, float divisor);

	///
	/// Arithmetic sum of two vectors.
	/// @return x = addend1.X + addend2.X
	///			y = addend1.Y + addend2.Y
	///			z = addend1.Z + addend2.Z	
	///
	DLL Vector operator+(const Vector &addend1, cv::Point3f addend2);
	///
	/// Arithmetic difference of two vectors.
	/// @return x = minuend.X - subtrahend.X
	///			y = minuend.Y - subtrahend.Y
	///			z = minuend.Z - subtrahend.Z	
	///
	DLL Vector operator-(const Vector &minuend, cv::Point3f subtrahend);
	///
	/// Arithmetic product of two vectors.
	/// @return x = factor1.X * factor2.X
	///			y = factor1.Y * factor2.Y
	///			z = factor1.Z * factor2.Z	
	///
	DLL Vector operator*(const Vector &factor1, cv::Point3f factor2);
	///
	/// Arithmetic quotient of two vectors.
	/// @return x = dividend.X / divisor.X
	///			y = dividend.Y / divisor.Y
	///			z = dividend.Z / divisor.Z	
	///
	DLL Vector operator/(const Vector &dividend, cv::Point3f divisor);
	///
	/// Arithmetic sum of two vectors.
	/// @return x = addend1.X + addend2.X
	///			y = addend1.Y + addend2.Y
	///			z = addend1.Z + addend2.Z	
	///
	DLL Vector operator+=(Vector &addend1, cv::Point3f addend);
	///
	/// Arithmetic difference of two vectors.
	/// minuend is assigned the new value.
	/// @return x = minuend.X - subtrahend.X
	///			y = minuend.Y - subtrahend.Y
	///			z = minuend.Z - subtrahend.Z	
	///
	DLL Vector operator-=(Vector &minuend, cv::Point3f subtrahend);
	///
	/// Arithmetic product of two vectors.
	/// factor1 is assigned the new value.
	/// @return x = factor1.X * factor2.X
	///			y = factor1.Y * factor2.Y
	///			z = factor1.Z * factor2.Z	
	///
	DLL Vector operator*=(Vector &factor1, cv::Point3f factor2);
	///
	/// Arithmetic quotient of two vectors.
	/// dividend is assigned the new value.
	/// @return x = dividend.X / divisor.X
	///			y = dividend.Y / divisor.Y
	///			z = dividend.Z / divisor.Z	
	///
	DLL Vector operator/=(Vector &dividend, cv::Point3f divisor);	

	///
	/// Arithmetic sum of a vector and a float.
	/// @return x = addend1 + addend2.X
	///			y = addend1 + addend2.Y
	///			z = addend1 + addend2.Z	
	///
	DLL Vector operator+(float addend1, const Vector &addend2);
	///
	/// Arithmetic difference of a vector and a float.
	/// @return x = minuend - subtrahend.X
	///			y = minuend - subtrahend.Y
	///			z = minuend - subtrahend.Z	
	///
	DLL Vector operator-(float minuend, const Vector &subtrahend);
	///
	/// Arithmetic product of a vector and a float.
	/// @return x = factor1 * factor2.X
	///			y = factor1 * factor2.Y
	///			z = factor1 * factor2.Z	
	///
	DLL Vector operator*(float factor1, const Vector &factor2);
	///
	/// Arithmetic quotient of a vector and a float.
	/// @return x = dividend / divisor.X
	///			y = dividend / divisor.Y
	///			z = dividend / divisor.Z	
	///
	DLL Vector operator/(float dividend, const Vector &divisor);
	
	///
	/// Arithmetic sum of two vectors.
	/// @return x = addend1.X + addend2.X
	///			y = addend1.Y + addend2.Y
	///			z = addend1.Z + addend2.Z	
	///
	DLL Vector operator+(cv::Point3f addend1, const Vector &addend2);
	///
	/// Arithmetic difference of two vectors.
	/// @return x = minuend.X - subtrahend.X
	///			y = minuend.Y - subtrahend.Y
	///			z = minuend.Z - subtrahend.Z	
	///
	DLL Vector operator-(cv::Point3f minuend, const Vector &subtrahend);
	///
	/// Arithmetic product of two vectors.
	/// @return x = factor1.X * factor2.X
	///			y = factor1.Y * factor2.Y
	///			z = factor1.Z * factor2.Z	
	///
	DLL Vector operator*(cv::Point3f factor1, const Vector &factor2);
	///
	/// Arithmetic quotient of two vectors.
	/// @return x = dividend.X / divisor.X
	///			y = dividend.Y / divisor.Y
	///			z = dividend.Z / divisor.Z	
	///
	DLL Vector operator/(cv::Point3f dividend, const Vector &divisor);


	/*----------------- Condition Operators --------------*/


	///
	/// True if operand1 is greater than operand2.
	///
	DLL Condition operator>(const Scalar &operand1, const Scalar &operand2);
	///
	/// True if operand1 is less than operand2.
	///
	DLL Condition operator<(const Scalar &operand1, const Scalar &operand2);
	///
	/// True if operand1 is greater than or equ.al to operand2
	///
	DLL Condition operator>=(const Scalar &operand1, const Scalar &operand2);
	///
	/// True if operand1 is less than or equal. to operand2
	///
	DLL Condition operator<=(const Scalar &operand1, const Scalar &operand2);
	///
	/// True if operand1 is equal to operand2.
	///
	DLL Condition operator==(const Scalar &operand1, const Scalar &operand2);
	///
	/// True if operand1 is equal to operand2.
	///
	//DLL Condition operator==(const Vector &operand1, const Vector &operand2);
	///
	/// True if operand1 is not equal to operand2.
	///
	DLL Condition operator!=(const Scalar &operand1, const Scalar &operand2);
	///
	/// True if operand1 is not equal to operand2.
	///
	DLL Condition operator!=(const Vector &operand1, const Vector &operand2);
	///
	/// True if both operand1 and operand2 are true.
	///
	DLL Condition operator&&(const Condition &operand1, const Condition &operand2);
	///
	/// True if either operand1 or operand2 or both are true.
	///
	DLL Condition operator||(const Condition &operand1, const Condition &operand2);
	///
	/// True if operand is false.
	///
	DLL Condition operator!(const Condition &operand);

	///
	/// True if operand1 is greater than operand2.
	///
	DLL Condition operator>(const Scalar &operand1, float operand2);
	///
	/// True if operand1 is less than operand2.
	///
	DLL Condition operator<(const Scalar &operand1, float operand2);
	///
	/// True if operand1 is greater than or equ.al to operand2
	///
	DLL Condition operator>=(const Scalar &operand1, float operand2);
	///
	/// True if operand1 is less than or equal. to operand2
	///
	DLL Condition operator<=(const Scalar &operand1, float operand2);
	///
	/// True if operand1 is equal to operand2.
	/// All components of operand1 (x, y and z) must be equal of to operand1.
	///
	DLL Condition operator==(const Scalar &operand1, float operand2);
	///
	/// True if operand1 is equal to operand2.
	///	
	//DLL Condition operator==(const Vector &operand1, float operand2);
	///
	/// True if operand1 is equal to operand2.
	///	
	//DLL Condition operator==(const Vector &operand1, cv::Point3f);
	///
	/// True if operand1 is not equal to operand2.
	///
	DLL Condition operator!=(const Scalar &operand1, float operand2);
	///
	/// False if all components of operand1 (x, y and z) are equal to operand2.
	///
	DLL Condition operator!=(const Vector &operand1, float operand2);
	///
	/// True if operand1 is not equal to operand2.
	///
	DLL Condition operator!=(const Vector &operand1, cv::Point3f);
	///
	/// True if both operand1 and operand2 are true.
	///
	DLL Condition operator&&(const Condition &operand1, bool);
	///
	/// True if either operand1 or operand2 or both are true.
	///
	DLL Condition operator||(const Condition &operand1, bool);

	///
	/// True if operand1 is greater than operand2.
	///
	DLL Condition operator>(float operand1, const Scalar &operand2);
	///
	/// True if operand1 is less than operand2.
	///
	DLL Condition operator<(float operand1, const Scalar &operand2);
	///
	/// True if operand1 is greater than or equ.al to operand2
	///
	DLL Condition operator>=(float operand1, const Scalar &operand2);
	///
	/// True if operand1 is less than or equal. to operand2
	///
	DLL Condition operator<=(float operand1, const Scalar &operand2);
	///
	/// True if operand1 is equal to operand2.
	/// Operand1 must be qual to all components of operand2 (x, y and z).
	///
	DLL Condition operator==(float operand1, const Scalar &operand2);
	///
	/// True if operand1 is equal to operand2.
	///
	//DLL Condition operator==(float operand1, const Vector &operand2);
	///
	/// True if operand1 is equal to operand2.
	///
	//DLL Condition operator==(cv::Point3f operand1, const Vector &operand2);
	///
	/// True if operand1 is not equal to operand2.
	///
	DLL Condition operator!=(float operand1, const Scalar &operand2);
	///
	/// False if operand1 is equal to all components of operand2 (x, y and z).
	///
	DLL Condition operator!=(float operand1, const Vector &operand2);
	///
	/// True if operand1 is not equal to operand2.
	///
	DLL Condition operator!=(cv::Point3f operand1, const Vector &operand2);
	///
	/// True if both operand1 and operand2 are true.
	///
	DLL Condition operator&&(bool operand1, const Condition &operand2);
	///
	/// True if either operand1 or operand2 or both are true.
	///
	DLL Condition operator||(bool operand1, Condition const &operand2);

	///
	/// The x value for the given Vector.
	/// @param vector The vector to take the x value of.
	///
	DLL Scalar x(const Vector &vector);
	///
	/// The y value for the given Vector.
	/// @param vector The vector to take the y value of.
	///
	DLL Scalar y(const Vector &vector);
	///
	/// The z value for the given Vector.
	/// @param vector The vector to take the z value of.
	///
	DLL Scalar z(const Vector &vector);
	///
	/// The magnitude value of the given Vector.
	///
	DLL Scalar magnitude(const Vector &vector);
	///
	/// The dot product of two vectors
	/// @param a The first vector.
	/// @param b The second vector.
	///
	DLL Scalar dot(const Vector &a, const Vector &b);
	///
	/// Normalize the value of a scalar against the maximum value it has ever been.
	/// @param scalar The scalar to normalize.
	///
	DLL Scalar normalize(const Scalar &);
	///
	/// Normalize the value of a scalar against a specified maximum value.
	/// @param scalar The scalar to normalize.
	/// @param max The maximum value to normalize against.
	///
	DLL Scalar normalize(const Scalar &, float);
	///
	/// Normalize the value of a scalar against a specified maximum value.
	/// @param scalar The scalar to normalize.
	/// @param max The maximum value to normalize against.
	///
	DLL Scalar normalize(const Scalar &, const Scalar &);
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
	DLL Scalar constrain(const Scalar &input, float deadzone, float range, float grace, bool mirror);
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
	DLL Scalar constrain(const Scalar &input, float deadzone, float range, const Scalar &grace, bool mirror);
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
	DLL Scalar constrain(const Scalar &input, float deadzone, const Scalar &range, float grace, bool mirror);
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
	DLL Scalar constrain(const Scalar &input, float deadzone, const Scalar &range, const Scalar& grace, bool mirror);
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
	DLL Scalar constrain(const Scalar &input, const Scalar& deadzone, float range, float grace, bool mirror);
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
	DLL Scalar constrain(const Scalar &input, const Scalar& deadzone, float range, const Scalar& grace, bool mirror);
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
	DLL Scalar constrain(const Scalar &input, const Scalar& deadzone, const Scalar &range, float grace, bool mirror);
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
	DLL Scalar constrain(const Scalar &input, const Scalar& deadzone, const Scalar &range, const Scalar& grace, bool mirror);
	///
	/// Take the absolute value of a scalar.
	/// @param input The scalar to take the absolute value of.
	///
	DLL Scalar abs(const Scalar &input);
	///
	/// Take the arc cosine of a scalar.
	/// @param input The scalar to take the arc cosine of.
	///
	DLL Scalar acos(const Scalar &input);
	///
	/// -1 or 1 depending on a condition.
	/// Value == *input ? -1.f : 1.f
	/// Used to invert other values depending on a condition.
	/// @param input The condition which dictates Value.
	///
	DLL Scalar invert(const Condition &input);
	///
	/// The change in a scalar since the last update.
	/// Will give the first derivitive of a scalar.
	/// @param input The scalar to track changes in.
	///
	DLL Scalar delta(const Scalar &input);
	///
	/// The scalar projection of one vector onto another.
	/// a is projected onto b.
	/// @param a The projecting vector.
	/// @param b The vector being projected on.
	///
	DLL Scalar project(const Vector &a, const Vector &b);
	///
	/// Value is one of two inputs depending on a condition.
	/// Value = *condition ? t : f;
	/// @param condition Condition up which the output is dependant.
	/// @param t Value == t if condition is true.
	/// @param f Value == f if condition is false.
	///
	DLL Scalar ifScalar(const Condition &condition, float t, float f);
	///
	/// Value is one of two inputs depending on a condition.
	/// Value = *condition ? t : f;
	/// @param condition Condition up which the output is dependant.
	/// @param t Value == t if condition is true.
	/// @param f Value == f if condition is false.
	///
	DLL Scalar ifScalar(const Condition &condition, float t, const Scalar &f);
	///
	/// Value is one of two inputs depending on a condition.
	/// Value = *condition ? a : b;
	/// @param condition Condition up which the output is dependant.
	/// @param a Value == a if condition is true.
	/// @param b Value == b if condition is false.
	///
	DLL Scalar ifScalar(const Condition &condition, const Scalar &t, float f);
	///
	/// Value is one of two inputs depending on a condition.
	/// Value = *condition ? a : b;
	/// @param condition Condition up which the output is dependant.
	/// @param a Value == a if condition is true.
	/// @param b Value == b if condition is false.
	///
	DLL Scalar ifScalar(const Condition &condition, const Scalar &t, const Scalar &f);
	///
	/// Value is set by an opencv track bar.
	/// Takes parameters to define how the integer value, starting at 0, that the tracker supplies is converted to a float.
	/// @param title The name of the tracker bar.
	/// @param max the maximum value the track bar can have.
	/// @param scale How the tracker input (between 0 and max) should be scaled to convert it to a float.
	/// @param shift How the tracker input (between 0 and max) should be shifted to convert it to a float.
	/// @param value The initial value of the track bar.
	///
	DLL Scalar tracker(string title, const int max, float scale = 1.f, float shift = 0.f, const int value = 0);


	//-------------------------------------------------------------------------------------------------------------


	///
	/// Normalize the value of a vector so its magnitude is 1.
	/// @param vector The Vector to normalize.
	/// @param max The maximum value to normalize against.
	///
	DLL Vector normalize(const Vector &vector);
	///
	/// Get the cross product of two vectors.
	/// @param a The first vector.
	/// @param b The second vector.
	///
	DLL Vector cross(const Vector &a, const Vector &b);
	///
	/// Take only specified axes from a vector.
	/// @param input The vector to limit.
	/// @param x Whether to take the x axis.
	/// @param y Whether to take the y axis.
	/// @param z Whether to take the z axis.
	///
	DLL Vector limit(const Vector &input, bool x, bool y, bool z);
	///
	/// The change in a vector since the last update.
	/// Will give the first derivitive of a vector in all three axes.
	/// @param input The vector to track changes in.
	///
	DLL Vector delta(const Vector &input);
	///
	/// Take a snapshot of a vector whenever a condition becomes true.
	/// Every time *condition == true Value is set to vector.
	/// @param vector The vector to take snapshots of.
	/// @param condition Every time this becomes true a snapshot is taken.
	DLL Vector snapshot(const Vector &input, const Condition &);
	///
	/// Take the momenum of an input vector.
	/// As long as condition is true Value == vector.
	/// As soon as condition becomes false Value is taken as the last position before the condition become false + 
	/// the delta between the penultimate true position and the final true position. 
	/// After this the same delta keeps on being applied and reduced until the magnitude of the delta approaches zero.
	/// @param input The vector to take the momentum of.
	/// @param condition If true Value is input. Otherwise Value is taken from the momentum of the final position before condition became false.
	///
	DLL Vector momentum(const Vector &input, const Condition &condition);
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
	DLL Vector momentum(const Vector &input, const Condition &condition, float decrement, float threshold);
	///
	/// Scale a vector so that its magnitude is scale.
	/// @param input the vector to scale.
	/// @param scale The magnitude Value is to have in the direction of input.
	/// 
	DLL Vector scale(const Vector &input, const Scalar &scale);
	///
	/// Scale a vector so that its magnitude is scale.
	/// @param input the vector to scale.
	/// @param scale The magnitude Value is to have in the direction of input.
	/// 
	DLL Vector scale(const Vector &input, float scale);
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
	DLL Vector intersect(const Vector &pPlane, const Vector &normalPlane, const Vector &pLine, const Vector &dirLine);
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
	DLL Vector joint(const int joint);	
	
	///
	/// True if all points are reasonable spread out.
	/// Guards against incorrect input when all the points cluster together.
	/// Reasonably spread out is defined as the area contained in the circle head->handR->footR->footL->handL->head.
	///
	DLL Condition closeGuard();
	///
	/// True if all points are reasonable spread out.
	/// Guards against incorrect input when all the points cluster together.
	/// Reasonably spread out is defined as the area contained in the circle head->handR->footR->footL->handL->head.
	/// @param scale The threshold for the area of the points within the circle.
	///
	DLL Condition closeGuard(float scale);
	///
	/// True if all points are reasonable spread out.
	/// Guards against incorrect input when all the points cluster together.
	/// Reasonably spread out is defined as the area contained in the circle head->handR->footR->footL->handL->head.
	/// @param scale The threshold for the area of the points within the circle.
	///
	DLL Condition closeGuard(Scalar *scale);
	///
	/// True if the specified hand is in a fist. False otherwise
	/// @param rightHand If true, track the right hand. False, left hand.
	/// 
	DLL Condition fist(bool rightHand);
	///
	/// True once when condition becomes true.
	/// Once the true value has been accessed it returns to false.
	/// @param condition The condition which is being checked for clicking.
	///
	DLL Condition click(const Condition &condition);
	///
	/// True if condition has been true for longer than a set threshold.
	/// @param condition The condition which is being checked for holding.
	///
	DLL Condition hold(const Condition &condition);	

	/// 
	/// The interface for factory classes which instantiate NuiLib objects.
	/// Contains generic template code which can instantiate any classes with the correct format.
	/// Deals with Vectors, Scalars and Conditions.
	/// Also provides some extra control over how polling is done.
	///
	class DLL INuiFactory : public IObservable {
	protected:
		///
		/// Initialise the factory.
		/// Specifies the type of the factory and its name.
		/// @param type The type of factory this is.
		/// @param name The name of this factory.
		///
		INuiFactory(string type, string name) { }


	public:
		///
		/// Delete all components this factory created.
		///
		virtual ~INuiFactory();

		///
		/// Initialise the factory. Any logic to do with starting up the factory should go here.
		///
		virtual bool Init() = 0;

		///
		/// Set whether the factory auto polls.
		/// If true the factory will automatically update all entities it has created whenever the Nui device sends and update or only update them when the Poll method is called.
		/// @param value True to auto poll. False to only poll when the method is called.
		/// @see Poll
		/// @see GetAutoPoll
		///
		virtual void SetAutoPoll(bool) = 0;
		///
		/// Get whether the factory auto polls.
		/// If true the factory will automatically update all entities it has regstered whenever the Nui device sends and update or only update them when the Poll method is called.
		/// @param value True to auto poll. False to only poll when the method is called.
		/// @see Poll
		/// @see SetAutoPoll
		///
		virtual bool GetAutoPoll() = 0;
		///
		/// Force the factory to poll the Nui device for new data.
		/// When the device has been polled all entities which take data from the device will be updated. If AutoPoll is false calling this method is the only time the device will be polled.
		/// @see SetAutoPoll
		///
		virtual void Poll() = 0;

		///
		/// Look up a Scalar from its name.
		/// Designed to be used after a call to LoadFromXML. LoadFromXML imports a series of components. GetScalar then gets a given scalar.
		/// If no scalar is found with the given name a zero Scalar is returned.
		///
		virtual Scalar GetScalar(string name) = 0;
		///
		/// Look up a Condition from its name.
		/// Designed to be used after a call to LoadFromXML. LoadFromXML imports a series of components. GetCondition then gets a given condition.
		/// If no scalar is found with the given name a zero Condition is returned.
		///
		virtual Condition GetCondition(string name) = 0;
		///
		/// Look up a Vector from its name.
		/// Designed to be used after a call to LoadFromXML. LoadFromXML imports a series of components. GetVector then gets a given vector.
		/// If no vector is found with the given name a zero Vector is returned.
		///
		virtual Vector GetVector(string name) = 0;

		///
		/// Get the current depth frame.
		///
		virtual const cv::Mat GetDepth() = 0;
		///
		/// Get the current colour frame.
		///
		virtual const cv::Mat GetColour() = 0;


		///
		/// Convert a point in 3D space to a 2D coordinate on the depth image.
		///
		virtual cv::Point SkeletonToDepth(Vector point) = 0;
		///
		/// Convert a point in 3D space to a 2D coordinate on the colour image.
		///
		virtual cv::Point SkeletonToColour(Vector point) = 0;

		///
		/// Convert a point in 2D depth image coordinates to a point in 3D space.
		///
		virtual cv::Point3f DepthToSkeleton(cv::Point point) = 0;
		///
		/// Convert a point in 2D colour image coordinates to a point in 3D space.
		///
		virtual cv::Point3f ColourToSkeleton(cv::Point point) = 0;

		///
		/// Write all components created by this factory, and their relationships, to XML.
		/// Not currently implemented.
		///
		virtual string ToXML() = 0;
		///
		/// Initialise the factory based on an XML string.
		/// Not currently implemented.
		/// @param xml The XML string to load.
		///
		virtual void LoadXML(string xml) = 0;

		///
		/// Get an array of pointers to the actual NUI devices that are powering the system.
		/// Depending on which factory implementation is linked the actual devices returned will be different.
		///
		virtual void **GetNuiDevices() = 0;
	};

	///
	/// Get the factory which can be used to interact with the NUI device.
	///
	DLL INuiFactory *NuiFactory();
}
#endif



/**

\mainpage Home

\section license License
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

\section blog Blog / Source
The blog for this project is available <a href="blogs.cs.st-andrews.ac.uk/johnmcc">here</a>. This includes more information how to use and extend the library. The repository for the source code is <a href="https://github.com/JohnMcCaffery/NuiLib">here</a>.

\section instructions Instructions
<strong>&lt;NuiLib dir&gt;</strong> specifies the directory where NuiLib is installed to. It includes the sub directories "Include" and "Src" and has the solution, prebuild and makefile files in it.<br>
<strong>&lt;flags&gt;</strong> can be d, v, dv or nothing. D specifies debug, V specifies visual. Visual mode means that, if the depth stream is enabled, the depth frames will be output, overlaid with debug information such as the position of joints and how the fist finder algorithm is operating.

\subsection setup Setting Up:
This project relies upon <a href="http://sourceforge.net/projects/opencvlibrary/">OpenCV</a>. To work you will need <a href="http://sourceforge.net/projects/opencvlibrary/">OpenCV</a> to be present on your computer. Before attempting to compile NuiLib or any project that links to NuiLib you will need to download and install <a href="http://sourceforge.net/projects/opencvlibrary/">OpenCV</a> which can be downloaded <a href="http://sourceforge.net/projects/opencvlibrary/files/latest/download">here</a>. Once installed you need to copy the relevant libraries to your Bin/ directory. If you want to use the pre-setup build tools such as the makefile and Visual Studio projects you also need to set up a global environment variable called "OPENCV_DIR" which points to the <a href="http://sourceforge.net/projects/opencvlibrary/">OpenCV</a> installation. To do both these things automatically run run prebuild.bat (Visual Studio) or prebuild.sh (MingW).

\subsection build How To Build:
\subsubsection build_vc Visual Studio 10
1. Open NuiLib.sln
2. Select a profile
3. Build all (F6)
Libraries compile to 'NuiLib-VC10&lt;flags&gt;.lib' and 'NuiLib-Kinect-MS-VC10&lt;flags&gt;.lib' in Lib/.

\subsubsection build_gpp MingW + Make + G++
1. Run the make target "DLL".
Library compiles to "NuiLib.dll" in Lib/. "libNuiLib.dll.a" is also compiled to the same folder for linking. "NuiLib.dll" is copied to Bin/.

\subsection include How To Include:
Add /Include/ to the include path.<br>
<em>VS-10</em>: &lt;NuiLib dir&gt;Project-&gt;Properties-&gt;VC++ Directories-&gt;Include Directories.<br>
<em>G++</em>: -I &lt;NuiLib dir&gt;/Include/<br>
<strong>#include &lt;NuiLib-API.h&gt;</strong> at the top of any relevant source. This will give access to everything documented <a href="http://www.cs.st-andrews.ac.uk/~johnmcc/Documentation/">here</a>. 

\subsection link How To Link:
\subsubsection link_vc10 Visual Studio 10
Add the following to the library path (Project-&gt;Properties-&gt;VC++ Directories-&gt;Library Directories):<br>
<em>&lt;NuiLib dir&gt;\Lib\ <br>
$(OPENCV_DIR)build\x86\vc10\lib\ <br>
$(KINECTSDK10_DIR)lib\x86</em>

Add the following to the linker's additional dependencies (Project-&gt;Properties-&gt;Linker-&gt;Input-&gt;Additional Dependencies):<br>
<em>NuiLib-VC10&lt;flags&gt;.lib<br>
NuiLib-Kinect-MS-VC10&lt;flags&gt;.lib<br>
opencv_core241[d].lib<br>
opencv_highgui241[d].lib<br>
opencv_imgproc241[d].lib<br>
opencv_objdetect241[d].lib<br>
kinect10.lib</em>

\subsubsection link_gpp G++
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

Full flag list: -L &lt;NuiLib dir&gt;/Lib/ -L $(OPENCV_DIR)build/x86/mingw/lib/ -L $(KINECTSDK10_DIR)lib/x86/</em> -l NuiLib.dll -l opencv_core241.dll -l opencv_highgui241.dll -l opencv_imgproc241.dll -l opencv_objdetect241.dll -l Kinect10.lib

\subsection components How To Add Components:

Add /Include/ to the include path.
<em>VS-10</em>: &lt;NuiLib dir&gt;Project-&gt;Properties-&gt;VC++ Directories-&gt;Include Directories.
<em>G++</em>: -I &lt;NuiLib dir&gt;/Include/
<strong>#include &lt;NuiLib-Extendable.h&gt;</strong> at the top of any relevant source. This will give access to everything documented <a href="http://www.cs.st-andrews.ac.uk/~johnmcc/Documentation-Full/">here</a>. Specifically IComponent, IScalar, IVector and ICondition.
To be compatible new components must extend from IScalar, IVector or ICondition (or one of their pre-existing abstract sub-classes such as VectorWrappingScalar. These are found in 'NuiLib-Vectors.h', 'NuiLib-Conditions.h' or 'NuiLib-Scalars.h').
Generally all that is necessary is to implement a new component is to declare the class and one or more factory methods. The class must have a 0 argument constructor. It must also override the 'CalculateValue' method and implement 'static string GetName()' method. It may also need to provide mechanisms for setting any fields it needs to work with. If it has generic behaviour such as wrapping a Vector a pre-existing abstract super class may exist that does this for you.
The factory methods should return a Scalar, Vector or Condition with the _p field assigned to a new instance of the class that has been fully initialized.

\subsection drivers How To Add Support for Other Drivers/Devices:
Add /Include/ to the include path.
<em>VS-10</em>: &lt;NuiLib dir&gt;Project-&gt;Properties-&gt;VC++ Directories-&gt;Include Directories.
<em>G++</em>: -I &lt;NuiLib dir&gt;/Include/
<strong>#include &lt;NuiLib-Extendable.h&gt;</strong> at the top of any relevant source. This will give access to everything documented <a href="http://www.cs.st-andrews.ac.uk/~johnmcc/Documentation-Full/">here</a>. Specifically INuiFactoryExtension.
The main implementation is done via a factory class which extends INuiFactoryExtension. This should take care of polling the device and updating any components which need to be updated.
As well as extending the INuiFactoryExtension class the following methods must be implemented:<br>
INuiFactory()<br>
ExtensionFactory()<br>
GetHandProcessor()<br>
joint(const int joint)<br>
The first two should return a pointer to a singleton instance of the new factory class. The last should return a Vector that will contain the coordinates of a skeleton joint.

\subsection info More Info:
\subsubsection prebuild Prebuild
Prebuild does 3 things. Firstly it creates a global environment variable $(OPENCV_DIR) and asks the user to enter a value for it. This value should point to the main directory where Open CV has been installed and built. This variable will be used in the Visual Studio projects and the makefile to resolve dependencies. Secondly the prebuild creates a Bin/ directory. Thirdly it copies all the required libraries from the Open CV installation and the Microsoft Kinect SDK installation into Bin/.

\section notes Notes
This project has only been tested on Windows 7 using Visual Studio 10 and MingW. 

**/
