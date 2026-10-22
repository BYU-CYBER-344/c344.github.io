---
title: "HW 5: Synchronization Primitives"
---

In this lab you will start with a C program similar to the one we used in class. It launches two threads. One that prints the numbers from 0 to 25 and the other which prints the letters from A to Z. It has two issues that lead to non-deterministic results. One is that the letters and numbers can be interleaved since both threads are running at the same time. The other is that which thread starts first is indeterminate.

Of course, this is an artificial example. You could avoid all contention by not creating new threads and simply calling the two functions in order as follows:

```c
int main(int argc, char* argv[])
{
    print_numbers(NULL);
    print_letters(NULL);
    return 0;
}
```

However, the goal of this activity is to deliberately create thread contention and then use synchronization primitives to produce consistent results.

### References

You will use the following system functions in this assignment:

* [`pthread_mutex_lock()` reference](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3.html)
* [`pthread_mutex_unlock()` reference](https://man7.org/linux/man-pages/man3/pthread_mutex_unlock.3.html)
* [`sem_init()` reference](https://man7.org/linux/man-pages/man3/sem_init.3.html)
* [`sem_wait()` reference](https://man7.org/linux/man-pages/man3/sem_wait.3.html)
* [`sem_post()` reference](https://man7.org/linux/man-pages/man3/sem_post.3.html)
* [`waitpid()` reference](https://man7.org/linux/man-pages/man2/waitpid.2.html)

## A: Download, compile, and run the sample.

Download the starter source code from here: [threads.c](/HW-5/threads.c){: download="threads.c"}

Compile and run the program:

```sh
gcc threads.c
./a.out
```

Run it several times. You should see numbers and letters appearing in different orders and sometimes interleaved. However, since thread order is indeterminate, you may find that it does the same thing every time or most of the time.

## B: Use a mutex to ensure that numbers and letters are not interleaved.

Remember from class that a mutex, sometimes called a lock, is used to ensure exclusive access to a resource. In this case, `print_numbers` and `print_letters` should use a common mutex to take exclusive control of terminal output (by way of `printf`) for the duration of their loop.

You create and initialize a mutex with the following code:
```c
pthread_mutex_t g_terminal_mutex = PTHREAD_MUTEX_INITIALIZER;
```

You should declare this as a global variable before any functions thereby ensuring that both thread functions have access to it. Of course, you can name the variable (`g_terminal_mutex`) anything you want.

You lock a mutex with the following code:
```c
pthread_mutex_lock(&g_terminal_mutex);
```

And you unlock it with this code:
```c
pthread_mutex_unlock(&g_terminal_mutex);
```

Lock the mutex at the beginning of `print_numbers` and unlock it at the end of of that function (**right before the `return`**). Do the same with `print_letters`.

Compile the program and run the it multiple times. You should see that the letters and numbers appear in different orders but that they are never interleaved.

## C: Use semaphores to trigger each thread to run in order.

In class we used a semaphore as an alternative to a mutex but semaphores can be used in many ways.

When you create a semaphore you give it an initial integer value. When you `wait` on a semaphore then the thread will block so long as the semaphore is zero. When you `post` to a semaphore you increment its value by one thereby releasing one thread that is waiting on it. If multiple threads are waiting, only one waiting thread will be released because as the thread is released the value decrements by one.

So, in class we initialized the semaphore with a 1 and when a thread waited on the semaphore that worked like locking a mutex. And when we posted to the semaphore it was like unlocking a mutex.

Here, we will use semaphores as signals rather than locks. In this case we will initialize a semaphore with a zero. Then a post to the semaphore is an indication that the thread should be released.

You declare a semaphore with this code:
```c
sem_t g_example_semaphore;
```

And you initialize it with this code. In this case we are initializing it to zero (the last argument).
```c
sem_init(&g_example_semaphore, 0, 0);
```

You wait until a semaphore is non-zero with this code:
```c
sem_wait(&g_example_semaphore);
```

You increment a semaphore's value with this code:
```c
sem_post(&g_example_semaphore);
```

1. Declare two semaphores called `g_numbers_semaphore` and `g_letters_semaphore` as global variables.
2. At the beginning of `main()`, initialize both semaphores with a value of zero.
3. At the beginning of `print_numbers()`, before you lock the mutex, `wait` for the `g_numbers_semaphore`.
4. At the beginning of `print_letters()`, before you lock the mutex, `wait` for the `g_letters_semaphore`.
5. After creating the threads, but before waiting for them to complete, `post` to the `g_numbers_semaphore` to release that thread.
6. At the end of `print_numbers()`, after unlocking the mutex, `post` to the `g_letters_semaphore` to release that thread.

Compile the program and run the it multiple times. It should produce consistent results every time.

## D: Answer a question in a comment.

Insert a comment toward the beginning of your program and answer the following question:

* With the semaphores in place, is the mutex necessary?

## Submission and Scoring

To complete this homework, **insert a comment at the top of your source code with your name, the date, and the title of the homework.** Then submit the source to LearningSuite.

Scoring is as follows:

* [2 Points] Name, Date, and Homework Title in a comment
* [5 Points] Your program compiles and prints numbers, then letters with no interleaving.
* [5 Points] You use mutexes appropriately to ensure numbers and letters are not interleaved.
* [5 Points] You use semaphores to trigger the start of print_numbers and print_letters.
* [8 Points] Your answer to the question in Part D.
* [5 points] Code is clean and well-formatted.

