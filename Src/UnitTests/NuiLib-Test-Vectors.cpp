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
#include <iostream>
#include "NuiLib-Test-API.h"

using namespace NuiLib;

cv::Point3f start1v(1.5f, 2.3f, 8.1f);
cv::Point3f start2v(3.25f, -4.2f, 1.f);
cv::Point3f change1v(-.5f, 2.6f, -1.9f);
cv::Point3f change2v(.8f, -.3f, -.1f);

float start1f = 1.5f;
float start2f = 3.25f;
float change1f = -.5f;
float change2f = .8f;

function<void (cv::Point3f, Vector)> Checkv() { 
	return [](cv::Point3f raw, Vector comp) { 
		CHECK_CLOSE(raw.x, comp.X(), .001f);
		CHECK_CLOSE(raw.y, comp.Y(), .001f);
		CHECK_CLOSE(raw.z, comp.Z(), .001f);
	}; 
}
function<void (float, Vector)> Checkvf() { 
	return [](float raw, Vector comp) { 
		CHECK_CLOSE(raw, comp.X(), .001f);
		CHECK_CLOSE(raw, comp.Y(), .001f);
		CHECK_CLOSE(raw, comp.Z(), .001f);
	}; 
}

TEST(TestVectorSum) { 
	TestOperator<cv::Point3f, Vector, cv::Point3f, Vector>(Checkv(), [](Vector c, cv::Point3f v) { c._p->Set(v.x,v.y,v.z); }, [](cv::Point3f f1, cv::Point3f f2) { return cv::Point3f(f1.x+f2.x, f1.y+f2.y, f1.z+f2.z); }, NuiLib::operator+, NuiLib::operator+, NuiLib::operator+, start1v, start2v, change1v, change2v); 
	TestOperator<float, Vector, float, Vector>(Checkvf(), [](Vector c, float v) { c._p->Set(v,v,v); }, [](float f1, float f2) { return f1+f2; }, NuiLib::operator+, NuiLib::operator+, NuiLib::operator+, start1f, start2f, change1f, change2f); 
}
TEST(TestVectorDifference) { 
	TestOperator<cv::Point3f, Vector, cv::Point3f, Vector>(Checkv(), [](Vector c, cv::Point3f v) { c._p->Set(v.x,v.y,v.z); }, [](cv::Point3f f1, cv::Point3f f2) { return cv::Point3f(f1.x-f2.x, f1.y-f2.y, f1.z-f2.z); }, NuiLib::operator-, NuiLib::operator-, NuiLib::operator-, start1v, start2v, change1v, change2v); 
	TestOperator<float, Vector, float, Vector>(Checkvf(), [](Vector c, float v) { c._p->Set(v,v,v); }, [](float f1, float f2) { return f1-f2; }, NuiLib::operator-, NuiLib::operator-, NuiLib::operator-, start1f, start2f, change1f, change2f); 
}
TEST(TestVectorProduct) { 
	TestOperator<cv::Point3f, Vector, cv::Point3f, Vector>(Checkv(), [](Vector c, cv::Point3f v) { c._p->Set(v.x,v.y,v.z); }, [](cv::Point3f f1, cv::Point3f f2) { return cv::Point3f(f1.x*f2.x, f1.y*f2.y, f1.z*f2.z); }, NuiLib::operator*, NuiLib::operator*, NuiLib::operator*, start1v, start2v, change1v, change2v); 
	TestOperator<float, Vector, float, Vector>(Checkvf(), [](Vector c, float v) { c._p->Set(v,v,v); }, [](float f1, float f2) { return f1*f2; }, NuiLib::operator*, NuiLib::operator*, NuiLib::operator*, start1f, start2f, change1f, change2f); 
}
TEST(TestVectorQuotient) { 
	TestOperator<cv::Point3f, Vector, cv::Point3f, Vector>(Checkv(), [](Vector c, cv::Point3f v) { c._p->Set(v.x,v.y,v.z); }, [](cv::Point3f f1, cv::Point3f f2) { return cv::Point3f(f1.x/f2.x, f1.y/f2.y, f1.z/f2.z); }, NuiLib::operator/, NuiLib::operator/, NuiLib::operator/, start1v, start2v, change1v, change2v); 
	TestOperator<float, Vector, float, Vector>(Checkvf(), [](Vector c, float v) { c._p->Set(v,v,v); }, [](float f1, float f2) { return f1/f2; }, NuiLib::operator/, NuiLib::operator/, NuiLib::operator/, start1f, start2f, change1f, change2f); 
}

