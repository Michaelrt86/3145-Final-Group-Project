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

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    long long int number_of_tosses;
    long long int number_in_circle = 0;
    
    printf("Enter the number of tosses: ");
    scanf("%lld", &number_of_tosses);

    #pragma omp parallel
    {
        unsigned int seed = omp_get_thread_num();
        long long int local_number_in_circle = 0;

        #pragma omp for
        for (long long int toss = 0; toss < number_of_tosses; toss++) {
            double x = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
            double y = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
            double distance_squared = x * x + y * y;
            if (distance_squared <= 1) {
                local_number_in_circle++;
            }
        }

        #pragma omp atomic
        number_in_circle += local_number_in_circle;
    }

    double pi_estimate = 4.0 * (double)number_in_circle / (double)number_of_tosses;
    printf("Estimated value of pi: %f\n", pi_estimate);

    return 0;
}
