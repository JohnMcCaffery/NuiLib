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
#include "NuiLib-Test-API.h"

float start1 = 1.5f;
float start2 = 3.25f;
float change1 = -.5f;
float change2 = .8f;

using namespace NuiLib;

function<void (float, Scalar)> Checkf() { return [](float raw, Scalar comp) { CHECK_CLOSE(raw, *comp, .001f); }; }

TEST(TestScalarSum) { TestOperator<float, Scalar, float, Scalar>(Checkf(), [](Scalar c, float v) { c._p->Set(v); }, [](float f1, float f2) { return f1 + f2; }, NuiLib::operator+, NuiLib::operator+, NuiLib::operator+, start1, start2, change1, change2); }
TEST(TestScalarDifference) { TestOperator<float, Scalar, float, Scalar>(Checkf(), [](Scalar c, float v) { c._p->Set(v); }, [](float f1, float f2) { return f1 - f2; }, NuiLib::operator-, NuiLib::operator-, NuiLib::operator-, start1, start2, change1, change2); }
TEST(TestScalarProduct) { TestOperator<float, Scalar, float, Scalar>(Checkf(), [](Scalar c, float v) { c._p->Set(v); }, [](float f1, float f2) { return f1 * f2; }, NuiLib::operator*, NuiLib::operator*, NuiLib::operator*, start1, start2, change1, change2); }
TEST(TestScalarQuotient) { TestOperator<float, Scalar, float, Scalar>(Checkf(), [](Scalar c, float v) { c._p->Set(v); }, [](float f1, float f2) { return f1 / f2; }, NuiLib::operator/, NuiLib::operator/, NuiLib::operator/, start1, start2, change1, change2); }

TEST(TestScalarSumEquals) { TestEqualsOperator<float, Scalar, float, Scalar>(Checkf(), [](Scalar c, float v) { c._p->Set(v); }, [](float f1, float f2) { return f1 + f2; }, NuiLib::operator+=, NuiLib::operator+=, start1, start2, change1, change2); }
TEST(TestScalarDifferenceEquals) { TestEqualsOperator<float, Scalar, float, Scalar>(Checkf(), [](Scalar c, float v) { c._p->Set(v); }, [](float f1, float f2) { return f1 - f2; }, NuiLib::operator-=, NuiLib::operator-=, start1, start2, change1, change2); }
TEST(TestScalarProductEquals) { TestEqualsOperator<float, Scalar, float, Scalar>(Checkf(), [](Scalar c, float v) { c._p->Set(v); }, [](float f1, float f2) { return f1 * f2; }, NuiLib::operator*=, NuiLib::operator*=, start1, start2, change1, change2); }
TEST(TestScalarQuotientEquals) { TestEqualsOperator<float, Scalar, float, Scalar>(Checkf(), [](Scalar c, float v) { c._p->Set(v); }, [](float f1, float f2) { return f1 / f2; }, NuiLib::operator/=, NuiLib::operator/=, start1, start2, change1, change2); }

