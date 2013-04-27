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
#include <NuiLib-Scalars.h>
#include <cmath>

float mathacos(float value) {
	return acos(value);
}

using namespace NuiLib;



VectorScalar *NuiLib::xP(IVector *vector) {
	VectorScalar *scalar = ExtensionFactory()->Make<VectorScalar>("(" + vector->GetName() + "->X)");
	scalar->SetVector(vector);
	scalar->SetComponent('x');
	scalar->Changed(vector);
	return scalar;
}
VectorScalar *NuiLib::yP(IVector *vector) {
	VectorScalar *scalar = ExtensionFactory()->Make<VectorScalar>("(" + vector->GetName() + "->Y)");
	scalar->SetVector(vector);
	scalar->SetComponent('y');
	scalar->Changed(vector);
	return scalar;
}
VectorScalar *NuiLib::zP(IVector *vector) {
	VectorScalar *scalar = ExtensionFactory()->Make<VectorScalar>("(" + vector->GetName() + "->Z)");
	scalar->SetVector(vector);
	scalar->SetComponent('z');
	scalar->Changed(vector);
	return scalar;
}
VectorScalar *NuiLib::magnitudeP(IVector *vector) {
	VectorScalar *scalar = ExtensionFactory()->Make<VectorScalar>("|" + vector->GetName() + "|");
	scalar->SetVector(vector);
	scalar->SetComponent('m');
	scalar->Changed(vector);
	return scalar;
}
DotScalar *NuiLib::dotP(IVector *vector1, IVector *vector2) {
	DotScalar *scalar = ExtensionFactory()->Make<DotScalar>(vector1->GetName() + "." + vector2->GetName());
	scalar->SetVector1(vector1);
	scalar->SetVector2(vector2);
	scalar->Changed(vector1);
	return scalar;
}
MaxNormalizedScalar *NuiLib::normalizeP(IScalar *wrappedScalar) {
	MaxNormalizedScalar *scalar = ExtensionFactory()->Make<MaxNormalizedScalar>("normalize(" + wrappedScalar->GetName() + ")");
	scalar->SetScalar(wrappedScalar);
	scalar->Changed(wrappedScalar);
	return scalar;
}
ValueNormalizedScalar *NuiLib::normalizeP(IScalar *wrappedScalar, float max) {
	ValueNormalizedScalar *scalar = ExtensionFactory()->Make<ValueNormalizedScalar>("normalize(" + wrappedScalar->GetName() + ")");
	scalar->SetScalar(wrappedScalar);
	scalar->SetMax(max);
	scalar->Changed(wrappedScalar);
	return scalar;
}
ValueNormalizedScalar *NuiLib::normalizeP(IScalar *wrappedScalar, IScalar *max) {
	ValueNormalizedScalar *scalar = ExtensionFactory()->Make<ValueNormalizedScalar>("normalize(" + wrappedScalar->GetName() + ")");
	scalar->SetScalar(wrappedScalar);
	scalar->SetMax(max);
	scalar->Changed(wrappedScalar);
	return scalar;
}
template<typename TypeDeadzone, typename TypeRange, typename TypeGrace>
ConstrainedScalar *constrainT(IScalar *wrappedScalar, TypeDeadzone deadzone, TypeRange range, TypeGrace grace, bool mirror) {
	ConstrainedScalar *scalar = ExtensionFactory()->Make<ConstrainedScalar>("constrain(" + wrappedScalar->GetName() + ")");
	scalar->SetScalar(wrappedScalar);
	scalar->SetDeadzone(deadzone);
	scalar->SetRange(range);
	scalar->SetGrace(grace);
	scalar->SetMirror(mirror);
	scalar->Changed(wrappedScalar);
	return scalar;
}
ConstrainedScalar *NuiLib::constrainP(IScalar *wrappedScalar, float deadzone, float range, float grace, bool mirror) {
	return constrainT(wrappedScalar, deadzone, range, grace, mirror);
}
ConstrainedScalar *NuiLib::constrainP(IScalar *wrappedScalar, float deadzone, float range, IScalar *grace, bool mirror) {
	return constrainT(wrappedScalar, deadzone, range, grace, mirror);
}
ConstrainedScalar *NuiLib::constrainP(IScalar *wrappedScalar, float deadzone, IScalar *range, float grace, bool mirror) {
	return constrainT(wrappedScalar, deadzone, range, grace, mirror);
}
ConstrainedScalar *NuiLib::constrainP(IScalar *wrappedScalar, float deadzone, IScalar *range, IScalar *grace, bool mirror) {
	return constrainT(wrappedScalar, deadzone, range, grace, mirror);
}
ConstrainedScalar *NuiLib::constrainP(IScalar *wrappedScalar, IScalar *deadzone, float range, float grace, bool mirror) {
	return constrainT(wrappedScalar, deadzone, range, grace, mirror);
}
ConstrainedScalar *NuiLib::constrainP(IScalar *wrappedScalar, IScalar *deadzone, float range, IScalar *grace, bool mirror) {
	return constrainT(wrappedScalar, deadzone, range, grace, mirror);
}
ConstrainedScalar *NuiLib::constrainP(IScalar *wrappedScalar, IScalar *deadzone, IScalar *range, float grace, bool mirror) {
	return constrainT(wrappedScalar, deadzone, range, grace, mirror);
}
ConstrainedScalar *NuiLib::constrainP(IScalar *wrappedScalar, IScalar *deadzone, IScalar *range, IScalar *grace, bool mirror) {
	return constrainT(wrappedScalar, deadzone, range, grace, mirror);
}
ACosScalar *NuiLib::acosP(IScalar *wrappedScalar) {
	ACosScalar *scalar = ExtensionFactory()->Make<ACosScalar>("acos(" + wrappedScalar->GetName() + ")");
	scalar->SetScalar(wrappedScalar);
	scalar->Changed(wrappedScalar);
	return scalar;
}
InvertScalar *NuiLib::invertP(ICondition *condition) {
	InvertScalar *scalar = ExtensionFactory()->Make<InvertScalar>("invert(" + condition->GetName() + ")");
	scalar->SetCondition(condition);
	scalar->Changed(condition);
	return scalar;
}
DeltaScalar *NuiLib::deltaP(IScalar *wrappedScalar) {
	DeltaScalar *scalar = ExtensionFactory()->Make<DeltaScalar>("delta(" + wrappedScalar->GetName() + ")");
	scalar->SetScalar(wrappedScalar);
	scalar->Changed(wrappedScalar);
	return scalar;
}
ProjectionScalar *NuiLib::projectP(IVector *a, IVector *b) {
	ProjectionScalar *scalar = ExtensionFactory()->Make<ProjectionScalar>("project(" + a->GetName() + " onto " + b->GetName() + ")");
	scalar->SetVector1(a);
	scalar->SetVector2(b);
	scalar->Changed(a);
	return scalar;
}
AbsScalar *NuiLib::absP(IScalar *wrappedScalar) {
	AbsScalar *scalar = ExtensionFactory()->Make<AbsScalar>("abs(" + wrappedScalar->GetName() + ")");
	scalar->SetScalar(wrappedScalar);
	scalar->Changed(wrappedScalar);
	return scalar;
}
template<typename TrueT, typename FalseT>
ConditionalScalar *ifT(ICondition* condition, TrueT ifTrue, FalseT ifFalse, string name) {
	ConditionalScalar *scalar = ExtensionFactory()->Make<ConditionalScalar>(name);
	scalar->SetCondition(condition);
	scalar->SetScalar1(ifTrue);
	scalar->SetScalar2(ifFalse);
	scalar->Changed(condition);
	return scalar;
}
ConditionalScalar *NuiLib::ifScalarP(ICondition* conditional, float ifTrue, float ifFalse) {
	char name[500];
	SPRINTF(name, 500, "(%s ? %.3f : %.3f)", conditional, ifTrue, ifFalse);
	return ifT(conditional, ifTrue, ifFalse, string(name));
}
ConditionalScalar *NuiLib::ifScalarP(ICondition* conditional, float ifTrue, IScalar *ifFalse) {
	char name[500];
	SPRINTF(name, 500, "(%s ? %.3f : %s)", conditional, ifTrue, ifFalse);
	return ifT(conditional, ifTrue, ifFalse, string(name));
}
ConditionalScalar *NuiLib::ifScalarP(ICondition* conditional, IScalar *ifTrue, float ifFalse) {
	char name[500];
	SPRINTF(name, 500, "(%s ? %s : %.3f)", conditional, ifTrue, ifFalse);
	return ifT(conditional, ifTrue, ifFalse, string(name));
}
ConditionalScalar *NuiLib::ifScalarP(ICondition* conditional, IScalar *ifTrue, IScalar *ifFalse) {
	char name[500];
	SPRINTF(name, 500, "(%s ? %s : %s)", conditional, ifTrue, ifFalse);
	return ifT(conditional, ifTrue, ifFalse, string(name));
}
SmoothedScalar *NuiLib::smoothP(IScalar *toSmooth, IScalar *numFrames) {
	SmoothedScalar *vector = ExtensionFactory()->Make<SmoothedScalar>("smooth(" + toSmooth->GetName() + " by " + numFrames->GetName() + ")");
	vector->SetScalar(toSmooth);
	vector->SetNumFrames(numFrames);
	return vector;
}
SmoothedScalar *NuiLib::smoothP(IScalar *toSmooth, int numFrames) {
	SmoothedScalar *vector = ExtensionFactory()->Make<SmoothedScalar>("smooth(" + toSmooth->GetName() + ")");
	vector->SetScalar(toSmooth);
	vector->SetNumFrames((float) numFrames);
	return vector;
}


















