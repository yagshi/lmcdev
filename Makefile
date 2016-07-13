LDFLAGS = -lopencv_calib3d -lopencv_core
CPPFLAGS = -lg
lmccore:	lmccore.cpp yagcquaternion.o

yagcquaternion.o:	yagcquaternion.cpp yagcquaternion.hpp
