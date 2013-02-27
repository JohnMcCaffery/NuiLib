#ifndef CLRSAFE_H
#define CLRSAFE_H

#ifdef BUILD
   #define DLL __declspec(dllexport)
#else
   #define DLL __declspec(dllimport)
#endif

namespace NuiLib {
class DLL Student
{
private:
  char *_fullname;
  double _gpa;
  void *_scalar;
public:
  Student(char *name, double gpa);
  
  ~Student()
  {
    delete [] _fullname;
  }
  double getGpa()
  {
    return _gpa;
  }
  char *getName()
  {
    return _fullname;
  }
};


/*
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
	class SafeScalar {
		//private:
			//void *_p;

		public: 
			/// Value = 0
			///
			//SafeScalar();
			///
			/// Value = 0
			///
			//SafeScalar(void *p);

			///
			/// Value = value
			///
			SafeScalar(float value);

			///
			/// Destructor
			///
			~SafeScalar();

			///
			/// Value = value
			///
			//SafeScalar(char* name, float value);

			///
			/// Dereference this Scalar to a float.
			/// @return Value.
			///
			//float operator*();

			///
			/// Get the value of the scalar. 
			/// The * operator calls this method.
			/// This method can be overrided if necessary to provide extra behaviour.
			///
			//virtual float Get();
			/// Set the value of this SafeScalar.
			///
			//void Set(float);

			///
			/// Triggered whenever this's value changes.
			///
			//void Changed();
	};	
*/
}

#endif //CLRFSAFE