Scalar NuiLib::x(const Vector &vector) {
	return xP(vector._p);
}
Scalar NuiLib::y(const Vector &vector){
	return yP(vector._p);
}
Scalar NuiLib::z(const Vector &vector) {
	return zP(vector._p);
}
Scalar NuiLib::magnitude(const Vector &vector) {
	return magnitudeP(vector._p);
}
Scalar NuiLib::dot(const Vector &vector1, const Vector &vector2) {
	return dotP(vector1._p, vector2._p);
}
Scalar NuiLib::normalize(const Scalar &wrappedScalar) {
	return normalizeP(wrappedScalar._p);
}
Scalar NuiLib::normalize(const Scalar &wrappedScalar, float max) {
	return normalizeP(wrappedScalar._p, max);
}
Scalar NuiLib::normalize(const Scalar &wrappedScalar, const Scalar &max) {
	return normalizeP(wrappedScalar._p, max._p);
}
Scalar NuiLib::constrain(const Scalar &wrappedScalar, float deadzone, float range, float grace, bool mirror) {
	return constrainP(wrappedScalar._p, deadzone, range, grace, mirror);
}
Scalar NuiLib::constrain(const Scalar &wrappedScalar, float deadzone, float range, const Scalar &grace, bool mirror) {
	return constrainP(wrappedScalar._p, deadzone, range, grace._p, mirror);
}
Scalar NuiLib::constrain(const Scalar &wrappedScalar, float deadzone, const Scalar &range, float grace, bool mirror) {
	return constrainP(wrappedScalar._p, deadzone, range._p, grace, mirror);
}
Scalar NuiLib::constrain(const Scalar &wrappedScalar, float deadzone, const Scalar &range, const Scalar &grace, bool mirror) {
	return constrainP(wrappedScalar._p, deadzone, range._p, grace._p, mirror);
}
Scalar NuiLib::constrain(const Scalar &wrappedScalar, const Scalar &deadzone, float range, float grace, bool mirror) {
	return constrainP(wrappedScalar._p, deadzone._p, range, grace, mirror);
}
Scalar NuiLib::constrain(const Scalar &wrappedScalar, const Scalar &deadzone, float range, const Scalar &grace, bool mirror) {
	return constrainP(wrappedScalar._p, deadzone._p, range, grace._p, mirror);
}
Scalar NuiLib::constrain(const Scalar &wrappedScalar, const Scalar &deadzone, const Scalar &range, float grace, bool mirror) {
	return constrainP(wrappedScalar._p, deadzone._p, range._p, grace, mirror);
}
Scalar NuiLib::constrain(const Scalar &wrappedScalar, const Scalar &deadzone, const Scalar &range, const Scalar &grace, bool mirror) {
	return constrainP(wrappedScalar._p, deadzone._p, range._p, grace._p, mirror);
}
Scalar NuiLib::acos(const Scalar &wrappedScalar) {
	return acosP(wrappedScalar._p);
}
Scalar NuiLib::invert(const Condition &condition) {
	return invertP(condition._p);
}
Scalar NuiLib::delta(const Scalar &wrappedScalar) {
	return deltaP(wrappedScalar._p);
}
Scalar NuiLib::project(const Vector &a, const Vector &b) {
	return projectP(a._p, b._p);
}
Scalar NuiLib::abs(const Scalar &wrappedScalar) {
	return absP(wrappedScalar._p);
}
Scalar NuiLib::ifScalar(const Condition& conditional, float ifTrue, float ifFalse) {
	return ifScalarP(conditional._p, ifTrue, ifFalse);
}
Scalar NuiLib::ifScalar(const Condition& conditional, float ifTrue, const Scalar &ifFalse) {
	return ifScalarP(conditional._p, ifTrue, ifFalse._p);
}
Scalar NuiLib::ifScalar(const Condition& conditional, const Scalar &ifTrue, float ifFalse) {
	return ifScalarP(conditional._p, ifTrue._p, ifFalse);
}
Scalar NuiLib::ifScalar(const Condition& conditional, const Scalar &ifTrue, const Scalar &ifFalse) {
	return ifScalarP(conditional._p, ifTrue._p, ifFalse._p);
}
Scalar NuiLib::smooth(const Scalar& toSmooth, const Scalar& numFrames) {
	return smoothP(toSmooth._p, numFrames._p);
}
Scalar NuiLib::smooth(const Scalar& toSmooth, int numFrames) {
	return smoothP(toSmooth._p, numFrames);
}



