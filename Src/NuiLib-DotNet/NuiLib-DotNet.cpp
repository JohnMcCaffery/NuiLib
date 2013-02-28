// This is the main DLL file.

#include "stdafx.h"

#include "NuiLib-DotNet.h"

using namespace NuiLibDotNet;

DotNetScalar ^DotNetScalar::operator+(DotNetScalar ^addend1, DotNetScalar ^addend2) {
	return Create(NuiLibSafe::sum((addend1->_ps), (addend2->_ps)));
}
DotNetScalar ^DotNetScalar::operator-(DotNetScalar ^minuend, DotNetScalar ^subtrahend) {
	return Create(NuiLibSafe::difference((minuend->_ps), (subtrahend->_ps)));
}
DotNetScalar ^DotNetScalar::operator*(DotNetScalar ^factor1, DotNetScalar ^factor2) {
	return Create(NuiLibSafe::product((factor1->_ps), (factor2->_ps)));
}
DotNetScalar ^DotNetScalar::operator/(DotNetScalar ^dividend, DotNetScalar ^divisor) {
	return Create(NuiLibSafe::quotient((dividend->_ps), (divisor->_ps)));
}
DotNetScalar ^DotNetScalar::operator+=(DotNetScalar ^addend1, DotNetScalar ^addend2) {
	return Create(NuiLibSafe::sum((addend1->_ps), (addend2->_ps)));
}
DotNetScalar ^DotNetScalar::operator-=(DotNetScalar ^minuend, DotNetScalar ^subtrahend) {
	return Create(NuiLibSafe::difference((minuend->_ps), (subtrahend->_ps)));
}
DotNetScalar ^DotNetScalar::operator*=(DotNetScalar ^factor1, DotNetScalar ^factor2) {
	return Create(NuiLibSafe::product((factor1->_ps), (factor2->_ps)));
}
DotNetScalar ^DotNetScalar::operator/=(DotNetScalar ^dividend, DotNetScalar ^divisor) {
	return Create(NuiLibSafe::quotient((dividend->_ps), (divisor->_ps)));
}


DotNetScalar ^DotNetScalar::operator+(DotNetScalar ^addend1, float addend2) {
	return Create(NuiLibSafe::sum((addend1->_ps), addend2));
}
DotNetScalar ^DotNetScalar::operator-(DotNetScalar ^minuend, float subtrahend) {
	return Create(NuiLibSafe::difference((minuend->_ps), subtrahend));
}
DotNetScalar ^DotNetScalar::operator*(DotNetScalar ^factor1, float factor2) {
	return Create(NuiLibSafe::product((factor1->_ps), factor2));
}
DotNetScalar ^DotNetScalar::operator/(DotNetScalar ^dividend, float divisor) {
	return Create(NuiLibSafe::quotient((dividend->_ps), divisor));
}
DotNetScalar ^DotNetScalar::operator+=(DotNetScalar ^addend1, float addend2) {
	return Create(NuiLibSafe::sum((addend1->_ps), addend2));
}
DotNetScalar ^DotNetScalar::operator-=(DotNetScalar ^minuend, float subtrahend) {
	return Create(NuiLibSafe::difference((minuend->_ps), subtrahend));
}
DotNetScalar ^DotNetScalar::operator*=(DotNetScalar ^factor1, float factor2) {
	return Create(NuiLibSafe::product((factor1->_ps), factor2));
}
DotNetScalar ^DotNetScalar::operator/=(DotNetScalar ^dividend, float divisor) {
	return Create(NuiLibSafe::quotient((dividend->_ps), divisor));
}

DotNetScalar ^DotNetScalar::operator+(float addend1, DotNetScalar ^addend2) {
	return Create(NuiLibSafe::sum(addend1, (addend2->_ps)));
}
DotNetScalar ^DotNetScalar::operator-(float minuend, DotNetScalar ^subtrahend) {
	return Create(NuiLibSafe::difference(minuend, (subtrahend->_ps)));
}
DotNetScalar ^DotNetScalar::operator*(float factor1, DotNetScalar ^factor2) {
	return Create(NuiLibSafe::product(factor1, (factor2->_ps)));
}
DotNetScalar ^DotNetScalar::operator/(float dividend, DotNetScalar ^divisor) {
	return Create(NuiLibSafe::quotient(dividend, (divisor->_ps)));
}

