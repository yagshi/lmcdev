#include <iostream>
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
