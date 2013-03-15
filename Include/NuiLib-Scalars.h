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
#include <NuiLib-Operators.h>
#include <NuiLib-Extendable.h>

#ifndef M_PI
#define M_PI		3.14159265358979323846	/* pi */ 
#endif

#ifndef SCALARS_H
#define SCALARS_H

namespace NuiLib {

	/*
	class DLL Scalar : public IScalar {
	public:
		Scalar();
		Scalar(float);
		Scalar(string, float);

		void SetValue(float);

		virtual string ToXML();
		virtual void LoadXML(string);

		inline static string GetTypeName() { return "Scalar"; }
	};
	*/

	//--------------Wrappers---------------------
	class DLL ScalarWrappingScalar : public IScalar {
	protected:
		ScalarOperand _scalar;

	public:
		ScalarWrappingScalar();
		ScalarWrappingScalar(string);
		ScalarWrappingScalar(string, IScalar *);

		void SetScalar (IScalar *);
		void SetScalar (float);
		float GetScalar();

		virtual string ToXML();
		virtual void LoadXML(string);
	};

	class DLL TwoScalarWrappingScalar : public IScalar {
	protected:
		ScalarOperand _scalar1;
		ScalarOperand _scalar2;

	public:
		TwoScalarWrappingScalar();
		TwoScalarWrappingScalar(string);
		TwoScalarWrappingScalar(string, IScalar *, IScalar *);

		void SetScalar1 (IScalar *);
		void SetScalar1 (float);
		float GetScalar1();

		void SetScalar2 (IScalar *);
		void SetScalar2 (float);
		float GetScalar2();

		virtual string ToXML();
		virtual void LoadXML(string);
		inline static bool FactoryUpdated() { return true; }
	};

	class DLL VectorWrappingScalar : public IScalar {
	protected:
		IVector *_vector;

	public:
		VectorWrappingScalar();
		VectorWrappingScalar(string);
		VectorWrappingScalar(string, IVector *);

		void SetVector (IVector *);
		IVector *GetVector();

		virtual string ToXML();
		virtual void LoadXML(string);
	};

	class DLL TwoVectorWrappingScalar : public IScalar {
	protected:
		IVector *_vector1;
		IVector *_vector2;

	public:
		TwoVectorWrappingScalar();
		TwoVectorWrappingScalar(string);
		TwoVectorWrappingScalar(string, IVector *, IVector *);

		void SetVector1 (IVector *);
		IVector *GetVector1();

		void SetVector2 (IVector *);
		IVector *GetVector2();

		virtual string ToXML();
		virtual void LoadXML(string);
		inline static bool FactoryUpdated() { return true; }
	};

	class DLL ConstrainedScalar : public ScalarWrappingScalar {
	private:
		ScalarOperand _deadzone;
		ScalarOperand _range;
		ScalarOperand _grace;

		bool _mirror;

	public:
		ConstrainedScalar();

		float CalculateValue();

		bool GetMirror();
		void SetMirror(bool mirror);

		float GetDeadzone();
		void SetDeadzone(IScalar *val);
		void SetDeadzone(float val);

		float GetRange();
		void SetRange(IScalar *val);
		void SetRange(float val);

		float GetGrace();
		void SetGrace(IScalar *val);
		void SetGrace(float val);

		string ToXML();
		void LoadXML(string);

		inline static string GetTypeName() { return "ConstrainedScalar"; }
	};

	//--------------Arithmetic---------------------

	class DLL ArithmeticScalar : public IScalar, public BinaryOperator<IScalar, float, float> {
	protected:
		inline float Value();

	public:
		ArithmeticScalar(Operator op);
		virtual ~ArithmeticScalar();

		float CalculateValue();

		inline static bool FactoryUpdated() { return true; }
	};

	class DLL SumScalar : public ArithmeticScalar {
	public: 
		SumScalar() : ArithmeticScalar(PLUS) { }
		inline static string GetTypeName() { return "SumScalar"; }
	};
	class DLL DifferenceScalar : public ArithmeticScalar {
	public: 
		DifferenceScalar() : ArithmeticScalar(MINUS) { }
		inline static string GetTypeName() { return "DifferenceScalar"; }
	};
	class DLL ProductScalar : public ArithmeticScalar {
	public: 
		ProductScalar() : ArithmeticScalar(MULTIPLY) { }
		inline static string GetTypeName() { return "ProductScalar"; }
	};
	class DLL QuotientScalar : public ArithmeticScalar {
	public: 
		QuotientScalar() : ArithmeticScalar(DIVIDE) { }
		inline static string GetTypeName() { return "QuotientScalar"; }
	};



	//-----------Normalizing-----------------


	class DLL ValueNormalizedScalar : public ScalarWrappingScalar {
	private:
		ScalarOperand _max;

	public:
		ValueNormalizedScalar();

		void SetMax(float);
		void SetMax(IScalar*);
		
		float CalculateValue();
		inline static string GetTypeName() { return "ValueNormalizedScalar"; }
	};

