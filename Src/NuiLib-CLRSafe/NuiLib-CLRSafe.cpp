#include <NuiLib-CLRSafe.h>
#include <NuiLib-Extendable.h>
#include <NuiLib-Scalars.h>
#include <NuiLib-Vectors.h>
#include <NuiLib-Conditions.h>
#include <opencv/highgui.h>


using namespace NuiLibSafe;

class Listener : public IObserver {
private:
	CallbackFunction _callback;

public:
	Listener(CallbackFunction callback) {
		_callback = callback;
	}

	void Changed(IObservable *source) {
		_callback();
	}
};

class NuiListener : public INuiListener {
private:
	CallbackFunction _tickCallback;
	SkeletonCallbackFunction _foundCallback;
	SkeletonCallbackFunction _lostCallback;
	SkeletonCallbackFunction _switchedCallback;

public:
	NuiListener(
		CallbackFunction tickCallback,
		SkeletonCallbackFunction foundCallback,
		SkeletonCallbackFunction lostCallback,
		SkeletonCallbackFunction switchedCallback) :
			_tickCallback(tickCallback), 
			_foundCallback(foundCallback), 
			_lostCallback(lostCallback), 
			_switchedCallback(switchedCallback) {

	}

	void Tick() { _tickCallback(); }
	void SkeletonFound(int index) { _foundCallback(); }
	void SkeletonLost(int index) { _lostCallback(); }
	void SkeletonSwitched(int index) { _switchedCallback(); }

};

NuiListener *listener;

void NuiLibSafe::RegisterCallbacks(
		CallbackFunction tickCallback,
		SkeletonCallbackFunction foundCallback,
		SkeletonCallbackFunction lostCallback,
		SkeletonCallbackFunction switchedCallback) {

	listener = new NuiListener(tickCallback, foundCallback, lostCallback, switchedCallback);
	NuiLib::NuiFactory()->AddNuiListener(listener);
}

cv::Mat depth;
std::vector<cv::Mat> inArray;
const double DEPTH_SCALE_FACTOR = 255./65535.;

bool NuiLibSafe::Init() {
	return NuiLib::NuiFactory()->Init();
}
void NuiLibSafe::SetAutoPoll(bool value) {
	NuiLib::NuiFactory()->SetAutoPoll(value);
}
void NuiLibSafe::Poll() {
	NuiLib::NuiFactory()->Poll();
}
void NuiLibSafe::Pause() {
	cv::waitKey();
}
void NuiLibSafe::Close() {
	NuiLib::NuiFactory()->RemoveNuiListener(listener);
	delete listener;

	NuiLib::NuiFactory()->Dispose();
}
bool NuiLibSafe::HasSkeleton() {
	return NuiLib::NuiFactory()->HasSkeleton();
}

uchar *NuiLibSafe::GetColourBytes() {
	return NuiLib::NuiFactory()->GetColour().data;
}
uchar *NuiLibSafe::GetDepthBytes() {
	cv::Mat in = NuiLib::NuiFactory()->GetDepth();

	inArray.clear();
	inArray.push_back(in);
	inArray.push_back(in);
	inArray.push_back(in);

	cv::merge(inArray, depth);

	depth.convertTo(depth, CV_8UC3, DEPTH_SCALE_FACTOR);

	return (depth).data;
}
Point NuiLibSafe::SkeletonToColour(SafeVector *v) {
	cv::Point p = NuiLib::NuiFactory()->SkeletonToColour(NuiLib::Vector((IVector *)v->_p));
	return Point(p.x, p.y);
}
Point NuiLibSafe::SkeletonToDepth(SafeVector *v) {
	cv::Point p = NuiLib::NuiFactory()->SkeletonToDepth(NuiLib::Vector((IVector *)v->_p));
	return Point(p.x, p.y);
}

int NuiLibSafe::GetColourWidth() {
	return NuiLib::NuiFactory()->GetColour().cols;
}
int NuiLibSafe::GetColourHeight() {
	return NuiLib::NuiFactory()->GetColour().rows;
}
int NuiLibSafe::GetColourStride() {
	return 4;
}

int NuiLibSafe::GetDepthWidth() {
	return NuiLib::NuiFactory()->GetDepth().cols;
}
int NuiLibSafe::GetDepthHeight() {
	return NuiLib::NuiFactory()->GetDepth().rows;
}
int NuiLibSafe::GetDepthStride() {
	return 3;
}

//uchar *NuiLibSafe::GetDebugBytes() {
	//return NuiLib::NuiFactory()->GetDebi().data;
//}


int SafeScalar::_count = 1;

