#include <NuiLib-CLRSafe.h>
#include <NuiLib-Extendable.h>
#include <NuiLib-Scalars.h>
#include <NuiLib-Vectors.h>
#include <NuiLib-Conditions.h>
#include <opencv/highgui.h>


using namespace NuiLibSafe;

bool NuiLibSafe::Init() {
	bool ret = NuiLib::NuiFactory()->Init();
	return ret;
}
void NuiLibSafe::SetAutoPoll(bool value) {
	NuiLib::NuiFactory()->SetAutoPoll(value);
}

void NuiLibSafe::Pause() {
	cv::waitKey();
}

///
/// Value = 0
///
SafeScalar::SafeScalar(void *p) : _p(p) { }

///
/// Value = value
///
SafeScalar::SafeScalar(float value)  {
	char name[50];
	SPRINTF(name, 50, "%.3f", value);
	IScalar * scalar = ExtensionFactory()->Make<IScalar>(string(name));
	scalar->Set(value);
	_p = scalar;
}

SafeScalar::~SafeScalar() { }

void SafeScalar::SetCallback(CallbackFunction callback) {
	IScalar *scalar = (IScalar*) _p;
	scalar->AddListener([callback] (IObservable* src) { callback(); });
}

///
/// Get the value of the scalar. 
/// The * operator calls this method.
/// This method can be overrided if necessary to provide extra behaviour.
///
float SafeScalar::Get() { 
	IScalar *p = (IScalar*) _p;
	return p->Get();
}
/// Set the value of this SafeScalar.
///
void SafeScalar::Set(float value) { 
	IScalar *p = (IScalar*) _p;
	p->Set(value);
}


//-------------------------------------------------------------------------

SafeScalar *NuiLibSafe::sum(SafeScalar *addend1, SafeScalar *addend2) {
	return new SafeScalar(NuiLib::sum(((IScalar*)addend1->_p), ((IScalar*)addend2->_p)));
}
SafeScalar *NuiLibSafe::difference(SafeScalar *minuend, SafeScalar *subtrahend) {
	return new SafeScalar(NuiLib::difference(((IScalar*)minuend->_p), ((IScalar*)subtrahend->_p)));
}
SafeScalar *NuiLibSafe::product(SafeScalar* factor1, SafeScalar* factor2) {
	return new SafeScalar(NuiLib::product(((IScalar*)factor1->_p), ((IScalar*)factor2->_p)));
}
SafeScalar *NuiLibSafe::quotient(SafeScalar *dividend, SafeScalar *divisor) {
	return new SafeScalar(NuiLib::quotient(((IScalar*)dividend->_p), ((IScalar*)divisor->_p)));
}

SafeScalar *NuiLibSafe::sum(SafeScalar *addend1, float addend2) {
	return new SafeScalar(NuiLib::sum(((IScalar*)addend1->_p), addend2));
}
SafeScalar *NuiLibSafe::difference(SafeScalar *minuend, float subtrahend) {
	return new SafeScalar(NuiLib::difference(((IScalar*)minuend->_p), subtrahend));
}
SafeScalar *NuiLibSafe::product(SafeScalar *factor1, float factor2) {
	return new SafeScalar(NuiLib::product(((IScalar*)factor1->_p), factor2));
}
SafeScalar *NuiLibSafe::quotient(SafeScalar *dividend, float divisor) {
	return new SafeScalar(NuiLib::quotient(((IScalar*)dividend->_p), divisor));
}

SafeScalar *NuiLibSafe::sum(float addend1, SafeScalar *addend2) {
	return new SafeScalar(NuiLib::sum(addend1, ((IScalar*)addend2->_p)));
}
SafeScalar *NuiLibSafe::difference(float minuend, SafeScalar *subtrahend) {
	return new SafeScalar(NuiLib::difference(minuend, ((IScalar*)subtrahend->_p)));
}
SafeScalar *NuiLibSafe::product(float factor1, SafeScalar *factor2) {
	return new SafeScalar(NuiLib::product(factor1, ((IScalar*)factor2->_p)));
}
SafeScalar *NuiLibSafe::quotient(float dividend, SafeScalar *divisor) {
	return new SafeScalar(NuiLib::quotient(dividend, ((IScalar*)divisor->_p)));
}

