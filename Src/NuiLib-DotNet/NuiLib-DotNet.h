// NuiLib-DotNet.h
#include <NuiLib-CLRSafe.h>

#pragma once

using namespace System;
using namespace NuiLib;

namespace NuiLibDotNet {
public ref class StudentWrapper
{
private:
  Student *_stu;
public:
  StudentWrapper(String ^fullname, double gpa)
  {
    _stu = new Student((char *) 
           System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(
           fullname).ToPointer(), 
      gpa);
  }
  ~StudentWrapper()
  {
    delete _stu;
    _stu = 0;
  }

  property String ^Name
  {
    String ^get()
    {
      return gcnew String(_stu->getName());
    }
  }
  property double Gpa
  {
    double get()
    {
      return _stu->getGpa();
    }
  }
};

	public ref class DotNetScalar
	{

	public:
		//NuiLib::SafeScalar *ps;
		DotNetScalar(float value) {
		//DotNetScalar(float value) : ps(new NuiLib::SafeScalar(value)) {
			//ps = new NuiLib::SafeScalar(value);
			//char name[50];
			//SPRINTF(name, 50, "%.3f", value);
			//NuiLib::SafeScalar * scalar = ExtensionFactory()->Make<NuiLib::SafeScalar>(string(name));
			//scalar->Set(value);
			//ps = scalar;
		}

		~DotNetScalar() { 
			//delete ps;
		}
	};
}
