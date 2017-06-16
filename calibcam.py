#!/usr/bin/python
# -*- coding: utf-8 -*-
# calibrateCamera だけをやるスクリプト
# 行単位でスペース区切りで数字を書く
# object, image はひとかたまりで一行。object と image と size の間に空行
# 1: object points0  x0 y0 z0 x1 y1 z1 ...
# 2: object points1  x0 y0 z0 x1 y1 z1 ...
# 3: 空行
# 4: image points0   x0 y0 x1 y1 x2 y2 ...
# 5: image points1   x0 y0 x1 y1 x2 y2 ...
# 6: 空行
# 7: image size     w h

import cv2
import numpy as np
import sys


while True:
    objPoints = []  # リストのリスト
    imgPoints = []  # リストのリスト
    
    # object points
    while True:
        p = []
        try:
            a = raw_input()
        except EOFError:
            break
        xyz = a.split()
        if len(xyz) == 0:
            break
        while len(xyz) > 0:
            p.append([float(xyz[0]), float(xyz[1]), float(xyz[2])])
            xyz = xyz[3:]
        objPoints.append(np.array(p, 'float32'))

    # image points
    while True:
        p = []
        try:
            a = raw_input()
        except EOFError:
            break
        xy = a.split()
        if len(xy) == 0:
            break
        while len(xy) > 0:
            p.append([float(xy[0]), float(xy[1])])
            xy = xy[2:]
        imgPoints.append(np.array(p, 'float32'))

    # image size
    try:
        a = raw_input()
    except EOFError:
        break
    size = []
    for wh in a.split():
        size.append(int(wh))

#    camMat = np.array([[f, 0, size[0] / 2], [0, f, size[1] / 2], [0, 0, 1]], 'float32')
    
    retVal, camMat, distCoefs, rVecs, tVecs = cv2.calibrateCamera(objPoints, imgPoints, tuple(size))

    print "pos: %f %f %f" % (tVecs[0][0], tVecs[0][1], tVecs[0][2])
    print distCoefs
    print "%3.8f %3.8f %3.8f" % (camMat[0][0], camMat[0][1], camMat[0][2])
    print "%3.8f %3.8f %3.8f" % (camMat[1][0], camMat[1][1], camMat[1][2])
    print "%3.8f %3.8f %3.8f" % (camMat[2][0], camMat[2][1], camMat[2][2])
    print "#"
    sys.stdout.flush()
