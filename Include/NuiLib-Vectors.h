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
#include <opencv\cv.h>
#include <NuiLib-API.h>
#include <NuiLib-Operators.h>
#include <NuiLib-Extendable.h>
#include <queue>

using namespace std;

#ifndef VECTORS_H
#define VECTORS_H

namespace NuiLib {

	/*
	class DLL Vector : public IVector {
	public:
		Vector();
		Vector(INuiFactory *factory);
		Vector(string name);
		Vector(float, float, float);
		Vector(float, float, float);
		Vector(string name, float, float, float);
		void SetX(float);
		void SetY(float);
		void SetZ(float);
		void Set(float, float, float);

		inline static string GetName() { return "Vector"; }
	};
	*/

	class DLL VectorWrappingVector : public IVector {
	protected:
		IVector *_vector;

	public:
		VectorWrappingVector();
		VectorWrappingVector(string);
		VectorWrappingVector(string, IVector *);

		void SetVector (IVector *);
		IVector *GetVector();

		virtual string ToXML();
		virtual void LoadXML(string);
	};

	class DLL TwoVectorWrappingVector : public IVector {
	protected:
		IVector *_vector1;
		IVector *_vector2;

	public:
		TwoVectorWrappingVector();
		TwoVectorWrappingVector(string);
		TwoVectorWrappingVector(string, IVector *, IVector *);

		void SetVector1 (IVector *);
		IVector *GetVector1();

		void SetVector2 (IVector *);
		IVector *GetVector2();

		virtual string ToXML();
		virtual void LoadXML(string);
		inline static bool FactoryUpdated() { return true; }
	};

	class DLL ThreeVectorWrappingVector : public TwoVectorWrappingVector{
	protected:
		IVector *_vector3;

	public:
		ThreeVectorWrappingVector();
		ThreeVectorWrappingVector(string);
		ThreeVectorWrappingVector(string, IVector *, IVector *, IVector *);

		void SetVector3 (IVector *);
		IVector *GetVector3();

		virtual string ToXML();
		virtual void LoadXML(string);
		inline static bool FactoryUpdated() { return true; }
	};

	class DLL FourVectorWrappingVector : public ThreeVectorWrappingVector{
	protected:
		IVector *_vector4;

	public:
		FourVectorWrappingVector();
		FourVectorWrappingVector(string);
		FourVectorWrappingVector(string, IVector *, IVector *, IVector *, IVector *);

		void SetVector4 (IVector *);
		IVector *GetVector4();

		virtual string ToXML();
		virtual void LoadXML(string);
		inline static bool FactoryUpdated() { return true; }
	};

	class DLL NVectorWrappingVector : public IVector {
	protected:
		set<IVector*> _vectors;

	public:
		NVectorWrappingVector();
		NVectorWrappingVector(string);

		void AddVector (IVector *);
		void RemoveVector(IVector *);

		virtual string ToXML();
		virtual void LoadXML(string);
		inline static bool FactoryUpdated() { return true; }
	};

	class DLL MaxNormalizedVector : public VectorWrappingVector {
	private:
		float _max;
		float _magMax;

	public:
		MaxNormalizedVector();
		MaxNormalizedVector(IVector *);

		float Magnitude();

		cv::Point3f CalculateValue();

		inline static string GetName() { return "MaxNormalizedVector"; }
	};

	class DLL ValueNormalizedVector : public TwoVectorWrappingVector {
	public:
		ValueNormalizedVector();
		ValueNormalizedVector(IVector *, IVector *);

		cv::Point3f CalculateValue();

		inline static string GetName() { return "ValueNormalizedVector"; }
	};

	class DLL NormalizedVector : public VectorWrappingVector {
	public:
		NormalizedVector();
		NormalizedVector(IVector *);

		cv::Point3f CalculateValue();

		inline static string GetName() { return "NormalizedVector"; }
	};

	class DLL JointVector : public IVector {
	protected:
		string _joint;

	public:
		JointVector(string);
		JointVector(string, string);

		virtual string GetJoint();
		virtual void SetJoint(string) = 0;

		virtual string ToXML() = 0;
		virtual void LoadXML(string) = 0;

		inline static string GetTypeName() { return "JointVector"; }
	};

	//--------------Arithmetic---------------------

	class DLL ArithmeticVector : public IVector, public BinaryOperator<IVector, float, cv::Point3f> {
	protected:
		inline cv::Point3f Value();

