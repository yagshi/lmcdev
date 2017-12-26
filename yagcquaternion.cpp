#include <iostream>
#include <cmath>
#include "yagcquaternion.hpp"

YagCQuaternion::YagCQuaternion() {};
YagCQuaternion::YagCQuaternion(double aW, double aX, double aY, double aZ) {
    w = aW;
    x = aX;
    y = aY;
    z = aZ;
}

YagCQuaternion YagCQuaternion::operator +(YagCQuaternion q) {
  YagCQuaternion ret(w + q.w, x + q.x, y + q.y, z + q.z);
  return ret;
}

YagCQuaternion YagCQuaternion::operator *(YagCQuaternion q) {
  YagCQuaternion ret(w * q.w - x * q.x - y * q.y - z * q.z,
		     x * q.w + w * q.x + y * q.z - z * q.y,
		     y * q.w + w * q.y + z * q.x - x * q.z,
		     z * q.w + w * q.z + x * q.y - y * q.x);
  return ret;
}

void YagCQuaternion::print(std::ostream &os) {
  os << w << " " << x << " " << y << " " << z << std::endl;
}

YagCQuaternion YagCQuaternion::operator -() {
  YagCQuaternion ret(-w, -x, -y, -z);
  return ret;
}

YagCQuaternion YagCQuaternion::operator -(YagCQuaternion q) {
  YagCQuaternion ret(w - q.w, x - q.x, y - q.y, z - q.z);
  return ret;
}

YagCQuaternion YagCQuaternion::conj() {
  YagCQuaternion ret(w, -x, -y, -z);
  return ret;
}

// 3x3 回転行列から Quaternion を得る。
// O'reilly のゲーム数学の本を参考にしているが
// あっちは座標が列ベクトルなので以下の計算は全部転置している点に注意
YagCQuaternion YagCQuaternion::fromRMat(double r11, double r12, double r13,
					double r21, double r22, double r23,
					double r31, double r32, double r33) {
  double w = sqrt(1 + r11 + r22 + r33) / 2;
  double x = sqrt(1 + r11 - r22 - r33) / 2;
  double y = sqrt(1 - r11 + r22 - r33) / 2;
  double z = sqrt(1 - r11 - r22 + r33) / 2;
  double rw, rx, ry, rz;   // return values
  int index = 0;  // 0, 1, 2, and 3 mean w, x, y, and z
  double max = w;
  if (x > max) {
    max = x;
    index = 1;
  }
  if (y > max) {
    max = y;
    index = 2;
  }
  if (z > max) {
    max = z;
    index = 3;
  }
  switch (index) {
  case 0:
    rw = w;
    rx = (r32 - r23) / w / 4;
    ry = (r13 - r31) / w / 4;
    rz = (r21 - r12) / w / 4;
    break;
  case 1:
    rw = (r32 - r23) / x / 4;
    rx = x;
    ry = (r21 + r12) / x / 4;
    rz = (r13 + r31) / x / 4;
    break;
  case 2:
    rw = (r13 - r31) / y / 4;
    rx = (r21 + r12) / y / 4;
    ry = y;
    rz = (r32 + r23) / y / 4;
    break;
  case 3:
    rw = (r21 - r12) / z / 4;
    rx = (r13 + r31) / z / 4;
    ry = (r32 + r23) / z / 4;
    rz = z;
  }
  return YagCQuaternion(rw, rx, ry, rz);
}
