#!/usr/bin/ruby
# coding: utf-8

require 'opencv'
include OpenCV

calibcam = open("|./calibcam.py", "r+")

w = GUI::Window.new("calibration")

allCorners = []
objPoints = []
7.times {|y|
  10.times {|x|
    objPoints.push(CvPoint3D32f.new(x * 0.024, -y * 0.024, 0))
  }
}


while f = ARGV.shift
  print "loading #{f}..."
  img = CvMat.load(f, CV_LOAD_IMAGE_COLOR)
  puts "done."
  corners, found = img.find_chessboard_corners(CvSize.new(10, 7))
  puts "# of cornders #{corners.size}"
  if found
    corners.each {|c|
      img.circle!(c, 5, :color => CvColor::Red, :thickness => 1)
    }
    corners = img.BGR2GRAY.find_corner_sub_pix(corners, CvSize.new(11, 11), CvSize.new(-1, -1), CvTermCriteria.new(20, 0.001))
    allCorners.push([])
    cPrev = nil
    corners.each {|c|
      img.circle!(c, 3, :color => CvColor::Yellow, :thickness => 1)
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
    calibcam.puts "%d %d" % img.size
    calibcam.each_line {|l|
      break if l =~ /^\s*#/
      puts l
    }
  else
    puts "no chessboard found."
  end
  w.show img
  k = GUI::wait_key(0)
  puts "----"
end
