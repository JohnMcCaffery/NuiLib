// NuiLib-.h
#include <NuiLib-CLRSafe.h>
#include <cstring>
#include <list>

#pragma once

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace NuiLibSafe;

namespace NuiLibDotNet {
	ref class Scalar;
	ref class Vector;
	ref class Condition;

	public delegate void ChangeDelegate();
	public delegate void SkeletonTrackDelegate();

	public ref class Observable {
	private:
		ChangeDelegate^ _onChange;

	protected:
		void ChangeListener() {
			OnChange();
		}

	public:
		event ChangeDelegate ^OnChange {
			void add (ChangeDelegate ^listener) {
				_onChange += listener;
			}
			void remove (ChangeDelegate ^listener) {
				_onChange -= listener;
			}
			void raise() {
				ChangeDelegate^ tmp = _onChange;
				if (tmp)
					tmp->Invoke();
			}
		}
	};

	public ref class Scalar : public Observable {
	private:
		Scalar(SafeScalar *scalar) : _ps(scalar) { }

	public:
		SafeScalar *_ps;
		ChangeDelegate^ _managedCallback;

		!Scalar() { 
			delete _ps;
		}

		static Scalar ^Create(SafeScalar *safe) {
			Scalar^ scalar = gcnew Scalar(safe);

			scalar->_managedCallback = gcnew ChangeDelegate(scalar, &ChangeListener);
			IntPtr unmanagedCallback = Marshal::GetFunctionPointerForDelegate(scalar->_managedCallback);
			GC::KeepAlive(scalar->_managedCallback);

			safe->SetCallback((CallbackFunction) (void*) unmanagedCallback);

			return scalar;
		}

		static Scalar ^Create(float value) {
			return Create(new SafeScalar(value));
		}
		static Scalar ^Create(String ^name, float value) {
			return Create(new SafeScalar((const char*) (Marshal::StringToHGlobalAnsi(name)).ToPointer(), value));
		}

		property String ^Name {
			String ^get() { return gcnew String(_ps->GetName()); }
			void set (String ^value) { _ps->SetName(((const char*)Marshal::StringToHGlobalAnsi(value).ToPointer())); }
		}

		property float Value {
			float get() { return _ps->Get(); }
			void set(float value) { _ps->Set(value); }
		}



		//--------------------------------------------------------------------------------------------

		/// <summary>
		/// Arithmetic sum of two scalars.
		/// </summary>
		static Scalar ^operator+(Scalar ^addend1, Scalar ^addend2);
		/// <summary>
		/// Arithmetic difference of two scalars.
		/// </summary>
		static Scalar ^operator-(Scalar ^minuend, Scalar ^subtrahend);
		/// <summary>
		/// Arithmetic product of two scalars.
		/// </summary>
		static Scalar ^operator*(Scalar ^factor1, Scalar ^factor2);
		/// <summary>
		/// Arithmetic quotient of two scalars.
		/// </summary>
		static Scalar ^operator/(Scalar ^dividend, Scalar ^divisor);
		/// <summary>
		/// Arithmetic sum of two scalars.
		/// </summary>
		static Scalar ^operator+=(Scalar ^addend1, Scalar ^addend);
		/// <summary>
		/// Arithmetic difference of two scalars.
		/// minuend is assigned the new value.
		/// </summary>
		static Scalar ^operator-=(Scalar ^minuend, Scalar ^subtrahend);
		/// <summary>
		/// Arithmetic product of two scalars.
		/// factor1 is assigned the new value.
		/// </summary>
		static Scalar ^operator*=(Scalar ^factor1, Scalar ^factor2);
		/// <summary>
		/// Arithmetic quotient of two scalars.
		/// dividend is assigned the new value.
		/// </summary>
		static Scalar ^operator/=(Scalar ^dividend, Scalar ^divisor);

		/// <summary>
		/// Arithmetic sum of two scalars.
		/// </summary>
		static Scalar ^operator+(Scalar ^addend1, float addend2);
		/// <summary>
		/// Arithmetic difference of two scalars.
		/// </summary>
		static Scalar ^operator-(Scalar ^minuend, float subtrahend);
		/// <summary>
		/// Arithmetic product of two scalars.
		/// </summary>
		static Scalar ^operator*(Scalar ^factor1, float factor2);
		/// <summary>
		/// Arithmetic quotient of two scalars.
		/// </summary>
		static Scalar ^operator/(Scalar ^dividend, float divisor);
		/// <summary>
		/// Arithmetic sum of two scalars.
		/// </summary>
		static Scalar ^operator+=(Scalar ^addend1, float addend);
		/// <summary>
		/// Arithmetic difference of two scalars.
		/// minuend is assigned the new value.
		/// </summary>
		static Scalar ^operator-=(Scalar ^minuend, float subtrahend);
		/// <summary>
		/// Arithmetic product of two scalars.
		/// factor1 is assigned the new value.
		/// </summary>
		static Scalar ^operator*=(Scalar ^factor1, float factor2);
		/// <summary>
		/// Arithmetic quotient of two scalars.
		/// </summary>
		static Scalar ^operator/=(Scalar ^dividend, float divisor);

		/// <summary>
		/// Arithmetic sum of two scalars.
		/// </summary>
		static Scalar ^operator+(float addend1, Scalar ^addend2);
		/// <summary>
		/// Arithmetic difference of two scalars.
		/// </summary>
		static Scalar ^operator-(float minuend, Scalar ^subtrahend);
		/// <summary>
		/// Arithmetic product of two scalars.
		/// </summary>
		static Scalar ^operator*(float factor1, Scalar ^factor2);
		/// <summary>
		/// Arithmetic quotient of two scalars.
		/// </summary>
		static Scalar ^operator/(float dividend, Scalar ^divisor);


		/// <summary>
		/// True if operand1 is not equal to operand2.
		/// </summary>
		static Condition ^operator!=(float operand1, Scalar ^operand2);
		/// <summary>
		/// True if operand1 is equal to operand2.
		/// Operand1 must be qual to all components of operand2 (x, y and z).
		/// </summary>
		static Condition ^operator==(float operand1, Scalar ^operand2);
		/// <summary>
		/// True if operand1 is less than or equal. to operand2
		/// </summary>
		static Condition ^operator<=(float operand1, Scalar ^operand2);
		/// <summary>
		/// True if operand1 is greater than operand2.
		/// </summary>
		static Condition ^operator>(float operand1, Scalar ^operand2);
		/// <summary>
		/// True if operand1 is less than operand2.
		/// </summary>
		static Condition ^operator<(float operand1, Scalar ^operand2);
		/// <summary>
		/// True if operand1 is greater than or equ.al to operand2
		/// </summary>
		static Condition ^operator>=(float operand1, Scalar ^operand2);
		/// <summary>
		/// True if operand1 is not equal to operand2.
		/// </summary>
		static Condition ^operator!=(Scalar ^operand1, float operand2);
		/// <summary>
		/// True if operand1 is equal to operand2.
		/// All components of operand1 (x, y and z) must be equal of to operand1.
		/// </summary>
		static Condition ^operator==(Scalar ^operand1, float operand2);
		/// <summary>
		/// True if operand1 is greater than operand2.
		/// </summary>
		static Condition ^operator>(Scalar ^operand1, float operand2);
		/// <summary>
		/// True if operand1 is less than operand2.
		/// </summary>
		static Condition ^operator<(Scalar ^operand1, float operand2);
		/// <summary>
		/// True if operand1 is greater than or equ.al to operand2
		/// </summary>
		static Condition ^operator>=(Scalar ^operand1, float operand2);
		/// <summary>
		/// True if operand1 is less than or equal. to operand2
		/// </summary>
		static Condition ^operator<=(Scalar ^operand1, float operand2);
		/// <summary>
		/// True if operand1 is not equal to operand2.
		/// </summary>
		static Condition ^operator!=(Scalar ^operand1, Scalar ^operand2);
		/// <summary>
		/// True if operand1 is greater than operand2.
		/// </summary>
		static Condition ^operator>(Scalar ^operand1, Scalar ^operand2);
		/// <summary>
		/// True if operand1 is less than operand2.
		/// </summary>
		static Condition ^operator<(Scalar ^operand1, Scalar ^operand2);
		/// <summary>
		/// True if operand1 is greater than or equ.al to operand2
		/// </summary>
		static Condition ^operator>=(Scalar ^operand1, Scalar ^operand2);
		/// <summary>
		/// True if operand1 is less than or equal. to operand2
		/// </summary>
		static Condition ^operator<=(Scalar ^operand1, Scalar ^operand2);
		/// <summary>
		/// True if operand1 is equal to operand2.
		/// </summary>
		static Condition ^operator==(Scalar ^operand1, Scalar ^operand2);


		//--------------------------------------------------------------------------------------------


	};