TEST(TestVectorSumEquals) { 
	TestEqualsOperator<cv::Point3f, Vector, cv::Point3f, Vector>(Checkv(), [](Vector c, cv::Point3f v) { c._p->Set(v.x,v.y,v.z); }, [](cv::Point3f f1, cv::Point3f f2) { return cv::Point3f(f1.x+f2.x, f1.y+f2.y, f1.z+f2.z); }, NuiLib::operator+=, NuiLib::operator+=, start1v, start2v, change1v, change2v); 
	TestEqualsOperator<float, Vector, float, Vector>(Checkvf(), [](Vector c, float v) { c._p->Set(v,v,v); }, [](float f1, float f2) { return f1+f2; }, NuiLib::operator+=, NuiLib::operator+=, start1f, start2f, change1f, change2f); 
}
TEST(TestVectorDifferenceEquals) { 
	TestEqualsOperator<cv::Point3f, Vector, cv::Point3f, Vector>(Checkv(), [](Vector c, cv::Point3f v) { c._p->Set(v.x,v.y,v.z); }, [](cv::Point3f f1, cv::Point3f f2) { return cv::Point3f(f1.x-f2.x, f1.y-f2.y, f1.z-f2.z); }, NuiLib::operator-=, NuiLib::operator-=, start1v, start2v, change1v, change2v); 
	TestEqualsOperator<float, Vector, float, Vector>(Checkvf(), [](Vector c, float v) { c._p->Set(v,v,v); }, [](float f1, float f2) { return f1-f2; }, NuiLib::operator-=, NuiLib::operator-=, start1f, start2f, change1f, change2f); 
}
TEST(TestVectorProductEquals) { 
	TestEqualsOperator<cv::Point3f, Vector, cv::Point3f, Vector>(Checkv(), [](Vector c, cv::Point3f v) { c._p->Set(v.x,v.y,v.z); }, [](cv::Point3f f1, cv::Point3f f2) { return cv::Point3f(f1.x*f2.x, f1.y*f2.y, f1.z*f2.z); }, NuiLib::operator*=, NuiLib::operator*=, start1v, start2v, change1v, change2v); 
	TestEqualsOperator<float, Vector, float, Vector>(Checkvf(), [](Vector c, float v) { c._p->Set(v,v,v); }, [](float f1, float f2) { return f1*f2; }, NuiLib::operator*=, NuiLib::operator*=, start1f, start2f, change1f, change2f); 
}
TEST(TestVectorQuotientEquals) { 
	TestEqualsOperator<cv::Point3f, Vector, cv::Point3f, Vector>(Checkv(), [](Vector c, cv::Point3f v) { c._p->Set(v.x,v.y,v.z); }, [](cv::Point3f f1, cv::Point3f f2) { return cv::Point3f(f1.x/f2.x, f1.y/f2.y, f1.z/f2.z); }, NuiLib::operator/=, NuiLib::operator/=, start1v, start2v, change1v, change2v); 
	TestEqualsOperator<float, Vector, float, Vector>(Checkvf(), [](Vector c, float v) { c._p->Set(v,v,v); }, [](float f1, float f2) { return f1/f2; }, NuiLib::operator/=, NuiLib::operator/=, start1f, start2f, change1f, change2f); 
}
/*
#define CHECK_VECTOR(vector, x, y, z) \
	CHECK_CLOSE(x, vector->GetX(), .001); \
	CHECK_CLOSE(y, vector->GetY(), .001); \
	CHECK_CLOSE(z, vector->GetZ(), .001);

#define CHECK_VECTOR_STATE(vector, l, x, y, z, trig) \
	CHECK_VECTOR(vector, x, y, z); \
	CHECK_EQUAL(trig, l.triggered); \
	l.triggered = false;

using namespace std;

TEST(TestIVector) {
	Vector vector1 = Vector(0, 0, 0);

	CHECK_EQUAL(0, vector1.ToString(10, 'x').compare("          |           x  0.000"));
	CHECK_EQUAL(0, vector1.ToString(10, 'y').compare("          |           y  0.000"));
	CHECK_EQUAL(0, vector1.ToString(10, 'z').compare("          |           z  0.000"));
	CHECK_EQUAL(0, vector1.ToString(10, 'm').compare("|                     m  0.000"));
	cout << vector1.ToString(10, 'x') + "\n";
	cout << vector1.ToString(10, 'y') + "\n";
	cout << vector1.ToString(10, 'z') + "\n";
	cout << vector1.ToString(10, 'm') + "\n";

	vector1.SetX(1);
	vector1.SetY(1);
	vector1.SetZ(1);

	CHECK_EQUAL(0, vector1.ToString(10, 'x').compare("          |---------- x  1.000"));
	CHECK_EQUAL(0, vector1.ToString(10, 'y').compare("          |---------- y  1.000"));
	CHECK_EQUAL(0, vector1.ToString(10, 'z').compare("          |---------- z  1.000"));
	CHECK_EQUAL(0, vector1.ToString(10, 'm').compare("--------------------| m  1.732"));
	cout << vector1.ToString(10, 'x') + "\n";
	cout << vector1.ToString(10, 'y') + "\n";
	cout << vector1.ToString(10, 'z') + "\n";
	cout << vector1.ToString(10, 'm') + "\n";

	vector1.SetX(-1);
	vector1.SetY(-1);
	vector1.SetZ(-1);

	CHECK_EQUAL(0, vector1.ToString(10, 'x').compare("----------|           x -1.000"));
	CHECK_EQUAL(0, vector1.ToString(10, 'y').compare("----------|           y -1.000"));
	CHECK_EQUAL(0, vector1.ToString(10, 'z').compare("----------|           z -1.000"));
	CHECK_EQUAL(0, vector1.ToString(10, 'm').compare("--------------------| m  1.732"));
	cout << vector1.ToString(10, 'x') + "\n";
	cout << vector1.ToString(10, 'y') + "\n";
	cout << vector1.ToString(10, 'z') + "\n";
	cout << vector1.ToString(10, 'm') + "\n";


	vector1.SetX(.5);
	vector1.SetY(.5);
	vector1.SetZ(.5);

	CHECK_EQUAL(0, vector1.ToString(10, 'x').compare("          |-----      x  0.500"));
	CHECK_EQUAL(0, vector1.ToString(10, 'y').compare("          |-----      y  0.500"));
	CHECK_EQUAL(0, vector1.ToString(10, 'z').compare("          |-----      z  0.500"));
	CHECK_EQUAL(0, vector1.ToString(10, 'm').compare("----------|           m  0.866"));
	cout << vector1.ToString(10, 'x') + "\n";
	cout << vector1.ToString(10, 'y') + "\n";
	cout << vector1.ToString(10, 'z') + "\n";
	cout << vector1.ToString(10, 'm') + "\n";

	vector1.SetX(-.5);
	vector1.SetY(-.5);
	vector1.SetZ(-.5);

	CHECK_EQUAL(0, vector1.ToString(10, 'x').compare("     -----|           x -0.500"));
	CHECK_EQUAL(0, vector1.ToString(10, 'y').compare("     -----|           y -0.500"));
	CHECK_EQUAL(0, vector1.ToString(10, 'z').compare("     -----|           z -0.500"));
	CHECK_EQUAL(0, vector1.ToString(10, 'm').compare("----------|           m  0.866"));
	cout << vector1.ToString(10, 'x') + "\n";
	cout << vector1.ToString(10, 'y') + "\n";
	cout << vector1.ToString(10, 'z') + "\n";
	cout << vector1.ToString(10, 'm') + "\n";

	vector1.SetX(1);
	vector1.SetY(2);
	vector1.SetZ(3);
	IVector* vector2 = new Vector(.5, 4, 3);

	IVector *sum = vector1 + vector2;
	IVector *difference = vector1 - vector2;
	IVector *product = vector1 * vector2;
	IVector *quotient = vector1 / vector2;

	CHECK_VECTOR((sum), 1 + .5, 2 + 4, 3 + 3);
	CHECK_VECTOR((difference), 1 - .5, 2 - 4, 3 - 3);
	CHECK_VECTOR((product), 1 * .5, 2 * 4, 3 * 3);
	CHECK_VECTOR((quotient), 1.0 / .5, 2.0 / 4.0, 3.0 / 3.0);

	CHECK_CLOSE(vector1.GetMagnitude(), sqrt(pow(1.0,2) + pow(2.0,2) + pow(3.0,2)), .001);
	CHECK_CLOSE(vector2->GetMagnitude(), sqrt(pow(.5,2) + pow(4.0,2) + pow(3.0,2)), .001);

	delete vector2;
}

TEST(TestVector) {
	Vector* vector = new Vector;
	CHECK_VECTOR(vector, 0, 0, 0);
	delete vector;

	vector = new Vector(1, 2.2, 3.33);
	CHECK_VECTOR(vector, 1, 2.2, 3.33);

	Listener l = Listener();
	l.AddAsListener(NULL, vector);

	vector->SetX(2);
	CHECK_VECTOR_STATE(vector, l, 2, 2.2, 3.33, true);

	vector->SetY(4.4);
	CHECK_VECTOR_STATE(vector, l, 2, 4.4, 3.33, true);

	vector->SetZ(6.66);
	CHECK_VECTOR_STATE(vector, l, 2, 4.4, 6.66, true);
	
	delete vector;
}

/*
TEST(TestSumVector) { 
	Vector vector1 = Vector(1, 2, 3);
	Vector vector2 = Vector(-.5, -4, -3);
	Vector vector3 = Vector(.25, .5, .75);

	SumVector sum = SumVector();
	Listener l = Listener();
	l.AddAsListener(&sum);
	CHECK_VECTOR((&sum), 0, 0, 0);

	sum.AddAddend(&vector1);
	CHECK_VECTOR_STATE((&sum), l, 1, 2, 3, true);

	vector1.SetX(2);
	CHECK_VECTOR_STATE((&sum), l, 2, 2, 3, true);

	vector1.SetY(4);
	CHECK_VECTOR_STATE((&sum), l, 2, 4, 3, true);

	vector1.SetZ(6);
	CHECK_VECTOR_STATE((&sum), l, 2, 4, 6, true);

	vector1.SetX(1);
	vector1.SetY(2);
	vector1.SetZ(3);
	CHECK_VECTOR_STATE((&sum), l, 1, 2, 3, true);

	sum.AddAddend(&vector2);
	CHECK_VECTOR_STATE((&sum), l, .5, -2, 0, true);

	sum.AddAddend(&vector3);
	CHECK_VECTOR_STATE((&sum), l, .75, -1.5, .75, true);

	sum.RemoveAddend(&vector1);
	CHECK_VECTOR_STATE((&sum), l, -.25, -3.5, -2.25, true);
	
	sum.RemoveAddend(&vector3);
	CHECK_VECTOR_STATE((&sum), l, -.5, -4, -3, true);

	vector1.SetX(5);
	CHECK_EQUAL(false, l.triggered);
}

TEST(TestDifferenceVector) {
	Vector vector1 = Vector(1, 2, 3);
	Vector vector2 = Vector(3.75, 2.5, 1.25);
	Vector vector3 = Vector(5, 5, 5);

	DifferenceVector diff = DifferenceVector();
	Listener l = Listener();
	l.AddAsListener(NULL, &diff);

	CHECK_VECTOR((&diff), 0, 0, 0);

	diff.SetVector1(&vector1);
	CHECK_VECTOR_STATE((&diff), l, 1, 2, 3, true);

	diff.SetVector2(&vector2);
	CHECK_VECTOR_STATE((&diff), l, -2.75, -.5, 1.75, true);

	vector1.SetX(4);
	CHECK_VECTOR_STATE((&diff), l, .25, -.5, 1.75, true);

	vector1.SetY(3);
	CHECK_VECTOR((&diff), .25, .5, 1.75);

	vector1.SetZ(0);
	CHECK_VECTOR_STATE((&diff), l, .25, .5, -1.25, true);


	vector2.SetX(4);
	CHECK_VECTOR_STATE((&diff), l, 0, .5, -1.25, true);

	vector2.SetY(3);
	CHECK_VECTOR_STATE((&diff), l, 0, 0, -1.25, true);

	vector2.SetZ(0);
	CHECK_VECTOR_STATE((&diff), l, 0, 0, 0, true);


	diff.SetVector1(&vector3);
	CHECK_VECTOR_STATE((&diff), l, 1, 2, 5, true);

	diff.SetVector2(&vector1);
	CHECK_VECTOR_STATE((&diff), l, 1, 2, 5, false);

	vector2.SetX(1);
	vector2.SetY(2);
	vector2.SetZ(3);
	CHECK_EQUAL(false, l.triggered);

	//TODO - See what happens when you start removing crap.
	//Do listeners get dealt with correctly?
}

TEST(TestMaxNormalizedVector) {
	Vector vector = Vector(0, 0, 0);

	MaxNormalizedVector mv = MaxNormalizedVector(&vector);
	IVector *v = &mv;
	Listener l = Listener();
	l.AddAsListener(NULL, &mv);

	CHECK_VECTOR_STATE(v, l, 0, 0, 0, false);

	vector.SetX(1);
	CHECK_VECTOR_STATE(v, l, 1, 0, 0, true);
	CHECK_CLOSE(1, v->GetMagnitude(), .001);

	vector.SetY(1);
	CHECK_VECTOR_STATE(v, l, 1, 1, 0, true);
	CHECK_CLOSE(1, v->GetMagnitude(), .001);

	vector.SetZ(1);
	CHECK_VECTOR_STATE(v, l, 1, 1, 1, true);
	CHECK_CLOSE(1, v->GetMagnitude(), .001);

	vector.SetX(2);
	CHECK_VECTOR_STATE(v, l, 1, .5, .5, true);
	CHECK_CLOSE(1, v->GetMagnitude(), .001);

	vector.SetX(.5);
	CHECK_VECTOR_STATE(v, l, .25, .5, .5, true);
	CHECK_CLOSE(sqrt(2.25) / sqrt(6.0), v->GetMagnitude(), .001);
}

*/