//-------------------------------------------- IScalar ---------------------------------------

IScalar::IScalar() : 
	IComponent("IScalar", "AnonymousScalar"),
	_printMax(0.f),
	_value(0.f) {}

IScalar::IScalar(string type) : 
	IComponent(type, "AnonymousScalar"), 
	_printMax(0.f), 
	_value(0.f) { }

IScalar::IScalar(string name, float value) : 
	IComponent("IScalar", name), 
	_printMax(0.f), 
	_value(value) { }

inline float IScalar::Get() { return _value; }
void IScalar::Set(float value) { 
	_value = value; 
	Trigger();
}

string IScalar::ToString() {
	char retArray[500];
	SPRINTF(retArray, 500, "%f", _value);
	return string(retArray);
}
string IScalar::ToString(string format) {
	char ret[50];
	SPRINTF(ret, 50, format.c_str(), _value);
	return string(ret);
}
string IScalar::ToString(int width) {
	_printMax = max (_printMax, std::abs(_value));
	float printValue = _printMax != 0 ? std::abs(_value) / _printMax : 0;
	int l = (int) (width * printValue);
	//char line[(width * 2) + 2];
	char line[500];

	if (_value < 0) {
		int blank = width - l;
		for (int i = 0; i < width; i++)
			line[i] = i < blank ? ' ' : '-';
		line[width] = '|';
		line[width + 1] = '\0';
	} else {
		for (int i = 0; i < width + 1 + l; i++)
			if (i < width)
				line[i] = ' ';
			else if (i == width)
				line[i] = '|';
			else
				line[i] = '-';
		line[width + 1 + l] = '\0';
	}
	//char retArray[(width * 2) + 50];
	char retArray[500];
	SPRINTF(retArray, 500, "%-*s % .3f", (width * 2) + 1, line, _value);
	return string(retArray);
}

