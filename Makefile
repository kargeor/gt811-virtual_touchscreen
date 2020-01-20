gt811-virtual_touchscreen: main.cpp GT811.h
	g++ main.cpp -lwiringPi -o gt811-virtual_touchscreen

GT811.h: GT811/GT811.h
	tail -n +3 GT811/GT811.h >GT811.h

