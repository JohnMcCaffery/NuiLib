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
#include <NuiLib-Conditions.h>

using namespace NuiLib;


FistCondition *NuiLib::fistP(bool rightHand) {
	FistCondition *fist = ExtensionFactory()->Make<FistCondition>(rightHand ? "FistRight" : "FistLeft");
	fist->SetHand(GetHandProcessor(rightHand));
	fist->Changed(ExtensionFactory());
	return fist;
}
ClickCondition *NuiLib::clickP(ICondition *condition) {
	ClickCondition *click = ExtensionFactory()->Make<ClickCondition>("Click(" + condition->GetName() + ")");
	click->SetCondition(condition);
	click->Changed(condition);
	return click;
}
HoldCondition *NuiLib::holdP(ICondition *condition) {
	HoldCondition *hold = ExtensionFactory()->Make<HoldCondition>("Hold(" + condition->GetName() + ")");
	hold->SetCondition(condition);
	hold->Changed(condition);
	return hold;
}
CloseGuard *NuiLib::closeGuardP() {
	CloseGuard *guard = ExtensionFactory()->Make<CloseGuard>("CloseGuard");
	return guard;
}
CloseGuard *NuiLib::closeGuardP(float scale) {
	CloseGuard *guard = ExtensionFactory()->Make<CloseGuard>("CloseGuard");
	guard->SetScale(scale);
	return guard;
}
CloseGuard *NuiLib::closeGuardP(IScalar *scale) {
	CloseGuard *guard = ExtensionFactory()->Make<CloseGuard>("CloseGuard");
	guard->SetScale(scale);
	return guard;
}

Condition NuiLib::fist(bool rightHand) {
	return Condition(fistP(rightHand));
}
Condition NuiLib::click(const Condition &condition) {
	return Condition(clickP(condition._p));
}
Condition NuiLib::hold(const Condition &condition) {
	return Condition(holdP(condition._p));
}
Condition NuiLib::closeGuard() {
	return Condition(closeGuardP());
}
Condition NuiLib::closeGuard(float scale) {
	return Condition(closeGuardP(scale));
}
Condition NuiLib::closeGuard(Scalar scale) {
	return Condition(closeGuardP(scale._p));
}

//-------------------------------------------- ICondition ---------------------------------------

ICondition::ICondition() : IComponent("ICondition", "AnonymousCondition"), _value(false) { }
ICondition::ICondition(string type) : IComponent(type, "AnonymousCondition"), _value(false) { }
//ICondition::ICondition(string type, string name) : IComponent(type, name), _value(false) { }
ICondition::ICondition(bool value) : IComponent(GetTypeName(), "AnonymousCondition"), _value(value) { }
//ICondition::ICondition(string type, bool value) : IComponent(GetTypeName(), "AnonymousCondition"), _value(value) { }
//ICondition::ICondition(string type, string name, bool value) : IComponent(GetTypeName(), name), _value(value) { }

inline bool ICondition::Get() { return _value; }
void ICondition::Set(bool value) { 
	_value = value; 
	Trigger();
}

string ICondition::ToString() {
	return string(_value ? "True" : "False");
}
string ICondition::ToString(string format) {
	char buffer[500];
	const char *str = _value ? "True" : "False";
	SPRINTF(buffer, 500, format.c_str(), str);
	return string(buffer);
}
string ICondition::ToString(int width) {
	char buffer[50];
	if (width < 5)
		SPRINTF(buffer, 50, "%-*s", width, _value ? "T" : "F");
	else
		SPRINTF(buffer, 50, "%-*s", width, _value ? "True" : "False");
	return string(buffer);
}
void ICondition::OnTrue(function<void(IObservable*)> listener) {
	_trueListeners.push_back(listener);
}
void ICondition::OnFalse(function<void(IObservable*)> listener) {
	_falseListeners.push_back(listener);
}

void ICondition::Changed(IObservable *source) {
	bool oldVal = _value;
	_value = CalculateValue();
	if (oldVal != _value) {
		Trigger();
		if (_value)
			for(auto i = _trueListeners.begin(); i != _trueListeners.end(); i++)
				(*i)(this);
		if (!_value)
			for(auto i = _falseListeners.begin(); i != _falseListeners.end(); i++)
				(*i)(this);
	}
}

inline bool ICondition::operator*() { return _value; }

string ICondition::ToXML() { return ""; }
void ICondition::LoadXML(string xml) { }