/*
#define CHECK_CALC(c, value, state) \
	CHECK_CLOSE(value, c.GetValue(), .001); \
	CHECK_EQUAL(state, c.GetState());

#define CHECK_CALC_STATE(c, l, value, state, trig) \
	CHECK_CLOSE(value, c.GetValue(), .001); \
	CHECK_EQUAL(state, c.GetState()); \
	CHECK_EQUAL(trig, l.triggered); \
	l.triggered = false;

using namespace std;

class TestScalar : public IScalar {
	public:
		TestScalar() : IScalar("TestScalar") { }
		void Set(float value, bool state) {
			Update(value, state);
		}

		string ToXML() { return ""; }
		void LoadXML(INuiFactory *, string) { }
};

TEST(TestIScalar) {
	TestScalar c = TestScalar();
	CHECK_CALC(c, 0, false);

	Listener l = Listener();
	l.AddAsListener(NULL, &c);

	c.Set(.5, true);
	CHECK_CALC_STATE(c, l, .5, true, true);

	c.Set(1, false);
	CHECK_CALC_STATE(c, l, 1, false, true);




	c.Set(0, true);
	CHECK_EQUAL(0, c.ToString(10).compare("          |            0.000"));
	cout << c.ToString(10) + "\n";

	c.Set(1, true);
	CHECK_EQUAL(0, c.ToString(10).compare("          |----------  1.000"));
	cout << c.ToString(10) + "\n";

	c.Set(-1, true);
	CHECK_EQUAL(0, c.ToString(10).compare("----------|           -1.000"));
	cout << c.ToString(10) + "\n";

	c.Set(.5, true);
	CHECK_EQUAL(0, c.ToString(10).compare("          |-----       0.500"));
	cout << c.ToString(10) + "\n";

	c.Set(-.5, true);
	CHECK_EQUAL(0, c.ToString(10).compare("     -----|           -0.500"));
	cout << c.ToString(10) + "\n";
}

TEST(TestConstrainedScalar) {
	TestScalar c = TestScalar();
	c.Set(.5, true);

	ConstrainedScalar cc = ConstrainedScalar();
	Listener l = Listener();
	l.AddAsListener(NULL, &cc);

	//Not setup - 0
	CHECK_CALC(cc, 0, false);

	//After calculator set - 0
	cc.SetScalar(&c);
	CHECK_CALC_STATE(cc, l, 0, false, false);

	//Range set so value == range - 1
	cc.SetRange(.5);
	CHECK_CALC_STATE(cc, l, 1, true, true);

	//Deadzone set so (value - deadzone) / range == .5 - .5
	cc.SetDeadzone(.25);
	CHECK_CALC_STATE(cc, l, .5, true, true);

	//Value set to deadzone - 0
	c.Set(.25, true);
	CHECK_CALC_STATE(cc, l, 0, false, true);

	//Value set to deadzone + range - 1
	c.Set(.75, true);
	CHECK_CALC_STATE(cc, l, 1, true, true);

	//Value set to deadzone + range + .1 - 0
	c.Set(.85, true);
	CHECK_CALC_STATE(cc, l, 0, false, true);

	//Grace set to .1 so value == deadzone + range + grace - 1
	cc.SetGrace(.99999);
	CHECK_CALC_STATE(cc, l, 1, true, true);

	//Value set so that deadzone + range + (grace / 2) - 1
	c.Set(.8, true);
	CHECK_CALC_STATE(cc, l, 1, true, false);

	//Value set so == deadzone + range + grace - 0
	c.Set(.86, true);
	CHECK_CALC_STATE(cc, l, 0, false, true);


	//----------Inverted-------------

	//Range set so value == -range, no mirror - 0
	cc.SetMirror(false);
	cc.SetDeadzone(0);
	cc.SetRange(.5);
	cc.SetGrace(0);
	c.Set(-.5, true);
	CHECK_CALC_STATE(cc, l, 0, false, false);
	//Range set so value == -range, mirror - 0
	cc.SetMirror(true);
	CHECK_CALC_STATE(cc, l, -1, true, true);

	//Deadzone set so (value - deadzone) / range == .5 no mirror - -.5
	cc.SetMirror(false);
	cc.SetDeadzone(.25);
	CHECK_CALC_STATE(cc, l, 0, false, true);
	//Deadzone set so (value - deadzone) / range == .5 mirror - -.5
	cc.SetMirror(true);
	CHECK_CALC_STATE(cc, l, -.5, true, true);

	//Value set to deadzone - no mirror - 0
	cc.SetMirror(false);
	c.Set(-.25, true);
	CHECK_CALC_STATE(cc, l, 0, false, true);
	//Value set to deadzone - mirror - 0
	cc.SetMirror(true);
	CHECK_CALC_STATE(cc, l, 0, false, false);

	//Value set to deadzone + range - no mirror - 1
	cc.SetMirror(false);
	c.Set(-.75, true);
	CHECK_CALC_STATE(cc, l, 0, false, false);
	//Value set to deadzone + range - mirror - -1
	cc.SetMirror(true);
	CHECK_CALC_STATE(cc, l, -1, true, true);

	//Value set to deadzone + range + .1 - no mirror - 0
	cc.SetMirror(false);
	c.Set(-.85, true);
	CHECK_CALC_STATE(cc, l, 0, false, true);
	//Value set to deadzone + range + .1 - mirror - 0
	cc.SetMirror(false);
	CHECK_CALC_STATE(cc, l, 0, false, false);

	//Grace set to .1 so value == deadzone + range + grace - no mirror - 0
	cc.SetMirror(false);
	cc.SetGrace(.100000001);
	CHECK_CALC_STATE(cc, l, 0, false, false);
	//Grace set to .1 so value == deadzone + range + grace - mirror - 1
	cc.SetMirror(true);
	CHECK_CALC_STATE(cc, l, -1, true, true);

	//Value set so that deadzone + range + (grace / 2) - no mirror - 0
	cc.SetMirror(false);
	c.Set(-.8, true);
	CHECK_CALC_STATE(cc, l, 0, false, true);
	//Value set so that deadzone + range + (grace / 2) - mirror - 1
	cc.SetMirror(true);
	CHECK_CALC_STATE(cc, l, -1, true, true);

	//Value set so == deadzone + range + grace - no mirror - 0
	cc.SetMirror(false);
	c.Set(-.86, true);
	CHECK_CALC_STATE(cc, l, 0, false, true);
	//Value set so == deadzone + range + grace - mirror - 0
	cc.SetMirror(true);
	CHECK_CALC_STATE(cc, l, 0, false, false);



	//----------------- Split ----------------------
	//In deadzone
	cc.SetMirror(false);
	cc.SetDeadzone(-.25);
	cc.SetRange(.5);
	cc.SetGrace(0);
	c.Set(-.25, true);
	CHECK_CALC_STATE(cc, l, 0, false, false);

	//In range, negative
	c.Set(-.15, true);
	CHECK_CALC_STATE(cc, l, .2, true, true);

	//In range, positive
	c.Set(.15, true);
	CHECK_CALC_STATE(cc, l, .8, true, true);

	//Past range
	c.Set(.26, true);
	CHECK_CALC_STATE(cc, l, 0, false, true);
}

/*
TEST(TestDifferenceScalar) {
	Vector dist = Vector(0, 0, 0);
	Vector max = Vector(1, 2, 3);

	ScalarDistanceScalar dc = ScalarDistanceScalar();

	Listener l = Listener();
	dc.AddListener(&l);

	CHECK_CALC_STATE(dc, l, 0, false, false);
	dc.SetDistance(&dist);
	CHECK_CALC_STATE(dc, l, 0, false, false);
	dc.AddMaxComponent(&max);
	CHECK_CALC_STATE(dc, l, 0, true, true);

	dist.SetX(.5);
	dist.SetY(1);
	dist.SetZ(1.5);
	CHECK_CALC_STATE(dc, l, .5, true, true);
	
	dist.SetX(1);
	dist.SetY(2);
	dist.SetZ(3);
	CHECK_CALC_STATE(dc, l, 1, true, true);

	
	max.SetX(0);
	max.SetY(0);
	max.SetZ(0);
	CHECK_CALC_STATE(dc, l, 0, true, true);
}

TEST(TestAngleScalar) {
	Vector vector1 = Vector(1, 0, 0);
	Vector vector2 = Vector(1, 0, 0);

	AngleScalar ac = AngleScalar();

	Listener l = Listener();
	l.AddAsListener(NULL, &ac);

	CHECK_CALC_STATE(ac, l, 0, false, false);
	ac.SetVector1(&vector1);
	CHECK_CALC_STATE(ac, l, 0, false, false);
	ac.SetVector2(&vector2);
	CHECK_CALC_STATE(ac, l, 0, true, true);

	//Positive

	vector2.SetY(1);
	CHECK_CALC_STATE(ac, l, .25, true, true);

	vector2.SetX(0);
	CHECK_CALC_STATE(ac, l, .5, true, true);

	vector2.SetX(-1);
	CHECK_CALC_STATE(ac, l, .75, true, true);

	vector2.SetX(0);
	vector2.SetY(0);
	vector2.SetZ(1);
	CHECK_CALC_STATE(ac, l, .5, true, true);

	vector2.SetZ(-1);
	CHECK_CALC_STATE(ac, l, .5, true, false);

	//Negative

	vector2.SetX(1);
	vector2.SetY(-1);
	vector2.SetZ(0);
	CHECK_CALC_STATE(ac, l, -.25, true, true);

	vector2.SetX(0);
	CHECK_CALC_STATE(ac, l, -.5, true, true);

	vector2.SetX(-1);
	CHECK_CALC_STATE(ac, l, -.75, true, true);
}

TEST(TestMagnitudeScalar) {
	Vector v = Vector(0, 0, 0);

	MagnitudeScalar mc = MagnitudeScalar(&v);
	CHECK_CALC(mc, 0, true);

	Listener l = Listener();
	l.AddAsListener(NULL, &mc);

	CHECK_CALC_STATE(mc, l, 0, true, false);

	v.SetX(1);
	CHECK_CALC_STATE(mc, l, 1, true, true);
}

*/