	public:
		ArithmeticVector(Operator op);
		virtual ~ArithmeticVector();

		void SetOperand1(float value);
		void SetOperand2(float value);

		using BinaryOperator<IVector, float, cv::Point3f>::SetOperand1;
		using BinaryOperator<IVector, float, cv::Point3f>::SetOperand2;

		cv::Point3f CalculateValue();
		inline static bool FactoryUpdated() { return true; }
	};

	class DLL SumVector : public ArithmeticVector {
	public: 
		SumVector() : ArithmeticVector(PLUS) { }
		inline static string GetTypeName() { return "SumVector"; }
	};
	class DLL DifferenceVector : public ArithmeticVector {
	public: 
		DifferenceVector() : ArithmeticVector(MINUS) { }
		inline static string GetTypeName() { return "DifferenceVector"; }
	};
	class DLL ProductVector : public ArithmeticVector {
	public: 
		ProductVector() : ArithmeticVector(MULTIPLY) { }
		inline static string GetTypeName() { return "ProductVector"; }
	};
	class DLL QuotientVector : public ArithmeticVector {
	public: 
		QuotientVector() : ArithmeticVector(DIVIDE) { }
		inline static string GetTypeName() { return "QuotientVector"; }
	};

	//-----------------------Vector Arithmetic ----------------------

	class DLL DotVector : public TwoVectorWrappingVector {
	public:
		DotVector();
		DotVector(IVector *, IVector *);

		cv::Point3f CalculateValue();

		inline static string GetTypeName() { return "DotVector"; }
	};

	class DLL CrossVector : public TwoVectorWrappingVector {
	public:
		CrossVector();

		cv::Point3f CalculateValue();

		inline static string GetTypeName() { return "CrossProductVector"; }
	};

	class DLL PartialVector : public VectorWrappingVector {
	private:
		bool _use[3];

	public:
		PartialVector();
		void UseX(bool);
		void UseY(bool);
		void UseZ(bool);
		cv::Point3f CalculateValue();
		inline static string GetTypeName() { return "PartialVector"; }
	};

	class DLL DeltaVector : public VectorWrappingVector {
	private:
		bool _set;
		float _oldX;
		float _oldY;
		float _oldZ;
	public:
		DeltaVector();
		cv::Point3f CalculateValue();
		inline static string GetTypeName() { return "DeltaVector"; }
	};

	class DLL SnapshotVector : public IVector {
	private:
		ICondition *_condition;
		IVector *_vector;

	public:
		SnapshotVector();
		cv::Point3f CalculateValue();
		void SetCondition(ICondition *);
		void SetVector(IVector *);
		inline static string GetTypeName() { return "SnapshotVector"; }
	};


	class DLL MomentumVector : public IVector {
	private:
		ICondition *_condition;
		IVector *_vector;
		cv::Point3f _delta;
		ScalarOperand _decrement;
		ScalarOperand _threshold;

	public:
		MomentumVector();

		cv::Point3f CalculateValue();
		void SetCondition(ICondition *);
		void SetVector(IVector *);
		void SetDecrement(float decrement);
		void SetThreshold(float threshold);
		void SetDecrement(IScalar *decrement);
		void SetThreshold(IScalar *threshold);

		inline static string GetTypeName() { return "MomentumVector"; }
		inline static bool FactoryUpdated() { return true; }
	};

	class DLL PlaneIntersectionVector : public FourVectorWrappingVector {
	private:
		IScalar *_numerator;
		IScalar *_denominator;
		IVector *_output;
		IVector *_pointDiff;
		IScalar *_dirQuotient;
		IVector *_lineScaled;

	public:
		PlaneIntersectionVector();
		PlaneIntersectionVector(IVector *, IVector *, IVector *, IVector *);

		cv::Point3f CalculateValue();
		void Setup();

		inline static string GetName() { return "PlaneIntersectionVector"; }
		static bool FactoryUpdated() { return false; }
	};

	class DLL MagnitudeScaledVector : public VectorWrappingVector {
	private:
		ScalarOperand _scale;

	public:
		MagnitudeScaledVector();
		MagnitudeScaledVector(IVector *, IScalar *);
		MagnitudeScaledVector(IVector *, float *);
		MagnitudeScaledVector(IVector *, float);

		void SetScale(IScalar *value);
		void SetScale(float *value);
		void SetScale(float value);

		cv::Point3f CalculateValue();
		inline static string GetName() { return "MagnitudeScaledVector"; }
	};