void IScalar::Changed(IObservable *source) {
	float oldVal = _value;
	_value = CalculateValue();
	if (oldVal != _value)
		Trigger();
}

inline float IScalar::operator*() {
	return _value;
}

string IScalar::ToXML() { return ""; }
void IScalar::LoadXML(string xml) { }



DLL IScalar *NuiLib::sum(IScalar *addend1, IScalar *addend2) {
	return CreateOperator<SumScalar, IScalar*, IScalar*>(addend1, addend2);
}
DLL IScalar *NuiLib::difference(IScalar *minuend, IScalar *subtrahend) {
	return CreateOperator<DifferenceScalar, IScalar*, IScalar*>(minuend, subtrahend);
}
DLL IScalar *NuiLib::product(IScalar* factor1, IScalar* factor2) {
	return CreateOperator<ProductScalar, IScalar*, IScalar*>(factor1, factor2);
}
DLL IScalar *NuiLib::quotient(IScalar *dividend, IScalar *divisor) {
	return CreateOperator<QuotientScalar, IScalar*, IScalar*>(dividend, divisor);
}


DLL IScalar *NuiLib::sum(IScalar *addend1, float addend2) {
	return CreateOperator<SumScalar, IScalar*, float>(addend1, addend2);
}
DLL IScalar *NuiLib::difference(IScalar *minuend, float subtrahend) {
	return CreateOperator<DifferenceScalar, IScalar*, float>(minuend, subtrahend);
}
DLL IScalar *NuiLib::product(IScalar *factor1, float factor2) {
	return CreateOperator<ProductScalar, IScalar*, float>(factor1, factor2);
}
DLL IScalar *NuiLib::quotient(IScalar *dividend, float divisor) {
	return CreateOperator<QuotientScalar, IScalar*, float>(dividend, divisor);
}


