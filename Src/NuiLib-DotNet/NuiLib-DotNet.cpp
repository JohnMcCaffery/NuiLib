// This is the main DLL file.

#include "stdafx.h"

#include "NuiLib-DotNet.h"

using namespace NuiLibDotNet;

Scalar ^Scalar::operator+(Scalar ^addend1, Scalar ^addend2) {
	return Create(NuiLibSafe::sum((addend1->_ps), (addend2->_ps)));
}
Scalar ^Scalar::operator-(Scalar ^minuend, Scalar ^subtrahend) {
	return Create(NuiLibSafe::difference((minuend->_ps), (subtrahend->_ps)));
}
Scalar ^Scalar::operator*(Scalar ^factor1, Scalar ^factor2) {
	return Create(NuiLibSafe::product((factor1->_ps), (factor2->_ps)));
}
Scalar ^Scalar::operator/(Scalar ^dividend, Scalar ^divisor) {
	return Create(NuiLibSafe::quotient((dividend->_ps), (divisor->_ps)));
}
Scalar ^Scalar::operator+=(Scalar ^addend1, Scalar ^addend2) {
	return Create(NuiLibSafe::sum((addend1->_ps), (addend2->_ps)));
}
Scalar ^Scalar::operator-=(Scalar ^minuend, Scalar ^subtrahend) {
	return Create(NuiLibSafe::difference((minuend->_ps), (subtrahend->_ps)));
}
Scalar ^Scalar::operator*=(Scalar ^factor1, Scalar ^factor2) {
	return Create(NuiLibSafe::product((factor1->_ps), (factor2->_ps)));
}
Scalar ^Scalar::operator/=(Scalar ^dividend, Scalar ^divisor) {
	return Create(NuiLibSafe::quotient((dividend->_ps), (divisor->_ps)));
}


Scalar ^Scalar::operator+(Scalar ^addend1, float addend2) {
	return Create(NuiLibSafe::sum((addend1->_ps), addend2));
}
Scalar ^Scalar::operator-(Scalar ^minuend, float subtrahend) {
	return Create(NuiLibSafe::difference((minuend->_ps), subtrahend));
}
Scalar ^Scalar::operator*(Scalar ^factor1, float factor2) {
	return Create(NuiLibSafe::product((factor1->_ps), factor2));
}
Scalar ^Scalar::operator/(Scalar ^dividend, float divisor) {
	return Create(NuiLibSafe::quotient((dividend->_ps), divisor));
}
Scalar ^Scalar::operator+=(Scalar ^addend1, float addend2) {
	return Create(NuiLibSafe::sum((addend1->_ps), addend2));
}
Scalar ^Scalar::operator-=(Scalar ^minuend, float subtrahend) {
	return Create(NuiLibSafe::difference((minuend->_ps), subtrahend));
}
Scalar ^Scalar::operator*=(Scalar ^factor1, float factor2) {
	return Create(NuiLibSafe::product((factor1->_ps), factor2));
}
Scalar ^Scalar::operator/=(Scalar ^dividend, float divisor) {
	return Create(NuiLibSafe::quotient((dividend->_ps), divisor));
}

Scalar ^Scalar::operator+(float addend1, Scalar ^addend2) {
	return Create(NuiLibSafe::sum(addend1, (addend2->_ps)));
}
Scalar ^Scalar::operator-(float minuend, Scalar ^subtrahend) {
	return Create(NuiLibSafe::difference(minuend, (subtrahend->_ps)));
}
Scalar ^Scalar::operator*(float factor1, Scalar ^factor2) {
	return Create(NuiLibSafe::product(factor1, (factor2->_ps)));
}
Scalar ^Scalar::operator/(float dividend, Scalar ^divisor) {
	return Create(NuiLibSafe::quotient(dividend, (divisor->_ps)));
}

