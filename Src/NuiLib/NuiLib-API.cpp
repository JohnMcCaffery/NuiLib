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
#include <iostream> //TODO remove
#include <NuiLib-API.h>
#include <NuiLib-Extendable.h>

using namespace NuiLib;


Scalar::Scalar() : _p(NULL) { }
Scalar::Scalar(IScalar *p) : _p(p) {}
Scalar::Scalar(float value)  {
	char name[50];
	SPRINTF(name, 50, "%.3f", value);
	IScalar * scalar = ExtensionFactory()->Make<IScalar>(string(name));
	scalar->Set(value);
	_p = scalar;
}
Scalar::Scalar(string name, float value)  {
	IScalar * scalar = ExtensionFactory()->Make<IScalar>(name);
	scalar->Set(value);
	_p = scalar;
}

//void Scalar::AddListener(function<void(IObservable*)> listener) {
	//_p->AddListener(listener);
//}

float Scalar::operator*() { return Get(); }
float Scalar::Get() { return _p ? **_p : 0.f; }

Condition::Condition() : _p(NULL) {}
Condition::Condition(ICondition *p) : _p(p) {}
Condition::Condition(bool value)  {
	ICondition * condition = ExtensionFactory()->Make<ICondition>(value ? "true" : "false");
	condition->Set(value);
	_p = condition;
}
Condition::Condition(string name, bool value)  {
	ICondition * condition = ExtensionFactory()->Make<ICondition>(name);
	condition->Set(value);
	_p = condition;
}
void Condition::AddListener(function<void(IObservable*)> listener) {
	_p->AddListener(listener);
}
void Condition::OnTrue(function<void(IObservable*)> listener) {
	_p->OnTrue(listener);
}
void Condition::OnFalse(function<void(IObservable*)> listener) {
	_p->OnFalse(listener);
}
bool Condition::operator*() { return Get(); }
bool Condition::Get() { return _p ? **_p : false; }

Vector::Vector() : _p(NULL) {
	char name[50];
	SPRINTF(name, 50, "0,0,0");
	IVector * vector = ExtensionFactory()->Make<IVector>(string(name));
	vector->Set(0.f, 0.f, 0.f);
	_p = vector;
}
Vector::Vector(IVector *p) : _p(p) {}
Vector::Vector(float value)  {
	char name[50];
	SPRINTF(name, 50, "%.3f,%.3f,%.3f", value, value, value);
	IVector * vector = ExtensionFactory()->Make<IVector>(string(name));
	vector->Set(value, value, value);
	_p = vector;
}
Vector::Vector(cv::Point3f value)  {
	char name[50];
	SPRINTF(name, 50, "%.3f,%.3f,%.3f", value.x, value.y, value.z);
	IVector * vector = ExtensionFactory()->Make<IVector>(string(name));
	vector->Set(value.x, value.y, value.z);
	_p = vector;
}
Vector::Vector(string name, cv::Point3f value) {
	IVector * vector = ExtensionFactory()->Make<IVector>(name);
	vector->Set(value.x, value.y, value.z);
	_p = vector;
}
Vector::Vector(string name, float value) {
	IVector * vector = ExtensionFactory()->Make<IVector>(name);
	vector->Set(value, value, value);
	_p = vector;
}

float Vector::X() { return _p ? (*_p)[X_AXIS] : 0.f; }
float Vector::Y() { return _p ? (*_p)[Y_AXIS] : 0.f; }
float Vector::Z() { return _p ? (*_p)[Z_AXIS] : 0.f; }

const float Vector::operator[](AXIS axis) { return _p ? (*_p)[axis] : 0.f; }
void Vector::AddListener(function<void(IObservable*)> listener) {
	_p->AddListener(listener);
}

void Vector::Set(cv::Point3f value) { _p->Set(value.x, value.y, value.z); }
void Vector::Set(float x, float y, float z) { _p->Set(x, y, z); }

//-------------------------------------------- IObserver ----------------------------

IObserver::IObserver() :
       	_singleObservable(NULL) { 
}

IObserver::IObserver(IObservable *singleObservable) : 
	_singleObservable(singleObservable) { 
	singleObservable->AddListener(this);
}

IObserver::~IObserver() {
	StopObserving();
}

void IObserver::AddAsListener(IObservable *previous, IObservable *target) {
	if (_singleObservable)
		return;
	if (previous)
		RemoveAsListener(previous);
	if (target) {
		target->AddListener(this);
		_observed.push_back(target);
	}
}

void IObserver::RemoveAsListener(IObservable *root) {
	if (root) {
		root->RemoveListener(this);
		list<IObservable*>::iterator position = _observed.end();
		for (auto i = _observed.begin(); i != _observed.end(); i++)
			if (root == *i)
				position = i;
		if (position != _observed.end())
			_observed.erase(position);
	}
}

void IObserver::SetFactoryObserver(bool value) {
	if (value) {
		StopObserving();
		_singleObservable = NuiFactory();
		NuiFactory()->AddListener(this);
	}
}

void IObserver::StopObserving() {
	if (_singleObservable)
		_singleObservable->RemoveListener(this);
	for (auto t = _observed.begin(); t != _observed.end(); t++)
		(*t)->RemoveListener(this);
}

//-------------------------------------------- IObservable ---------------------------------------

IObservable::~IObservable() {
	set<IObserver*> toRemove = set<IObserver*>(_observers);

	for (auto t = toRemove.begin(); t != toRemove.end(); t++) 
		(*t)->RemoveAsListener(this);

	_observers.clear();
}

void IObservable::Trigger() {
	for (auto t = _observers.begin(); t != _observers.end(); t++) 
		(*t)->Changed(this);
	for (auto t = _listeners.begin(); t != _listeners.end(); t++) 
		(*t)(this);
}

void IObservable::AddListener(IObserver* observer){
	_observers.insert(observer);
}

void IObservable::RemoveListener(IObserver* observer) {
	set<IObserver*>::iterator position = _observers.end();
	for (auto i = _observers.begin(); i != _observers.end(); i++)
		if (observer == *i)
			position = i;
	if (position != _observers.end())
		_observers.erase(position);
}

void IObservable::AddListener(function<void(IObservable*)> listener) {
	_listeners.push_back(listener);
}

//-------------------------------------------- IComponent ---------------------------------------

IComponent::IComponent(string type, string name) : 
	_type(type), 
	_name(name) { }

IComponent::~IComponent() {
	//cout << _name;
	//cout << " destroyed\n";
	ExtensionFactory()->UnregisterCreated(this);
}

string IComponent::GetType() { return _type; }

string IComponent::GetName() { return _name; }
const char *IComponent::GetCName() { return _name.c_str(); }
void IComponent::SetName(string name) { _name = name; }

//----------------------------------- NuiFactory --------------------------

INuiFactory::~INuiFactory() { }
