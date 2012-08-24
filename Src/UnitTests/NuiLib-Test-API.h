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
#include <NuiLib-API.h>
#include <NuiLib-Extendable.h>
#include <UnitTest++.h>

using namespace NuiLib;

class Listener : public IObserver {
	public: 
		Listener(IObservable *);
		bool triggered;
		void Changed(IObservable *);
};

template<typename RawT, typename ComponentT> 
void TestComponent (
	const function<void()> change, function<void (RawT, ComponentT)> compare, 
	RawT start, 
	RawT changed, 
	const ComponentT component, 
	bool factoryUpdated) {

		Listener l(component._p);
		compare(start, component);
		change();
		if (factoryUpdated)
			NuiFactory()->Poll();
		CHECK(l.triggered);
		compare(changed, component);
}

template<typename RawT, typename ComponentT, typename InputT, typename InputComponentT>
void TestOperator(
	const function<void (RawT, ComponentT)> Check, 
	const function<void (InputComponentT, InputT)> set, 
	const function<RawT (InputT, InputT)> result, 
	ComponentT(*op)(const InputComponentT&, const InputComponentT&), 
	ComponentT(*opSF)(const InputComponentT&, InputT), 
	ComponentT(*opFS)(InputT, const InputComponentT&), 
	InputT start1, 
	InputT start2, 
	InputT change1, 
	InputT change2) { 

		InputComponentT s1 = start1;
		InputComponentT s2 = start2;
		ComponentT valSS = op(s1, s2);
		TestComponent<RawT, ComponentT>([&]() { set(s1, change1); set(s2, change2); }, Check, result(start1, start2), result(change1, change2), valSS, true);

		s1 = start1;
		ComponentT valSF = opSF(s1, start2);
		TestComponent<RawT, ComponentT>([&]() { set(s1, change1); }, Check, result(start1, start2), result(change1, start2), valSF, true);

		s2 = start2;
		ComponentT valFS = opFS(start1, s2);
		TestComponent<RawT, ComponentT>([&]() { set(s2, change2); }, Check, result(start1, start2), result(start1, change2), valFS, true);
}

template<typename RawT, typename ComponentT, typename InputT, typename InputComponentT>
void TestEqualsOperator(
	const function<void (RawT, ComponentT)> Check, 
	const function<void (InputComponentT, InputT)> set, 
	const function<RawT (InputT, InputT)> result, 
	ComponentT(*op)(InputComponentT&, const InputComponentT&), 
	ComponentT(*opSF)(InputComponentT&, InputT), 
	InputT start1, 
	InputT start2, 
	InputT change1, 
	InputT change2) { 

	InputComponentT s1 = start1;
	InputComponentT s2 = start2;
	ComponentT valSS = s1;
	op(valSS, s2);
	TestComponent<RawT, ComponentT>([&]() { set(s1, change1); set(s2, change2); }, Check, result(start1, start2), result(change1, change2), valSS, true);

	s1 = start1;
	ComponentT valSF = s1;
	opSF(valSF, start2);
	TestComponent<RawT, ComponentT>([&]() { set(s1, change1); }, Check, result(start1, start2), result(change1, start2), valSF, true);
}
