CFLAGS=-Wall 
OPENCVLIB=-lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio -lopencv_video -lopencv_videostab
CXX=g++
test: test_opencv.cpp
	$(CXX) -g $(OPENCVLIB) test_opencv.cpp