DLL ICondition *NuiLib::pGreater(IScalar *operand1, IScalar *operand2) {
	return CreateOperator<GreaterCondition, IScalar*, IScalar*>(operand1, operand2);
}
DLL ICondition *NuiLib::pLess(IScalar *operand1, IScalar *operand2) {
	return CreateOperator<LessCondition, IScalar*, IScalar*>(operand1, operand2);
}
DLL ICondition *NuiLib::pGreaterEqual(IScalar *operand1, IScalar *operand2) {
	return CreateOperator<GreaterEqualCondition, IScalar*, IScalar*>(operand1, operand2);
}
DLL ICondition *NuiLib::pLessEqual(IScalar *operand1, IScalar *operand2) {
	return CreateOperator<LessEqualCondition, IScalar*, IScalar*>(operand1, operand2);
}
DLL ICondition *NuiLib::pEqualTo(IScalar *operand1, IScalar *operand2) {
	return CreateOperator<EqualCondition, IScalar*, IScalar*>(operand1, operand2);
}
DLL ICondition *NuiLib::pNotEqualTo(IScalar *operand1, IScalar *operand2) {
	return CreateOperator<NotEqualCondition, IScalar*, IScalar*>(operand1, operand2);
}
DLL ICondition *NuiLib::pNotEqualTo(IVector *operand1, IVector *operand2) {
	return CreateOperator<NotEqualVectorCondition, IVector*, IVector*>(operand1, operand2);
}
DLL ICondition *NuiLib::pAnd(ICondition *operand1, ICondition *operand2) {
	return CreateOperator<AndCondition, ICondition*, ICondition*>(operand1, operand2);
}
DLL ICondition *NuiLib::pOr(ICondition *operand1, ICondition *operand2) {
	return CreateOperator<OrCondition, ICondition*, ICondition*>(operand1, operand2);
}
DLL ICondition *NuiLib::pNot(ICondition *operand) {
	char *buffer = new char[999];
	SPRINTF(buffer, 999, "!%s", operand->GetName().c_str());
	NotCondition *ret = ExtensionFactory()->Make<NotCondition>(string(buffer));
	ret->SetCondition(operand);
	return ret;
}

DLL ICondition *NuiLib::pGreater(IScalar *operand1, float operand2) {
	return CreateOperator<GreaterCondition, IScalar*, float>(operand1, operand2);
}
DLL ICondition *NuiLib::pLess(IScalar *operand1, float operand2) {
	return CreateOperator<LessCondition, IScalar*, float>(operand1, operand2);
}
DLL ICondition *NuiLib::pGreaterEqual(IScalar *operand1, float operand2) {
	return CreateOperator<GreaterEqualCondition, IScalar*, float>(operand1, operand2);
}
DLL ICondition *NuiLib::pLessEqual(IScalar *operand1, float operand2) {
	return CreateOperator<LessEqualCondition, IScalar*, float>(operand1, operand2);
}
DLL ICondition *NuiLib::pEqualTo(IScalar *operand1, float operand2) {
	return CreateOperator<EqualCondition, IScalar*, float>(operand1, operand2);
}
DLL ICondition *NuiLib::pNotEqualTo(IScalar *operand1, float operand2) {
	return CreateOperator<NotEqualCondition, IScalar*, float>(operand1, operand2);
}
DLL ICondition *NuiLib::pNotEqualTo(IVector *operand1, float operand2) {
	return CreateOperator<NotEqualVectorCondition, IVector*, float>(operand1, operand2);
}
DLL ICondition *NuiLib::pNotEqualTo(IVector *operand1, cv::Point3f operand2) {
	return CreateOperator<NotEqualVectorCondition, IVector*, cv::Point3f>(operand1, operand2);
}
DLL ICondition *NuiLib::pAnd(ICondition *operand1, bool operand2) {
	return CreateOperator<AndCondition, ICondition*, bool>(operand1, operand2);
}
DLL ICondition *NuiLib::pOr(ICondition *operand1, bool operand2) {
	return CreateOperator<OrCondition, ICondition*, bool>(operand1, operand2);
}



