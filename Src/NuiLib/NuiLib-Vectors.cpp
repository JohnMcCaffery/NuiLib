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
#include <NuiLib-Vectors.h>
#include <NuiLib-Scalars.h>
#include <cmath>

// Disable warning message 'warning C4351: new behavior: elements of array 'X' will be default initialized'
#pragma warning( disable : 4351 )

#define IS_INDEX(c) c-'x' >=0 && c-'x' < 3
#define INDEX(c) c-'x'

const int MAG = 3;

using namespace NuiLib;

NormalizedVector *NuiLib::normalizeP(IVector *wrappedVector) {
	NormalizedVector *vector = ExtensionFactory()->Make<NormalizedVector>("normalized(" + wrappedVector->GetName() + ")");
	vector->SetVector(wrappedVector);
	vector->Changed(wrappedVector);
	return vector;
}
DotVector *NuiLib::planeDotsP(IVector *vector1, IVector *vector2) {
	DotVector *vector = ExtensionFactory()->Make<DotVector>(vector1->GetName() + " . " + vector2->GetName());
	vector->SetVector1(vector1);
	vector->SetVector2(vector2);
	vector->Changed(vector1);
	return vector;
}
CrossVector *NuiLib::crossP(IVector *vector1, IVector *vector2) {
	CrossVector *vector = ExtensionFactory()->Make<CrossVector>(vector1->GetName() + " x " + vector2->GetName());
	vector->SetVector1(vector1);
	vector->SetVector2(vector2);
	vector->Changed(vector1);
	return vector;
}
PartialVector *NuiLib::limitP(IVector *wrappedVector, bool x, bool y, bool z) {
	string xStr = string(x ? "X" : "");
	string yStr = string(y ? "Y" : "");
	string zStr = string(z ? "Z" : "");
	PartialVector *vector = ExtensionFactory()->Make<PartialVector>(xStr + yStr + zStr +"Limited(" + wrappedVector->GetName() + ")");
	vector->SetVector(wrappedVector);
	vector->UseX(x);
	vector->UseY(y);
	vector->UseZ(z);
	vector->Changed(wrappedVector);
	return vector;
}
DeltaVector *NuiLib::deltaP(IVector *wrappedVector) {
	DeltaVector *vector = ExtensionFactory()->Make<DeltaVector>("delta(" + wrappedVector->GetName() + ")");
	vector->SetVector(wrappedVector);
	vector->Changed(wrappedVector);
	return vector;
}
SnapshotVector *NuiLib::snapshotP(IVector *wrappedVector, ICondition *condition) {
	SnapshotVector *vector = ExtensionFactory()->Make<SnapshotVector>("snapshot(" + wrappedVector->GetName() + " when " + condition->GetName()+ ")");
	vector->SetVector(wrappedVector);
	vector->SetCondition(condition);
	vector->Changed(wrappedVector);
	return vector;
}
MomentumVector *NuiLib::momentumP(IVector *wrappedVector, ICondition *condition) {
	MomentumVector *vector = ExtensionFactory()->Make<MomentumVector>("momentum(" + wrappedVector->GetName() + " when " + condition->GetName() + ")");
	vector->SetVector(wrappedVector);
	vector->SetCondition(condition);
	vector->Changed(wrappedVector);
	return vector;
}
MomentumVector *NuiLib::momentumP(IVector *wrappedVector, ICondition *condition, float decrement, float threshold) {
	//TODO this is weird - use factory
	MomentumVector *vector = momentumP(wrappedVector, condition);
	vector->SetDecrement(decrement);
	vector->SetThreshold(threshold);
	vector->Changed(wrappedVector);
	return vector;
}
MagnitudeScaledVector *NuiLib::scaleP(IVector *wrappedVector, IScalar *scale) {
	MagnitudeScaledVector *vector = ExtensionFactory()->Make<MagnitudeScaledVector>("scaled(" + wrappedVector->GetName() + ")");
	vector->SetVector(wrappedVector);
	vector->SetScale(scale);
	vector->Changed(wrappedVector);
	return vector;
}
MagnitudeScaledVector *NuiLib::scaleP(IVector *wrappedVector, float scale) {
	MagnitudeScaledVector *vector = ExtensionFactory()->Make<MagnitudeScaledVector>("scaled(" + wrappedVector->GetName() + ")");
	vector->SetVector(wrappedVector);
	vector->SetScale(scale);
	vector->Changed(wrappedVector);
	return vector;
}
PlaneIntersectionVector *NuiLib::intersectP(IVector *pPlane, IVector *planeNormal, IVector *pLine, IVector *lineDir) {
	PlaneIntersectionVector *vector = ExtensionFactory()->Make<PlaneIntersectionVector>("intersection(" + pPlane->GetName() + "/" + planeNormal->GetName() + " and " + pLine->GetName() + "/" + lineDir->GetName() + ")");
	vector->SetVector1(pPlane);
	vector->SetVector2(planeNormal);
	vector->SetVector3(pLine);
	vector->SetVector4(lineDir);
	vector->Changed(pPlane);
	return vector;
}
ScalarVector *NuiLib::vectorP(float value) {
	char name[500];
	SPRINTF(name, 500, "(%.3f,%.3f,%.3f)", value, value, value);
	ScalarVector *vector = ExtensionFactory()->Make<ScalarVector>(string(name));
	vector->SetX(value);
	vector->SetY(value);
	vector->SetZ(value);
	vector->Changed(ExtensionFactory());
	return vector;
}
ScalarVector *NuiLib::vectorP(IScalar *value) {
	char name[500];
	SPRINTF(name, 500, "(%s,%s,%s)", value->GetCName(), value->GetCName(), value->GetCName());
	ScalarVector *vector = ExtensionFactory()->Make<ScalarVector>(string(name));
	vector->SetX(value);
	vector->SetY(value);
	vector->SetZ(value);
	vector->Changed(value);
	return vector;
}
template<typename XT, typename YT, typename ZT>
ScalarVector *vectorT(XT x, YT y, ZT z, string name) {
	ScalarVector *vector = ExtensionFactory()->Make<ScalarVector>(name);
	vector->SetX(x);
	vector->SetY(y);
	vector->SetZ(z);
	vector->Changed(ExtensionFactory());
	return vector;
}
ScalarVector *NuiLib::vectorP(float x, float y, float z) {
	char name[500];
	SPRINTF(name, 500, "(%.3f,%.3f,%.3f)", x, y, z);
	return vectorT(x, y, z, string(name));
}
ScalarVector *NuiLib::vectorP(float x, float y, IScalar *z) {
	char name[500];
	SPRINTF(name, 500, "(%.3f,%.3f,%s)", x, y, z->GetCName());
	return vectorT(x, y, z, string(name));
}
ScalarVector *NuiLib::vectorP(float x, IScalar *y, float z) {
	char name[500];
	SPRINTF(name, 500, "(%.3f,%s,%.3f)", x, y->GetCName(), z);
	return vectorT(x, y, z, string(name));
}
ScalarVector *NuiLib::vectorP(float x, IScalar *y, IScalar *z) {
	char name[500];
	SPRINTF(name, 500, "(%.3f,%s,%s)", x, y->GetCName(), z->GetCName());
	return vectorT(x, y, z, string(name));
}
ScalarVector *NuiLib::vectorP(IScalar *x, float y, float z) {
	char name[500];
	SPRINTF(name, 500, "(%s,%.3f,%.3f)", x->GetCName(), y, z);
	return vectorT(x, y, z, string(name));
}
ScalarVector *NuiLib::vectorP(IScalar *x, float y, IScalar *z) {
	char name[500];
	SPRINTF(name, 500, "(%s,%.3f,%s)", x->GetCName(), y, z->GetCName());
	return vectorT(x, y, z, string(name));
}
ScalarVector *NuiLib::vectorP(IScalar *x, IScalar *y, float z) {
	char name[500];
	SPRINTF(name, 500, "(%s,%s,%.3f)", x->GetCName(), y->GetCName(), z);
	return vectorT(x, y, z, string(name));
}
ScalarVector *NuiLib::vectorP(IScalar *x, IScalar *y, IScalar *z) {
	char name[500];
	SPRINTF(name, 500, "(%s,%s,%s)", x->GetCName(), y->GetCName(), z->GetCName());
	return vectorT(x, y, z, string(name));
}









