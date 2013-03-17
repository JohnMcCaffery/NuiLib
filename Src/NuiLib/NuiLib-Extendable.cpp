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
#include <NuiLib-Extendable.h>

using namespace NuiLib;

//----------------------------------- Private -----------------------------

void INuiFactoryExtension::UnregisterCreated(IComponent *component) {
	if (!_destroying)
		_created.erase(component);
}


//----------------------------------- Protected -----------------------------
void INuiFactoryExtension::Clear() {
	list<IComponent*> togo;
	for (auto it = _created.begin(); it != _created.end(); it++)
		togo.push_back(*it);

	for (auto it = togo.begin(); it != togo.end(); it++)
		delete *it;
}

//----------------------------------- Public -----------------------------

INuiFactoryExtension::~INuiFactoryExtension() {
	//Clear();
}
