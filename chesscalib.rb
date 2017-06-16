#!/usr/bin/ruby
# coding: utf-8

require 'opencv'
include OpenCV
Kwidth = 1280
Kheight = 1080

calibcam = open("|./calibcam.py", "r+")

cap = CvCapture.open
cap.size = CvSize.new(Kwidth, Kheight)
w = GUI::Window.new("calibration")

allCorners = []
objPoints = []
7.times {|y|
  10.times {|x|
    objPoints.push(CvPoint3D32f.new(x * 0.024, -y * 0.024, 0))
  }
}


while true
  cap.grab
  img = cap.query
  k = GUI::wait_key(100)
  case k
  when ' '.ord
    corners = img.find_chessboard_corners(CvSize.new(10, 7))
    if corners[0].size == 70
      allCorners.push([])
      cPrev = nil
      corners[0].each {|c|
        img.circle!(c, 4, :color => CvColor::Yellow, :thickness => 2)
        allCorners.last.push(c)
        if cPrev != nil
          img.line!(cPrev, c, :thickness => 1, :color => CvColor::Blue)
        end
        cPrev = c
      }
      # calibratecamera する
      allCorners.size.times {|i|
        objPoints.each {|p|
          calibcam.print "%f %f %f " % p
        }
        calibcam.puts
      }
      calibcam.puts
      allCorners.each {|c|
        c.each {|p|
          calibcam.print "%f %f " % p
        }
        calibcam.puts
      }
      calibcam.puts
      calibcam.puts "%d %d" % [Kwidth, Kheight]
      calibcam.each_line {|l|
        break if l =~ /^\s*#/
        puts l
      }
    end
  end
  w.show img
end