Vector NuiLib::normalize(const Vector &wrappedVector) {
	return Vector(normalizeP(wrappedVector._p));
}
Vector NuiLib::planeDots(const Vector &vector1, const Vector &vector2) {
	return Vector(planeDotsP(vector1._p, vector2._p));
}
Vector NuiLib::cross(const Vector &vector1, const Vector &vector2) {
	return Vector(crossP(vector1._p, vector2._p));
}
Vector NuiLib::limit(const Vector &wrappedVector, bool x, bool y, bool z) {
	return Vector(limitP(wrappedVector._p, x, y, z));
}
Vector NuiLib::delta(const Vector &wrappedVector) {
	return Vector(deltaP(wrappedVector._p));
}
Vector NuiLib::snapshot(const Vector &wrappedVector, const Condition &condition) {
	return Vector(snapshotP(wrappedVector._p, condition._p));
}
Vector NuiLib::momentum(const Vector &wrappedVector, const Condition &condition) {
	return Vector(momentumP(wrappedVector._p, condition._p));
}
Vector NuiLib::momentum(const Vector &wrappedVector, const Condition &condition, const float decrement, const float threshold) {
	return Vector(momentumP(wrappedVector._p, condition._p, decrement, threshold));
}
Vector NuiLib::scale(const Vector &wrappedVector, const Scalar &scale) {
	return Vector(scaleP(wrappedVector._p, scale._p));
}
Vector NuiLib::scale(const Vector &wrappedVector, const float scale) {
	return Vector(scaleP(wrappedVector._p, scale));
}
Vector NuiLib::intersect(const Vector &pPlane, const Vector &planeNormal, const Vector &pLine, const Vector &lineDir) {
	return Vector(intersectP(pPlane._p, planeNormal._p, pLine._p, lineDir._p));
}

Vector::Vector(const Scalar &value) : _p(vectorP(value._p, value._p, value._p)) { }

Vector::Vector(float x, float y, float z) : _p(vectorP(x, y, z)) { }
Vector::Vector(float x, float y, const Scalar &z) : _p(vectorP(x, y, z._p)) { }
Vector::Vector(float x, const Scalar &y, float z) : _p(vectorP(x, y._p, z)) { }
Vector::Vector(float x, const Scalar &y, const Scalar &z) : _p(vectorP(x, y._p, z._p)) { }
Vector::Vector(const Scalar &x, const Scalar &y, const Scalar &z) : _p(vectorP(x._p, y._p, z._p)) { }
Vector::Vector(const Scalar &x, const Scalar &y, float z) : _p(vectorP(x._p, y._p, z)) { }
Vector::Vector(const Scalar &x, float y, const Scalar &z) : _p(vectorP(x._p, y, z._p)) { }
Vector::Vector(const Scalar &x, float y, float z) : _p(vectorP(x._p, y, z)) { }

