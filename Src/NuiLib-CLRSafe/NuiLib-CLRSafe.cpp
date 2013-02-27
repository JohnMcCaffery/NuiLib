#include <NuiLib-CLRSafe.h>
#include <NuiLib-Extendable.h>
#include <vector>

using namespace NuiLib;

///
/// Value = 0
///
/*SafeScalar::SafeScalar() { 
	_p = new IScalar();
}
*/
/// Value = 0
///
/*SafeScalar::SafeScalar(void *p) : _p(p) { 
	IScalar *s = (IScalar*) p;
	//s->AddListener([this] (IObservable* src) { Changed(); });
}
*/

///
/// Value = value
///
SafeScalar::SafeScalar(float value) {
	char name[50];
	SPRINTF(name, 50, "%.3f", value);
	IScalar * scalar = ExtensionFactory()->Make<IScalar>(string(name));
	scalar->Set(value);
	_p = scalar;
	//scalar->AddListener([this] (IObservable* src) { Changed(); });
}


///
/// Value = value
///
/*SafeScalar::SafeScalar(char* name, float value) {
	IScalar * scalar = ExtensionFactory()->Make<IScalar>(string(name));
	scalar->Set(value);
	_p = scalar;
	//scalar->AddListener([this] (IObservable* src) { Changed(); });
}
*/

///
/// Dereference this Scalar to a float.
/// @return Value.
///
//float SafeScalar::operator*() { return Get(); }

///
/// Get the value of the scalar. 
/// The * operator calls this method.
/// This method can be overrided if necessary to provide extra behaviour.
///
/*float SafeScalar::Get() { 
	IScalar *p = (IScalar*) _p;
	return p->Get();
	return 0.0F;
}
/// Set the value of this SafeScalar.
///
void SafeScalar::Set(float value) { 
	IScalar *p = (IScalar*) _p;
	p->Set(value);
}
*/

//void Changed(IObservable *source);
