class YagCQuaternion {
public:
  double w, x, y, z;
  YagCQuaternion();
  YagCQuaternion(double aW, double aX, double aY, double aZ);
  YagCQuaternion operator +(YagCQuaternion q);
  YagCQuaternion operator -(YagCQuaternion q);
  YagCQuaternion operator *(YagCQuaternion q);
  YagCQuaternion operator -();
  YagCQuaternion conj();
  static YagCQuaternion fromRMat(double r11, double r12, double r13,
				 double r21, double r22, double r23,
				 double r31, double r32, double r33);
  void print(std::ostream &os = std::cout);
};