Vector::Vector(string name, float x, float y, float z) : _p(vectorP(x, y, z)) { _p->SetName(name); }
Vector::Vector(string name, float x, float y, const Scalar &z) : _p(vectorP(x, y, z._p)) { _p->SetName(name); }
Vector::Vector(string name, float x, const Scalar &y, float z) : _p(vectorP(x, y._p, z)) { _p->SetName(name); }
Vector::Vector(string name, float x, const Scalar &y, const Scalar &z) : _p(vectorP(x, y._p, z._p)) { _p->SetName(name); }
Vector::Vector(string name, const Scalar &x, const Scalar &y, const Scalar &z) : _p(vectorP(x._p, y._p, z._p)) { _p->SetName(name); }
Vector::Vector(string name, const Scalar &x, const Scalar &y, float z) : _p(vectorP(x._p, y._p, z)) { _p->SetName(name); }
Vector::Vector(string name, const Scalar &x, float y, const Scalar &z) : _p(vectorP(x._p, y, z._p)) { _p->SetName(name); }
Vector::Vector(string name, const Scalar &x, float y, float z) : _p(vectorP(x._p, y, z)) { _p->SetName(name); }



//-------------------------------------------- IVector ---------------------------------------

IVector::IVector() : IComponent("IVector", "AnonymousVector"), _value(), _maxs() {
	Init(0, 0, 0);
}

IVector::IVector(string type) : IComponent(type, "UnNamed Vector"), _value(), _maxs(){
	Init(0, 0, 0);
}
//IVector::IVector(string type, string name) : IComponent(type, name), _value(), _maxs(){ Init(0, 0, 0); }
//IVector::IVector(string type, float x, float y, float z) : IComponent(type, "UnNamed Vector"), _value(x,y,z), _maxs() { Init (x, y, z); }
//IVector::IVector(string type, string name, float x, float y, float z) : IComponent(type, name), _value(x,y,z), _maxs() { Init (x, y, z); }
//IVector::IVector(float x, float y, float z) : IComponent(GetTypeName(), "UnNamed Vector"), _value(x, y, z), _maxs() { Init (x, y, z); }

void IVector::Init(float x, float y, float z) {
	_value.x = x;
	_value.y = y;
	_value.z = z;

	_maxs[0] = x;
	_maxs[1] = y;
	_maxs[2] = z;
	_maxs[3] = 0; 
}

void IVector::Changed(IObservable *source) {
	cv::Point3f oldVal = _value;
	_value = CalculateValue();
	if (oldVal.x != _value.x || oldVal.y != _value.y || oldVal.z != _value.z)
		Trigger();
}

inline cv::Point3f IVector::operator*() { return _value; }
const float IVector::operator[](AXIS axis) {
	switch(axis){
	case X_AXIS: return _value.x;
	case Y_AXIS: return _value.y;
	case Z_AXIS: return _value.z;
	default: return 0.f;
	 }
}

void IVector::X(float value) { _value.x = value; }
void IVector::Y(float value) { _value.y = value; }
void IVector::Z(float value) { _value.z = value; }

void IVector::Set(float x, float y, float z) { 
	_value.x = x;
	_value.y = y;
	_value.z = z;
	Trigger();
}

DLL IVector *NuiLib::sum(IVector *addend1, IVector *addend2) { return CreateOperator<SumVector, IVector*, IVector*>(addend1, addend2); }
DLL IVector *NuiLib::difference(IVector *minuend, IVector *subtrahend) { return CreateOperator<DifferenceVector, IVector*, IVector*>(minuend, subtrahend); }
DLL IVector *NuiLib::product(IVector *factor1, IVector *factor2) { return CreateOperator<ProductVector, IVector*, IVector*>(factor1, factor2); }
DLL IVector *NuiLib::quotient(IVector *dividend, IVector *divisor) { return CreateOperator<QuotientVector, IVector*, IVector*>(dividend, divisor); }

DLL IVector *NuiLib::sum(IVector *addend1, float addend2) { return CreateOperator<SumVector, IVector*, float>(addend1, addend2); }
DLL IVector *NuiLib::difference(IVector *minuend, float subtrahend) { return CreateOperator<DifferenceVector, IVector*, float>(minuend, subtrahend); }
DLL IVector *NuiLib::product(IVector *factor1, float factor2) { return CreateOperator<ProductVector, IVector*, float>(factor1, factor2); }
DLL IVector *NuiLib::quotient(IVector *dividend, float divisor) { return CreateOperator<QuotientVector, IVector*, float>(dividend, divisor); }

DLL IVector *NuiLib::sum(IVector *addend1, cv::Point3f addend2) { return CreateOperator<SumVector, IVector*, cv::Point3f>(addend1, addend2); }
DLL IVector *NuiLib::difference(IVector *minuend, cv::Point3f subtrahend) { return CreateOperator<DifferenceVector, IVector*, cv::Point3f>(minuend, subtrahend); }
DLL IVector *NuiLib::product(IVector *factor1, cv::Point3f factor2) { return CreateOperator<ProductVector, IVector*, cv::Point3f>(factor1, factor2); }
DLL IVector *NuiLib::quotient(IVector *dividend, cv::Point3f divisor) { return CreateOperator<QuotientVector, IVector*, cv::Point3f>(dividend, divisor); }

