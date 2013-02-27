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
#include <NuiLib-Extendable.h>

#ifndef OPERATORS_H
#define OPERATORS_H

namespace NuiLib {
	
	enum Operator { PLUS, MINUS, MULTIPLY, DIVIDE, GREATER, LESS, GREATER_EQUAL, LESS_EQUAL, EQUAL, NOT_EQUAL, AND, OR };

	template<typename WrapperT, typename SimpleT, typename OperandT> class Operand {
	private:
		struct _valueUnion {
			_valueUnion() : wrapper(NULL) { }
			_valueUnion(WrapperT *pWrapper) : wrapper(pWrapper) { }
			_valueUnion(OperandT vValue) : value(vValue) { }
			WrapperT *wrapper;
			OperandT value;
		} _value;
		short _type;
		IObserver *_observer;
		function<OperandT (SimpleT)> _converter;
		function<char *(const char*, OperandT)> _printer;
		OperandT _default;

	public:
		static function<OperandT (SimpleT)> Converter() {
			return [](SimpleT in) { return in; };
		}
		static function<char*(const char*, OperandT)> Printer() {
			return [](const char* format, OperandT in) -> char* {
				char *buffer = new char[50];
				SPRINTF(buffer, 50, format, in);
				return buffer;
			};
		}

		static const int WRAPPER = 0;
		static const int VALUE = 1;

		/*
		Operand(WrapperT *value, OperandT def) { 
			Init(value, def, Converter(), Printer());
		}
		Operand(OperandT value, OperandT def) { 
			Init(value, def, Converter(), Printer());
		}
		Operand(Operand<WrapperT,SimpleT,OperandT>* value) {
			Init(value, Converter(), Printer());
		}*/

		Operand(WrapperT *value, OperandT def, function<OperandT(SimpleT)> converter, 
			function<char *(const char *, OperandT)> printer) { 
				Init(value, def, converter, printer);
		}
		Operand(OperandT value, OperandT def, function<OperandT(SimpleT)> converter,
			function<char *(const char *, OperandT)> printer) { 
				Init(value, def, converter, printer);
		}
		Operand(Operand<WrapperT,SimpleT,OperandT>* value, function<OperandT(SimpleT)> converter,
			function<char *(const char *, OperandT)> printer) {
				Init(value, converter, printer);
		}

		void Init(OperandT def, function<OperandT(SimpleT)> converter, function<char *(const char *, OperandT)> printer) { 
			_converter = converter;
			_printer = printer;
			_default = def;
		}
		void Init(WrapperT *value, OperandT def, function<OperandT(SimpleT)> converter, 
			function<char *(const char *, OperandT)> printer) { 
				_value.wrapper =value;
				_type=WRAPPER;
				Init(def, converter, printer);
		}
		void Init(OperandT value, OperandT def, function<OperandT(SimpleT)> converter,
			function<char *(const char *, OperandT)> printer) { 
				_value.value = value;
				_type=VALUE;
				Init(def, converter, printer);
		}
		void Init(Operand<WrapperT,SimpleT,OperandT>* value, function<OperandT(SimpleT)> converter,
			function<char *(const char *, OperandT)> printer) { 
				_type = value->_type;
				switch(value->_type) {
				case WRAPPER: Set(value->_value.wrapper); break;
				case VALUE: Set(value->_value.value); break;
				}
				//Init(value, converter, printer); //Taking out this line was a hack
		}

		const OperandT operator*() { return Get(); }
		OperandT Get() {
			switch(_type) {
			case WRAPPER: return **_value.wrapper;
			case VALUE: return _value.value;
			default: return _default; break;
			}
		}

		void Set(WrapperT* value) {
			_type = WRAPPER;
			_value.wrapper = value;
		}
		//void Set(SimpleT value){
			//_type = VALUE;
			//_value.value = _converter(value);
		//}
		void Set(OperandT value){
			_type = VALUE;
			_value.value = value;
		}
		void Set(Operand<WrapperT,SimpleT,OperandT> input){
			_type = input._type;
			switch(input._type) {
			case WRAPPER: Set(input._value.wrapper); break;
			case VALUE: Set(input._value.value); break;
			}
		}

		inline IObservable *Observable() { return _type == WRAPPER ? _value.wrapper : NULL; }

		const char *GetName(const char *format) {
			return _type == WRAPPER ? _value.wrapper->GetCName() : _printer(format, _value.value);
		}

		//Operand<WrapperT, SimpleT, OperandT> operator=(WrapperT *in) { return Operand<WrapperT, SimpleT, OperandT>(in); } TODO taking out this line was a hack
		//Operand<WrapperT, SimpleT, OperandT> operator=(OperandT in) { return Operand<WrapperT, SimpleT, OperandT>(in); }	 //Taking out this line was a hack

	};


