/**
  Question 5.2 Dart Problem
  Suppose we toss darts randomly at a square dartboard, whose bullseye is at the origin, and whose sides are 2 feet in 
  length. Suppose also that there’s a circle inscribed in the square dartboard. The radius of the circle is 1 foot, 
  and it’s area is π square feet. If the points that are hit by the darts are uniformly distributed (and we always 
  hit the square), then the number of darts that hit inside the circle should approximately satisfy the equation
  since the ratio of the area of the circle to the area of the square is π/4.

  We can use this formula to estimate the value of π with a random number generator: 

  number in circle = 0;
for (toss = 0; toss < number of tosses; toss++)
{
x = random double between −1 and 1;
y = random double between −1 and 1;
distance squared = x∗x + y∗y;
if (distance squared <= 1)
{
  number in circle++;
}
pi estimate = 4∗number in circle/((double) number of tosses);

This is called a “Monte Carlo” method, since it uses randomness (the dart tosses). Write an OpenMP program that uses a 
Monte Carlo method to estimate π. Read in the total number of tosses before forking any threads. Use a reduction clause 
to find the total number of darts hitting inside the circle. Print the result after joining all the threads. You may 
want to use long long ints for the number of hits in the circle and the number of tosses, since both may have to be 
very large to get a reasonable estimate of π.

**/

/* File:  
 *    Problem-1.c
 *
 * Purpose:  
 *    To calculate pi using a dart board in which we track how many darts hit within a circle that is within a box. The box's side length is equal to the circle's diameter.
 *
 * Compile:
 *    gcc -g -Wall -fopenmp -o Problem-1 Problem-1.c
 * Run:
 *    ./Problem-1
 *
 * Input:
 *    An integer
 * Output:
 *    The estimated value of pi
 *    The number of darts within the cirlce
 */

#include <stdio.h>
#include <stdlib.h>

#ifdef _OPENMP
  #include <omp.h>
//This creates a thread number so the function can read through and 
  int threadNumber = omp_get_thread_num();
# else
  int threadNumber = 1;
#endif

double random_double(unsigned int *seed);
int is_inside_circle(double x, double y);
double pi_estimation(long long int num_tosses);

int main() {
    long long int num_tosses;
    long long int numInCircle = 0;
    printf("Enter the number of tosses: ");
    scanf("%lld", &num_tosses);

    double pi_estimate = pi_estimation(num_tosses);
    printf("Estimated value of pi: %f\n", pi_estimate);
    printf("Number of Darts within circle: %d", numInCircle);
    return 0;
}

// Randon number function for x and y
double random_double(unsigned int *seed) {
    return (double)rand_r(seed) / RAND_MAX * 2.0 - 1.0;
}

// Checks the value to see if the dart is in the circle
int is_inside_circle(double x, double y) {
    return x * x + y * y <= 1.0;
}

// Does the calculation to estimate pi based on the provided values 
double pi_estimation(long long int num_tosses) {
    long long int localInCircle = 0;

    #pragma omp parallel reduction(+:localInCircle)
    {
        unsigned int seed = threadNumber;

        #pragma omp for
        for (long long int toss = 0; toss < num_tosses; toss++) {
            double x = random_double(&seed);
            double y = random_double(&seed);

            if (is_inside_circle(x, y)) {
                localInCircle++;
                numInCircle++;
            }
        }
    }

    return 4.0 * (double)localInCircle / (double)num_tosses;
}