DLL IVector *NuiLib::sum(float addend1, IVector *addend2) { return CreateOperator<SumVector, float, IVector*>(addend1, addend2); }
DLL IVector *NuiLib::difference(float minuend, IVector *subtrahend) { return CreateOperator<DifferenceVector, float, IVector*>(minuend, subtrahend); }
DLL IVector *NuiLib::product(float factor1, IVector *factor2) { return CreateOperator<ProductVector, float, IVector*>(factor1, factor2); }
DLL IVector *NuiLib::quotient(float dividend, IVector *divisor) { return CreateOperator<QuotientVector, float, IVector*>(dividend, divisor); }

DLL IVector *NuiLib::sum(cv::Point3f addend1, IVector *addend2) { return CreateOperator<SumVector, cv::Point3f, IVector*>(addend1, addend2); }
DLL IVector *NuiLib::difference(cv::Point3f minuend, IVector *subtrahend) { return CreateOperator<DifferenceVector, cv::Point3f, IVector*>(minuend, subtrahend); }
DLL IVector *NuiLib::product(cv::Point3f factor1, IVector *factor2) { return CreateOperator<ProductVector, cv::Point3f, IVector*>(factor1, factor2); }
DLL IVector *NuiLib::quotient(cv::Point3f dividend, IVector *divisor) { return CreateOperator<QuotientVector, cv::Point3f, IVector*>(dividend, divisor); }







DLL Vector NuiLib::operator+(const Vector &addend1, const Vector &addend2) { return Vector(sum(addend1._p, addend2._p)); }
DLL Vector NuiLib::operator-(const Vector &minuend, const Vector &subtrahend) { return Vector(difference(minuend._p, subtrahend._p)); }
DLL Vector NuiLib::operator*(const Vector &factor1, const Vector &factor2) { return Vector(product(factor1._p, factor2._p)); }
DLL Vector NuiLib::operator/(const Vector &dividend, const Vector &divisor) { return Vector(quotient(dividend._p, divisor._p)); }
DLL Vector NuiLib::operator+=(Vector &addend1, const Vector &addend2) { return Vector(addend1._p = sum(addend1._p, addend2._p)); }
DLL Vector NuiLib::operator-=(Vector &minuend, const Vector &subtrahend) { return Vector(minuend._p = difference(minuend._p, subtrahend._p)); }
DLL Vector NuiLib::operator*=(Vector &factor1, const Vector &factor2) { return Vector(factor1._p = product(factor1._p, factor2._p)); }
DLL Vector NuiLib::operator/=(Vector &dividend, const Vector &divisor) { return Vector(dividend._p = quotient(dividend._p, divisor._p)); }

DLL Vector NuiLib::operator+(const Vector &addend1, float addend2) { return Vector(sum(addend1._p, addend2)); }
DLL Vector NuiLib::operator-(const Vector &minuend, float subtrahend) { return Vector(difference(minuend._p, subtrahend)); }
DLL Vector NuiLib::operator*(const Vector &factor1, float factor2) { return Vector(product(factor1._p, factor2)); }
DLL Vector NuiLib::operator/(const Vector &dividend, float divisor) { return Vector(quotient(dividend._p, divisor)); }
DLL Vector NuiLib::operator+=(Vector &addend1, float addend2) { return Vector(addend1._p = sum(addend1._p, addend2)); }
DLL Vector NuiLib::operator-=(Vector &minuend, float subtrahend) { return Vector(minuend._p = difference(minuend._p, subtrahend)); }
DLL Vector NuiLib::operator*=(Vector &factor1, float factor2) { return Vector(factor1._p = product(factor1._p, factor2)); }
DLL Vector NuiLib::operator/=(Vector &dividend, float divisor) { return Vector(dividend._p = quotient(dividend._p, divisor)); }

DLL Vector NuiLib::operator+(const Vector &addend1, cv::Point3f addend2) { return Vector(sum(addend1._p, addend2)); }
DLL Vector NuiLib::operator-(const Vector &minuend, cv::Point3f subtrahend) { return Vector(difference(minuend._p, subtrahend)); }
DLL Vector NuiLib::operator*(const Vector &factor1, cv::Point3f factor2) { return Vector(product(factor1._p, factor2)); }
DLL Vector NuiLib::operator/(const Vector &dividend, cv::Point3f divisor) { return Vector(quotient(dividend._p, divisor)); }
DLL Vector NuiLib::operator+=(Vector &addend1, cv::Point3f addend2) { return Vector(addend1._p = sum(addend1._p, addend2)); }
DLL Vector NuiLib::operator-=(Vector &minuend, cv::Point3f subtrahend) { return Vector(minuend._p = difference(minuend._p, subtrahend)); }
DLL Vector NuiLib::operator*=(Vector &factor1, cv::Point3f factor2) { return Vector(factor1._p = product(factor1._p, factor2)); }
DLL Vector NuiLib::operator/=(Vector &dividend, cv::Point3f divisor) { return Vector(dividend._p = quotient(dividend._p, divisor)); }

DLL Vector NuiLib::operator+(float addend1, const Vector &addend2) { return Vector(sum(addend1, addend2._p)); }
DLL Vector NuiLib::operator-(float minuend, const Vector &subtrahend) { return Vector(difference(minuend, subtrahend._p)); }
DLL Vector NuiLib::operator*(float factor1, const Vector &factor2) { return Vector(product(factor1, factor2._p)); }
DLL Vector NuiLib::operator/(float dividend, const Vector &divisor) { return Vector(quotient(dividend, divisor._p)); }