	class DLL ScalarOperand : public Operand<IScalar, float, float> {
	public:
		ScalarOperand() : 
		  Operand<IScalar, float, float>(0.f, 0.f, Converter(), Printer()) {}
		ScalarOperand(IScalar *value) : 
			Operand<IScalar, float, float>(value, 0.f, Converter(), Printer()) {}
		ScalarOperand(float value) : 
			Operand<IScalar, float, float>(value, 0.f, Converter(), Printer()) {}
	};


	class DLL ConditionalOperand : public Operand<ICondition, bool, bool> {
	public:
		ConditionalOperand() : 
			Operand<ICondition, bool, bool>(false, false, Converter(), Printer()) {}
		ConditionalOperand(ICondition *value) : 
			Operand<ICondition, bool, bool>(value, false, Converter(), Printer()) {}
		ConditionalOperand(bool value) : 
			Operand<ICondition, bool, bool>(value, false, Converter(), Printer()) {}
	};


	class DLL VectorOperand : public Operand<IVector, float, cv::Point3f> {
	public:
		VectorOperand() : 
		  Operand<IVector, float, cv::Point3f>(cv::Point3f(), cv::Point3f(), [](float in) { 
			  return cv::Point3f(in);
		  }, [](const char *format, cv::Point3f p) -> char* {
			  char *buff = new char[50];
			  SPRINTF(buff, 50, "%.3f %.3f %.3f", p.x, p.y, p.z);
			  return buff;
		  }) { }
		  VectorOperand(IVector *value) : 
		  Operand<IVector, float, cv::Point3f>(value, cv::Point3f(), [](float in) { 
			  return cv::Point3f(in);
		  }, [](const char *format, cv::Point3f p) -> char* {
			  char *buff = new char[50];
			  SPRINTF(buff, 50, "%.3f %.3f %.3f", p.x, p.y, p.z);
			  return buff;
		  }) { }		VectorOperand(float value) : 
		  Operand<IVector, float, cv::Point3f>(cv::Point3f(value), cv::Point3f(), [](float in) { 
			  return cv::Point3f(in);
		  }, [](const char *format, cv::Point3f p) -> char* {
			  char *buff = new char[50];
			  SPRINTF(buff, 50, "%.3f %.3f %.3f", p.x, p.y, p.z);
			  return buff;
		  }) { }		VectorOperand(cv::Point3f value) : 
		  Operand<IVector, float, cv::Point3f>(value, cv::Point3f(), [](float in) { 
			  return cv::Point3f(in);
		  }, [](const char *format, cv::Point3f p) -> char* {
			  char *buff = new char[50];
			  SPRINTF(buff, 50, "%.3f %.3f %.3f", p.x, p.y, p.z);
			  return buff;
		  }) { }	
	};
	template<typename O, typename T1, typename T2> O *CreateOperator(T1 operand1, T2 operand2);

