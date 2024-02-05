/*
 * File: perfect.cpp
 * ----------------
 * By: Christopher Patron
 * Project: CS 106B Assignment 1 - Perfect Numbers
 * Date: 10/02/2022
 *
 * Hello! I am a second year MS student studying structural engineering and this is my last quarter!
 * I'm taking this course to learn more about CS and to also help diversify my experience here at Stanford.
 * I think in general I learned a great deal of the c++ language with this warmup. I spent a good amount of
 * time on this section just simply searching up how to execute commands or smaller functions for the language.
 */

#include "console.h"
#include <iostream>
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;

/* The divisorSum function takes one argument `n` and calculates the
 * sum of proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division using the modulus operator %
 *
 * Note: the C++ long type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}

/* The isPerfect function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* The findPerfects function takes one argument `stop` and performs
 * an exhaustive search for perfect numbers over the range 1 to `stop`.
 * Each perfect number found is printed to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* smarterSum is an optimized version of the divisorSum function to
 * enchance program functionality. It recognizes the importance of
 * using the argument "n's" square root to find the divisors. To find divisors,
 * a loop iterates over all numbers from 1 to sqrt(n), testing for a
 * zero remainder from the division using the modulus operator %.
 */
long smarterSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor <= sqrt(n); divisor++) {
        if (n % divisor == 0 && ceil(sqrt(n)) == floor(sqrt(n))) { // If it's a perfect square
            total += divisor;
        }
        else if(n % divisor == 0){ // For all other cases
            total += divisor;
            total += n/divisor;
            }
    }
    if (n <= 1){ // For small numbers that can't produce a perfect number
        total = 0;
        return total;
    }
    else if(ceil(sqrt(n)) == floor(sqrt(n))){ // For the case that there is a perfect sqaure
        return total;
    }
    else{
        total -= n; // Fencepost issue since the divisor starts at 1
        return total;
    }
}

/* isPerfectSmarter is very similar to the original isPerfect function
 * except it now calls smarterSum instead of divisorSum. The function
 * takes one argument `n` and returns a boolean (true/false) value
 * indicating whether or not `n` is perfect. A perfect number is a
 * non-zero positive number whose sum of its proper divisors is equal to itself.
 */
bool isPerfectSmarter(long n) {
    return (n != 0) && (n == smarterSum(n));

}

/* findPerfectSmarter is very similar to the original
 * findPerfects function. The only differnece is that findPerfectSmarter
 * now calls isPerfectSmarter instead of isPerfect. The function takes
 * one argument `stop` and performs a search for perfect
 * numbers over the range 1 to `stop`. Each perfect number found is printed
 * to the console.
 */
void findPerfectsSmarter(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfectSmarter(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* findNthPerfectEuclid is an optimized function that helps find perfect numbers.
 * Using Mersenne prime numbers and mathematical theory developed by Euclid, this
 * program returns the perfect number corresponding to its "nth" place on the list
 * of perfect numbers. For example, n = 3 returns the value of 496 as it is in the
 * third spot after 6 and 28.
 */
long findNthPerfectEuclid(long n) {
    int counter = 0;
    int perf_number = 0;
    int m = 0;
    int k = 1;

    while(counter != n){
        m = (pow(2, k))-1;
        if (smarterSum(m) == 1){ // A perfect number has been found
            perf_number = pow(2, k-1)*(pow(2, k)-1);
            k += 1;
            counter += 1;
        }
        else {
            k += 1; // Continues looking for a perfect number
            }
    }
    return perf_number;
}


/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Time trial of findPerfects on input size 1000") {
    TIME_OPERATION(1000, findPerfects(1000));
}

// STUDENT_TESTS

STUDENT_TEST("Multiple time trials of findPerfects on increasing input sizes") {

    int smallest = 32000, largest = 256000;

    for (int size = smallest; size <= largest; size *= 2) {
        TIME_OPERATION(size, findPerfects(size));
    }
}

STUDENT_TEST("Testing runtime of isPerfect for n = {10, 1000}"){
    vector<int> nums = {10, 1000};
      for (int i = 0; i < nums.size(); i++){
          TIME_OPERATION(nums[i], findPerfects(nums[i]));
      }
}

STUDENT_TEST("Test negative values as inputs") {
    EXPECT(!isPerfect(-6));
    EXPECT(!isPerfect(-28));
    EXPECT(!isPerfect(-496));
    EXPECT(!isPerfect(-8128));
}

STUDENT_TEST("Testing smarterSum functionality"){
    EXPECT_EQUAL(divisorSum(-6), smarterSum(-6));
    EXPECT_EQUAL(divisorSum(496), smarterSum(496));
    EXPECT_EQUAL(divisorSum(-6), smarterSum(-6));
    EXPECT_EQUAL(divisorSum(25), smarterSum(25));
    EXPECT_EQUAL(divisorSum(1), smarterSum(1));
}


STUDENT_TEST("Multiple time trials of findPerfectSmarter on increasing input sizes") {

    int smallest = 512000, largest = 4096000;

    for (int size = smallest; size <= largest; size *= 2) {
        TIME_OPERATION(size, findPerfectsSmarter(size));
    }
}

STUDENT_TEST("Determine functionality of findNthPerfectEuclid") {
    EXPECT_EQUAL(findNthPerfectEuclid(2), 28);
    EXPECT(isPerfectSmarter(findNthPerfectEuclid(2)));
    EXPECT_NO_ERROR(findNthPerfectEuclid(3));
    EXPECT_EQUAL(findNthPerfectEuclid(5), 33550336);

}