DLL Vector NuiLib::operator+(cv::Point3f addend1, const Vector &addend2) { return Vector(sum(addend1, addend2._p)); }
DLL Vector NuiLib::operator-(cv::Point3f minuend, const Vector &subtrahend) { return Vector(difference(minuend, subtrahend._p)); }
DLL Vector NuiLib::operator*(cv::Point3f factor1, const Vector &factor2) { return Vector(product(factor1, factor2._p)); }
DLL Vector NuiLib::operator/(cv::Point3f dividend, const Vector &divisor) { return Vector(quotient(dividend, divisor._p)); }



float IVector::Magnitude() { return sqrt(pow(_value.x,2) + pow(_value.y,2) + pow(_value.z,2)); }

cv::Point3f IVector::Cross(IVector *vector) { 
	return cv::Point3f(
		(_value.x * vector->X()) - (_value.x * vector->X()),
		(_value.y * vector->Y()) - (_value.y * vector->Y()),
		(_value.z * vector->Z()) - (_value.z * vector->Z()));
}

cv::Point3f IVector::Normalize() { 
	float mag = Magnitude();
	if (mag == 0)
		return cv::Point3f();
	else
		return cv::Point3f(_value.x / mag, _value.x / mag, _value.x / mag); 
}

float IVector::X() { return _value.x; }
float IVector::Y() { return _value.y; }
float IVector::Z() { return _value.z; }

string IVector::ToString() { 
	char* ret = new char[50];
	SPRINTF(ret, 50, "<%f,%f,%f>", _value.x, _value.y, _value.z);
	return string(ret);
}
string IVector::ToString(int width) {
	return 
		MakeAxis(width, 'x') + " " + 
		MakeAxis(width, 'y') + " " + 
		MakeAxis(width, 'z');

}
string IVector::ToString(int width, char axis) {
	if (IS_INDEX(axis))
		return MakeAxis(width, axis);
	else if (axis == 'm')
		return MakeMagnitude(width);
	else
		return ToString(width);
}
string IVector::MakeMagnitude(int width) {
	float value = Magnitude();
	if (value >_maxs[MAG])
		_maxs[MAG] = value;

	int l = _maxs[MAG] != 0 ? (int) (width * 2 * (value / _maxs[MAG])) : 0;
	char line[500];
	for (int i = 0; i < l; i++)
		line[i] = '-';
	line[l] = '|';
	line[l+1] = '\0';
	char retArray[500];
	SPRINTF(retArray, 500, "%-*s m % .3f", (width * 2) + 1, line, value);
	return string(retArray);
}