DLL IScalar *NuiLib::sum(float addend1, IScalar *addend2) {
	return CreateOperator<SumScalar, float, IScalar*>(addend1, addend2);
}
DLL IScalar *NuiLib::difference(float minuend, IScalar *subtrahend) {
	return CreateOperator<DifferenceScalar, float, IScalar*>(minuend, subtrahend);
}
DLL IScalar *NuiLib::product(float factor1, IScalar *factor2) {
	return CreateOperator<ProductScalar, float, IScalar*>(factor1, factor2);
}
DLL IScalar *NuiLib::quotient(float dividend, IScalar *divisor) {
	return CreateOperator<QuotientScalar, float, IScalar*>(dividend, divisor);
}






DLL Scalar NuiLib::operator+(const Scalar &addend1, const Scalar &addend2) {
	return Scalar(sum(addend1._p, addend2._p));
}
DLL Scalar NuiLib::operator-(const Scalar &minuend, const Scalar &subtrahend) {
	return Scalar(difference(minuend._p, subtrahend._p));
}
DLL Scalar NuiLib::operator*(const Scalar& factor1, const Scalar& factor2) {
	return Scalar(product(factor1._p, factor2._p));
}
DLL Scalar NuiLib::operator/(const Scalar &dividend, const Scalar &divisor) {
	return Scalar(quotient(dividend._p, divisor._p));
}
DLL Scalar NuiLib::operator+=(Scalar &addend1, const Scalar &addend2) {
	return Scalar(addend1._p = sum(addend1._p, addend2._p));
}
DLL Scalar NuiLib::operator-=(Scalar &minuend, const Scalar &subtrahend) {
	return Scalar(minuend._p = difference(minuend._p, subtrahend._p));
}
DLL Scalar NuiLib::operator*=(Scalar& factor1, const Scalar& factor2) {
	return Scalar(factor1._p = product(factor1._p, factor2._p));
}
DLL Scalar NuiLib::operator/=(Scalar &dividend, const Scalar &divisor) {
	return Scalar(dividend._p = quotient(dividend._p, divisor._p));
}


DLL Scalar NuiLib::operator+(const Scalar &addend1, float addend2) {
	return Scalar(sum(addend1._p, addend2));
}
DLL Scalar NuiLib::operator-(const Scalar &minuend, float subtrahend) {
	return Scalar(difference(minuend._p, subtrahend));
}
DLL Scalar NuiLib::operator*(const Scalar &factor1, float factor2) {
	return Scalar(product(factor1._p, factor2));
}
DLL Scalar NuiLib::operator/(const Scalar &dividend, float divisor) {
	return Scalar(quotient(dividend._p, divisor));
}
DLL Scalar NuiLib::operator+=(Scalar &addend1, float addend2) {
	return Scalar(addend1._p = sum(addend1._p, addend2));
}
DLL Scalar NuiLib::operator-=(Scalar &minuend, float subtrahend) {
	return Scalar(minuend._p = difference(minuend._p, subtrahend));
}
DLL Scalar NuiLib::operator*=(Scalar &factor1, float factor2) {
	return Scalar(factor1._p = product(factor1._p, factor2));
}
DLL Scalar NuiLib::operator/=(Scalar &dividend, float divisor) {
	return Scalar(dividend._p = quotient(dividend._p, divisor));
}

