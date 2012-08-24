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
#include<iostream>
#include "NuiLib-Test-API.h"

int main() {
	int ret = UnitTest::RunAllTests();
	//int ret = 0;

	
	string blah;
	cout << "Press any key to exit\n";
	getline(cin, blah);

	return ret;
}