DLL ICondition *NuiLib::pGreater(float operand1, IScalar *operand2) {
	return CreateOperator<GreaterCondition, float, IScalar*>(operand1, operand2);
}
DLL ICondition *NuiLib::pLess(float operand1, IScalar *operand2) {
	return CreateOperator<LessCondition, float, IScalar*>(operand1, operand2);
}
DLL ICondition *NuiLib::pGreaterEqual(float operand1, IScalar *operand2) {
	return CreateOperator<GreaterEqualCondition, float, IScalar*>(operand1, operand2);
}
DLL ICondition *NuiLib::pLessEqual(float operand1, IScalar *operand2) {
	return CreateOperator<LessEqualCondition, float, IScalar*>(operand1, operand2);
}
DLL ICondition *NuiLib::pEqualTo(float operand1, IScalar *operand2) {
	return CreateOperator<EqualCondition, float, IScalar*>(operand1, operand2);
}
DLL ICondition *NuiLib::pNotEqualTo(float operand1, IScalar *operand2) {
	return CreateOperator<NotEqualCondition, float, IScalar*>(operand1, operand2);
}
DLL ICondition *NuiLib::pNotEqualTo(float operand1, IVector *operand2) {
	return CreateOperator<NotEqualVectorCondition, float, IVector*>(operand1, operand2);
}
DLL ICondition *NuiLib::pNotEqualTo(cv::Point3f operand1, IVector *operand2) {
	return CreateOperator<NotEqualVectorCondition, cv::Point3f, IVector*>(operand1, operand2);
}
DLL ICondition *NuiLib::pAnd(bool operand1, ICondition *operand2) {
	return CreateOperator<AndCondition, bool, ICondition*>(operand1, operand2);
}
DLL ICondition *NuiLib::pOr(bool operand1, ICondition *operand2) {
	return CreateOperator<OrCondition, bool, ICondition*>(operand1, operand2);
}












DLL Condition NuiLib::operator>(const Scalar &operand1, const Scalar &operand2) {
	return Condition(pGreater(operand1._p, operand2._p));
}
DLL Condition NuiLib::operator<(const Scalar &operand1, const Scalar &operand2) {
	return Condition(pLess(operand1._p, operand2._p));
}
DLL Condition NuiLib::operator>=(const Scalar &operand1, const Scalar& operand2) {
	return Condition(pGreaterEqual(operand1._p, operand2._p));
}
DLL Condition NuiLib::operator<=(const Scalar &operand1, const Scalar &operand2) {
	return Condition(pLessEqual(operand1._p, operand2._p));
}
DLL Condition NuiLib::operator==(const Scalar &operand1, const Scalar &operand2) {
	return Condition(pEqualTo(operand1._p, operand2._p));
}
DLL Condition NuiLib::operator!=(const Scalar &operand1, const Scalar &operand2) {
	return Condition(pNotEqualTo(operand1._p, operand2._p));
}
DLL Condition NuiLib::operator!=(const Vector &operand1, const Vector &operand2) {
	return Condition(pNotEqualTo(operand1._p, operand2._p));
}
DLL Condition NuiLib::operator&&(const Condition &operand1, const Condition & operand2) {
	return Condition(pAnd(operand1._p, operand2._p));
}
DLL Condition NuiLib::operator||(const Condition &operand1, const Condition &operand2) {
	return Condition(pOr(operand1._p, operand2._p));
}
DLL Condition NuiLib::operator!(const Condition &operand) {
	return Condition(pNot(operand._p));
}

DLL Condition NuiLib::operator>(const Scalar &operand1, float operand2) {
	return Condition(pGreater(operand1._p, operand2));
}
DLL Condition NuiLib::operator<(const Scalar &operand1, float operand2) {
	return Condition(pLess(operand1._p, operand2));
}
DLL Condition NuiLib::operator>=(const Scalar &operand1, float operand2) {
	return Condition(pGreaterEqual(operand1._p, operand2));
}
DLL Condition NuiLib::operator<=(const Scalar &operand1, float operand2) {
	return Condition(pLessEqual(operand1._p, operand2));
}
DLL Condition NuiLib::operator==(const Scalar &operand1, float operand2) {
	return Condition(pEqualTo(operand1._p, operand2));
}
DLL Condition NuiLib::operator!=(const Scalar &operand1, float operand2) {
	return Condition(pNotEqualTo(operand1._p, operand2));
}
DLL Condition NuiLib::operator!=(const Vector &operand1, float operand2) {
	return Condition(pNotEqualTo(operand1._p, operand2));
}
DLL Condition NuiLib::operator!=(const Vector &operand1, cv::Point3f operand2) {
	return Condition(pNotEqualTo(operand1._p, operand2));
}
DLL Condition NuiLib::operator&&(const Condition &operand1, bool operand2) {
	return Condition(pAnd(operand1._p, operand2));
}
DLL Condition NuiLib::operator||(const Condition &operand1, bool operand2) {
	return Condition(pOr(operand1._p, operand2));
}