DLL Scalar NuiLib::operator+(float addend1, const Scalar &addend2) {
	return Scalar(sum(addend1, addend2._p));
}
DLL Scalar NuiLib::operator-(float minuend, const Scalar &subtrahend) {
	return Scalar(difference(minuend, subtrahend._p));
}
DLL Scalar NuiLib::operator*(float factor1, const Scalar &factor2) {
	return Scalar(product(factor1, factor2._p));
}
DLL Scalar NuiLib::operator/(float dividend, const Scalar &divisor) {
	return Scalar(quotient(dividend, divisor._p));
}

/*
//-------------------------------------------- Scalar --------------------------------------

Scalar::Scalar() :
	IScalar(GetTypeName()) { }

Scalar::Scalar(float value) :
	IScalar(GetTypeName(), value) {
}
Scalar::Scalar(string name, float value) :
	IScalar(GetTypeName(), value) {
	SetName(name);
	SetFactory(factory);
}

void Scalar::SetValue(float value) {
	return value;
}

string Scalar::ToXML() { return ""; }
void Scalar::LoadXML(string) { }
*/

//-------------------------------------------- ScalarWrappingScalar --------------------------------------

ScalarWrappingScalar::ScalarWrappingScalar() :
	IScalar(GetTypeName()),
	_scalar() { }

ScalarWrappingScalar::ScalarWrappingScalar(string type) :
	IScalar(type),
	_scalar() { }

ScalarWrappingScalar::ScalarWrappingScalar(string type, IScalar *scalar) :
	IScalar(type),
	_scalar() {
	SetScalar(scalar);
}

inline float ScalarWrappingScalar::GetScalar() { return *_scalar; }
void ScalarWrappingScalar::SetScalar(IScalar *value) {
	AddAsListener(_scalar.Observable(), value);
	_scalar = value;
}

string ScalarWrappingScalar::ToXML() { return ""; }
void ScalarWrappingScalar::LoadXML(string) { }

//-------------------------------------------- TwoScalarWrappingScalar --------------------------------------

TwoScalarWrappingScalar::TwoScalarWrappingScalar() :
	IScalar(GetTypeName()),
	_scalar1(),
	_scalar2()  { }

TwoScalarWrappingScalar::TwoScalarWrappingScalar(string type) :
	IScalar(type),
	_scalar1(),
	_scalar2()  { }

TwoScalarWrappingScalar::TwoScalarWrappingScalar(string type, IScalar *scalar1, IScalar *scalar2) :
	IScalar(type),
	_scalar1(scalar1),
	_scalar2(scalar2)  { }

inline float TwoScalarWrappingScalar::GetScalar1() { return *_scalar1; }
void TwoScalarWrappingScalar::SetScalar1(IScalar *value) { _scalar1 = value; }
void TwoScalarWrappingScalar::SetScalar1(float value) { _scalar1 = value; }

inline float TwoScalarWrappingScalar::GetScalar2() { return *_scalar2; }
void TwoScalarWrappingScalar::SetScalar2(IScalar *value) { _scalar2 = value; }
void TwoScalarWrappingScalar::SetScalar2(float value) { _scalar2 = value; }

string TwoScalarWrappingScalar::ToXML() { return ""; }
void TwoScalarWrappingScalar::LoadXML(string) { }


//-------------------------------------------- VectorWrappingScalar --------------------------------------

VectorWrappingScalar::VectorWrappingScalar() :
	IScalar(GetTypeName()),
	_vector(NULL) { }

VectorWrappingScalar::VectorWrappingScalar(string type) :
	IScalar(type),
	_vector(NULL) { }

VectorWrappingScalar::VectorWrappingScalar(string type, IVector *vector) :
	IScalar(type),
	_vector(NULL) {
	SetVector(vector);
}

inline IVector *VectorWrappingScalar::GetVector() { return _vector; }
void VectorWrappingScalar::SetVector(IVector *value) {
	AddAsListener(_vector, value);
	_vector = value;
	Changed(value);
}

string VectorWrappingScalar::ToXML() { return ""; }
void VectorWrappingScalar::LoadXML(string) { }

//-------------------------------------------- TwoVectorWrappingScalar --------------------------------------

TwoVectorWrappingScalar::TwoVectorWrappingScalar() :
	IScalar(GetTypeName()),
	_vector1(NULL),
	_vector2(NULL)  { }

