// NuiLib-DotNet.h

#pragma once

using namespace System;

namespace NuiLibDotNet {

	public ref class DotNetScalar
	{

	public:
		NuiLib::SafeScalar *ps;
		DotNetScalar(float value) {
		//DotNetScalar(float value) : ps(new NuiLib::SafeScalar(value)) {
			ps = new NuiLib::SafeScalar(value);
			//char name[50];
			//SPRINTF(name, 50, "%.3f", value);
			//NuiLib::SafeScalar * scalar = ExtensionFactory()->Make<NuiLib::SafeScalar>(string(name));
			//scalar->Set(value);
			//ps = scalar;
		}

		~DotNetScalar() { 
			delete ps;
		}
	};
}