DLL Condition NuiLib::operator>(float operand1, const Scalar &operand2) {
	return Condition(pGreater(operand1, operand2._p));
}
DLL Condition NuiLib::operator<(float operand1, const Scalar &operand2) {
	return Condition(pLess(operand1, operand2._p));
}
DLL Condition NuiLib::operator>=(float operand1, const Scalar& operand2) {
	return Condition(pGreaterEqual(operand1, operand2._p));
}
DLL Condition NuiLib::operator<=(float operand1, const Scalar &operand2) {
	return Condition(pLessEqual(operand1, operand2._p));
}
DLL Condition NuiLib::operator==(float operand1, const Scalar &operand2) {
	return Condition(pEqualTo(operand1, operand2._p));
}
DLL Condition NuiLib::operator!=(float operand1, const Scalar &operand2) {
	return Condition(pNotEqualTo(operand1, operand2._p));
}
DLL Condition NuiLib::operator!=(float operand1, const Vector &operand2) {
	return Condition(pNotEqualTo(operand1, operand2._p));
}
DLL Condition NuiLib::operator!=(cv::Point3f operand1, const Vector &operand2) {
	return Condition(pNotEqualTo(operand1, operand2._p));
}
DLL Condition NuiLib::operator&&(bool operand1, const Condition & operand2) {
	return Condition(pAnd(operand1, operand2._p));
}
DLL Condition NuiLib::operator||(bool operand1, const Condition &operand2) {
	return Condition(pOr(operand1, operand2._p));
}

//-------------------------------------------- ComparisonCondition --------------------------------------

inline bool ComparisonCondition::Value() {
	return BinaryOperator::CalculateValue<float, bool>(GetOperand1(), GetOperand2());
}

ComparisonCondition::ComparisonCondition(Operator op) : 
ICondition(GetTypeName()),
BinaryOperator<IScalar, float, float>(op, "%.3f", 0.f, 0.f, Converter(), Printer()) { }
ComparisonCondition::~ComparisonCondition() { }

bool ComparisonCondition::CalculateValue() {
	return Value();
}

//-------------------------------------------- NotEqualVectorCondition --------------------------------------

///
/// Calculate a vector value from the inputs.
/// WARNING: The returned value is allocated on the stack. It WILL go out of scope so only use it to update other values. Don't try and store the array.
///
inline bool NotEqualVectorCondition::Value() {
	cv::Point3f op1 = GetOperand1();
	cv::Point3f op2 = GetOperand2();
	return op1.x == op2.x && op1.y == op2.y && op1.z == op2.z;
}

NotEqualVectorCondition::NotEqualVectorCondition() : 
ICondition(GetTypeName()),
	BinaryOperator<IVector, float, cv::Point3f>(NOT_EQUAL, "%.3f %.3f %.3f", cv::Point3f(), cv::Point3f(), [](float in) { 
		return cv::Point3f(in);
	}, [](const char *format, cv::Point3f p) -> char* {
	char *buff = new char[50];
	SPRINTF(buff, 50, "%.3f %.3f %.3f", p.x, p.y, p.z);
	return buff;
}) { }
NotEqualVectorCondition::~NotEqualVectorCondition() { }

bool NotEqualVectorCondition::CalculateValue() {
	return Value();
}

void NotEqualVectorCondition::SetOperand1(float value) {
	BinaryOperator<IVector, float, cv::Point3f>::SetOperand1(cv::Point3f(value));
}
void NotEqualVectorCondition::SetOperand2(float value) {
	BinaryOperator<IVector, float, cv::Point3f>::SetOperand1(cv::Point3f(value));
}

//-------------------------------------------- LogicCondition --------------------------------------

inline bool LogicCondition::Value() {
	return BinaryOperator::CalculateValue<bool, bool>(GetOperand1(), GetOperand2());
}

LogicCondition::LogicCondition(Operator op) : 
ICondition(GetTypeName()),
BinaryOperator<ICondition, bool, bool>(op, "%i", false, false, Converter(), Printer()) { }
LogicCondition::~LogicCondition() { }

bool LogicCondition::CalculateValue() {
	return Value();
}

//-------------------------------------------- FistCondition -------------------------------

FistCondition::FistCondition() : ICondition(GetTypeName()), _threshold(1.f), _hand(NULL) { }
void FistCondition::SetHand(HandProcessor *hand) { 
	AddAsListener(_hand, hand);
	_hand = hand; 
}