///
/// Value = 0
///
SafeCondition::SafeCondition(void *p) : _p(p) { }

///
/// Value = value
///
SafeCondition::SafeCondition(bool value)  {
	char *name = value ? "True": "False";
	ICondition * condition = ExtensionFactory()->Make<ICondition>(string(name));
	condition->Set(value);
	_p = condition;
}

SafeCondition::~SafeCondition() { }

void SafeCondition::SetCallback(CallbackFunction callback) {
	ICondition *condition = (ICondition*) _p;
	condition->AddListener([callback] (IObservable* src) { callback(); });
}

///
/// Get the value of the condition. 
/// The * operator calls this method.
/// This method can be overrided if necessary to provide extra behaviour.
///
bool SafeCondition::Get() { 
	ICondition *p = (ICondition*) _p;
	return p->Get();
}
/// Set the value of this SafeCondition.
///
void SafeCondition::Set(bool value) { 
	ICondition *p = (ICondition*) _p;
	p->Set(value);
}



//-------------------------------------------------------------------------
///
/// Value = 0
///
SafeVector::SafeVector(void *p) : _p(p) { }

SafeVector::SafeVector(float value)  {
	char name[150];
	SPRINTF(name, 150, "%.3f,%.3f,%.3f", value, value, value);
	IVector * vector = ExtensionFactory()->Make<IVector>(string(name));
	vector->Set(value, value, value);
	_p = vector;
}
SafeVector::SafeVector(float x, float y, float z)  {
	char name[150];
	SPRINTF(name, 150, "%.3f,%.3f,%.3f", x, y, z);
	IVector * vector = ExtensionFactory()->Make<IVector>(string(name));
	vector->Set(x, y, z);
	_p = vector;
}

SafeVector::~SafeVector() { }

void SafeVector::SetCallback(CallbackFunction callback) {
	IVector *vector = (IVector*) _p;
	vector->AddListener([callback] (IObservable* src) { callback(); });
}

float SafeVector::X() { 
	IVector *p = (IVector*) _p;
	return p->X();
}
float SafeVector::Y() { 
	IVector *p = (IVector*) _p;
	return p->Y();
}
float SafeVector::Z() { 
	IVector *p = (IVector*) _p;
	return p->Z();
}

void SafeVector::Set(float x, float y, float z) {
	IVector *p = (IVector*) _p;
	p->Set(x, y, z);
}
void SafeVector::SetX(float value) { 
	IVector *p = (IVector*) _p;
	p->Set(value, p->Y(), p->Z());
}
void SafeVector::SetY(float value) { 
	IVector *p = (IVector*) _p;
	p->Set(p->X(), value, p->Z());
}
void SafeVector::SetZ(float value) { 
	IVector *p = (IVector*) _p;
	p->Set(p->X(), p->Y(), value);
}

//-------------------------------------------------------------------------