TwoVectorWrappingScalar::TwoVectorWrappingScalar(string type) :
	IScalar(type),
	_vector1(NULL),
	_vector2(NULL)  { }

TwoVectorWrappingScalar::TwoVectorWrappingScalar(string type, IVector *vector1, IVector *vector2) :
	IScalar(type),
	_vector1(vector1),
	_vector2(vector2)  { }

inline IVector *TwoVectorWrappingScalar::GetVector1() { return _vector1; }
void TwoVectorWrappingScalar::SetVector1(IVector *value) {
	AddAsListener(_vector1, value);
	_vector1 = value;
	Changed(value);
}

inline IVector *TwoVectorWrappingScalar::GetVector2() { return _vector2; }
void TwoVectorWrappingScalar::SetVector2(IVector *value) {
	AddAsListener(_vector2, value);
	_vector2 = value;
	Changed(value);
}

string TwoVectorWrappingScalar::ToXML() { return ""; }
void TwoVectorWrappingScalar::LoadXML(string) { }

//-------------------------------------------- ArithmeticScalar --------------------------------------

inline float ArithmeticScalar::Value() {
	return BinaryOperator::CalculateValue<float, float>(GetOperand1(), GetOperand2());
}

ArithmeticScalar::ArithmeticScalar(Operator op) : 
IScalar(GetTypeName()), 
	BinaryOperator<IScalar, float, float>(op, "%.3f", 0.f, 0.f, Converter(), Printer())  { }
ArithmeticScalar::~ArithmeticScalar() { }

float ArithmeticScalar::CalculateValue() {
	return Value();
}

//------------------------------------------- ConstrainedScalar --------------------------------------

ConstrainedScalar::ConstrainedScalar() : 
	ScalarWrappingScalar(GetTypeName()), 
	_deadzone(0.0), 
	_range(0.0), 
	_grace(0.0),
	_mirror(true){ }

float ConstrainedScalar::CalculateValue() {
	float _value = *_scalar;
	float invert = _mirror && _value < 0 ? -1.0f : 1.0f;
	if (_mirror)
		_value = std::abs(_value);
	if (_value <= *_deadzone)
		return 0.0f;
	else if (_value <= *_deadzone + *_range)
		return ((_value - *_deadzone) / *_range) * invert;
	else if (_value <= *_deadzone + *_range + *_grace)
		return 1 * invert;
	else
		return 0.0f;
}

inline bool ConstrainedScalar::GetMirror() { return _mirror; }
inline float ConstrainedScalar::GetDeadzone(){ return *_deadzone; }
inline float ConstrainedScalar::GetRange() { return *_range; }
inline float ConstrainedScalar::GetGrace() { return *_grace; }


void ConstrainedScalar::SetMirror(bool mirror) { _mirror = mirror; }
void ConstrainedScalar::SetDeadzone(float val) { _deadzone = val; }
void ConstrainedScalar::SetRange(float val) { _range = val; }
void ConstrainedScalar::SetGrace(float val) { _grace = val; }

void ConstrainedScalar::SetDeadzone(IScalar *val) { _deadzone = val; }
void ConstrainedScalar::SetRange(IScalar *val) { _range = val; }
void ConstrainedScalar::SetGrace(IScalar *val) { _grace = val; }

string ConstrainedScalar::ToXML() { return ""; }
void ConstrainedScalar::LoadXML(string xml) {}

//------------------------ Vector Scalar -------------------------

VectorScalar::VectorScalar() : 
	VectorWrappingScalar(GetTypeName()) {}

float VectorScalar::CalculateValue() {
	if (!_vector)
		return _value;
	else if (_value == 'x')
		return _vector->X();
	else if (_value == 'y')
		return _vector->Y();
	else if (_value == 'z')
		return _vector->Z();
	else 
		return _vector->Magnitude();
}

inline char VectorScalar::GetComponent() { return _value; }
void VectorScalar::SetComponent(char value) {
	_value = value;
	Changed(_vector);
}

//------------------------ ACos Scalar -------------------------

ACosScalar::ACosScalar() :
       	ScalarWrappingScalar(GetTypeName()) { }

	float ACosScalar::CalculateValue() {
	return mathacos(*_scalar);
}

//------------------------ Dot Scalar -------------------------

