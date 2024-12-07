## Introduction

This project is a rough attempt at simulating the core logic of a modern CPU task scheduler, while utilizing basic C++ functions to achieve this goal. This project is aimed to understand the algorithm many operating system kernels (e.g. Windows, Linux, and Unix) implement under the hood to seamlessly switch between innumerable user-space programs depending on their levels of priority to give us the typical smooth experience of using a modern computing system.

## How to compile and run the program

For the sake of convenience, this project includes a `Makefile` in the root directory. Simply typing in the following in a typical Unix-like command shell (Terminal) will be sufficient to compile and run the program, and even clean up afterwards to save up on a little bit of disk space ;D

```shell
$ make
$ ./scheduler ./data/Datafile.txt
============================= FIFO Scheduler =============================
Arrival time	CPU Burst length	Priority 	Completion Times
    10  		 22  		 1  			   32
    68  		 12  		 6  			   80
    98  		 34  		 8  			  132
   148  		 28  		10  			  176
   192  		  3  		 3  			  195
   223  		  8  		 3  			  231
   266  		 18  		 2  			  284
   268  		 14  		10  			  298
   268  		 30  		 4  			  328
   296  		 10  		 7  			  338
   354  		  6  		 3  			  360
   375  		  2  		 8  			  377
   377  		 31  		 2  			  408
   406  		 10  		 4  			  418
   457  		 21  		 9  			  478
   469  		  1  		10  			  479
   529  		  9  		 6  			  538
   559  		 12  		 5  			  571
   581  		 39  		 1  			  620
   630  		 17  		 8  			  647

=================== SJF Scheduler (Without Preemption) ===================
Arrival time	CPU Burst length	Priority 	Completion Times
    10  		 22  		 1  			   32
    68  		 12  		 6  			   80
    98  		 34  		 8  			  132
   148  		 28  		10  			  176
   192  		  3  		 3  			  195
   223  		  8  		 3  			  231
   266  		 18  		 2  			  284
   268  		 14  		10  			  298
   268  		 30  		 4  			  338
   296  		 10  		 7  			  308
   354  		  6  		 3  			  360
   375  		  2  		 8  			  377
   377  		 31  		 2  			  408
   406  		 10  		 4  			  418
   457  		 21  		 9  			  478
   469  		  1  		10  			  479
   529  		  9  		 6  			  538
   559  		 12  		 5  			  571
   581  		 39  		 1  			  620
   630  		 17  		 8  			  647

================== PRIORITY Scheduler (With Preemption) ==================
Arrival time	CPU Burst length	Priority 	Completion Times
    10  		 22  		 1  			   32
    68  		 12  		 6  			   80
    98  		 34  		 8  			  132
   148  		 28  		10  			  176
   192  		  3  		 3  			  195
   223  		  8  		 3  			  231
   266  		 18  		 2  			  338
   268  		 14  		10  			  282
   268  		 30  		 4  			  322
   296  		 10  		 7  			  306
   354  		  6  		 3  			  360
   375  		  2  		 8  			  377
   377  		 31  		 2  			  418
   406  		 10  		 4  			  416
   457  		 21  		 9  			  479
   469  		  1  		10  			  470
   529  		  9  		 6  			  538
   559  		 12  		 5  			  571
   581  		 39  		 1  			  620
   630  		 17  		 8  			  647
```

...And just as shown above, the program will take a file populated with data that describes an inflow of programs being sent to the scheduler's queue to be executed, and output the results of the simulation in a tabular format (truncated in the case shown above) - for each of the three basic scheduling algorithms implemented in our rudimentary project.

Also, the program can of course be modified to show the full output instead of a truncated version with a simple command-line argument being appended at the end, as shown in the following example:

```shell
$ ./scheduler ./data/Datafile.txt --display-all
============================= FIFO Scheduler =============================
Displaying All Processes:

Arrival time	CPU Burst length	Priority 	Completion Times
    10  		 22  		 1  			   32
    68  		 12  		 6  			   80
    98  		 34  		 8  			  132
   148  		 28  		10  			  176
   192  		  3  		 3  			  195
   223  		  8  		 3  			  231
   266  		 18  		 2  			  284
   268  		 14  		10  			  298
   268  		 30  		 4  			  328
   296  		 10  		 7  			  338
   ...           ...        ...               ... 
   ...           ...        ...               ... 
   ...           ...        ...               ... 

=================== SJF Scheduler (Without Preemption) ===================
Displaying All Processes:

Arrival time	CPU Burst length	Priority 	Completion Times
    10  		 22  		 1  			   32
    68  		 12  		 6  			   80
    98  		 34  		 8  			  132
   148  		 28  		10  			  176
   192  		  3  		 3  			  195
   223  		  8  		 3  			  231
   266  		 18  		 2  			  284
   268  		 14  		10  			  298
   268  		 30  		 4  			  338
   296  		 10  		 7  			  308
   354  		  6  		 3  			  360
   ...           ...        ...               ... 
   ...           ...        ...               ... 
   ...           ...        ...               ... 

================== PRIORITY Scheduler (With Preemption) ==================
Displaying All Processes:

Arrival time	CPU Burst length	Priority 	Completion Times
    10  		 22  		 1  			   32
    68  		 12  		 6  			   80
    98  		 34  		 8  			  132
   148  		 28  		10  			  176
   192  		  3  		 3  			  195
   223  		  8  		 3  			  231
   266  		 18  		 2  			  338
   268  		 14  		10  			  282
   268  		 30  		 4  			  322
   296  		 10  		 7  			  306
   354  		  6  		 3  			  360
   375  		  2  		 8  			  377
   377  		 31  		 2  			  418
  ...           ...        ...               ... 
  ...           ...        ...               ... 
  ...           ...        ...               ... 
```

*NOTE: Even though the `--display-all` flag at the end looks like a typical command-line flag that can appended anywhere to work properly when executing the compiled program, in truth it strictly needs to be appended at the 2nd argument position to work as intended (refer to the source code in `./src/main.cpp`).*

After verifying that everything works as intended and the simulation outputs match our expectations, we run `make clean` to remove the build files and the binary executable from the project root directory to keep everything nice and tidy. 

## Final Conclusion

As a matter of course, actual CPU schedulers working at the core of every major Operating System have slightly different behavior and much higher complexity compared to the schedulers simulated by this rudimentary program. 

However, that does not mean the core of them is any different. 

This program helps us to understand the core methodology used by modern CPU schedulers working at the heart of Operating System kernels, and also helps us better manage any user-space programs we write as programmers and developers -- to better take advantage of the modern computing hardware in specific mission-critical applications. 

...Or to simply optimize our programs a little more in order to run better on low-computing-power machines.
