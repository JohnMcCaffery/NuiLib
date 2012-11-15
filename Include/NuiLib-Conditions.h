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
#include <NuiLib-HandProcessing.h>
#include <NuiLib-Extendable.h>
#include <NuiLib-Vectors.h>

#include <time.h>

using namespace std;

#ifndef CONDITIONS_H
#define CONDITIONS_H

namespace NuiLib {
	//--------------Comparison---------------------

	class DLL ComparisonCondition : public ICondition, public BinaryOperator<IScalar, float, float> {
	private:
		bool _nameSet;

	protected:
		inline bool Value();

	public:
		ComparisonCondition(Operator op);
		virtual ~ComparisonCondition();

		bool CalculateValue();
		inline static bool FactoryUpdated() { return true; }
	};

	class DLL GreaterCondition : public ComparisonCondition {
	public: 
		GreaterCondition() : ComparisonCondition(GREATER) { }
		inline static std::string GetTypeName() { return "GreaterCondition"; }
	};
	class DLL LessCondition : public ComparisonCondition {
	public: 
		LessCondition() : ComparisonCondition(LESS) { }
		inline static std::string GetTypeName() { return "LessCondition"; }
	};
	class DLL GreaterEqualCondition : public ComparisonCondition {
	public: 
		GreaterEqualCondition() : ComparisonCondition(GREATER_EQUAL) { }
		inline static std::string GetTypeName() { return "GreaterEqualCondition"; }
	};
	class DLL LessEqualCondition : public ComparisonCondition {
	public: 
		LessEqualCondition() : ComparisonCondition(LESS_EQUAL) { }
		inline static std::string GetTypeName() { return "LessEqualCondition"; }
	};
	class DLL EqualCondition : public ComparisonCondition {
	public: 
		EqualCondition() : ComparisonCondition(EQUAL) { }
		inline static std::string GetTypeName() { return "EqualCondition"; }
	};
	class DLL NotEqualCondition : public ComparisonCondition {
	public: 
		NotEqualCondition() : ComparisonCondition(NOT_EQUAL) { }
		inline static std::string GetTypeName() { return "NotEqualCondition"; }
	};
	class DLL NotEqualVectorCondition : public ICondition, public BinaryOperator<IVector, float, cv::Point3f>  {
	protected:
		inline bool Value();

	public:
		NotEqualVectorCondition();
		virtual ~NotEqualVectorCondition();

		void SetOperand1(float value);
		void SetOperand2(float value);

		using BinaryOperator<IVector, float, cv::Point3f>::SetOperand1;
		using BinaryOperator<IVector, float, cv::Point3f>::SetOperand2;

		bool CalculateValue();
		inline static bool FactoryUpdated() { return true; }
		inline static std::string GetTypeName() { return "NotEqualVectorCondition"; }
	};







	//--------------Logic---------------------

	class DLL LogicCondition : public ICondition, public BinaryOperator<ICondition, bool, bool> {
	protected:
		inline bool Value();

	public:
		LogicCondition(Operator op);
		virtual ~LogicCondition();

		bool CalculateValue();
		inline static bool FactoryUpdated() { return true; }
	};

	class DLL AndCondition : public LogicCondition {
	public: 
		AndCondition() : LogicCondition(AND) { }
		inline static std::string GetTypeName() { return "AndCondition"; }
	};
	class DLL OrCondition : public LogicCondition {
	public: 
		OrCondition() : LogicCondition(OR) { }
		inline static std::string GetTypeName() { return "OrCondition"; }
	};

	class DLL NotCondition : public ICondition {
	private:
		ICondition *_condition;
	public: 
		NotCondition() : ICondition(GetTypeName()), _condition(NULL) { }

		void SetCondition(ICondition *condition) { 
			AddAsListener(_condition, condition);
			_condition = condition; 
		}
		ICondition *GetCondition() { return _condition; }

		bool CalculateValue() {
			return !**_condition;
		}

		inline static std::string GetTypeName() { return "NotCondition"; }
		inline static bool IsMultiConditional() { return true; }
	};

	//----------------------- Hand ---------------------

