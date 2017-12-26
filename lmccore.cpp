// 座標変換の計算をする
// カレントディレクトリにカメラパラメタ設定ファイルを
// camparam.dat として置いておくこと。
//
// 入力は標準入力から以下のとおり
//
// QR コード 4 隅の座標 tlx tly trx try blx bly brx bry (空白区切りで一行)
// QR コードデコード文字列
// 空行

#define KCameraParameterFileName "camparam.dat"

#include <iostream>
#include <fstream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include "yagcquaternion.hpp"

using namespace std;

cv::Mat camMat(3, 3, CV_64FC1);
cv::Mat distCoeffs(1, 5, CV_64FC1);

// 空白《を》読み飛ばす
char* skipWhiteSpace(char *p) {
  while (*p == ' ' || *p == '\t') p++;
  return p;
}
// 空白《まで》読み飛ばす
char *skipToWhiteSpace(char *p) {
  while (*p != ' ' && *p != '\t') p++;
  return p;
}
// 指定文字まで読み飛ばす
char *skipTo(char *p, char ch) {
  while (*p != ch && *p != 0) p++;
  return p;
}

// 軸まわりの回転クォータニオンを作る
YagCQuaternion qx(double th) 
{
  YagCQuaternion q(cos(th / 2), sin(th / 2), 0, 0);
  return q;
}

YagCQuaternion qy(double th) 
{
  YagCQuaternion q(cos(th / 2), 0, sin(th / 2), 0);
  return q;
}

YagCQuaternion qz(double th) 
{
  YagCQuaternion q(cos(th / 2), 0, 0, sin(th / 2));
  return q;
}
  
  




// カメラパラメタファイルを cam, dist に 読み込む
void readCamParam(cv::Mat &cam, cv::Mat &dist) {
  ifstream ifs;
  char buf[1024];
  ifs.open(KCameraParameterFileName, ifstream::in);
  if (!ifs) {
    cerr << "Can't open camera parameter file\n";
    exit(-1);
  }
  for (int row = 0; !ifs.eof();) {
    ifs.getline(buf, 1024);
    char *ptr = skipWhiteSpace(buf);
    if (*ptr == '#') continue;
    if (*ptr == 0) continue;
    if (row < 3) {
      double f0, f1, f2;
      sscanf(buf, "%lf %lf %lf", &f0, &f1, &f2);
      //    camMat.at<CV_64FC1>(0,0)[0];
      cam.at<double>(row, 0) = f0;
      cam.at<double>(row, 1) = f1;
      cam.at<double>(row, 2) = f2;
    } else {
      double f0, f1, f2, f3, f4;
      sscanf(buf, "%lf %lf %lf %lf %lf", &f0, &f1, &f2, &f3, &f4);
      dist.at<double>(0, 0) = f0;
      dist.at<double>(0, 1) = f1;
      dist.at<double>(0, 2) = f2;
      dist.at<double>(0, 3) = f3;
      dist.at<double>(0, 4) = f4;
    }
    row++;
  }
}

void parseLMC(istream &ifs, cv::Point3d &qrPos, vector<cv::Point3d> &obj, string &rotOrder, double rotAngle[]) {
  char buf[1024];
  double len;

  while (ifs){
    ifs.getline(buf, 1024);
    char *ptr = skipWhiteSpace(buf);
    if (*ptr == 0) continue;
    if (strncmp("pos:", ptr, 4) == 0) {
      // 中心座標読み込み
      ptr += strlen("pos:");
      qrPos.x = atof(ptr);
      ptr = skipWhiteSpace(skipTo(ptr, ',') + 1);
      qrPos.y = atof(ptr);
      ptr = skipWhiteSpace(skipTo(ptr, ',') + 1);
      qrPos.z = atof(ptr);
      continue;
    }
    if ((tolower(*ptr) == 'x' || tolower(*ptr) == 'y' || tolower(*ptr) == 'z')
	&& ((tolower(*(ptr + 1)) == 'x'
	     || tolower(*(ptr + 1)) == 'y'
	     || tolower(*(ptr + 1)) == 'z'))
	&& ((tolower(*(ptr + 2)) == 'x'
	     || tolower(*(ptr + 2)) == 'y'
	     || tolower(*(ptr + 2)) == 'z'))) {
      // オイラー角読み込み
      rotOrder += tolower(*ptr);
      rotOrder += tolower(*(ptr + 1));
      rotOrder += tolower(*(ptr + 2));
      ptr = skipWhiteSpace(skipTo(ptr, ':') + 1);
      rotAngle[0] = atof(ptr);
      ptr = skipWhiteSpace(skipTo(ptr, ',') + 1);
      rotAngle[1] = atof(ptr);
      ptr = skipWhiteSpace(skipTo(ptr, ',') + 1);
      rotAngle[2] = atof(ptr);
      continue;
    }
    if (strncmp("len:", ptr, 4) == 0) {
      // サイズ読み込み
      len = atof(ptr + 4);
      continue;
    }
  }
  obj[0].x = len / 2;
  obj[0].y = len / 2;
  obj[0].z = 0;
  obj[1].x = len / 2;
  obj[1].y = -len / 2;
  obj[1].z = 0;
  obj[2].x = -len / 2;
  obj[2].y = len / 2;
  obj[2].z = 0;
  obj[3].x = -len / 2;
  obj[3].y = -len / 2;
  obj[3].z = 0;
}