///
/// The x value for the given DotNetVector.
/// @param vector The vector to take the x value of.
///
DotNetScalar ^DotNetScalar::x(DotNetVector ^vector) {
	return Create(NuiLibSafe::x(vector->_ps));
}
///
/// The y value for the given DotNetVector.
/// @param vector The vector to take the y value of.
///
DotNetScalar ^DotNetScalar::y(DotNetVector ^vector) {
	return Create(NuiLibSafe::y(vector->_ps));
}
///
/// The z value for the given DotNetVector.
/// @param vector The vector to take the z value of.
///
DotNetScalar ^DotNetScalar::z(DotNetVector ^vector) {
	return Create(NuiLibSafe::z(vector->_ps));
}
///
/// The magnitude value of the given DotNetVector.
///
DotNetScalar ^DotNetScalar::magnitude(DotNetVector ^vector) {
	return Create(NuiLibSafe::magnitude(vector->_ps));
}
///
/// The dot product of two vectors
/// @param a The first vector.
/// @param b The second vector.
///
DotNetScalar ^DotNetScalar::dot(DotNetVector ^a, DotNetVector ^b) {
	return Create(NuiLibSafe::dot(a->_ps, b->_ps));
}
///
/// Normalize the value of a scalar against the maximum value it has ever been.
/// @param scalar The scalar to normalize.
///
DotNetScalar ^DotNetScalar::normalize(DotNetScalar ^scalar) {
	return Create(NuiLibSafe::normalizeScalar(scalar->_ps));
}
///
/// Normalize the value of a scalar against a specified maximum value.
/// @param scalar The scalar to normalize.
/// @param max The maximum value to normalize against.
///
DotNetScalar ^DotNetScalar::normalize(DotNetScalar ^scalar, float max) {
	return Create(NuiLibSafe::normalize(scalar->_ps, max));
}
///
/// Normalize the value of a scalar against a specified maximum value.
/// @param scalar The scalar to normalize.
/// @param max The maximum value to normalize against.
///
DotNetScalar ^DotNetScalar::normalize(DotNetScalar ^scalar, DotNetScalar ^max) {
	return Create(NuiLibSafe::normalize(scalar->_ps, max->_ps));
}
///
/// Constrain a scalar to a specific range.
/// The final output is a value between 0.f and 1.f.
/// If the input is < deadzone Value == 0.f
/// If the input is < deadzone + range Value == (*input - deadzone) / range.
/// If the input is < deadzone + range + grace Value == 1.f
/// If the input is >= deadzone + range + grace Value == 0.f
/// @param input The scalar to constrain.
/// @param deadzone The minimum threshold. If *input < deadzone Value == 0.f.
/// @param range The range of values accepted. If *input < deadzone + range Value == (*input - deadzone) / range.
/// @param grace The grace period past the end of the range where the value stays 1.f. If *input < deadzome + range + grace Value == 1.f.
/// @param mirror Whether to mirror the constraints. If true calculations are done against the absolute value if *input.
///
DotNetScalar ^DotNetScalar::constrain(DotNetScalar ^input, float deadzone, float range, float grace, bool mirror) {
	return Create(NuiLibSafe::constrain(input->_ps, deadzone, range, grace, mirror));
}
///
/// Constrain a scalar to a specific range.
/// The final output is a value between 0.f and 1.f.
/// If the input is < deadzone Value == 0.f
/// If the input is < deadzone + range Value == (*input - deadzone) / range.
/// If the input is < deadzone + range + grace Value == 1.f
/// If the input is >= deadzone + range + grace Value == 0.f
/// @param input The scalar to constrain.
/// @param deadzone The minimum threshold. If *input < deadzone Value == 0.f.
/// @param range The range of values accepted. If *input < deadzone + range Value == (*input - deadzone) / range.
/// @param grace The grace period past the end of the range where the value stays 1.f. If *input < deadzome + range + grace Value == 1.f.
/// @param mirror Whether to mirror the constraints. If true calculations are done against the absolute value if *input.
///
DotNetScalar ^DotNetScalar::constrain(DotNetScalar ^input, float deadzone, float range, DotNetScalar ^grace, bool mirror) {
	return Create(NuiLibSafe::constrain(input->_ps, deadzone, range, grace->_ps, mirror));
}
///
/// Constrain a scalar to a specific range.
/// The final output is a value between 0.f and 1.f.
/// If the input is < deadzone Value == 0.f
/// If the input is < deadzone + range Value == (*input - deadzone) / range.
/// If the input is < deadzone + range + grace Value == 1.f
/// If the input is >= deadzone + range + grace Value == 0.f
/// @param input The scalar to constrain.
/// @param deadzone The minimum threshold. If *input < deadzone Value == 0.f.
/// @param range The range of values accepted. If *input < deadzone + range Value == (*input - deadzone) / range.
/// @param grace The grace period past the end of the range where the value stays 1.f. If *input < deadzome + range + grace Value == 1.f.
/// @param mirror Whether to mirror the constraints. If true calculations are done against the absolute value if *input.
///
DotNetScalar ^DotNetScalar::constrain(DotNetScalar ^input, float deadzone, DotNetScalar ^range, float grace, bool mirror) {
	return Create(NuiLibSafe::constrain(input->_ps, deadzone, range->_ps, grace, mirror));
}
///
/// Constrain a scalar to a specific range.
/// The final output is a value between 0.f and 1.f.
/// If the input is < deadzone Value == 0.f
/// If the input is < deadzone + range Value == (*input - deadzone) / range.
/// If the input is < deadzone + range + grace Value == 1.f
/// If the input is >= deadzone + range + grace Value == 0.f
/// @param input The scalar to constrain.
/// @param deadzone The minimum threshold. If *input < deadzone Value == 0.f.
/// @param range The range of values accepted. If *input < deadzone + range Value == (*input - deadzone) / range.
/// @param grace The grace period past the end of the range where the value stays 1.f. If *input < deadzome + range + grace Value == 1.f.
/// @param mirror Whether to mirror the constraints. If true calculations are done against the absolute value if *input.
///
DotNetScalar ^DotNetScalar::constrain(DotNetScalar ^input, float deadzone, DotNetScalar ^range, DotNetScalar^ grace, bool mirror) {
	return Create(NuiLibSafe::constrain(input->_ps, deadzone, range->_ps, grace->_ps, mirror));
}
///
/// Constrain a scalar to a specific range.
/// The final output is a value between 0.f and 1.f.
/// If the input is < deadzone Value == 0.f
/// If the input is < deadzone + range Value == (*input - deadzone) / range.
/// If the input is < deadzone + range + grace Value == 1.f
/// If the input is >= deadzone + range + grace Value == 0.f
/// @param input The scalar to constrain.
/// @param deadzone The minimum threshold. If *input < deadzone Value == 0.f.
/// @param range The range of values accepted. If *input < deadzone + range Value == (*input - deadzone) / range.
/// @param grace The grace period past the end of the range where the value stays 1.f. If *input < deadzome + range + grace Value == 1.f.
/// @param mirror Whether to mirror the constraints. If true calculations are done against the absolute value if *input.
///
DotNetScalar ^DotNetScalar::constrain(DotNetScalar ^input, DotNetScalar^ deadzone, float range, float grace, bool mirror) {
	return Create(NuiLibSafe::constrain(input->_ps, deadzone->_ps, range, grace, mirror));
}
///
/// Constrain a scalar to a specific range.
/// The final output is a value between 0.f and 1.f.
/// If the input is < deadzone Value == 0.f
/// If the input is < deadzone + range Value == (*input - deadzone) / range.
/// If the input is < deadzone + range + grace Value == 1.f
/// If the input is >= deadzone + range + grace Value == 0.f
/// @param input The scalar to constrain.
/// @param deadzone The minimum threshold. If *input < deadzone Value == 0.f.
/// @param range The range of values accepted. If *input < deadzone + range Value == (*input - deadzone) / range.
/// @param grace The grace period past the end of the range where the value stays 1.f. If *input < deadzome + range + grace Value == 1.f.
/// @param mirror Whether to mirror the constraints. If true calculations are done against the absolute value if *input.
///
DotNetScalar ^DotNetScalar::constrain(DotNetScalar ^input, DotNetScalar^ deadzone, float range, DotNetScalar^ grace, bool mirror) {
	return Create(NuiLibSafe::constrain(input->_ps, deadzone->_ps, range, grace->_ps, mirror));
}
///
/// Constrain a scalar to a specific range.
/// The final output is a value between 0.f and 1.f.
/// If the input is < deadzone Value == 0.f
/// If the input is < deadzone + range Value == (*input - deadzone) / range.
/// If the input is < deadzone + range + grace Value == 1.f
/// If the input is >= deadzone + range + grace Value == 0.f
/// @param input The scalar to constrain.
/// @param deadzone The minimum threshold. If *input < deadzone Value == 0.f.
/// @param range The range of values accepted. If *input < deadzone + range Value == (*input - deadzone) / range.
/// @param grace The grace period past the end of the range where the value stays 1.f. If *input < deadzome + range + grace Value == 1.f.
/// @param mirror Whether to mirror the constraints. If true calculations are done against the absolute value if *input.
///
DotNetScalar ^DotNetScalar::constrain(DotNetScalar ^input, DotNetScalar^ deadzone, DotNetScalar ^range, float grace, bool mirror) {
	return Create(NuiLibSafe::constrain(input->_ps, deadzone->_ps, range->_ps, grace, mirror));
}
///
/// Constrain a scalar to a specific range.
/// The final output is a value between 0.f and 1.f.
/// If the input is < deadzone Value == 0.f
/// If the input is < deadzone + range Value == (*input - deadzone) / range.
/// If the input is < deadzone + range + grace Value == 1.f
/// If the input is >= deadzone + range + grace Value == 0.f
/// @param input The scalar to constrain.
/// @param deadzone The minimum threshold. If *input < deadzone Value == 0.f.
/// @param range The range of values accepted. If *input < deadzone + range Value == (*input - deadzone) / range.
/// @param grace The grace period past the end of the range where the value stays 1.f. If *input < deadzome + range + grace Value == 1.f.
/// @param mirror Whether to mirror the constraints. If true calculations are done against the absolute value if *input.
///
DotNetScalar ^DotNetScalar::constrain(DotNetScalar ^input, DotNetScalar^ deadzone, DotNetScalar ^range, DotNetScalar^ grace, bool mirror) {
	return Create(NuiLibSafe::constrain(input->_ps, deadzone->_ps, range->_ps, grace->_ps, mirror));
}
///
/// Take the absolute value of a scalar.
/// @param input The scalar to take the absolute value of.
///
DotNetScalar ^DotNetScalar::abs(DotNetScalar ^input) {
	return Create(NuiLibSafe::abs(input->_ps));
}
///
/// Take the arc cosine of a scalar.
/// @param input The scalar to take the arc cosine of.
///
DotNetScalar ^DotNetScalar::acos(DotNetScalar ^input) {
	return Create(NuiLibSafe::acos(input->_ps));
}
///
/// -1 or 1 depending on a condition.
/// Value == *input ? -1.f : 1.f
/// Used to invert other values depending on a condition.
/// @param input The condition which dictates Value.
///
DotNetScalar ^DotNetScalar::invert(DotNetCondition ^input) {
	return Create(NuiLibSafe::invert(input->_ps));
}
///
/// The change in a scalar since the last update.
/// Will give the first derivitive of a scalar.
/// @param input The scalar to track changes in.
///
DotNetScalar ^DotNetScalar::delta(DotNetScalar ^input) {
	return Create(NuiLibSafe::deltaScalar(input->_ps));
}
///
/// The scalar projection of one vector onto another.
/// a is projected onto b.
/// @param a The projecting vector.
/// @param b The vector being projected on.
///
DotNetScalar ^DotNetScalar::project(DotNetVector ^a, DotNetVector ^b) {
	return Create(NuiLibSafe::project(a->_ps, b->_ps));
}
/*
///
/// Value is one of two inputs depending on a condition.
/// Value = *condition ? t : f);
/// @param condition DotNetCondition up which the output is dependant.
/// @param t Value == t if condition is true.
/// @param f Value == f if condition is false.
///
DotNetScalar ^DotNetScalar::ifScalar(DotNetCondition ^condition, float t, float f) {
	return Create(NuiLibSafe::ifScalar(condition->_ps, t, f));
}
///
/// Value is one of two inputs depending on a condition.
/// Value = *condition ? t : f);
/// @param condition DotNetCondition up which the output is dependant.
/// @param t Value == t if condition is true.
/// @param f Value == f if condition is false.
///
DotNetScalar ^DotNetScalar::ifScalar(DotNetCondition ^condition, float t, DotNetScalar ^f) {
	return Create(NuiLibSafe::ifScalar(condition->_ps, t, f->_ps));
}
///
/// Value is one of two inputs depending on a condition.
/// Value = *condition ? a : b);
/// @param condition DotNetCondition up which the output is dependant.
/// @param a Value == a if condition is true.
/// @param b Value == b if condition is false.
///
DotNetScalar ^DotNetScalar::ifScalar(DotNetCondition ^condition, DotNetScalar ^t, float f) {
	return Create(NuiLibSafe::ifScalar(condition->_ps, t->_ps, f));
}
///
/// Value is one of two inputs depending on a condition.
/// Value = *condition ? a : b);
/// @param condition DotNetCondition up which the output is dependant.
/// @param a Value == a if condition is true.
/// @param b Value == b if condition is false.
///
DotNetScalar ^DotNetScalar::ifScalar(DotNetCondition ^condition, DotNetScalar ^t, DotNetScalar ^f) {
	return Create(NuiLibSafe::ifScalar(condition->_ps, t->_ps, f->_ps));
}
*/
///
/// Value is set by an opencv track bar.
/// Takes parameters to define how the integer value, starting at 0, that the tracker supplies is converted to a float.
/// @param title The name of the tracker bar.
/// @param max the maximum value the track bar can have.
/// @param scale How the tracker input (between 0 and max) should be scaled to convert it to a float.
/// @param shift How the tracker input (between 0 and max) should be shifted to convert it to a float.
/// @param value The initial value of the track bar.
///
//DotNetScalar ^DotNetScalar::tracker(string title, int max, float scale = 1.f, float shift = 0.f, int value = 0) {
//return Create(NuiLibSafe::tracker(string title, max, scale, shift, value->_ps));
//}