string IVector::MakeAxis(int width, char axis) {
	float value = 0.f;
	if (axis == 'x')
		value = _value.x;
	if (axis == 'y')
		value = _value.y;
	else if (axis == 'z')
		value = _value.z;
	else
		value = Magnitude();
	float absValue = std::abs(value);
	if (absValue > _maxs[INDEX(axis)])
		_maxs[INDEX(axis)] = absValue;
	int l = _maxs[INDEX(axis)] != 0 ? (int) (width * (value / _maxs[INDEX(axis)])) : 0;
	//char line[(width * 2) + 50];
	char line[500];

	if (value < 0) {
		int blank = width + l;
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
	char retArray[500];
	SPRINTF(retArray, 500, "%-*s %c % .3f", (width * 2) + 1, line, axis, value);
	string ret = string(retArray);
	return ret;
}

string IVector::ToXML() { return ""; }
void IVector::LoadXML(string) { }

//-------------------------------------------- VectorWrappingVector --------------------------------------

VectorWrappingVector::VectorWrappingVector() :
IVector(GetTypeName()),
	_vector(NULL) { }

VectorWrappingVector::VectorWrappingVector(string type) :
IVector(type),
	_vector(NULL) { }

VectorWrappingVector::VectorWrappingVector(string type, IVector *vector) :
IVector(type),
	_vector(NULL) {
		SetVector(vector);
}

inline IVector *VectorWrappingVector::GetVector() { return _vector; }
void VectorWrappingVector::SetVector(IVector *value) {
	AddAsListener(_vector, value);
	_vector = value;
	Changed(value);
}

string VectorWrappingVector::ToXML() { return ""; }
void VectorWrappingVector::LoadXML(string) { }

//-------------------------------------------- TwoVectorWrappingVector --------------------------------------

TwoVectorWrappingVector::TwoVectorWrappingVector() :
IVector(GetTypeName()),
	_vector1(NULL),
	_vector2(NULL) { }

TwoVectorWrappingVector::TwoVectorWrappingVector(string type) :
IVector(type),
	_vector1(NULL),
	_vector2(NULL)  { }

TwoVectorWrappingVector::TwoVectorWrappingVector(string type, IVector *vector1, IVector *vector2) :
IVector(type),
	_vector1(vector1),
	_vector2(vector2)  { }

inline IVector *TwoVectorWrappingVector::GetVector1() { return _vector1; }
void TwoVectorWrappingVector::SetVector1(IVector *value) {
	AddAsListener(_vector1, value);
	_vector1 = value;
	Changed(value);
}

inline IVector *TwoVectorWrappingVector::GetVector2() { return _vector2; }
void TwoVectorWrappingVector::SetVector2(IVector *value) {
	AddAsListener(_vector2, value);
	_vector2 = value;
	Changed(value);
}

string TwoVectorWrappingVector::ToXML() { return ""; }
void TwoVectorWrappingVector::LoadXML(string) { }


//-------------------------------------------- ThreeVectorWrappingVector --------------------------------------

ThreeVectorWrappingVector::ThreeVectorWrappingVector() :
TwoVectorWrappingVector(GetTypeName()),
	_vector3(NULL) { }

ThreeVectorWrappingVector::ThreeVectorWrappingVector(string type) :
TwoVectorWrappingVector(type),
	_vector3(NULL)  { }

ThreeVectorWrappingVector::ThreeVectorWrappingVector(string type, IVector *vector1, IVector *vector2, IVector *vector3) :
TwoVectorWrappingVector(type, vector1, vector2),
	_vector3(vector3)  { }

inline IVector *ThreeVectorWrappingVector::GetVector3() { return _vector3; }
void ThreeVectorWrappingVector::SetVector3(IVector *value) {
	AddAsListener(_vector3, value);
	_vector3 = value;
	Changed(value);
}

string ThreeVectorWrappingVector::ToXML() { return ""; }
void ThreeVectorWrappingVector::LoadXML(string) { }

//-------------------------------------------- FourVectorWrappingVector --------------------------------------

FourVectorWrappingVector::FourVectorWrappingVector() :
ThreeVectorWrappingVector(GetTypeName()),
	_vector4(NULL) { }

FourVectorWrappingVector::FourVectorWrappingVector(string type) :
ThreeVectorWrappingVector(type),
	_vector4(NULL)  { }

FourVectorWrappingVector::FourVectorWrappingVector(string type, IVector *vector1, IVector *vector2, IVector *vector3, IVector *vector4) :
ThreeVectorWrappingVector(type, vector1, vector2, vector3),
	_vector4(vector4)  { }

inline IVector *FourVectorWrappingVector::GetVector4() { return _vector4; }
void FourVectorWrappingVector::SetVector4(IVector *value) {
	AddAsListener(_vector4, value);
	_vector4 = value;
	Changed(value);
}

string FourVectorWrappingVector::ToXML() { return ""; }
void FourVectorWrappingVector::LoadXML(string) { }

//-------------------------------------------- NVectorWrappingVector --------------------------------------

NVectorWrappingVector::NVectorWrappingVector() :
IVector(GetTypeName()) { }

NVectorWrappingVector::NVectorWrappingVector(string type) :
IVector(type) { }

void NVectorWrappingVector::AddVector(IVector *value) { 
	AddAsListener(NULL, value);
	_vectors.insert(value);
	Changed(value);
}
void NVectorWrappingVector::RemoveVector(IVector *value) {
	RemoveAsListener(value);
	_vectors.erase(value);
	Changed(NULL);
}

string NVectorWrappingVector::ToXML() { return ""; }
void NVectorWrappingVector::LoadXML(string) { }


//----------------------- Vector ---------------------------
/*
Vector::Vector() : IVector(GetTypeName()) { }
Vector::Vector(INuiFactory *factory) : IVector(GetTypeName()) { SetFactory(factory); }
Vector::Vector(string name) : IVector(GetTypeName(), name) { SetFactory(factory); }
Vector::Vector(float x, float y, float z) : IVector(GetTypeName(), x, y, z) { }
Vector::Vector(float x, float y, float z) : IVector(GetTypeName(), x, y, z) { SetFactory(factory); }
Vector::Vector(string name, float x, float y, float z) : IVector(GetTypeName(), name, x, y, z) { SetFactory(factory); }
void Vector::SetX(float x) { Update(x, Y(), Z()); }
void Vector::SetY(float y) { Update(X(), y, Z()); }
void Vector::SetZ(float z) { Update(X(), Y(), z); }
void Vector::Set(float x, float y, float z) { Update(x, y, z); }
*/

//------------------------ Joint Vector -----------------

JointVector::JointVector(string type) : IVector(type) { }
JointVector::JointVector(string type, string joint) : IVector(type), _joint(joint) { }

string JointVector::GetJoint() { return _joint; }

//-------------------------------------------- ArithmeticVector --------------------------------------

///
/// Calculate a vector value from the inputs.
/// WARNING: The returned value is allocated on the stack. It WILL go out of scope so only use it to update other values. Don't try and store the array.
///
inline cv::Point3f ArithmeticVector::Value() {
	cv::Point3f op1 = GetOperand1();
	cv::Point3f op2 = GetOperand2();
	float x = BinaryOperator::CalculateValue<float, float>(op1.x, op2.x);
	float y = BinaryOperator::CalculateValue<float, float>(op1.y, op2.y);
	float z = BinaryOperator::CalculateValue<float, float>(op1.z, op2.z);
	return cv::Point3f(x, y, z);
}

ArithmeticVector::ArithmeticVector(Operator op) : 
IVector(GetTypeName()),
	//BinaryOperator<IVector, float, float*>(op, "%.3f %.3f %.3f") { }
	BinaryOperator<IVector, float, cv::Point3f>(op, "%.3f %.3f %.3f", cv::Point3f(), cv::Point3f(), [](float in) { 
		return cv::Point3f(in);
}, [](const char *format, cv::Point3f p) -> char* {
	char *buff = new char[50];
	SPRINTF(buff, 50, "%.3f %.3f %.3f", p.x, p.y, p.z);
	return buff;
}) { }
ArithmeticVector::~ArithmeticVector() { }

cv::Point3f ArithmeticVector::CalculateValue() {
	return Value();
}

void ArithmeticVector::SetOperand1(float value) {
	BinaryOperator<IVector, float, cv::Point3f>::SetOperand1(cv::Point3f(value, value, value));
}
void ArithmeticVector::SetOperand2(float value) {
	BinaryOperator<IVector, float, cv::Point3f>::SetOperand2(cv::Point3f(value, value, value));
}

//----------------------- Dot Vector ---------------------------

DotVector::DotVector() : TwoVectorWrappingVector(GetTypeName()) { }
DotVector::DotVector(IVector *vector1, IVector *vector2) : 
TwoVectorWrappingVector(GetTypeName(), vector1, vector2) { }

cv::Point3f DotVector::CalculateValue() {
	if (_vector1 == NULL || _vector2 == NULL || _vector1->Magnitude() == 0 || _vector2->Magnitude() == 0)
		return _value;
	else  {
		cv::Point3f x1 = cv::Point3f(_vector1->Y(), _vector1->Z(), 0);
		cv::Point3f y1 = cv::Point3f(_vector1->X(), _vector1->Z(), 0);
		cv::Point3f z1 = cv::Point3f(_vector1->X(), _vector1->Y(), 0);

		cv::Point3f x2 = cv::Point3f(_vector2->Y(), _vector2->Z(), 0);
		cv::Point3f y2 = cv::Point3f(_vector2->X(), _vector2->Z(), 0);
		cv::Point3f z2 = cv::Point3f(_vector2->X(), _vector2->Y(), 0);

		float dotX = (x1.x * x2.x) + (x1.y * x2.y);
		float dotY = (y1.x * y2.x) + (y1.y * y2.y);
		float dotZ = (z1.x * z2.x) + (z1.y * z2.y);

		return cv::Point3f(dotX, dotY, dotZ);
	}
}


//----------------------- Cross Vector ---------------------------

CrossVector::CrossVector() : TwoVectorWrappingVector(GetTypeName()) { }

cv::Point3f CrossVector::CalculateValue() {
	if (_vector1 == NULL || _vector2 == NULL || _vector1->Magnitude() == 0 || _vector2->Magnitude() == 0)
		return _value;
	else  {
		return cv::Point3f((_vector1->Y() * _vector2->Z()) - (_vector1->Z() * _vector2->Y()),
			(_vector1->X() * _vector2->Z()) - (_vector1->Z() * _vector2->X()),
			(_vector1->X() * _vector2->Y()) - (_vector1->Y() * _vector2->X()));
	}
}

//-------------------------------------------- MaxNormalizedVector ---------------------------------------

MaxNormalizedVector::MaxNormalizedVector() : 
VectorWrappingVector(GetTypeName()),
	_max(0), 
	_magMax(0) { }
MaxNormalizedVector::MaxNormalizedVector(IVector *vector) : 
VectorWrappingVector(GetTypeName(), vector),
	_max(0), 
	_magMax(0) {
		Changed(vector);
}

cv::Point3f MaxNormalizedVector::CalculateValue() {
	if (_vector == NULL) {
		_max = 0;
		_magMax = 0;
		return _value;
	}
	float x = _vector->X();
	float y = _vector->Y();
	float z = _vector->Z();

	_max = max(_max, max(x, max(y, z)));

	if (_max == 0)
		return cv::Point3f(0, 0, 0);
	else
		return cv::Point3f(x / _max, y / _max, z / _max);
}

float MaxNormalizedVector::Magnitude() {
	if (_vector == NULL)
		return 0;
	float mag = _vector->Magnitude();
	_magMax = max(_magMax, mag);
	return _magMax != 0 ? mag / _magMax : 0;
}

//------------------------ PartialVector -------------------------

PartialVector::PartialVector() : 
VectorWrappingVector(GetTypeName()) {
	_use[0] = false;
	_use[1] = false;
	_use[2] = false;
}

void PartialVector::UseX(bool use) {
	_use[0] = use;
}
void PartialVector::UseY(bool use) {
	_use[1] = use;
}
void PartialVector::UseZ(bool use) {
	_use[2] = use;
}

cv::Point3f PartialVector::CalculateValue() {
	if (_vector == NULL)
		return _value;
	return cv::Point3f( _use[0] ? _vector->X() : 0,
		_use[1] ? _vector->Y() : 0,
		_use[2] ? _vector->Z() : 0);
}

//------------------------ Value Normalized Vector -------------------------

ValueNormalizedVector::ValueNormalizedVector() : 
TwoVectorWrappingVector(GetTypeName()) { }

ValueNormalizedVector::ValueNormalizedVector(IVector *vector, IVector *max) : 
TwoVectorWrappingVector(GetTypeName(), vector, max) { }

cv::Point3f ValueNormalizedVector::CalculateValue() {
	if (_vector1 == NULL || _vector2 == NULL)
		return _value;
	return cv::Point3f(_vector2->X() != 0 ? _vector1->X() / _vector2->X() : 0, 
		_vector2->Y() != 0 ? _vector1->Y() / _vector2->Y() : 0, 
		_vector2->Z() != 0 ? _vector1->Z() / _vector2->Z() : 0);
}

//------------------------ Normalized Vector -------------------------

NormalizedVector::NormalizedVector() : 
VectorWrappingVector(GetTypeName()) { }

NormalizedVector::NormalizedVector(IVector *vector) : 
VectorWrappingVector(GetTypeName(), vector) { }

cv::Point3f NormalizedVector::CalculateValue() {
	if (_vector == NULL)
		return _value;
	float mag = _vector->Magnitude();
	return cv::Point3f(mag != 0 ? _vector->X() / mag : 0, 
		mag != 0 ? _vector->Y() / mag : 0, 
		mag != 0 ? _vector->Z() / mag : 0);
}

//------------------------ Delta Vector -------------------------

DeltaVector::DeltaVector() : 
VectorWrappingVector(GetTypeName()) ,
	_set(false),
	_oldX(0),
	_oldY(0),
	_oldZ(0) { }

cv::Point3f DeltaVector::CalculateValue() {
	if (_vector == NULL)
		return _value;
	if (!_set) {
		_set = true;
		return _value;
	}
	float x = _vector->X();
	float y = _vector->Y();
	float z = _vector->Z();
	cv::Point3f ret = cv::Point3f(_oldX - x, _oldY - y, _oldZ - z);
	_oldX = x;
	_oldY = y;
	_oldZ = z;
	return ret;
}


//------------------------ SnapshotVector -------------------------

SnapshotVector::SnapshotVector() : 
IVector(GetTypeName()) ,
	_condition(NULL),
	_vector(NULL) { }


void SnapshotVector::SetCondition(ICondition *condition) {
	AddAsListener(_condition, condition);
	_condition = condition;
}

void SnapshotVector::SetVector(IVector *vector) {
	_vector = vector;
}

cv::Point3f SnapshotVector::CalculateValue() {
	if (_vector == NULL || _condition == NULL || !**_condition)
		return _value;
	return cv::Point3f(_vector->X(), _vector->Y(), _vector->Z());
}

//------------------------ MomentumVector -------------------------

MomentumVector::MomentumVector() : 
IVector(GetTypeName()),
	_condition(NULL),
	_vector(NULL),
	_delta(0.f, 0.f, 0.f),
	_decrement(.3f),
	_threshold(.1f) { }

inline void MomentumVector::SetCondition(ICondition *condition) { _condition = condition; }
inline void MomentumVector::SetVector(IVector *vector) { _vector = vector; }
inline void MomentumVector::SetDecrement(float decrement) { _decrement = decrement; }
inline void MomentumVector::SetThreshold(float threshold) { _threshold = threshold; }

cv::Point3f MomentumVector::CalculateValue() {
	if (_vector == NULL || _condition == NULL)
		return cv::Point3f();
	if (**_condition) {
		_delta.x = _vector->X() - _value.x;
		_delta.y = _vector->Y() - _value.y;
		_delta.z = _vector->Z() - _value.z;
		return cv::Point3f(_vector->X(), _vector->Y(), _vector->Z());
	} else {
		float magO = sqrt(pow(_delta.x, 2) + pow(_delta.y, 2) + pow(_delta.z, 2));
		float magN = magO - *_decrement;
		if (magN > *_threshold) {
			_delta.x = (_delta.x / magO) * magN;
			_delta.y = (_delta.y / magO) * magN;
			_delta.z = (_delta.z / magO) * magN;
			return cv::Point3f(_value.x + _delta.x, _value.y + _delta.y, _value.z + _delta.z);
		} else {
			_delta = cv::Point3f(0, 0, 0);
			return cv::Point3f(_vector->X(), _vector->Y(), _vector->Z());
		}
		
	}
}

//------------------------ MagnitudeScaledVector -------------------------

MagnitudeScaledVector::MagnitudeScaledVector() : 
VectorWrappingVector(GetTypeName()), _scale(0.0f) { }

MagnitudeScaledVector::MagnitudeScaledVector(IVector *vector1, IScalar *scale) : 
VectorWrappingVector(GetTypeName(), vector1),
	_scale(scale) { }

MagnitudeScaledVector::MagnitudeScaledVector(IVector *vector1, float scale) : 
VectorWrappingVector(GetTypeName(), vector1),
	_scale(scale) { }

void MagnitudeScaledVector::SetScale(IScalar *scale) { _scale.Set(scale); }
void MagnitudeScaledVector::SetScale(float scale) { _scale.Set(scale); }

cv::Point3f MagnitudeScaledVector::CalculateValue() {
	if (!_vector) 
		return _value;
	float mag = _vector->Magnitude();
	float scale = *_scale;
	return cv::Point3f((_vector->X() / mag) * scale,
		(_vector->Y() / mag) * scale,
		(_vector->Z() / mag) * scale);
}

//------------------------ PlaneIntersectionVector -------------------------

PlaneIntersectionVector::PlaneIntersectionVector() : 
FourVectorWrappingVector(GetTypeName()), _output(NULL) { }

PlaneIntersectionVector::PlaneIntersectionVector(IVector *pPlane, IVector *planeNormal, IVector *pLine, IVector *lineDir) : 
FourVectorWrappingVector(GetTypeName(), pPlane, planeNormal, pLine, lineDir), _output(NULL) { }

cv::Point3f PlaneIntersectionVector::CalculateValue() {
	if (!_vector1 || !_vector2 || !_vector3 || !_vector4)  
		return _value;
	if (!_output)
		Setup();

	if (**_denominator != 0)
		return cv::Point3f(_output->X(), _output->Y(), _output->Z());
	else
		return _value;
}

void PlaneIntersectionVector::Setup() {
	_numerator = dotP(difference(_vector1, _vector3), _vector2);
	_denominator = dotP(_vector4, _vector2);
	_output = sum(_vector3, scaleP(_vector4, quotient(_numerator, _denominator)));
}



//------------------------ ScalarVector -------------------------

ScalarVector::ScalarVector() : 
IVector(GetTypeName())  { }

void ScalarVector::SetX(IScalar *value) { _scalarX.Set(value); }
void ScalarVector::SetX(float value) { _scalarX.Set(value); }

void ScalarVector::SetY(IScalar *value) { _scalarY.Set(value); }
void ScalarVector::SetY(float value) { _scalarY.Set(value); }

void ScalarVector::SetZ(IScalar *value) { _scalarZ.Set(value); }
void ScalarVector::SetZ(float value) { _scalarZ.Set(value); }

cv::Point3f ScalarVector::CalculateValue() {
	return cv::Point3f(*_scalarX, *_scalarY, *_scalarZ);
}