	public ref class Vector : public Observable {
	private:
		Vector(SafeVector *vector) : _ps(vector) { }

	public:
		ChangeDelegate^ _managedCallback;
		SafeVector *_ps;

		!Vector() { delete _ps; }

		static Vector ^Create(SafeVector *safe) {
			Vector^ vector = gcnew Vector(safe);

			vector->_managedCallback = gcnew ChangeDelegate(vector, &ChangeListener);
			IntPtr unmanagedCallback = Marshal::GetFunctionPointerForDelegate(vector->_managedCallback);
			GC::KeepAlive(vector->_managedCallback);

			safe->SetCallback((CallbackFunction) (void*) unmanagedCallback);

			return vector;
		}

		static Vector ^Create(float value) {
			return Create(new SafeVector(value));
		}
		static Vector ^Create(String ^name, float value) {
			return Create(new SafeVector((const char*) (Marshal::StringToHGlobalAnsi(name)).ToPointer(), value));
		}

		static Vector ^Create(float x, float y, float z) {
			return Create(new SafeVector(x, y, z));
		}
		static Vector ^Create(String ^name, float x, float y, float z) {
			return Create(new SafeVector((const char*) (Marshal::StringToHGlobalAnsi(name)).ToPointer(), x, y, z));
		}

		property String ^Name {
			String ^get() { return gcnew String(_ps->GetName()); }
			void set (String ^value) { _ps->SetName(((const char*)Marshal::StringToHGlobalAnsi(value).ToPointer())); }
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

		//--------------------------------------------------------------------------------------------

		/// <summary>
		/// Arithmetic sum of two scalars.
		/// </summary>
		static Vector ^operator+(Vector ^addend1, Vector ^addend2);
		/// <summary>
		/// Arithmetic difference of two scalars.
		/// </summary>
		static Vector ^operator-(Vector ^minuend, Vector ^subtrahend);
		/// <summary>
		/// Arithmetic product of two scalars.
		/// </summary>
		static Vector ^operator*(Vector ^factor1, Vector ^factor2);
		/// <summary>
		/// Arithmetic quotient of two scalars.
		/// </summary>
		static Vector ^operator/(Vector ^dividend, Vector ^divisor);
		/// <summary>
		/// Arithmetic sum of two scalars.
		/// </summary>
		static Vector ^operator+=(Vector ^addend1, Vector ^addend);
		/// <summary>
		/// Arithmetic difference of two scalars.
		/// minuend is assigned the new value.
		/// </summary>
		static Vector ^operator-=(Vector ^minuend, Vector ^subtrahend);
		/// <summary>
		/// Arithmetic product of two scalars.
		/// factor1 is assigned the new value.
		/// </summary>
		static Vector ^operator*=(Vector ^factor1, Vector ^factor2);
		/// <summary>
		/// Arithmetic quotient of two scalars.
		/// dividend is assigned the new value.
		/// </summary>
		static Vector ^operator/=(Vector ^dividend, Vector ^divisor);

		/// <summary>
		/// Arithmetic sum of two scalars.
		/// </summary>
		static Vector ^operator+(Vector ^addend1, float addend2);
		/// <summary>
		/// Arithmetic difference of two scalars.
		/// </summary>
		static Vector ^operator-(Vector ^minuend, float subtrahend);
		/// <summary>
		/// Arithmetic product of two scalars.
		/// </summary>
		static Vector ^operator*(Vector ^factor1, float factor2);
		/// <summary>
		/// Arithmetic quotient of two scalars.
		/// </summary>
		static Vector ^operator/(Vector ^dividend, float divisor);
		/// <summary>
		/// Arithmetic sum of two scalars.
		/// </summary>
		static Vector ^operator+=(Vector ^addend1, float addend);
		/// <summary>
		/// Arithmetic difference of two scalars.
		/// minuend is assigned the new value.
		/// </summary>
		static Vector ^operator-=(Vector ^minuend, float subtrahend);
		/// <summary>
		/// Arithmetic product of two scalars.
		/// factor1 is assigned the new value.
		/// </summary>
		static Vector ^operator*=(Vector ^factor1, float factor2);
		/// <summary>
		/// Arithmetic quotient of two scalars.
		/// </summary>
		static Vector ^operator/=(Vector ^dividend, float divisor);

		/// <summary>
		/// Arithmetic sum of two scalars.
		/// </summary>
		static Vector ^operator+(float addend1, Vector ^addend2);
		/// <summary>
		/// Arithmetic difference of two scalars.
		/// </summary>
		static Vector ^operator-(float minuend, Vector ^subtrahend);
		/// <summary>
		/// Arithmetic product of two scalars.
		/// </summary>
		static Vector ^operator*(float factor1, Vector ^factor2);
		/// <summary>
		/// Arithmetic quotient of two scalars.
		/// </summary>
		static Vector ^operator/(float dividend, Vector ^divisor);

		/// <summary>
		/// False if operand1 is equal to all components of operand2 (x, y and z).
		/// </summary>
		static Condition ^operator!=(float operand1, Vector ^operand2);
		/// <summary>
		/// False if all components of operand1 (x, y and z) are equal to operand2.
		/// </summary>
		static Condition ^operator!=(Vector ^operand1, float operand2);
		/// <summary>
		/// True if operand1 is not equal to operand2.
		/// </summary>
		static Condition ^operator!=(Vector ^operand1, Vector ^operand2);
		/// <summary>
		/// True if operand1 is equal to operand2.
		/// </summary>
		//static Condition ^operator==(float operand1, Vector ^operand2);
		/// <summary>
		/// True if operand1 is equal to operand2.
		///	</summary>
		//static Condition ^operator==(Vector ^operand1, float operand2);
		/// <summary>
		/// True if operand1 is equal to operand2.
		/// </summary>
		//static Condition ^operator==(Vector ^operand1, Vector ^operand2);


	};


	public ref class Condition : public Observable {
	private:
		Condition(SafeCondition *condition) : _ps(condition) { }

	public:
		SafeCondition *_ps;
		ChangeDelegate^ _managedCallback;

		!Condition() { delete _ps; }

		static Condition ^Create(SafeCondition *safe) {
			Condition^ condition = gcnew Condition(safe);

			condition->_managedCallback = gcnew ChangeDelegate(condition, &ChangeListener);
			IntPtr unmanagedCallback = Marshal::GetFunctionPointerForDelegate(condition->_managedCallback);
			GC::KeepAlive(condition->_managedCallback);

			safe->SetCallback((CallbackFunction) (void*) unmanagedCallback);

			return condition;
		}

		static Condition ^Create(bool value) {
			return Create(new SafeCondition(value));
		}
		static Condition ^Create(String ^name, bool value) {
			return Create(new SafeCondition((const char*) (Marshal::StringToHGlobalAnsi(name)).ToPointer(), value));
		}

		property String ^Name {
			String ^get() { return gcnew String(_ps->GetName()); }
			void set (String ^value) { _ps->SetName(((const char*)Marshal::StringToHGlobalAnsi(value).ToPointer())); }
		}

		property bool Value {
			bool get() { return _ps->Get(); }
			void set(bool value) { _ps->Set(value); }
		}





		/// <summary>
		/// True if operand is false.
		/// </summary>
		static Condition ^operator!(Condition ^operand);

		/// <summary>
		/// True if both operand1 and operand2 are true.
		/// </summary>
		static Condition ^operator&&(Condition ^operand1, Condition ^operand2);
		/// <summary>
		/// True if both operand1 and operand2 are true.
		/// </summary>
		static Condition ^operator&&(Condition ^operand1, bool operand2);
		/// <summary>
		/// True if both operand1 and operand2 are true.
		/// </summary>
		static Condition ^operator&&(bool operand1, Condition ^operand2);

		/// <summary>
		/// True if either operand1 or operand2 or both are true.
		/// </summary>
		static Condition ^operator||(Condition ^operand1, Condition ^operand2);
		/// <summary>
		/// True if either operand1 or operand2 or both are true.
		/// </summary>
		static Condition ^operator||(Condition ^operand1, bool);
		/// <summary>
		/// True if either operand1 or operand2 or both are true.
		/// </summary>
		static Condition ^operator||(bool operand1, Condition ^operand2);

		/// <summary>
		/// True if both operand1 and operand2 are true.
		/// </summary>
		static Condition ^And(Condition ^operand1, Condition ^operand2);
		/// <summary>
		/// True if both operand1 and operand2 are true.
		/// </summary>
		static Condition ^And(Condition ^operand1, bool operand2);
		/// <summary>
		/// True if both operand1 and operand2 are true.
		/// </summary>
		static Condition ^And(bool operand1, Condition ^operand2);

		/// <summary>
		/// True if either operand1 or operand2 or both are true.
		/// </summary>
		static Condition ^Or(Condition ^operand1, Condition ^operand2);
		/// <summary>
		/// True if either operand1 or operand2 or both are true.
		/// </summary>
		static Condition ^Or(Condition ^operand1, bool);
		/// <summary>
		/// True if either operand1 or operand2 or both are true.
		/// </summary>
		static Condition ^Or(bool operand1, Condition ^operand2);
	};

	public ref class Nui {
		private:
			static Nui ^_nui;
			static ChangeDelegate^ _tickCallback;
			static SkeletonTrackDelegate^ _skeletonFoundCallback;
			static SkeletonTrackDelegate^ _skeletonLostCallback;
			static SkeletonTrackDelegate^ _skeletonSwitchedCallback;

			static ChangeDelegate^ _tick;
			static SkeletonTrackDelegate^ _skeletonFound;
			static SkeletonTrackDelegate^ _skeletonLost;
			static SkeletonTrackDelegate^ _skeletonSwitched;

			void TickListener() { Tick(); }
			void SkeletonFoundListener() { SkeletonFound(); }
			void SkeletonLostListener() { SkeletonLost(); }
			void SkeletonSwitchedListener() { SkeletonSwitched(); }

			static void RegisterListeners() {
				_nui = gcnew Nui();

				_tickCallback = gcnew ChangeDelegate(_nui, &TickListener);
				IntPtr tickCallback = Marshal::GetFunctionPointerForDelegate(_tickCallback);

				_skeletonFoundCallback = gcnew SkeletonTrackDelegate(_nui, &SkeletonFoundListener);
				IntPtr foundCallback = Marshal::GetFunctionPointerForDelegate(_skeletonFoundCallback);

				_skeletonLostCallback = gcnew SkeletonTrackDelegate(_nui, &SkeletonLostListener);
				IntPtr lostCallback = Marshal::GetFunctionPointerForDelegate(_skeletonLostCallback);

				_skeletonSwitchedCallback = gcnew SkeletonTrackDelegate(_nui, &SkeletonSwitchedListener);
				IntPtr switchedCallback = Marshal::GetFunctionPointerForDelegate(_skeletonSwitchedCallback);

				NuiLibSafe::RegisterCallbacks(
					(CallbackFunction) (void*) tickCallback,
					(SkeletonCallbackFunction) (void*) foundCallback,
					(SkeletonCallbackFunction) (void*) lostCallback,
					(SkeletonCallbackFunction) (void*) switchedCallback);
			}

			static System::Drawing::Bitmap ^MakeFrame(unsigned char *bytes, System::Drawing::Imaging::PixelFormat format) {
					System::Drawing::Rectangle bounds = System::Drawing::Rectangle(0, 0, 640, 480);
					System::Drawing::Imaging::ImageLockMode mode = System::Drawing::Imaging::ImageLockMode::WriteOnly;

					System::Drawing::Bitmap ^b = gcnew System::Drawing::Bitmap(640, 480, format);
					System::Drawing::Imaging::BitmapData ^x = b->LockBits(bounds, mode, format);

					x->Scan0 = IntPtr( (void *) bytes);

					b->UnlockBits(x);

					return b;
			}

			static System::Drawing::Bitmap ^MakeFrame(array<Byte> ^bytes, System::Drawing::Imaging::PixelFormat format) {
					System::Drawing::Rectangle bounds = System::Drawing::Rectangle(0, 0, 640, 480);
					System::Drawing::Imaging::ImageLockMode mode = System::Drawing::Imaging::ImageLockMode::WriteOnly;

					System::Drawing::Bitmap ^b = gcnew System::Drawing::Bitmap(640, 480, format);
					System::Drawing::Imaging::BitmapData ^x = b->LockBits(bounds, mode, format);

					System::Runtime::InteropServices::Marshal::Copy(bytes, 0, x->Scan0, bytes.Length);

					b->UnlockBits(x);

					return b;
			}

		public:

			static event ChangeDelegate ^Tick {
				void add (ChangeDelegate ^listener) {
					if (_tickCallback == nullptr)
						RegisterListeners();

					_tick += listener; 
				}
				void remove (ChangeDelegate ^listener) { _tick -= listener; }
				void raise() {
					ChangeDelegate^ tmp = _tick;
					if (tmp)
						tmp->Invoke();
				}
			}	

			static event SkeletonTrackDelegate ^SkeletonFound {
				void add (SkeletonTrackDelegate ^listener) {
					if (_tickCallback == nullptr)
						RegisterListeners();

					_skeletonFound += listener; 
				}
				void remove (SkeletonTrackDelegate ^listener) { _skeletonFound -= listener; }
				void raise() {
					SkeletonTrackDelegate^ tmp = _skeletonFound;
					if (tmp)
						tmp->Invoke();
				}
			}

			static event SkeletonTrackDelegate ^SkeletonLost {
				void add (SkeletonTrackDelegate ^listener) {
					if (_tickCallback == nullptr)
						RegisterListeners();

					_skeletonLost += listener; 
				}
				void remove (SkeletonTrackDelegate ^listener) { _skeletonLost -= listener; }
				void raise() {
					SkeletonTrackDelegate^ tmp = _skeletonLost;
					if (tmp)
						tmp->Invoke();
				}
			}

			static event SkeletonTrackDelegate ^SkeletonSwitched {
				void add (SkeletonTrackDelegate ^listener) { 
					if (_tickCallback == nullptr)
						RegisterListeners();

					_skeletonSwitched += listener; 
				}
				void remove (SkeletonTrackDelegate ^listener) { _skeletonSwitched -= listener; }
				void raise() {
					SkeletonTrackDelegate^ tmp = _skeletonSwitched;
					if (tmp)
						tmp->Invoke();
				}
			}

			/// <summary>
			/// The current frame from the Kinect's colour camera. Wrapped as a bitmap.
			/// WARNING! NuiLib will create a new Bitmap object every time this property is called.
			/// It will never dispose of these bitmap objects, so when you are done with a frame you need to call the Dispose method to clean up the memory allocated for the image.
			/// </summary>
			static property System::Drawing::Bitmap ^ColourFrame {
				System::Drawing::Bitmap ^get() {
					System::Drawing::Imaging::PixelFormat format = System::Drawing::Imaging::PixelFormat::Format32bppRgb;
					return MakeFrame(ColourBytes, format);
					//return MakeFrame(NuiLibSafe::GetColourBytes(), format);
				}
			}

			/// <summary>
			/// The current frame from the Kinect's depth camera. Wrapped as a bitmap.
			/// WARNING! NuiLib will create a new Bitmap object every time this property is called.
			/// It will never dispose of these bitmap objects, so when you are done with a frame you need to call the Dispose method to clean up the memory allocated for the image.
			/// </summary>
			static property System::Drawing::Bitmap ^DepthFrame {
				System::Drawing::Bitmap ^get() {
					System::Drawing::Imaging::PixelFormat format = System::Drawing::Imaging::PixelFormat::Format24bppRgb;
					return MakeFrame(DepthBytes, format);
					//return MakeFrame(NuiLibSafe::GetDepthBytes(), format);
				}
			}

			static System::Drawing::Point SkeletonToColour(Vector ^v) {
				NuiLibSafe::Point p = NuiLibSafe::SkeletonToColour((SafeVector *)v->_ps);
				return System::Drawing::Point(p.X, p.Y);
			}

			static System::Drawing::Point SkeletonToDepth(Vector ^v) {
				NuiLibSafe::Point p = NuiLibSafe::SkeletonToDepth((SafeVector *)v->_ps);
				return System::Drawing::Point(p.X, p.Y);
			}

			static property array<Byte> ^DepthBytes {
				array<Byte> ^get() {
					int w = NuiLibSafe::GetDepthWidth();
					int h = NuiLibSafe::GetDepthHeight();
					int s = NuiLibSafe::GetDepthStride();
					int l = w * h * s;

					array<Byte> ^ret = gcnew array<Byte>(l);

					System::Runtime::InteropServices::Marshal::Copy( IntPtr( (void * ) NuiLibSafe::GetDepthBytes()), ret, 0, l);

					return ret;
				}
			}

			static property array<Byte> ^ColourBytes {
				array<Byte> ^get() {
					int w = NuiLibSafe::GetColourWidth();
					int h = NuiLibSafe::GetColourHeight();
					int s = NuiLibSafe::GetColourStride();

					array<Byte> ^ret = gcnew array<Byte>(w * h * s);

					System::Runtime::InteropServices::Marshal::Copy( IntPtr( (void * ) NuiLibSafe::GetColourBytes()), ret, 0, w * h * s);

					return ret;
				}
			}

			static property bool HasSkeleton {
				bool get () { return NuiLibSafe::HasSkeleton(); }
			}

			static void SelectSkeleton(float x, float y, float radius) {
				return NuiLibSafe::SelectSkeleton(x, y, radius);
			}

			static bool Init() {
				return NuiLibSafe::Init();
			}
			static void Pause() {
				return NuiLibSafe::Pause();
			}
			static void SetAutoPoll(bool value) {
				NuiLibSafe::SetAutoPoll(value);
			}
			static void Poll() {
				NuiLibSafe::Poll();
			}
			static void Close() {
				NuiLibSafe::Close();
			}

			!Nui() {
				Console::WriteLine("Nui being destroyed");
				NuiLibSafe::Close();
			}


			//----------------------------Scalars------------------------------


			/// <summary>
			/// The x value for the given Vector.
			/// </summary>
			/// <param name="vector"> The vector to take the x value of.</param></param>
			static Scalar ^x(Vector ^vector);
			/// <summary>
			/// The y value for the given Vector.
			/// </summary>
			/// <param name="vector"> The vector to take the y value of.</param>
			static Scalar ^y(Vector ^vector);
			/// <summary>
			/// The z value for the given Vector.
			/// </summary>
			/// <param name="vector"> The vector to take the z value of.</param>
			static Scalar ^z(Vector ^vector);
			/// <summary>
			/// The magnitude value of the given Vector.
			/// </summary>
			static Scalar ^magnitude(Vector ^vector);
			/// <summary>
			/// The dot product of two vectors
			/// </summary>
			/// <param name="a"> The first vector.</param>
			/// <param name="b"> The second vector.</param>
			static Scalar ^dot(Vector ^a, Vector ^b);
			/// <summary>
			/// Normalize the value of a scalar against the maximum value it has ever been.
			/// </summary>
			/// <param name="scalar"> The scalar to normalize.</param>
			static Scalar ^normalize(Scalar ^scalar);
			/// <summary>
			/// Normalize the value of a scalar against a specified maximum value.
			/// </summary>
			/// <param name="scalar"> The scalar to normalize.</param>
			/// <param name="max"> The maximum value to normalize against.</param>
			static Scalar ^normalize(Scalar ^scalar, float max);
			/// <summary>
			/// Normalize the value of a scalar against a specified maximum value.
			/// </summary>
			/// <param name="scalar"> The scalar to normalize.</param>
			/// <param name="max"> The maximum value to normalize against.</param>
			static Scalar ^normalize(Scalar ^scalar, Scalar ^max);
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
			static Scalar ^constrain(Scalar ^input, float deadzone, float range, float grace, bool mirror);
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
			static Scalar ^constrain(Scalar ^input, float deadzone, float range, Scalar ^grace, bool mirror);
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
			static Scalar ^constrain(Scalar ^input, float deadzone, Scalar ^range, float grace, bool mirror);
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
			static Scalar ^constrain(Scalar ^input, float deadzone, Scalar ^range, Scalar^ grace, bool mirror);
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
			static Scalar ^constrain(Scalar ^input, Scalar^ deadzone, float range, float grace, bool mirror);
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
			static Scalar ^constrain(Scalar ^input, Scalar^ deadzone, float range, Scalar^ grace, bool mirror);
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
			static Scalar ^constrain(Scalar ^input, Scalar^ deadzone, Scalar ^range, float grace, bool mirror);
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
			static Scalar ^constrain(Scalar ^input, Scalar^ deadzone, Scalar ^range, Scalar^ grace, bool mirror);
			/// <summary>
			/// Take the absolute value of a scalar.
			/// </summary>
			/// <param name="input"> The scalar to take the absolute value of.</param>
			static Scalar ^abs(Scalar ^input);
			/// <summary>
			/// Take the arc cosine of a scalar.
			/// </summary>
			/// <param name="input"> The scalar to take the arc cosine of.</param>
			static Scalar ^acos(Scalar ^input);
			/// <summary>
			/// -1 or 1 depending on a condition.
			/// Value == *input ? -1.f : 1.f
			/// Used to invert other values depending on a condition.
			/// </summary>
			/// <param name="input"> The condition which dictates Value.</param>
			static Scalar ^invert(Condition ^input);
			/// <summary>
			/// The change in a scalar since the last update.
			/// Will give the first derivitive of a scalar.
			/// </summary>
			/// <param name="input"> The scalar to track changes in.</param>
			static Scalar ^delta(Scalar ^input);
			/// <summary>
			/// The scalar projection of one vector onto another.
			/// a is projected onto b.
			/// </summary>
			/// <param name="a"> The projecting vector.</param>
			/// <param name="b"> The vector being projected on.</param>
			static Scalar ^project(Vector ^a, Vector ^b);
			/// <summary>
			/// Value is one of two inputs depending on a condition.
			/// Value = *condition ? t : f);
			/// </summary>
			/// <param name="condition"> Condition up which the output is dependant.</param>
			/// <param name="t"> Value == t if condition is true.</param>
			/// <param name="f"> Value == f if condition is false.</param>
			static Scalar ^ifScalar(Condition ^condition, float t, float f);
			/// <summary>
			/// Value is one of two inputs depending on a condition.
			/// Value = *condition ? t : f);
			/// </summary>
			/// <param name="condition"> Condition up which the output is dependant.</param>
			/// <param name="t"> Value == t if condition is true.</param>
			/// <param name="f"> Value == f if condition is false.</param>
			static Scalar ^ifScalar(Condition ^condition, float t, Scalar ^f);
			/// <summary>
			/// Value is one of two inputs depending on a condition.
			/// Value = *condition ? a : b);
			/// </summary>
			/// <param name="condition"> Condition up which the output is dependant.</param>
			/// <param name="a"> Value == a if condition is true.</param>
			/// <param name="b"> Value == b if condition is false.</param>
			static Scalar ^ifScalar(Condition ^condition, Scalar ^t, float f);
			/// <summary>
			/// Value is one of two inputs depending on a condition.
			/// Value = *condition ? a : b);
			/// </summary>
			/// <param name="condition"> Condition up which the output is dependant.</param>
			/// <param name="a"> Value == a if condition is true.</param>
			/// <param name="b"> Value == b if condition is false.</param>
			static Scalar ^ifScalar(Condition ^condition, Scalar ^t, Scalar ^f);
			/// <summary>
			/// Value is set by an opencv track bar.
			/// Takes parameters to define how the integer value, starting at 0, that the tracker supplies is converted to a float.
			/// </summary>
			/// <param name="title"> The name of the tracker bar.</param>
			/// <param name="max"> The maximum value the track bar can have.</param>
			/// <param name="scale"> The minumum value the track bar can have.</param>
			/// <param name="value"> The initial value of the track bar.</param>
			static Scalar ^tracker(String ^title, float max, float min, float value);
			/// <summary>
			/// Take a scalar and smooth it. Smoothing is done by averaging the values of the last x frames.
			/// x is the current value of numFrames.
			/// </summary>
			/// <param name="toSmooth">The scalar to smooth.</param>
			/// <param name="numFrames">How many frames to average together to get the smoothing.</param>
			static Scalar ^smooth(Scalar ^toSmooth, Scalar ^numFrames);
			/// <summary>
			/// Take a scalar and smooth it. Smoothing is done by averaging the values of the last x frames.
			/// x is numFrames.
			/// </summary>
			/// <param name="toSmooth">The scalar to smooth.</param>
			/// <param name="numFrames">How many frames to average together to get the smoothing.</param>
			static Scalar ^smooth(Scalar ^toSmooth, int numFrames);


			//----------------------------Vectors------------------------------


			/// <summary>
			/// Normalize the value of a vector so its magnitude is 1.
			/// </summary>
			/// <param name="vector"> The SafeVector to normalize.</param>
			/// <param name="max"> The maximum value to normalize against.</param>
			static Vector ^normalize(Vector ^vector);
			/// <summary>
			/// Get the cross product of two vectors.
			/// </summary>
			/// <param name="a"> The first vector.</param>
			/// <param name="b"> The second vector.</param>
			static Vector ^cross(Vector ^a, Vector ^b);
			/// <summary>
			/// Take only specified axes from a vector.
			/// </summary>
			/// <param name="input"> The vector to limit.</param>
			/// <param name="x"> Whether to take the x axis.</param>
			/// <param name="y"> Whether to take the y axis.</param>
			/// <param name="z"> Whether to take the z axis.</param>
			static Vector ^limit(Vector ^input, bool x, bool y, bool z);
			/// <summary>
			/// The change in a vector since the last update.
			/// Will give the first derivitive of a vector in all three axes.
			/// </summary>
			/// <param name="input"> The vector to track changes in.</param>
			static Vector ^delta(Vector ^input);
			/// <summary>
			/// Take a snapshot of a vector whenever a condition becomes true.
			/// Every time *condition == true Value is set to vector.
			/// </summary>
			/// <param name="vector"> The vector to take snapshots of.</param>
			/// <param name="condition"> Every time this becomes true a snapshot is taken.</param>
			static Vector ^snapshot(Vector ^input, Condition ^);
			/// <summary>
			/// Take the momenum of an input vector.
			/// As long as condition is true Value == vector.
			/// As soon as condition becomes false Value is taken as the last position before the condition become false + 
			/// the delta between the penultimate true position and the final true position. 
			/// After this the same delta keeps on being applied and reduced until the magnitude of the delta approaches zero.
			/// </summary>
			/// <param name="input"> The vector to take the momentum of.</param>
			/// <param name="condition"> If true Value is input. Otherwise Value is taken from the momentum of the final position before condition became false.</param>
			static Vector ^momentum(Vector ^input, Condition ^condition);
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
			static Vector ^momentum(Vector ^input, Condition ^condition, float decrement, float threshold);
			/// <summary>
			/// Scale a vector so that its magnitude is scale.
			/// </summary>
			/// <param name="input"> the vector to scale.</param>
			/// <param name="scale"> The magnitude Value is to have in the direction of input.</param>
			static Vector ^scale(Vector ^input, Scalar ^scale);
			/// <summary>
			/// Scale a vector so that its magnitude is scale.
			/// </summary>
			/// <param name="input"> the vector to scale.</param>
			/// <param name="scale"> The magnitude Value is to have in the direction of input.</param>
			static Vector ^scale(Vector ^input, float scale);
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
			static Vector ^intersect(Vector ^pPlane, Vector ^normalPlane, Vector ^pLine, Vector ^dirLine);
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
			static Vector ^joint(const int joint);
			/// <summary>
			/// Take a vector and smooth it. Smoothing is done by averaging the values of the last x frames.
			/// x is the current value of numFrames.
			/// </summary>
			/// <param name="toSmooth">The vector to smooth.</param>
			/// <param name="numFrames">How many frames to average together to get the smoothing.</param>
			static Vector ^smooth(Vector ^toSmooth, Scalar ^numFrames);
			/// <summary>
			/// Take a vector and smooth it. Smoothing is done by averaging the values of the last x frames.
			/// x is numFrames.
			/// </summary>
			/// <param name="toSmooth">The vector to smooth.</param>
			/// <param name="numFrames">How many frames to average together to get the smoothing.</param>
			static Vector ^smooth(Vector ^toSmooth, int numFrames);


			static int Hip_Centre=0;
			static int Spine=1;
			static int Shoulder_Centre=2;
			static int Head=3;
			static int Shoulder_Left=4;
			static int Elbow_Left=5;
			static int Wrist_Left=6;
			static int Hand_Left=7;
			static int Shoulder_Right=8;
			static int Elbow_Right=9;
			static int Wrist_Right=10;
			static int Hand_Right=11;
			static int Hip_Left=12;
			static int Knee_Left=13;
			static int Ankle_Left=14;
			static int Foot_Left=15;
			static int Hip_Right=16;
			static int Knee_Right=17;
			static int Ankle_Right=18;
			static int Foot_Right=19;
	};
}