///
/// Value = 0
///
SafeScalar::SafeScalar(void *p) : _listener(NULL), _p(p) { }

///
/// Value = value
///
SafeScalar::SafeScalar(float value) : _listener(NULL) {
	char name[50];
	SPRINTF(name, 50, "Scalar%i", _count++);
	IScalar * scalar = ExtensionFactory()->Make<IScalar>(string(name));
	scalar->Set(value);
	_p = scalar;
}

SafeScalar::SafeScalar(const char *name, float value) : _listener(NULL) {
	IScalar * scalar = ExtensionFactory()->Make<IScalar>(string(name));
	scalar->Set(value);
	_p = scalar;
}

SafeScalar::~SafeScalar() {
	if (_listener) {
		Listener *listener = (Listener*) _listener;
		IScalar *scalar = (IScalar*) _p;
		listener->RemoveAsListener(scalar);
	}
}

void SafeScalar::SetCallback(CallbackFunction callback) {
	IScalar *scalar = (IScalar*) _p;
	Listener *listener = new Listener(callback);
	listener->AddAsListener(NULL, scalar);
	_listener = listener;
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
	NuiFactory()->Poll();
}

const char *SafeScalar::GetName() {
	IScalar *p = (IScalar*) _p;
	return p->GetCName();
}

void SafeScalar::SetName(const char *name) {
	IScalar *p = (IScalar*) _p;
	return p->SetName(name);
}


int SafeCondition::_count = 1;

///
/// Value = 0
///
SafeCondition::SafeCondition(void *p) : _listener(NULL), _p(p) { }

///
/// Value = value
///
SafeCondition::SafeCondition(bool value)  : _listener(NULL) {
	char *name = new char[50];
	SPRINTF(name, 50, "Condition%i", _count++);
	ICondition * condition = ExtensionFactory()->Make<ICondition>(string(name));
	condition->Set(value);
	_p = condition;
}
SafeCondition::SafeCondition(const char *name, bool value) : _listener(NULL)  {
	ICondition * condition = ExtensionFactory()->Make<ICondition>(string(name));
	condition->Set(value);
	_p = condition;
}

SafeCondition::~SafeCondition() {
	if (_listener) {
		Listener *listener = (Listener*) _listener;
		IScalar *scalar = (IScalar*) _p;
		listener->RemoveAsListener(scalar);
	}
}

void SafeCondition::SetCallback(CallbackFunction callback) {
	ICondition *condition = (ICondition*) _p;
	Listener *listener = new Listener(callback);
	listener->AddAsListener(NULL, condition);
	_listener = listener;
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
	NuiFactory()->Poll();
}

const char *SafeCondition::GetName() {
	ICondition *p = (ICondition*) _p;
	return p->GetCName();
}
void SafeCondition::SetName(const char *name) {
	ICondition *p = (ICondition*) _p;
	return p->SetName(name);
}




//-------------------------------------------------------------------------
int SafeVector::_count = 1;
///
/// Value = 0
///
SafeVector::SafeVector(void *p) : _listener(NULL), _p(p)  { }

SafeVector::SafeVector(float value)  : _listener(NULL) {
	char name[50];
	SPRINTF(name, 50, "Vector%i", _count++);
	IVector * vector = ExtensionFactory()->Make<IVector>(string(name));
	vector->Set(value, value, value);
	_p = vector;
}
SafeVector::SafeVector(const char *name, float value)  : _listener(NULL) {
	IVector * vector = ExtensionFactory()->Make<IVector>(string(name));
	vector->Set(value, value, value);
	_p = vector;
}
SafeVector::SafeVector(float x, float y, float z)  : _listener(NULL) {
	char name[50];
	SPRINTF(name, 50, "Vector%i", _count++);
	IVector * vector = ExtensionFactory()->Make<IVector>(string(name));
	vector->Set(x, y, z);
	_p = vector;
}
SafeVector::SafeVector(const char *name, float x, float y, float z)  : _listener(NULL) {
	IVector * vector = ExtensionFactory()->Make<IVector>(string(name));
	vector->Set(x, y, z);
	_p = vector;
}

SafeVector::~SafeVector() {
	if (_listener) {
		Listener *listener = (Listener*) _listener;
		IScalar *scalar = (IScalar*) _p;
		listener->RemoveAsListener(scalar);
	}
}