	class DLL MaxNormalizedScalar : public ScalarWrappingScalar {
	private:
		float _max;

	public:
		MaxNormalizedScalar();
		float CalculateValue();
		inline static string GetTypeName() { return "MaxNormalizedScalar"; }
	};

	//----------------From Vectors-----------------------

	class DLL VectorScalar : public VectorWrappingScalar {
	private:
		char _value;
	public:
		VectorScalar();
		float CalculateValue();

		char GetComponent();
		void SetComponent(char);

		inline static string GetTypeName() { return "VectorScalar"; }
	};

	class DLL ACosScalar : public ScalarWrappingScalar {
	public:
		ACosScalar();
		float CalculateValue();
		inline static string GetTypeName() { return "ACosScalar"; }
	};

	class DLL DotScalar : public TwoVectorWrappingScalar  {
	public:
		DotScalar();
		float CalculateValue();
		inline static string GetTypeName() { return "DotScalar"; }
	};

	class DLL InvertScalar : public IScalar {
	private:
		ICondition *_condition;

	public:
		InvertScalar();

		ICondition *GetCondition();
		void SetCondition(ICondition *);

		float CalculateValue();
		inline static string GetTypeName() { return "InvertScalar"; }
	};

	class DLL DeltaScalar : public ScalarWrappingScalar {
	private:
		bool _set;
		float _oldValue;
	public:
		DeltaScalar();
		float CalculateValue();
		inline static string GetTypeName() { return "DeltaScalar"; }
	};

	class DLL ProjectionScalar : public TwoVectorWrappingScalar {
	private:
		IScalar *_dot;
		IScalar *_magnitude;
		IScalar *_output;

	public:
		ProjectionScalar();
		float CalculateValue();
		inline static string GetTypeName() { return "ProjectionScalar"; }
	};

	class DLL AbsScalar : public ScalarWrappingScalar {
	public:
		AbsScalar();
		float CalculateValue();
		inline static string GetTypeName() { return "AbsScalar"; }
	};

	class DLL ConditionalScalar : public TwoScalarWrappingScalar {
	private:
		ConditionalOperand _condition;

	public:
		ConditionalScalar();

		void SetCondition(ICondition *value);
		void SetCondition(bool value);

		float CalculateValue();
		inline static string GetTypeName() { return "ConditionalScalar"; }
	};

	DLL VectorScalar *xP(IVector *);
	DLL VectorScalar *yP(IVector *);
	DLL VectorScalar *zP(IVector *);
	DLL VectorScalar *magnitudeP(IVector *);
	DLL DotScalar *dotP(IVector *, IVector *);
	DLL MaxNormalizedScalar *normalizeP(IScalar *);
	DLL ValueNormalizedScalar *normalizeP(IScalar *, float);
	DLL ValueNormalizedScalar *normalizeP(IScalar *, IScalar *);
	DLL ConstrainedScalar *constrainP(IScalar *, float, float, float, bool);
	DLL ConstrainedScalar *constrainP(IScalar *, float, float, IScalar*, bool);
	DLL ConstrainedScalar *constrainP(IScalar *, float, IScalar*, float, bool);
	DLL ConstrainedScalar *constrainP(IScalar *, float, IScalar*, IScalar*, bool);
	DLL ConstrainedScalar *constrainP(IScalar *, IScalar*, float, float, bool);
	DLL ConstrainedScalar *constrainP(IScalar *, IScalar*, float, IScalar*, bool);
	DLL ConstrainedScalar *constrainP(IScalar *, IScalar*, IScalar*, float, bool);
	DLL ConstrainedScalar *constrainP(IScalar *, IScalar*, IScalar*, IScalar*, bool);
	DLL AbsScalar *absP(IScalar *);
	DLL ACosScalar *acosP(IScalar *);
	DLL InvertScalar *invertP(ICondition *);
	DLL DeltaScalar *deltaP(IScalar *);
	DLL ProjectionScalar *projectP(IVector *, IVector *);
	DLL ConditionalScalar *ifScalarP(ICondition*, float, float);
	DLL ConditionalScalar *ifScalarP(ICondition*, float, IScalar*);
	DLL ConditionalScalar *ifScalarP(ICondition*, IScalar*, float);
	DLL ConditionalScalar *ifScalarP(ICondition*, IScalar*, IScalar*);









	DLL IScalar *sum(IScalar*, IScalar*);
	DLL IScalar *sum(float, IScalar*);
	DLL IScalar *sum(IScalar*, float);

	DLL IScalar *difference(IScalar*, IScalar*);
	DLL IScalar *difference(float, IScalar*);
	DLL IScalar *difference(IScalar*, float);

	DLL IScalar *product(IScalar*, IScalar*);
	DLL IScalar *product(float, IScalar*);
	DLL IScalar *product(IScalar*, float);

	DLL IScalar *quotient(IScalar*, IScalar*);
	DLL IScalar *quotient(float, IScalar*);
	DLL IScalar *quotient(IScalar*, float);
}
#endif