SafeScalar *NuiLibSafe::x(SafeVector *vector) {
	return new SafeScalar(xP(((IVector *)vector->_p)));
}
SafeScalar *NuiLibSafe::y(SafeVector *vector){
	return new SafeScalar(yP(((IVector *)vector->_p)));
}
SafeScalar *NuiLibSafe::z(SafeVector *vector) {
	return new SafeScalar(zP(((IVector *)vector->_p)));
}
SafeScalar *NuiLibSafe::magnitude(SafeVector *vector) {
	return new SafeScalar(magnitudeP(((IVector *)vector->_p)));
}
SafeScalar *NuiLibSafe::dot(SafeVector *vector1, SafeVector *vector2) {
	return new SafeScalar(dotP(((IVector *)vector1->_p), ((IVector *)vector2->_p)));
}
SafeScalar *NuiLibSafe::normalizeScalar(SafeScalar *wrappedSafeScalar) {
	return new SafeScalar(normalizeP(((IScalar *)wrappedSafeScalar->_p)));
}
SafeScalar *NuiLibSafe::normalize(SafeScalar *wrappedSafeScalar, float max) {
	return new SafeScalar(normalizeP(((IScalar *)wrappedSafeScalar->_p), max));
}
SafeScalar *NuiLibSafe::normalize(SafeScalar *wrappedSafeScalar, SafeScalar *max) {
	return new SafeScalar(normalizeP(((IScalar *)wrappedSafeScalar->_p, (IScalar *)max->_p)));
}
SafeScalar *NuiLibSafe::constrain(SafeScalar *wrappedSafeScalar, float deadzone, float range, float grace, bool mirror) {
	return new SafeScalar(constrainP(((IScalar *)wrappedSafeScalar->_p), deadzone, range, grace, mirror));
}
SafeScalar *NuiLibSafe::constrain(SafeScalar *wrappedSafeScalar, float deadzone, float range, SafeScalar *grace, bool mirror) {
	return new SafeScalar(constrainP(((IScalar *)wrappedSafeScalar->_p), deadzone, range, ((IScalar *)grace->_p), mirror));
}
SafeScalar *NuiLibSafe::constrain(SafeScalar *wrappedSafeScalar, float deadzone, SafeScalar *range, float grace, bool mirror) {
	return new SafeScalar(constrainP(((IScalar *)wrappedSafeScalar->_p), deadzone, ((IScalar *)range->_p), grace, mirror));
}
SafeScalar *NuiLibSafe::constrain(SafeScalar *wrappedSafeScalar, float deadzone, SafeScalar *range, SafeScalar *grace, bool mirror) {
	return new SafeScalar(constrainP(((IScalar *)wrappedSafeScalar->_p), deadzone, ((IScalar *)range->_p), ((IScalar *)grace->_p), mirror));
}
SafeScalar *NuiLibSafe::constrain(SafeScalar *wrappedSafeScalar, SafeScalar *deadzone, float range, float grace, bool mirror) {
	return new SafeScalar(constrainP(((IScalar *)wrappedSafeScalar->_p), ((IScalar *)deadzone->_p), range, grace, mirror));
}
SafeScalar *NuiLibSafe::constrain(SafeScalar *wrappedSafeScalar, SafeScalar *deadzone, float range, SafeScalar *grace, bool mirror) {
	return new SafeScalar(constrainP(((IScalar *)wrappedSafeScalar->_p), ((IScalar *)deadzone->_p), range, ((IScalar *)grace->_p), mirror));
}
SafeScalar *NuiLibSafe::constrain(SafeScalar *wrappedSafeScalar, SafeScalar *deadzone, SafeScalar *range, float grace, bool mirror) {
	return new SafeScalar(constrainP(((IScalar *)wrappedSafeScalar->_p), ((IScalar *)deadzone->_p), ((IScalar *)range->_p), grace, mirror));
}
SafeScalar *NuiLibSafe::constrain(SafeScalar *wrappedSafeScalar, SafeScalar *deadzone, SafeScalar *range, SafeScalar *grace, bool mirror) {
	return new SafeScalar(constrainP(((IScalar *)wrappedSafeScalar->_p), ((IScalar *)deadzone->_p), ((IScalar *)range->_p), ((IScalar *)grace->_p), mirror));
}
SafeScalar *NuiLibSafe::acos(SafeScalar *wrappedSafeScalar) {
	return new SafeScalar(acosP(((IScalar *)wrappedSafeScalar->_p)));
}
SafeScalar *NuiLibSafe::invert(SafeCondition *condition) {
	return new SafeScalar(invertP(((ICondition *)condition->_p)));
}
SafeScalar *NuiLibSafe::deltaScalar(SafeScalar *wrappedSafeScalar) {
	return new SafeScalar(deltaP(((IScalar *)wrappedSafeScalar->_p)));
}
SafeScalar *NuiLibSafe::project(SafeVector *a, SafeVector *b) {
	return new SafeScalar(projectP(((IVector *)a->_p), ((IVector *)b->_p)));
}
SafeScalar *NuiLibSafe::abs(SafeScalar *wrappedSafeScalar) {
	return new SafeScalar(absP(((IScalar *)wrappedSafeScalar->_p)));
}
/*
SafeScalar *NuiLibSafe::ifSafeScalar(SafeCondition *conditional, float ifTrue, float ifFalse) {
	return new SafeScalar(ifSafeScalarP(((ICondition *)conditional->_p), ifTrue, ifFalse));
}
SafeScalar *NuiLibSafe::ifSafeScalar(SafeCondition *conditional, float ifTrue, SafeScalar *ifFalse) {
	return new SafeScalar(ifSafeScalarP(((ICondition *)conditional->_p, ifTrue, (ICondition *)ifFalse->_p)));
}
SafeScalar *NuiLibSafe::ifSafeScalar(SafeCondition *conditional, SafeScalar *ifTrue, float ifFalse) {
	return new SafeScalar(ifSafeScalarP(((ICondition *)conditional->_p, (IScalar *)ifTrue->_p), ifFalse));
}
SafeScalar *NuiLibSafe::ifSafeScalar(SafeCondition *conditional, SafeScalar *ifTrue, SafeScalar *ifFalse) {
	return new SafeScalar(ifSafeScalarP(((ICondition *)conditional->_p, (IScalar *)ifTrue->_p, (IScalar *)ifFalse->_p)));
}
*/