	class DLL ScalarVector : public IVector {
	private:
		ScalarOperand _scalarX;
		ScalarOperand _scalarY;
		ScalarOperand _scalarZ;

	public:
		ScalarVector();

		void SetX(IScalar *);
		void SetY(IScalar *);
		void SetZ(IScalar *);

		void SetX(float);
		void SetY(float);
		void SetZ(float);

		cv::Point3f CalculateValue();
		inline static string GetName() { return "ScalarVector"; }
	};

	class DLL SmoothedVector : public VectorWrappingVector {
	private:
		ScalarOperand _numFrames;
		queue<cv::Point3f> _frames;

	public:
		SmoothedVector();

		void SetNumFrames(IScalar *value);
		void SetNumFrames(float value);

		cv::Point3f CalculateValue();
		inline static string GetTypeName() { return "SmoothedVector"; }
		inline static bool FactoryUpdated() { return true; }
	};


	DLL NormalizedVector *normalizeP(IVector *);
	DLL DotVector *planeDotsP(IVector *, IVector *);
	DLL CrossVector *crossP(IVector *, IVector *);
	DLL PartialVector *limitP(IVector *, bool, bool, bool);
	DLL DeltaVector *deltaP(IVector *);
	DLL SnapshotVector *snapshotP(IVector *, ICondition *);
	DLL MomentumVector *momentumP(IVector *vector, ICondition *condition);
	DLL MomentumVector *momentumP(IVector *vector, ICondition *condition, float decrement, float threshold);
	DLL MagnitudeScaledVector *scaleP(IVector *vector, IScalar *scale);
	DLL MagnitudeScaledVector *scaleP(IVector *vector, float scale);
	DLL PlaneIntersectionVector *intersectP(IVector *pPlane, IVector *planeNormal, IVector *pLine, IVector *lineDir);
	DLL ScalarVector *vectorP(IScalar*);
	DLL ScalarVector *vectorP(float);
	DLL ScalarVector *vectorP(float, float, float);
	DLL ScalarVector *vectorP(float, float, IScalar *);
	DLL ScalarVector *vectorP(float, IScalar *, float);
	DLL ScalarVector *vectorP(float, IScalar *, IScalar *);
	DLL ScalarVector *vectorP(IScalar *, float, float);
	DLL ScalarVector *vectorP(IScalar *, float, IScalar *);
	DLL ScalarVector *vectorP(IScalar *, IScalar *, float);
	DLL ScalarVector *vectorP(IScalar *, IScalar *, IScalar *);

	DLL Vector normalize(Vector &);
	DLL Vector planeDots(const Vector &, const Vector &);
	DLL Vector cross(Vector &, Vector &);
	DLL Vector limit(Vector &, bool, bool, bool);
	DLL Vector delta(Vector &);
	DLL Vector snapshot(Vector &, Condition &);
	DLL Vector momentum(Vector &vector, Condition &condition);
	DLL Vector momentum(Vector &vector, Condition &condition, float decrement, float threshold);
	DLL Vector scale(Vector &vector, Scalar &scale);
	DLL Vector scale(Vector &vector, float scale);
	DLL Vector intersect(Vector &pPlane, Vector &planeNormal, Vector &pLine, Vector &lineDir);
	DLL Vector vector(Scalar &);
	DLL Vector joint(int joint);


	DLL IVector *sum(IVector*, IVector*);
	DLL IVector *sum(float, IVector*);
	DLL IVector *sum(cv::Point3f, IVector*);
	DLL IVector *sum(IVector*, float);
	DLL IVector *sum(IVector*, cv::Point3f);

	DLL IVector *difference(IVector*, IVector*);
	DLL IVector *difference(float, IVector*);
	DLL IVector *difference(cv::Point3f, IVector*);
	DLL IVector *difference(IVector*, float);
	DLL IVector *difference(IVector*, cv::Point3f);

	DLL IVector *product(IVector*, IVector*);
	DLL IVector *product(float, IVector*);
	DLL IVector *product(cv::Point3f, IVector*);
	DLL IVector *product(IVector*, float);
	DLL IVector *product(IVector*, cv::Point3f);

	DLL IVector *quotient(IVector*, IVector*);
	DLL IVector *quotient(float, IVector*);
	DLL IVector *quotient(cv::Point3f, IVector*);
	DLL IVector *quotient(IVector*, float);
	DLL IVector *quotient(IVector*, cv::Point3f);
}

#endif