///
/// Normalize the value of a vector so its magnitude is 1.
/// @param vector The SafeVector to normalize.
/// @param max The maximum value to normalize against.
///
DotNetVector ^DotNetVector::normalize(DotNetVector ^vector) {
	return Create(NuiLibSafe::normalize(vector->_ps));
}
///
/// Get the cross product of two vectors.
/// @param a The first vector.
/// @param b The second vector.
///
DotNetVector ^DotNetVector::cross(DotNetVector ^a, DotNetVector ^b) {
	return Create(NuiLibSafe::cross(a->_ps, b->_ps));
}
///
/// Take only specified axes from a vector.
/// @param input The vector to limit.
/// @param x Whether to take the x axis.
/// @param y Whether to take the y axis.
/// @param z Whether to take the z axis.
///
DotNetVector ^DotNetVector::limit(DotNetVector ^input, bool x, bool y, bool z) {
	return Create(NuiLibSafe::limit(input->_ps, x, y, z));
	}
///
/// The change in a vector since the last update.
/// Will give the first derivitive of a vector in all three axes.
/// @param input The vector to track changes in.
///
DotNetVector ^DotNetVector::delta(DotNetVector ^input) {
	return Create(NuiLibSafe::delta(input->_ps));
	}
///
/// Take a snapshot of a vector whenever a condition becomes true.
/// Every time *condition == true Value is set to vector.
/// @param vector The vector to take snapshots of.
/// @param condition Every time this becomes true a snapshot is taken.
DotNetVector ^DotNetVector::snapshot(DotNetVector ^input, DotNetCondition ^condition) {
	return Create(NuiLibSafe::snapshot(input->_ps, condition->_ps));
	}