bool FistCondition::CalculateValue() {
	if (_hand) 
		return _hand->WasFound() && _hand->GetHandRatio() >= _threshold;
	return _value;
}

//-------------------------------------------- ClickCondition -------------------------------

ClickCondition::ClickCondition() : ICondition(GetTypeName()), _condition(NULL), _threshold(-1), _set(false) { }
bool ClickCondition::GetValue() {
	bool ret = ICondition::Get();
	return false;
	_set = false;
	return ret;
}
void ClickCondition::SetCondition(ICondition *condition) { 
	AddAsListener(_condition, condition);
	_condition = condition;
}
bool ClickCondition::CalculateValue() {
	if (**_condition && !_set) {
		time(&_press);
		_set = true;
		return _value;
	} else {
		if (_threshold > 0) {
			time_t now;
			time(&now);
			double held = difftime(now, _press);
			return held < _threshold;
		} else 
			return true;
	}
}

//-------------------------------------------- HoldCondition -------------------------------

HoldCondition::HoldCondition() : ICondition(GetTypeName()), _condition(NULL), _threshold(2.f), _set(false) { }

void HoldCondition::SetCondition(ICondition *condition) { 
	AddAsListener(_condition, condition);
	_condition = condition;
}
bool HoldCondition::CalculateValue() {
	if (!**_condition)
		return false;
	else if (!_set) {
		time(&_press);
		_set = true;
		return _value;
	} else {
		time_t now;
		time(&now);
		return difftime(now, _press) > _threshold;
	}
}


	//----------------------- CloseGuard ---------------------

CloseGuard::CloseGuard() : 
ICondition(GetTypeName()), 
	_scale(2000.f), 
	_head(joint(NuiLib::HEAD)), 
	_handR(joint(NuiLib::HAND_RIGHT)), 
	_handL(joint(NuiLib::HAND_LEFT)), 
	_footR(joint(NuiLib::FOOT_RIGHT)), 
	_footL(joint(NuiLib::FOOT_LEFT)) { }

CloseGuard::CloseGuard(float val) : 
ICondition(GetTypeName()), 
	_scale(val),
	_head(joint(NuiLib::HEAD)), 
	_handR(joint(NuiLib::HAND_RIGHT)), 
	_handL(joint(NuiLib::HAND_LEFT)), 
	_footR(joint(NuiLib::FOOT_RIGHT)), 
	_footL(joint(NuiLib::FOOT_LEFT)){ } 

CloseGuard::CloseGuard(IScalar* val) : 
ICondition(GetTypeName()), 
	_scale(val),
	_head(joint(NuiLib::HEAD)), 
	_handR(joint(NuiLib::HAND_RIGHT)), 
	_handL(joint(NuiLib::HAND_LEFT)), 
	_footR(joint(NuiLib::FOOT_RIGHT)), 
	_footL(joint(NuiLib::FOOT_LEFT)){ }

void CloseGuard::SetScale(float val) { _scale.Set(val); }
void CloseGuard::SetScale(IScalar* val) { _scale.Set(val); }

cv::Point extreme(std::vector<cv::Point> points, bool min, bool x) {
	cv::Point current = points.front();
	for (auto p = points.begin(); p != points.end(); p++) {
		if (x && min && (*p).x < current.x)
			current = *p;
		if (!x && min && (*p).y < current.y)
			current = *p;
		if (x && !min && (*p).x > current.x)
			current = *p;
		if (!x && !min && (*p).y > current.y)
			current = *p;
		return current;
	}
}

bool CloseGuard::CalculateValue() {
	std::vector<cv::Point> allPoints;
	allPoints.push_back(ExtensionFactory()->SkeletonToDepth(_head));
	allPoints.push_back(ExtensionFactory()->SkeletonToDepth(_handR));
	allPoints.push_back(ExtensionFactory()->SkeletonToDepth(_footR));
	allPoints.push_back(ExtensionFactory()->SkeletonToDepth(_footL));
	allPoints.push_back(ExtensionFactory()->SkeletonToDepth(_handL));

	std::vector<cv::Point> points;
	points.push_back(extreme(allPoints, true, true));
	points.push_back(extreme(allPoints, true, false));
	points.push_back(extreme(allPoints, false, true));
	points.push_back(extreme(allPoints, false, false));

	float area = cv::contourArea(points);
	if (area > *_scale)
		return true;
	else {
		if (area > 0.f)
			cv::polylines(ExtensionFactory()->GetDebugFrame(), points, true, cv::Scalar(0, 0, 255), 2);
		return false;
	}
}
