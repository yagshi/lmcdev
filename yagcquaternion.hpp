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
  void print(std::ostream &os = std::cout);
};
