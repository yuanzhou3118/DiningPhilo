# DiningPhilo

### This project is about the "dinner philosopher problem".  
philo_sem1 demonstrates the deadlock problem of the "dinner philosopher problem".
```c
gcc -std=c99 -Wall -pthread -lrt -o philo_sem1 philo_sem1.c
        
./philo_sem1
```
philo_sem2-philo_sem4 demonstrates various techniques to prevent deadlocks in problems.
```c
gcc -std=c99 -Wall -pthread -lrt -o philo_sem2 philo_sem2.c
        
./philo_sem2


gcc -std=c99 -Wall -pthread -lrt -o philo_sem3 philo_sem3.c

./philo_sem3


gcc -std=c99 -Wall -pthread -lrt -o philo_sem4 philo_sem4.c

./philo_sem4
```