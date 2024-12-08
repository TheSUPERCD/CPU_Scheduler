make:
	g++ -Wall -O3 src/main.cpp src/plist.cpp -o scheduler

clean:
	rm -f scheduler
