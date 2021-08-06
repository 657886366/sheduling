# sheduling
You are to implement the Partial Ordering, dependency graph, and scheduling problem. This project is a bit more coding to 
write than those of your previous project; therefore, it is a 12 pts project, please start early!  Early submission earns extra pt.

There are four options (listed below) for constructing a scheduling table (as well as given in the Lecture Notes and in 
the supplemental guide.)  In this specs, nodes and jobs are the same thing and used interchangeable. 

Option 1:  using limited processors and all nodes/jobs have equal (constant) processing time.
Option 2:  using un-limited processors and all nodes/jobs have equal (constant) processing time
Option 3:  using limited processors and nodes/jobs may have different processing time
Option 4:  using un-limited processors and nodes/job may have different processing time

In this project, you will implement an algorithm that combines the above four options into one program without writing 
four different methods to handle the four options. The algorithm steps (abstract) is given hereafter.  

You will be given 2 sets of test data to test the correctness of your program, where each set includes two files -- 
a file contains the dependency graph (a list of partial relations) and a file contains processing time for each node.  The number of processors to be used is given by you via command-line.

