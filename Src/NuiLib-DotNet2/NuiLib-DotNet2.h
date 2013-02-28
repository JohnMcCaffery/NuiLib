// NuiLib-DotNet2.h
#include <NuiLib-CLRSafe.h>

#pragma once

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace NuiLib;

namespace NuiLibDotNet2 {
	public ref class DotNetScalar {
	private:
		SafeScalar *ps;
		void ChangeListener() {
			//if (OnChange)
				//OnChange();
		}

	public:
		delegate void OnChangeEvt();

		DotNetScalar()  {
			//ps = new NuiLib::SafeScalar(value);
			//char name[50];
			//SPRINTF(name, 50, "%.3f", value);
			//NuiLib::SafeScalar * scalar = ExtensionFactory()->Make<NuiLib::SafeScalar>(string(name));
			//scalar->Set(value);
			//ps = scalar;
		}

		void Init(SafeScalar *scalar) {
			ps = scalar;
		}

		property float Value {
			float get () { return ps->get(); }
			void set (float value) { ps->set(value); }
		}

		~DotNetScalar() { 
			delete ps;
		}

		event OnChangeEvt ^OnChange;

		static DotNetScalar ^Create(float value) {
			DotNetScalar^ scalar = gcnew DotNetScalar();
			OnChangeEvt^ managedCallback = gcnew OnChangeEvt(scalar, &ChangeListener);
			IntPtr unmanagedCallback = Marshal::GetFunctionPointerForDelegate(managedCallback);
			SafeScalar *safe = new NuiLib::SafeScalar(value, (CallbackFunction) (void*)unmanagedCallback);
			GC::KeepAlive(managedCallback);

			scalar->Init(safe);
			return scalar;
		}
	};
}
