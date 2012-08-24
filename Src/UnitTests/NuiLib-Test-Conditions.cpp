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

//TODO think of what the numbers need to be.
bool start1c = false;
bool start2c = true; 
bool change1c = false;
bool change2c = true;

float start1cfg = 1.5f;
float start2cfg = 3.25f;
float change1cfg = .5f;
float change2cfg = -.8f;

float start1cfe = 1.5f;
float start2cfe = 3.25f;
float change1cfe = -.5f;
float change2cfe = .8f;

using namespace NuiLib;

function<void (bool, Condition)> Checkc() { 
	return [](bool raw, Condition comp) { 
		if(raw) 
			CHECK(*comp); 
		else
			CHECK(!*comp); 
	}; 
}

TEST(TestConditionGreater) { 
	TestOperator<bool, Condition, float, Scalar>(Checkc(), [](Scalar c, float v) { c._p->Set(v); }, [](float f1, float f2) { return f1 > f2; }, NuiLib::operator>, NuiLib::operator>, NuiLib::operator>, start1cfg, start2cfg, change1cfg, change2cfg); 
	TestOperator<bool, Condition, float, Scalar>(Checkc(), [](Scalar c, float v) { c._p->Set(v); }, [](float f1, float f2) { return f1 > f2; }, NuiLib::operator>, NuiLib::operator>, NuiLib::operator>, start1cfe, start2cfe, change1cfe, change2cfe); 
}
TEST(TestConditionLess) { 
	TestOperator<bool, Condition, float, Scalar>(Checkc(), [](Scalar c, float v) { c._p->Set(v); }, [](float f1, float f2) { return f1 < f2; }, NuiLib::operator<, NuiLib::operator<, NuiLib::operator<, start1cfg, start2cfg, change1cfg, change2cfg); 
	TestOperator<bool, Condition, float, Scalar>(Checkc(), [](Scalar c, float v) { c._p->Set(v); }, [](float f1, float f2) { return f1 < f2; }, NuiLib::operator<, NuiLib::operator<, NuiLib::operator<, start1cfe, start2cfe, change1cfe, change2cfe); 
}
TEST(TestConditionGreaterEqual) { 
	TestOperator<bool, Condition, float, Scalar>(Checkc(), [](Scalar c, float v) { c._p->Set(v); }, [](float f1, float f2) { return f1 >= f2; }, NuiLib::operator>=, NuiLib::operator>=, NuiLib::operator>=, start1cfg, start2cfg, change1cfg, change2cfg); 
	TestOperator<bool, Condition, float, Scalar>(Checkc(), [](Scalar c, float v) { c._p->Set(v); }, [](float f1, float f2) { return f1 >= f2; }, NuiLib::operator>=, NuiLib::operator>=, NuiLib::operator>=, start1cfe, start2cfe, change1cfe, change2cfe); 
}
TEST(TestConditionLessEqual) { 
	TestOperator<bool, Condition, float, Scalar>(Checkc(), [](Scalar c, float v) { c._p->Set(v); }, [](float f1, float f2) { return f1 <= f2; }, NuiLib::operator<=, NuiLib::operator<=, NuiLib::operator<=, start1cfg, start2cfg, change1cfg, change2cfg); 
	TestOperator<bool, Condition, float, Scalar>(Checkc(), [](Scalar c, float v) { c._p->Set(v); }, [](float f1, float f2) { return f1 <= f2; }, NuiLib::operator<=, NuiLib::operator<=, NuiLib::operator<=, start1cfe, start2cfe, change1cfe, change2cfe); 
}

TEST(TestConditionEqual) { TestOperator<bool, Condition, float, Scalar>(Checkc(), [](Scalar c, float v) { c._p->Set(v); }, [](float f1, float f2) { return f1 == f2; }, NuiLib::operator==, NuiLib::operator==, NuiLib::operator==, start1cfg, start2cfg, change1cfg, change2cfg); }
TEST(TestConditionNotEqual) { TestOperator<bool, Condition, float, Scalar>(Checkc(), [](Scalar c, float v) { c._p->Set(v); }, [](float f1, float f2) { return f1 != f2; }, NuiLib::operator!=, NuiLib::operator!=, NuiLib::operator!=, start1cfg, start2cfg, change1cfg, change2cfg); }

TEST(TestConditionOr) { TestOperator<bool, Condition, bool, Condition>(Checkc(), [](Condition c, bool v) { c._p->Set(v); }, [](bool f1, bool f2) { return f1 || f2; }, NuiLib::operator||, NuiLib::operator||, NuiLib::operator||, start1c, start2c, change1c, change2c); }
TEST(TestConditionAnd) { TestOperator<bool, Condition, bool, Condition>(Checkc(), [](Condition c, bool v) { c._p->Set(v); }, [](bool f1, bool f2) { return f1 && f2; }, NuiLib::operator&&, NuiLib::operator&&, NuiLib::operator&&, start1c, start2c, change1c, change2c); }

//TODO implement
TEST(TestConditionNot) { }