	template<typename WrapperT, typename SimpleT, typename OperandT> class BinaryOperator {
	private:
		Operand<WrapperT, SimpleT, OperandT> _operand1;
		Operand<WrapperT, SimpleT, OperandT> _operand2;
		Operator _operator;
		const char *_format;

		string MakeName() {
			char *buffer = new char[5000];
			const char *name1 = _operand1.GetName(_format);
			const char *name2 = _operand2.GetName(_format);
			switch (_operator) {
			case PLUS: SPRINTF(buffer, 5000, "(%s +  %s)", name1, name2); break;
			case MINUS: SPRINTF(buffer, 5000, "(%s - %s)", name1, name2); break;
			case MULTIPLY: SPRINTF(buffer, 5000, "(%s * %s)", name1, name2); break;
			case DIVIDE: SPRINTF(buffer, 5000, "(%s / %s)", name1, name2); break;
			case GREATER: SPRINTF(buffer, 5000, "(%s > %s)", name1, name2); break;
			case LESS: SPRINTF(buffer, 5000, "(%s < %s)", name1, name2); break;
			case GREATER_EQUAL: SPRINTF(buffer, 5000, "(%s >= %s)", name1, name2); break;
			case LESS_EQUAL: SPRINTF(buffer, 5000, "(%s <= %s)", name1, name2); break;
			case EQUAL: SPRINTF(buffer, 5000, "(%s == %s)", name1, name2); break;
			case NOT_EQUAL: SPRINTF(buffer, 5000, "(%s != %s)", name1, name2); break;
			case AND: SPRINTF(buffer, 5000, "(%s && %s)", name1, name2); break;
			case OR: SPRINTF(buffer, 5000, "(%s || %s)", name1, name2); break;
			default: return string("Operator Name"); break;
			}	
			//delete name1;
			//delete name2;
			return string(buffer);
		}
	protected:
		static function<OperandT (SimpleT)> Converter() {
			return [](SimpleT in) { return in; };
		}
		static function<char*(const char*, OperandT)> Printer() {
			return [](const char* format, OperandT in) -> char* {
				char *buffer = new char[50];
				SPRINTF(buffer, 50, format, in);
				return buffer;
			};
		}

		/*BinaryOperator(Operator op, const char *format, OperandT start, OperandT def) : 
			 _operand1(start, def),
				 _operand2(start, def),
				 _operator(op), 
				 _format(format) { 
			 }
			 */
		BinaryOperator(Operator op, const char *format, OperandT start, OperandT def, 
			function<OperandT(SimpleT)> converter,
			function<char *(const char*,OperandT)> printer) : 
			 _operand1(start, def, converter, printer),
				 _operand2(start, def, converter, printer),
				 _operator(op), 
				 _format(format) { 
			 }

		template<typename I, typename O>
		O CalculateValue(I operand1, I operand2) {
			switch (_operator) {
			case PLUS: return operand1 + operand2;
			case MINUS: return operand1 - operand2;
			case MULTIPLY: return operand1 * operand2;
			case DIVIDE: return operand1 / operand2;
			case GREATER: return operand1 > operand2;
			case LESS: return operand1 < operand2;
			case GREATER_EQUAL: return operand1 >= operand2;
			case LESS_EQUAL: return operand1 <= operand2;
			case EQUAL: return operand1 == operand2;
			case NOT_EQUAL: return operand1 != operand2;
			case AND: return operand1 && operand2;
			case OR: return operand1 || operand2;
			default: return 0;
			}
		}		

	public:
		virtual ~BinaryOperator() { }
		OperandT GetOperand1() { return _operand1.Get(); }
		OperandT GetOperand2() { return _operand2.Get(); }

		const char* GetOperand1Name(char* format) { return _operand1.GetName(format); }
		const char* GetOperand2Name(char* format) { return _operand2.GetName(format); }

		void SetOperand1(WrapperT* value) { _operand1.Set(value); }
		//void SetOperand1(SimpleT value) { _operand1.Set(value); }
		void SetOperand1(OperandT value) { _operand1.Set(value); }
		void SetOperand1(Operand<WrapperT,SimpleT,OperandT> value) { _operand1.Set(value); }

		void SetOperand2(WrapperT* value) { _operand2.Set(value); }
		//void SetOperand1(SimpleT value) { _operand1.Set(value); }
		void SetOperand2(OperandT value) { _operand2.Set(value); }
		void SetOperand2(Operand<WrapperT,SimpleT,OperandT> value) { _operand2.Set(value); }

		template<typename O, typename T1, typename T2> friend O *CreateOperator(T1 operand1, T2 operand2);
	};

	template<typename O, typename T1, typename T2> O *CreateOperator(T1 operand1, T2 operand2) {
		O *ret = ExtensionFactory()->Make<O>("Create");
		ret->SetOperand1(operand1);
		ret->SetOperand2(operand2);
		ret->SetName(ret->MakeName());
		ret->Changed(NULL);
		return ret;
	};
}
#endif