//-------------------------------------------------------------------------

SafeVector *NuiLibSafe::normalize(SafeVector *wrappedVector) {
	return new SafeVector(normalizeP((((IVector *)wrappedVector->_p))));
}
//SafeVector *NuiLibSafe::planeDots(SafeVector *vector1, SafeVector *vector2) {
	//return new SafeVector(planeDotsP(((IVector *)vector1->_p, (IVector *)vector2->_p)));
//}
SafeVector *NuiLibSafe::cross(SafeVector *vector1, SafeVector *vector2) {
	return new SafeVector(crossP(((IVector *)vector1->_p), ((IVector *)vector2->_p)));
}
SafeVector *NuiLibSafe::limit(SafeVector *wrappedVector, bool x, bool y, bool z) {
	return new SafeVector(limitP(((IVector *)wrappedVector->_p), x, y, z));
}
SafeVector *NuiLibSafe::delta(SafeVector *wrappedVector) {
	return new SafeVector(deltaP(((IVector *)wrappedVector->_p)));
}
SafeVector *NuiLibSafe::snapshot(SafeVector *wrappedVector, SafeCondition *condition) {
	return new SafeVector(snapshotP(((IVector *)wrappedVector->_p), ((ICondition *)condition->_p)));
}
SafeVector *NuiLibSafe::momentum(SafeVector *wrappedVector, SafeCondition *condition) {
	return new SafeVector(momentumP(((IVector *)wrappedVector->_p), ((ICondition *)condition->_p)));
}
SafeVector *NuiLibSafe::momentum(SafeVector *wrappedVector, SafeCondition *condition, float decrement, float threshold) {
	return new SafeVector(momentumP(((IVector *)wrappedVector->_p), ((ICondition *)condition->_p), decrement, threshold));
}
SafeVector *NuiLibSafe::scale(SafeVector *wrappedVector, SafeScalar *scale) {
	return new SafeVector(scaleP(((IVector *)wrappedVector->_p), ((IScalar *)scale->_p)));
}
SafeVector *NuiLibSafe::scale(SafeVector *wrappedVector, float scale) {
	return new SafeVector(scaleP(((IVector *)wrappedVector->_p), scale));
}
SafeVector *NuiLibSafe::intersect(SafeVector *pPlane, SafeVector *planeNormal, SafeVector *pLine, SafeVector *lineDir) {
	return new SafeVector(intersectP(((IVector *)pPlane->_p), ((IVector *)planeNormal->_p), ((IVector *)pLine->_p), ((IVector *)lineDir->_p)));
}
SafeVector *NuiLibSafe::joint(int joint) {
	return new SafeVector(NuiLib::joint(joint)._p);
}


//-------------------------------------------------------------------------


/*
SafeCondition *NuiLibSafe::fist(bool rightHand) {
	return new SafeCondition(fistP(rightHand));
}
SafeCondition *NuiLibSafe::click(SafeCondition *condition) {
	return new SafeCondition(clickP(((ICondition *)condition->_p)));
}
SafeCondition *NuiLibSafe::hold(SafeCondition *condition) {
	return new SafeCondition(holdP(((ICondition *)condition->_p)));
}
SafeCondition *NuiLibSafe::closeGuard() {
	return new SafeCondition(closeGuardP());
}
SafeCondition *NuiLibSafe::closeGuard(float scale) {
	return new SafeCondition(closeGuardP(scale));
}
SafeCondition *NuiLibSafe::closeGuard(SafeScalar *scale) {
	return new SafeCondition(closeGuardP(((IScalar *)scale->_p)));
}*/