# c_programs
Some C programs I wrote while enjoying my university course about operating systems

1.) Main Functionality : hexdump.c : exdump a given File with arguments Pointer to FILE, length you want to dump and a                  char buffer, where you want the input to be stored.

2.) Main Functionality : shellsim.c : Simple Shell Simulation, which sends user input to the kernel, and terminates when
              input == "exit"

3.) Main Functionality : list.c : LinkedList Implementation with functionality of insert, append and remove. 

4.) Main Functionality : threadit.c : Generates multiple threads which count to a given parameter k. threadsched.c is a scheduler-simulation 
              for different thread scheduling algorithms like RoundRobin (RR), PriorityRoundRobin (PRR) or 
              ShortestRemainingNext (SRN)
              
5.) Main Functionality : syncem.c : Thread synchornization when accessing shared ressources.
 executing the program from the terminal you can give additional parameters: -n == number of threads, from 1 to including 10.
 -l == execute the wirte() syscalls in a critical section. -f == all outputs of a single file into a single critical section. Both -l and -f can´t be used at the same time. We are using a mutex for the critical sections.