///
/// The x value for the given Vector.
/// @param vector The vector to take the x value of.
///
Scalar ^Nui::x(Vector ^vector) {
	return Scalar::Create(NuiLibSafe::x(vector->_ps));
}
///
/// The y value for the given Vector.
/// @param vector The vector to take the y value of.
///
Scalar ^Nui::y(Vector ^vector) {
	return Scalar::Create(NuiLibSafe::y(vector->_ps));
}
///
/// The z value for the given Vector.
/// @param vector The vector to take the z value of.
///
Scalar ^Nui::z(Vector ^vector) {
	return Scalar::Create(NuiLibSafe::z(vector->_ps));
}
///
/// The magnitude value of the given Vector.
///
Scalar ^Nui::magnitude(Vector ^vector) {
	return Scalar::Create(NuiLibSafe::magnitude(vector->_ps));
}
///
/// The dot product of two vectors
/// @param a The first vector.
/// @param b The second vector.
///
Scalar ^Nui::dot(Vector ^a, Vector ^b) {
	return Scalar::Create(NuiLibSafe::dot(a->_ps, b->_ps));
}
///
/// Normalize the value of a scalar against the maximum value it has ever been.
/// @param scalar The scalar to normalize.
///
Scalar ^Nui::normalize(Scalar ^scalar) {
	return Scalar::Create(NuiLibSafe::normalizeScalar(scalar->_ps));
}
///
/// Normalize the value of a scalar against a specified maximum value.
/// @param scalar The scalar to normalize.
/// @param max The maximum value to normalize against.
///
Scalar ^Nui::normalize(Scalar ^scalar, float max) {
	return Scalar::Create(NuiLibSafe::normalize(scalar->_ps, max));
}
///
/// Normalize the value of a scalar against a specified maximum value.
/// @param scalar The scalar to normalize.
/// @param max The maximum value to normalize against.
///
Scalar ^Nui::normalize(Scalar ^scalar, Scalar ^max) {
	return Scalar::Create(NuiLibSafe::normalize(scalar->_ps, max->_ps));
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
Scalar ^Nui::constrain(Scalar ^input, float deadzone, float range, float grace, bool mirror) {
	return Scalar::Create(NuiLibSafe::constrain(input->_ps, deadzone, range, grace, mirror));
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
Scalar ^Nui::constrain(Scalar ^input, float deadzone, float range, Scalar ^grace, bool mirror) {
	return Scalar::Create(NuiLibSafe::constrain(input->_ps, deadzone, range, grace->_ps, mirror));
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
Scalar ^Nui::constrain(Scalar ^input, float deadzone, Scalar ^range, float grace, bool mirror) {
	return Scalar::Create(NuiLibSafe::constrain(input->_ps, deadzone, range->_ps, grace, mirror));
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
Scalar ^Nui::constrain(Scalar ^input, float deadzone, Scalar ^range, Scalar^ grace, bool mirror) {
	return Scalar::Create(NuiLibSafe::constrain(input->_ps, deadzone, range->_ps, grace->_ps, mirror));
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
Scalar ^Nui::constrain(Scalar ^input, Scalar^ deadzone, float range, float grace, bool mirror) {
	return Scalar::Create(NuiLibSafe::constrain(input->_ps, deadzone->_ps, range, grace, mirror));
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
Scalar ^Nui::constrain(Scalar ^input, Scalar^ deadzone, float range, Scalar^ grace, bool mirror) {
	return Scalar::Create(NuiLibSafe::constrain(input->_ps, deadzone->_ps, range, grace->_ps, mirror));
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
Scalar ^Nui::constrain(Scalar ^input, Scalar^ deadzone, Scalar ^range, float grace, bool mirror) {
	return Scalar::Create(NuiLibSafe::constrain(input->_ps, deadzone->_ps, range->_ps, grace, mirror));
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
Scalar ^Nui::constrain(Scalar ^input, Scalar^ deadzone, Scalar ^range, Scalar^ grace, bool mirror) {
	return Scalar::Create(NuiLibSafe::constrain(input->_ps, deadzone->_ps, range->_ps, grace->_ps, mirror));
}
///
/// Take the absolute value of a scalar.
/// @param input The scalar to take the absolute value of.
///
Scalar ^Nui::abs(Scalar ^input) {
	return Scalar::Create(NuiLibSafe::abs(input->_ps));
}
///
/// Take the arc cosine of a scalar.
/// @param input The scalar to take the arc cosine of.
///
Scalar ^Nui::acos(Scalar ^input) {
	return Scalar::Create(NuiLibSafe::acos(input->_ps));
}
///
/// -1 or 1 depending on a condition.
/// Value == *input ? -1.f : 1.f
/// Used to invert other values depending on a condition.
/// @param input The condition which dictates Value.
///
Scalar ^Nui::invert(Condition ^input) {
	return Scalar::Create(NuiLibSafe::invert(input->_ps));
}
///
/// The change in a scalar since the last update.
/// Will give the first derivitive of a scalar.
/// @param input The scalar to track changes in.
///
Scalar ^Nui::delta(Scalar ^input) {
	return Scalar::Create(NuiLibSafe::deltaScalar(input->_ps));
}
///
/// The scalar projection of one vector onto another.
/// a is projected onto b.
/// @param a The projecting vector.
/// @param b The vector being projected on.
///
Scalar ^Nui::project(Vector ^a, Vector ^b) {
	return Scalar::Create(NuiLibSafe::project(a->_ps, b->_ps));
}
///
/// Value is one of two inputs depending on a condition.
/// Value = *condition ? t : f);
/// @param condition Condition up which the output is dependant.
/// @param t Value == t if condition is true.
/// @param f Value == f if condition is false.
///
Scalar ^Nui::ifScalar(Condition ^condition, float t, float f) {
	return Scalar::Create(NuiLibSafe::ifScalar(condition->_ps, t, f));
}
///
/// Value is one of two inputs depending on a condition.
/// Value = *condition ? t : f);
/// @param condition Condition up which the output is dependant.
/// @param t Value == t if condition is true.
/// @param f Value == f if condition is false.
///
Scalar ^Nui::ifScalar(Condition ^condition, float t, Scalar ^f) {
	return Scalar::Create(NuiLibSafe::ifScalar(condition->_ps, t, f->_ps));
}
///
/// Value is one of two inputs depending on a condition.
/// Value = *condition ? a : b);
/// @param condition Condition up which the output is dependant.
/// @param a Value == a if condition is true.
/// @param b Value == b if condition is false.
///
Scalar ^Nui::ifScalar(Condition ^condition, Scalar ^t, float f) {
	return Scalar::Create(NuiLibSafe::ifScalar(condition->_ps, t->_ps, f));
}
///
/// Value is one of two inputs depending on a condition.
/// Value = *condition ? a : b);
/// @param condition Condition up which the output is dependant.
/// @param a Value == a if condition is true.
/// @param b Value == b if condition is false.
///
Scalar ^Nui::ifScalar(Condition ^condition, Scalar ^t, Scalar ^f) {
	return Scalar::Create(NuiLibSafe::ifScalar(condition->_ps, t->_ps, f->_ps));
}
///
/// Value is set by an opencv track bar.
/// Takes parameters to define how the integer value, starting at 0, that the tracker supplies is converted to a float.
/// @param title The name of the tracker bar.
/// @param max The maximum value the track bar can have.
/// @param min The minumum value the track bar can have.
/// @param value The initial value of the track bar.
///
Scalar ^Nui::tracker(String ^title, float max, float min, float value) {
	return Scalar::Create(new SafeScalar(value));
}









Vector ^Vector::operator+(Vector ^addend1, Vector ^addend2) {
	return Create(NuiLibSafe::sum((addend1->_ps), (addend2->_ps)));
}
Vector ^Vector::operator-(Vector ^minuend, Vector ^subtrahend) {
	return Create(NuiLibSafe::difference((minuend->_ps), (subtrahend->_ps)));
}
Vector ^Vector::operator*(Vector ^factor1, Vector ^factor2) {
	return Create(NuiLibSafe::product((factor1->_ps), (factor2->_ps)));
}
Vector ^Vector::operator/(Vector ^dividend, Vector ^divisor) {
	return Create(NuiLibSafe::quotient((dividend->_ps), (divisor->_ps)));
}
Vector ^Vector::operator+=(Vector ^addend1, Vector ^addend2) {
	return Create(NuiLibSafe::sum((addend1->_ps), (addend2->_ps)));
}
Vector ^Vector::operator-=(Vector ^minuend, Vector ^subtrahend) {
	return Create(NuiLibSafe::difference((minuend->_ps), (subtrahend->_ps)));
}
Vector ^Vector::operator*=(Vector ^factor1, Vector ^factor2) {
	return Create(NuiLibSafe::product((factor1->_ps), (factor2->_ps)));
}
Vector ^Vector::operator/=(Vector ^dividend, Vector ^divisor) {
	return Create(NuiLibSafe::quotient((dividend->_ps), (divisor->_ps)));
}


Vector ^Vector::operator+(Vector ^addend1, float addend2) {
	return Create(NuiLibSafe::sum((addend1->_ps), addend2));
}
Vector ^Vector::operator-(Vector ^minuend, float subtrahend) {
	return Create(NuiLibSafe::difference((minuend->_ps), subtrahend));
}
Vector ^Vector::operator*(Vector ^factor1, float factor2) {
	return Create(NuiLibSafe::product((factor1->_ps), factor2));
}
Vector ^Vector::operator/(Vector ^dividend, float divisor) {
	return Create(NuiLibSafe::quotient((dividend->_ps), divisor));
}
Vector ^Vector::operator+=(Vector ^addend1, float addend2) {
	return Create(NuiLibSafe::sum((addend1->_ps), addend2));
}
Vector ^Vector::operator-=(Vector ^minuend, float subtrahend) {
	return Create(NuiLibSafe::difference((minuend->_ps), subtrahend));
}
Vector ^Vector::operator*=(Vector ^factor1, float factor2) {
	return Create(NuiLibSafe::product((factor1->_ps), factor2));
}
Vector ^Vector::operator/=(Vector ^dividend, float divisor) {
	return Create(NuiLibSafe::quotient((dividend->_ps), divisor));
}

Vector ^Vector::operator+(float addend1, Vector ^addend2) {
	return Create(NuiLibSafe::sum(addend1, (addend2->_ps)));
}
Vector ^Vector::operator-(float minuend, Vector ^subtrahend) {
	return Create(NuiLibSafe::difference(minuend, (subtrahend->_ps)));
}
Vector ^Vector::operator*(float factor1, Vector ^factor2) {
	return Create(NuiLibSafe::product(factor1, (factor2->_ps)));
}
Vector ^Vector::operator/(float dividend, Vector ^divisor) {
	return Create(NuiLibSafe::quotient(dividend, (divisor->_ps)));
}


///
/// Normalize the value of a vector so its magnitude is 1.
/// @param vector The SafeVector to normalize.
/// @param max The maximum value to normalize against.
///
Vector ^Nui::normalize(Vector ^vector) {
	return Vector::Create(NuiLibSafe::normalize(vector->_ps));
}
///
/// Get the cross product of two vectors.
/// @param a The first vector.
/// @param b The second vector.
///
Vector ^Nui::cross(Vector ^a, Vector ^b) {
	return Vector::Create(NuiLibSafe::cross(a->_ps, b->_ps));
}
///
/// Take only specified axes from a vector.
/// @param input The vector to limit.
/// @param x Whether to take the x axis.
/// @param y Whether to take the y axis.
/// @param z Whether to take the z axis.
///
Vector ^Nui::limit(Vector ^input, bool x, bool y, bool z) {
	return Vector::Create(NuiLibSafe::limit(input->_ps, x, y, z));
}
///
/// The change in a vector since the last update.
/// Will give the first derivitive of a vector in all three axes.
/// @param input The vector to track changes in.
///
Vector ^Nui::delta(Vector ^input) {
	return Vector::Create(NuiLibSafe::delta(input->_ps));
}
///
/// Take a snapshot of a vector whenever a condition becomes true.
/// Every time *condition == true Value is set to vector.
/// @param vector The vector to take snapshots of.
/// @param condition Every time this becomes true a snapshot is taken.
Vector ^Nui::snapshot(Vector ^input, Condition ^condition) {
	return Vector::Create(NuiLibSafe::snapshot(input->_ps, condition->_ps));
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
Vector ^Nui::momentum(Vector ^input, Condition ^condition) {
	return Vector::Create(NuiLibSafe::momentum(input->_ps, condition->_ps));
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
Vector ^Nui::momentum(Vector ^input, Condition ^condition, float decrement, float threshold) {
	return Vector::Create(NuiLibSafe::momentum(input->_ps, condition->_ps, decrement, threshold));
}
///
/// Scale a vector so that its magnitude is scale.
/// @param input the vector to scale.
/// @param scale The magnitude Value is to have in the direction of input.
/// 
Vector ^Nui::scale(Vector ^input, Scalar ^scale) {
	return Vector::Create(NuiLibSafe::scale(input->_ps, scale->_ps));
}
///
/// Scale a vector so that its magnitude is scale.
/// @param input the vector to scale.
/// @param scale The magnitude Value is to have in the direction of input.
/// 
Vector ^Nui::scale(Vector ^input, float scale) {
	return Vector::Create(NuiLibSafe::scale(input->_ps, scale));
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
Vector ^Nui::intersect(Vector ^pPlane, Vector ^normalPlane, Vector ^pLine, Vector ^dirLine) {
	return Vector::Create(NuiLibSafe::intersect(pPlane->_ps, normalPlane->_ps, pLine->_ps, dirLine->_ps));
}
Vector ^Nui::joint(const int joint) {
	return Vector::Create(NuiLibSafe::joint(joint));
}






Condition ^Scalar::operator>(Scalar ^operand1, Scalar ^operand2) {
	return Condition::Create(NuiLibSafe::Greater(operand1->_ps, operand2->_ps));
}
Condition ^Scalar::operator<(Scalar ^operand1, Scalar ^operand2) {
	return Condition::Create(NuiLibSafe::Less(operand1->_ps, operand2->_ps));
}
Condition ^Scalar::operator>=(Scalar ^operand1, Scalar ^operand2) {
	return Condition::Create(NuiLibSafe::GreaterEqual(operand1->_ps, operand2->_ps));
}
Condition ^Scalar::operator<=(Scalar ^operand1, Scalar ^operand2) {
	return Condition::Create(NuiLibSafe::LessEqual(operand1->_ps, operand2->_ps));
}
Condition ^Scalar::operator==(Scalar ^operand1, Scalar ^operand2) {
	return Condition::Create(NuiLibSafe::Equal(operand1->_ps, operand2->_ps));
}
//Condition ^Vector::operator==(Vector ^operand1, Vector ^operand2) {
	//return Condition::Create(NuiLibSafe::Equal(operand1->_ps, operand2->_ps));
//}
Condition ^Scalar::operator!=(Scalar ^operand1, Scalar ^operand2) {
	return Condition::Create(NuiLibSafe::NotEqual(operand1->_ps, operand2->_ps));
}
Condition ^Vector::operator!=(Vector ^operand1, Vector ^operand2) {
	return Condition::Create(NuiLibSafe::NotEqual(operand1->_ps, operand2->_ps));
}
Condition ^Condition::operator&&(Condition ^operand1, Condition ^operand2) {
	return Create(NuiLibSafe::And(operand1->_ps, operand2->_ps));
}
Condition ^Condition::operator||(Condition ^operand1, Condition ^operand2) {
	return Create(NuiLibSafe::Or(operand1->_ps, operand2->_ps));
}
Condition ^Condition::operator!(Condition ^operand) {
	return Create(NuiLibSafe::Not(operand->_ps));
}
Condition ^Condition::And(Condition ^operand1, Condition ^operand2) {
	return Create(NuiLibSafe::And(operand1->_ps, operand2->_ps));
}
Condition ^Condition::Or(Condition ^operand1, Condition ^operand2) {
	return Create(NuiLibSafe::Or(operand1->_ps, operand2->_ps));
}

Condition ^Scalar::operator>(Scalar ^operand1, float operand2) {
	return Condition::Create(NuiLibSafe::Greater(operand1->_ps, operand2));
}
Condition ^Scalar::operator<(Scalar ^operand1, float operand2) {
	return Condition::Create(NuiLibSafe::Less(operand1->_ps, operand2));
}
Condition ^Scalar::operator>=(Scalar ^operand1, float operand2) {
	return Condition::Create(NuiLibSafe::GreaterEqual(operand1->_ps, operand2));
}
Condition ^Scalar::operator<=(Scalar ^operand1, float operand2) {
	return Condition::Create(NuiLibSafe::LessEqual(operand1->_ps, operand2));
}
Condition ^Scalar::operator==(Scalar ^operand1, float operand2) {
	return Condition::Create(NuiLibSafe::Equal(operand1->_ps, operand2));
}
//Condition ^Vector::operator==(Vector ^operand1, float operand2) {
	//return Create(NuiLibSafe::Equal(operand1->_ps, operand2));
//}
//Condition ^Vector::operator==(Vector ^operand1, cv::Point3f) {
	//return Create(NuiLibSafe::Equal(operand1->_ps, cv::Point3f));
//}
Condition ^Scalar::operator!=(Scalar ^operand1, float operand2) {
	return Condition::Create(NuiLibSafe::NotEqual(operand1->_ps, operand2));
}
Condition ^Vector::operator!=(Vector ^operand1, float operand2) {
	return Condition::Create(NuiLibSafe::NotEqual(operand1->_ps, operand2));
}
Condition ^Condition::operator&&(Condition ^operand1, bool operand2) {
	return Create(NuiLibSafe::And(operand1->_ps, operand2));
}
Condition ^Condition::operator||(Condition ^operand1, bool operand2) {
	return Create(NuiLibSafe::Or(operand1->_ps, operand2));
}
Condition ^Condition::And(Condition ^operand1, bool operand2) {
	return Create(NuiLibSafe::And(operand1->_ps, operand2));
}
Condition ^Condition::Or(Condition ^operand1, bool operand2) {
	return Create(NuiLibSafe::Or(operand1->_ps, operand2));
}

Condition ^Scalar::operator>(float operand1, Scalar ^operand2) {
	return Condition::Create(NuiLibSafe::Greater(operand1, operand2->_ps));
}
Condition ^Scalar::operator<(float operand1, Scalar ^operand2) {
	return Condition::Create(NuiLibSafe::Less(operand1, operand2->_ps));
}
Condition ^Scalar::operator>=(float operand1, Scalar ^operand2) {
	return Condition::Create(NuiLibSafe::GreaterEqual(operand1, operand2->_ps));
}
Condition ^Scalar::operator<=(float operand1, Scalar ^operand2) {
	return Condition::Create(NuiLibSafe::LessEqual(operand1, operand2->_ps));	
}
Condition ^Scalar::operator==(float operand1, Scalar ^operand2) {
	return Condition::Create(NuiLibSafe::Equal(operand1, operand2->_ps));	
}
//Condition ^Vector::operator==(float operand1, Vector ^operand2)  {
	//return Condition::Create(NuiLibSafe::Equal(operand1, operand2->_ps) ){
//}
//Condition ^Vector::operator==(cv::Point3f operand1, Vector ^operand2) {
	//return Condition::Create(NuiLibSafe::Equal(cv::Point3f operand1, Vector ^operand2));
//}
Condition ^Scalar::operator!=(float operand1, Scalar ^operand2) {
	return Condition::Create(NuiLibSafe::NotEqual(operand1, operand2->_ps));	
}
Condition ^Vector::operator!=(float operand1, Vector ^operand2) {
	return Condition::Create(NuiLibSafe::NotEqual(operand1, operand2->_ps));	
}
Condition ^Condition::operator&&(bool operand1, Condition ^operand2) {
	return Condition::Create(NuiLibSafe::And(operand1, operand2->_ps));	
}
Condition ^Condition::operator||(bool operand1, Condition ^operand2) {
	return Condition::Create(NuiLibSafe::Or(operand1, operand2->_ps));	
}
Condition ^Condition::And(bool operand1, Condition ^operand2) {
	return Condition::Create(NuiLibSafe::And(operand1, operand2->_ps));	
}
Condition ^Condition::Or(bool operand1, Condition ^operand2) {
	return Condition::Create(NuiLibSafe::Or(operand1, operand2->_ps));	
}