///
/// Take the momenum of an input vector.
/// As long as condition is true Value == vector.
/// As soon as condition becomes false Value is taken as the last position before the condition become false + 
/// the delta between the penultimate true position and the final true position. 
/// After this the same delta keeps on being applied and reduced until the magnitude of the delta approaches zero.
/// @param input The vector to take the momentum of.
/// @param condition If true Value is input. Otherwise Value is taken from the momentum of the final position before condition became false.
///
DotNetVector ^DotNetVector::momentum(DotNetVector ^input, DotNetCondition ^condition) {
	return Create(NuiLibSafe::momentum(input->_ps, condition->_ps));
	}
///
/// Take the momenum of an input vector.
/// As long as condition is true Value == vector.
/// As soon as condition becomes false Value is taken as the last position before the condition become false + 
/// the delta between the penultimate true position and the final true position. 
/// After this the same delta keeps on being applied and reduced by decrement until the magnitude of the delta is less than threshold.
/// Momentum updates with every factory trigger.
/// @param input The vector to take the momentum of.
/// @param condition If true Value is input. Otherwise Value is taken from the momentum of the final position before condition became false.
/// @param decrement Whilst applying momentum the delta is reduced by this much each iteration.
/// @param threshold Momentum stops being applied when the magnitude of the delta is less than this.
///
DotNetVector ^DotNetVector::momentum(DotNetVector ^input, DotNetCondition ^condition, float decrement, float threshold) {
	return Create(NuiLibSafe::momentum(input->_ps, condition->_ps, decrement, threshold));
	}