	class DLL FistCondition : public ICondition {
	private:
		double _threshold;
		HandProcessor *_hand;

	public:
		FistCondition();
		void SetHand(HandProcessor *hand);

		bool CalculateValue();

		inline static std::string GetTypeName() { return "FistCondition"; }
	};


	//----------------------- Click ---------------------

	class DLL ClickCondition : public ICondition {
	private:
		ICondition *_condition;
		time_t _press;
		float _threshold;
		bool _set;

	public:
		ClickCondition();
		virtual bool GetValue();
		void SetCondition(ICondition *condition);
		bool CalculateValue();
		inline static std::string GetTypeName() { return "ClickCondition"; }
	};


	//----------------------- Hold ---------------------

	class DLL HoldCondition : public ICondition {
	private:
		ICondition *_condition;
		time_t _press;
		float _threshold;
		bool _set;

	public:
		HoldCondition();
		void SetCondition(ICondition *condition);
		bool CalculateValue();
		inline static std::string GetTypeName() { return "HoldCondition"; }
	};

	//----------------------- CloseGuard ---------------------

	class DLL CloseGuard : public ICondition {
	private:
		ScalarOperand _scale;
		Vector _head;
		Vector _handR;
		Vector _handL;
		Vector _footR;
		Vector _footL;

	public:
		CloseGuard();
		CloseGuard(float);
		CloseGuard(IScalar*);

		void SetScale(float);
		void SetScale(IScalar*);

		bool CalculateValue();
		inline static std::string GetTypeName() { return "CloseGuard"; }
		inline static bool FactoryUpdated() { return true; }
	};

	FistCondition *fistP(bool rightHand);
	ClickCondition *clickP(ICondition *condition);
	HoldCondition *holdP(ICondition *condition);
	CloseGuard *closeGuardP();
	CloseGuard *closeGuardP(float scale);
	CloseGuard *closeGuardP(IScalar *scale);


	DLL Condition fist(bool rightHand);
	DLL Condition click(Condition &condition);
	DLL Condition hold(Condition &condition);
	DLL Condition closeGuard();
	DLL Condition closeGuard(float scale);
	DLL Condition closeGuard(Scalar scale);


	DLL ICondition *pGreater(IScalar*, IScalar*);
	DLL ICondition *pLess(IScalar*, IScalar*);
	DLL ICondition *pGreaterEqual(IScalar*, IScalar*);
	DLL ICondition *pLessEqual(IScalar*, IScalar*);
	DLL ICondition *pEqualTo(IScalar*, IScalar*);
	DLL ICondition *pNotEqualTo(IScalar*, IScalar*);
	DLL ICondition *pNotEqualTo(IVector*, IVector*);
	DLL ICondition *pAnd(ICondition*, ICondition*);
	DLL ICondition *pOr(ICondition*, ICondition*);
	DLL ICondition *pNot(ICondition*);

	DLL ICondition *pGreater(IScalar*, float);
	DLL ICondition *pLess(IScalar*, float);
	DLL ICondition *pGreaterEqual(IScalar*, float);
	DLL ICondition *pLessEqual(IScalar*, float);
	DLL ICondition *pEqualTo(IScalar*, float);
	DLL ICondition *pNotEqualTo(IScalar*, float);
	DLL ICondition *pNotEqualTo(IVector*, float);
	DLL ICondition *pNotEqualTo(IVector*, cv::Point3f);
	DLL ICondition *pAnd(ICondition*, bool);
	DLL ICondition *pOr(ICondition*, bool);

	DLL ICondition *pGreater(float, IScalar*);
	DLL ICondition *pLess(float, IScalar*);
	DLL ICondition *pGreaterEqual(float, IScalar*);
	DLL ICondition *pLessEqual(float, IScalar*);
	DLL ICondition *pEqualTo(float, IScalar*);
	DLL ICondition *pNotEqualTo(float, IScalar*);
	DLL ICondition *pNotEqualTo(float, IVector*);
	DLL ICondition *pNotEqualTo(cv::Point3f, IVector*);
	DLL ICondition *pAnd(bool, ICondition*);
	DLL ICondition *pOr(bool, ICondition*);
}


#endif