void SafeVector::SetCallback(CallbackFunction callback) {
	IVector *vector = (IVector*) _p;
	Listener *listener = new Listener(callback);
	listener->AddAsListener(NULL, vector);
	_listener = listener;
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
	NuiFactory()->Poll();
}
void SafeVector::SetX(float value) { 
	IVector *p = (IVector*) _p;
	p->Set(value, p->Y(), p->Z());
	NuiFactory()->Poll();
}
void SafeVector::SetY(float value) { 
	IVector *p = (IVector*) _p;
	p->Set(p->X(), value, p->Z());
	NuiFactory()->Poll();
}
void SafeVector::SetZ(float value) { 
	IVector *p = (IVector*) _p;
	p->Set(p->X(), p->Y(), value);
	NuiFactory()->Poll();
}

const const char *SafeVector::GetName() {
	IVector *p = (IVector*) _p;
	return p->GetCName();
}
void SafeVector::SetName(const const char *name) {
	IVector *p = (IVector*) _p;
	return p->SetName(name);
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
SafeScalar *NuiLibSafe::ifScalar(SafeCondition *conditional, float ifTrue, float ifFalse) {
	return new SafeScalar(ifScalarP(((ICondition *)conditional->_p), ifTrue, ifFalse));
}
SafeScalar *NuiLibSafe::ifScalar(SafeCondition *conditional, float ifTrue, SafeScalar *ifFalse) {
	return new SafeScalar(ifScalarP(((ICondition *)conditional->_p), ifTrue, ((IScalar *)ifFalse->_p)));
}
SafeScalar *NuiLibSafe::ifScalar(SafeCondition *conditional, SafeScalar *ifTrue, float ifFalse) {
	return new SafeScalar(ifScalarP(((ICondition *)conditional->_p), ((IScalar *)ifTrue->_p), ifFalse));
}
SafeScalar *NuiLibSafe::ifScalar(SafeCondition *conditional, SafeScalar *ifTrue, SafeScalar *ifFalse) {
	return new SafeScalar(ifScalarP(((ICondition *)conditional->_p), ((IScalar *)ifTrue->_p), ((IScalar *)ifFalse->_p)));
}

//-------------------------------------------------------------------------

SafeVector *NuiLibSafe::sum(SafeVector *addend1, SafeVector *addend2) {
	return new SafeVector(NuiLib::sum(((IVector*)addend1->_p), ((IVector*)addend2->_p)));
}
SafeVector *NuiLibSafe::difference(SafeVector *minuend, SafeVector *subtrahend) {
	return new SafeVector(NuiLib::difference(((IVector*)minuend->_p), ((IVector*)subtrahend->_p)));
}
SafeVector *NuiLibSafe::product(SafeVector* factor1, SafeVector* factor2) {
	return new SafeVector(NuiLib::product(((IVector*)factor1->_p), ((IVector*)factor2->_p)));
}
SafeVector *NuiLibSafe::quotient(SafeVector *dividend, SafeVector *divisor) {
	return new SafeVector(NuiLib::quotient(((IVector*)dividend->_p), ((IVector*)divisor->_p)));
}

SafeVector *NuiLibSafe::sum(SafeVector *addend1, float addend2) {
	return new SafeVector(NuiLib::sum(((IVector*)addend1->_p), addend2));
}
SafeVector *NuiLibSafe::difference(SafeVector *minuend, float subtrahend) {
	return new SafeVector(NuiLib::difference(((IVector*)minuend->_p), subtrahend));
}
SafeVector *NuiLibSafe::product(SafeVector *factor1, float factor2) {
	return new SafeVector(NuiLib::product(((IVector*)factor1->_p), factor2));
}
SafeVector *NuiLibSafe::quotient(SafeVector *dividend, float divisor) {
	return new SafeVector(NuiLib::quotient(((IVector*)dividend->_p), divisor));
}

SafeVector *NuiLibSafe::sum(float addend1, SafeVector *addend2) {
	return new SafeVector(NuiLib::sum(addend1, ((IVector*)addend2->_p)));
}
SafeVector *NuiLibSafe::difference(float minuend, SafeVector *subtrahend) {
	return new SafeVector(NuiLib::difference(minuend, ((IVector*)subtrahend->_p)));
}
SafeVector *NuiLibSafe::product(float factor1, SafeVector *factor2) {
	return new SafeVector(NuiLib::product(factor1, ((IVector*)factor2->_p)));
}
SafeVector *NuiLibSafe::quotient(float dividend, SafeVector *divisor) {
	return new SafeVector(NuiLib::quotient(dividend, ((IVector*)divisor->_p)));
}

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

///
/// True if operand1 is greater than operand2.
///
SafeCondition *NuiLibSafe::Greater(SafeScalar *operand1, SafeScalar *operand2) {
	return new SafeCondition(NuiLib::pGreater(((IScalar *)operand1->_p), ((IScalar *)operand2->_p)));
}
///
/// True if operand1 is less than operand2.
///
SafeCondition *NuiLibSafe::Less(SafeScalar *operand1, SafeScalar *operand2) {
	return new SafeCondition(NuiLib::pLess(((IScalar *)operand1->_p), ((IScalar *)operand2->_p)));
}
///
/// True if operand1 is greater than or equ.al to operand2
///
SafeCondition *NuiLibSafe::GreaterEqual(SafeScalar *operand1, SafeScalar *operand2) {
	return new SafeCondition(NuiLib::pGreaterEqual(((IScalar *)operand1->_p), ((IScalar *)operand2->_p)));
}
///
/// True if operand1 is less than or equal. to operand2
///
SafeCondition *NuiLibSafe::LessEqual(SafeScalar *operand1, SafeScalar *operand2) {
	return new SafeCondition(NuiLib::pLessEqual(((IScalar *)operand1->_p), ((IScalar *)operand2->_p)));
}
///
/// True if operand1 is equal to operand2.
///
SafeCondition *NuiLibSafe::Equal(SafeScalar *operand1, SafeScalar *operand2) {
	return new SafeCondition(NuiLib::pEqualTo(((IScalar*) operand1->_p), ((IScalar*) operand2->_p)));
}
///
/// True if operand1 is equal to operand2.
///
//SafeCondition *NuiLibSafe::Equal(SafeVector *operand1, SafeVector *operand2) {
	//return new SafeCondition(NuiLib::pEqual(((IVector*) operand1->_p), ((IVector*) operand2->_p)));
//}
///
/// True if operand1 is not equal to operand2.
///
SafeCondition *NuiLibSafe::NotEqual(SafeScalar *operand1, SafeScalar *operand2) {
	return new SafeCondition(NuiLib::pNotEqualTo(((IScalar*) operand1->_p), ((IScalar*) operand2->_p)));
}
///
/// True if operand1 is not equal to operand2.
///
SafeCondition *NuiLibSafe::NotEqual(SafeVector *operand1, SafeVector *operand2) {
	return new SafeCondition(NuiLib::pNotEqualTo(((IVector*) operand1->_p), ((IVector*) operand2->_p)));
}
///
/// True if both operand1 and operand2 are true.
///
SafeCondition *NuiLibSafe::And(SafeCondition *operand1, SafeCondition *operand2) {
	return new SafeCondition(NuiLib::pAnd(((ICondition*) operand1->_p), ((ICondition*) operand2->_p)));
}
///
/// True if either operand1 or operand2 or both are true.
///
SafeCondition *NuiLibSafe::Or(SafeCondition *operand1, SafeCondition *operand2) {
	return new SafeCondition(NuiLib::pOr(((ICondition*) operand1->_p), ((ICondition*) operand2->_p)));
}
///
/// True if operand is false.
///
SafeCondition *NuiLibSafe::Not(SafeCondition *operand) {
	return new SafeCondition(NuiLib::pNot(((ICondition*) operand->_p)));
}

///
/// True if operand1 is greater than operand2.
///
SafeCondition *NuiLibSafe::Greater(SafeScalar *operand1, float operand2) {
	return new SafeCondition(NuiLib::pGreater(((IScalar*) operand1->_p), operand2));
}
///
/// True if operand1 is less than operand2.
///
SafeCondition *NuiLibSafe::Less(SafeScalar *operand1, float operand2) {
	return new SafeCondition(NuiLib::pLess(((IScalar*) operand1->_p), operand2));
}
///
/// True if operand1 is greater than or equ.al to operand2
///
SafeCondition *NuiLibSafe::GreaterEqual(SafeScalar *operand1, float operand2) {
	return new SafeCondition(NuiLib::pGreaterEqual(((IScalar*) operand1->_p), operand2));
}
///
/// True if operand1 is less than or equal. to operand2
///
SafeCondition *NuiLibSafe::LessEqual(SafeScalar *operand1, float operand2) {
	return new SafeCondition(NuiLib::pLessEqual(((IScalar*) operand1->_p), operand2));
}
///
/// True if operand1 is equal to operand2.
/// All components of operand1 (x, y and z) must be equal of to operand1.
///
SafeCondition *NuiLibSafe::Equal(SafeScalar *operand1, float operand2) {
	return new SafeCondition(NuiLib::pEqualTo(((IScalar*) operand1->_p), operand2));
}
///
/// True if operand1 is equal to operand2.
///	
//SafeCondition *NuiLibSafe::Equal(SafeVector *operand1, float operand2) {
	//return new SafeCondition(NuiLib::pEqual(((IVector*) operand1->_p), operand2));
//}
///
/// True if operand1 is equal to operand2.
///	
//SafeCondition *NuiLibSafe::Equal(SafeVector *operand1, cv::Point3f) {
	//return new SafeCondition(NuiLib::pEqual(((IVector*) operand1->_p), cv::Point3f));
//}
///
/// True if operand1 is not equal to operand2.
///
SafeCondition *NuiLibSafe::NotEqual(SafeScalar *operand1, float operand2) {
	return new SafeCondition(NuiLib::pNotEqualTo(((IScalar*) operand1->_p), operand2));
}
///
/// False if all components of operand1 (x, y and z) are equal to operand2.
///
SafeCondition *NuiLibSafe::NotEqual(SafeVector *operand1, float operand2) {
	return new SafeCondition(NuiLib::pNotEqualTo(((IVector*) operand1->_p), operand2));
}
///
/// True if both operand1 and operand2 are true.
///
SafeCondition *NuiLibSafe::And(SafeCondition *operand1, bool operand2) {
	return new SafeCondition(NuiLib::pAnd(((ICondition*) operand1->_p), operand2));
}
///
/// True if either operand1 or operand2 or both are true.
///
SafeCondition *NuiLibSafe::Or(SafeCondition *operand1, bool operand2) {
	return new SafeCondition(NuiLib::pOr(((ICondition*) operand1->_p), operand2));
}

///
/// True if operand1 is greater than operand2.
///
SafeCondition *NuiLibSafe::Greater(float operand1, SafeScalar *operand2) {
	return new SafeCondition(NuiLib::pGreater(operand1, ((IScalar*) operand2->_p)));
}
///
/// True if operand1 is less than operand2.
///
SafeCondition *NuiLibSafe::Less(float operand1, SafeScalar *operand2) {
	return new SafeCondition(NuiLib::pLess(operand1, ((IScalar*) operand2->_p)));
}
///
/// True if operand1 is greater than or equ.al to operand2
///
SafeCondition *NuiLibSafe::GreaterEqual(float operand1, SafeScalar *operand2) {
	return new SafeCondition(NuiLib::pGreaterEqual(operand1, ((IScalar*) operand2->_p)));
}
///
/// True if operand1 is less than or equal. to operand2
///
SafeCondition *NuiLibSafe::LessEqual(float operand1, SafeScalar *operand2) {
	return new SafeCondition(NuiLib::pLessEqual(operand1, ((IScalar*) operand2->_p)));	
}
///
/// True if operand1 is equal to operand2.
/// Operand1 must be qual to all components of operand2 (x, y and z).
///
SafeCondition *NuiLibSafe::Equal(float operand1, SafeScalar *operand2) {
	return new SafeCondition(NuiLib::pEqualTo(operand1, ((IScalar*) operand2->_p)));	
}
///
/// True if operand1 is equal to operand2.
///
//SafeCondition *NuiLibSafe::Equal(float operand1, SafeVector *operand2)  {
	//return new SafeCondition(NuiLib::pEqual(operand1, ((IVector*) operand2->_p)) ){
//}
///
/// True if operand1 is equal to operand2.
///
//SafeCondition *NuiLibSafe::Equal(cv::Point3f operand1, SafeVector *operand2) {
	//return new SafeCondition(NuiLib::pEqual(cv::Point3f operand1, SafeVector *operand2));
//}
///
/// True if operand1 is not equal to operand2.
///
SafeCondition *NuiLibSafe::NotEqual(float operand1, SafeScalar *operand2) {
	return new SafeCondition(NuiLib::pNotEqualTo(operand1, ((IScalar*) operand2->_p)));	
}
///
/// False if operand1 is equal to all components of operand2 (x, y and z).
///
SafeCondition *NuiLibSafe::NotEqual(float operand1, SafeVector *operand2) {
	return new SafeCondition(NuiLib::pNotEqualTo(operand1, ((IVector*) operand2->_p)));	
}
///
/// True if both operand1 and operand2 are true.
///
SafeCondition *NuiLibSafe::And(bool operand1, SafeCondition *operand2) {
	return new SafeCondition(NuiLib::pAnd(operand1, ((ICondition*) operand2->_p)));	
}
///
/// True if either operand1 or operand2 or both are true.
///
SafeCondition *NuiLibSafe::Or(bool operand1, SafeCondition *operand2) {
	return new SafeCondition(NuiLib::pOr(operand1, ((ICondition*) operand2->_p)));	
}


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