DotScalar::DotScalar() :
       	TwoVectorWrappingScalar(GetTypeName()) { }

float DotScalar::CalculateValue() {
	if (_vector1 == NULL || _vector2 == NULL)
		return _value;
	else {
		float dot = (**_vector1).dot(**_vector2);
		return (**_vector1).dot(**_vector2);
	}
}

//------------------------ Value Normalized Scalar -------------------------

ValueNormalizedScalar::ValueNormalizedScalar() : 
	ScalarWrappingScalar(GetTypeName()) { }

float ValueNormalizedScalar::CalculateValue() {
	return *_max != 0 ? *_scalar / *_max : 0;
}

void ValueNormalizedScalar::SetMax(float max) { _max = max; }
void ValueNormalizedScalar::SetMax(IScalar *max) { _max = max; }

//------------------------ Max Normalized Scalar -------------------------

MaxNormalizedScalar::MaxNormalizedScalar() : 
	ScalarWrappingScalar(GetTypeName()), 
       	_max(0) { }

float MaxNormalizedScalar::CalculateValue() {
	float value = *_scalar;
	_max = max(_max, std::abs(value));
	return _max != 0 ? value / _max : 0;
}

//------------------------ Invert Scalar -------------------------

InvertScalar::InvertScalar() : 
	IScalar(GetTypeName()), _condition(NULL) { }

ICondition *InvertScalar::GetCondition() {
	return _condition;
}
void InvertScalar::SetCondition(ICondition *value) {
	AddAsListener(_condition, value);
	_condition = value;
}

float InvertScalar::CalculateValue() {
	if (_condition == NULL)
		return _value;
	return **_condition ? 1.0f : -1.0f;
}

//------------------------ Delta Scalar -------------------------

DeltaScalar::DeltaScalar() : 
	ScalarWrappingScalar(GetTypeName()),
	_set(false),
	_oldValue(0) { }

float DeltaScalar::CalculateValue() {
	if (!_set) {
		_set = true;
		return _value;
	}
	float value = *_scalar;
	float ret = _oldValue - value;
	_oldValue = value;
	return ret;
}

//------------------------ ProjectionScalar -------------------------

ProjectionScalar::ProjectionScalar() : 
	TwoVectorWrappingScalar(GetTypeName()), _output(NULL) { }

float ProjectionScalar::CalculateValue() {
	if (!_vector1 || !_vector2)
		return _value;
	if (!_output) {
		_dot = dotP(_vector1, _vector2);
		_magnitude = magnitudeP(_vector2);
		_output = quotient(_dot, _magnitude);
	}
	if (_vector2->Magnitude() == 0.f)
		return 0.f;

	_dot->Changed(_vector1);
	_magnitude->Changed(_vector2);
	_output->Changed(_dot);

	return  **_output;
}


//------------------------ AbsScalar -------------------------

AbsScalar::AbsScalar() : 
	ScalarWrappingScalar(GetTypeName()) { }

float AbsScalar::CalculateValue() {
	return std::abs(*_scalar);
}


//------------------------ ConditionalScalar -------------------------

ConditionalScalar::ConditionalScalar() : 
TwoScalarWrappingScalar(GetTypeName()) { 
}

void ConditionalScalar::SetCondition(ICondition *value) { _condition = value; }
void ConditionalScalar::SetCondition(bool value) { _condition = value; }

float ConditionalScalar::CalculateValue() {
	return *_condition ? *_scalar1 : *_scalar2;
}


//------------------------ SmoothedScalar -------------------------

SmoothedScalar::SmoothedScalar() : 
ScalarWrappingScalar(GetTypeName()) { 
}

void SmoothedScalar::SetNumFrames(IScalar *value) { _numFrames = value; }
void SmoothedScalar::SetNumFrames(float value) { _numFrames = value; }

float SmoothedScalar::CalculateValue() {
	//Add the newest value to the queue
	_frames.push(*_scalar);

	//Remove all frames which are too old
	while (_frames.size() > *_numFrames)
		_frames.pop();

	//Tally up all the frames stored
	float value = 0.F;
	for (auto it = _frames._Get_container().begin(); it != _frames._Get_container().end(); it++)
		value += *it;

	//Return the average of the frames stored
	return value / _frames.size();
}