///
/// Scale a vector so that its magnitude is scale.
/// @param input the vector to scale.
/// @param scale The magnitude Value is to have in the direction of input.
/// 
DotNetVector ^DotNetVector::scale(DotNetVector ^input, DotNetScalar ^scale) {
	return Create(NuiLibSafe::scale(input->_ps, scale->_ps));
	}
///
/// Scale a vector so that its magnitude is scale.
/// @param input the vector to scale.
/// @param scale The magnitude Value is to have in the direction of input.
/// 
DotNetVector ^DotNetVector::scale(DotNetVector ^input, float scale) {
	return Create(NuiLibSafe::scale(input->_ps, scale));
	}
///
/// Calculate the intersection of a line and a plane.
/// The plane is specified as a point and a normal.
/// The line is specified as a point and a direction.
/// @param pPlane A point on the plane.
/// @param normalPlane The normal of the plane. Is expected to be normalized.
/// @param pLine A point on the line.
/// @param dirLine The direction of the line. Expected to be normalized.
/// @return The point in 3D space where the line intersects the plane.
///
DotNetVector ^DotNetVector::intersect(DotNetVector ^pPlane, DotNetVector ^normalPlane, DotNetVector ^pLine, DotNetVector ^dirLine) {
	return Create(NuiLibSafe::intersect(pPlane->_ps, normalPlane->_ps, pLine->_ps, dirLine->_ps));
	}


DotNetVector ^DotNetVector::joint(const int joint) {
	return Create(NuiLibSafe::joint(joint));
}