int main(int argc, char *argv[]) {
  readCamParam(camMat, distCoeffs);

  string rotOrder = "";
  double rotAngle[3];
  vector<cv::Point3d> objPoints(4);
  vector<cv::Point2d> imgPoints(4);
  cv::Point3d qrPos;

  // 4 頂点のスクリーン座標読み込み
  double tlX, tlY, trX, trY, blX, blY, brX, brY;
  cin >> tlX >> tlY >> trX >> trY >> blX >> blY >> brX >> brY;
  imgPoints[0].x = tlX;
  imgPoints[0].y = tlY;
  imgPoints[1].x = trX;
  imgPoints[1].y = trY;
  imgPoints[2].x = blX;
  imgPoints[2].y = blY;
  imgPoints[3].x = brX;
  imgPoints[3].y = brY;

  
  parseLMC(cin, qrPos, objPoints, rotOrder, rotAngle);
  cv::Mat rvec, tvec, rmat, camPos, camDir, rmatTr;
  cv::Mat unitVector = (cv::Mat_<double>(3, 1) << 0, 0, 1);
  solvePnP(objPoints, imgPoints, camMat, distCoeffs, rvec, tvec);
  Rodrigues(rvec, rmat);
  cv::transpose(rmat, rmatTr);
  camPos = rmatTr * (-tvec);
  camDir = rmatTr * unitVector;
  // {QR} -> {W} 座標変換
  YagCQuaternion q(1, camPos.at<double>(0), camPos.at<double>(1), camPos.at<double>(2));
  YagCQuaternion q2(1, camDir.at<double>(0), camDir.at<double>(1), camDir.at<double>(2));
  YagCQuaternion q3(1, 0, 0, 0);
  YagCQuaternion qq2w(1, 0, 0, 0);  // {Q}->{W} 変換
  YagCQuaternion q4                 // {Q} -> {cam}
    = YagCQuaternion::fromRMat(rmat.at<double>(0, 0), rmat.at<double>(0, 1), rmat.at<double>(0, 2),
			       rmat.at<double>(1, 0), rmat.at<double>(1, 1), rmat.at<double>(1, 2),
			       rmat.at<double>(2, 0), rmat.at<double>(2, 1), rmat.at<double>(2, 2)
			       );
  for (int i = 0; i < 3; i++) {
    YagCQuaternion qtmp;
    switch (rotOrder[i]) {
    case 'x':
      qtmp = qx(rotAngle[i] * M_PI / 180.0);
      break;
    case 'y':
      qtmp = qy(rotAngle[i] * M_PI / 180.0);
      break;
    case 'z':
      qtmp = qz(rotAngle[i] * M_PI / 180.0);
      break;
    }
    qq2w = qtmp * qq2w;
  }
  YagCQuaternion qq2wconj = qq2w.conj();
  YagCQuaternion wCamAttitude = qq2w * (q4.conj());
  q = qq2w * q * qq2wconj;
  q2 = qq2w * q2 * qq2wconj;
  cout << qrPos.x + q.x << " "
       << qrPos.y + q.y << " "
       << qrPos.z + q.z << endl;
  cout << wCamAttitude.w << " " << wCamAttitude.x << " " << wCamAttitude.y << " " << wCamAttitude.z << endl;
  return 0;
}
