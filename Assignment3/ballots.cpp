/*
 * File: ballots.cpp
 * ----------------
 * By: Christopher Patron
 * Project: CS 106B Assignment 3 - Ballots
 * Date: 10/10/2022
 *
 * ballots.cpp is a program that tallies the ballots for two candidates,
 * A and B. It keeps track of the possible orderings of the votes and
 * also calculates how many good orderings there are.
 */
#include "recursion.h"
#include "SimpleTest.h"
using namespace std;

/*** Function Prototypes ***/
int countAllOrderings(int a, int b);
int countAllOrderings(int a, int b, int& num_orderings);
int countGoodOrderings(int a, int b);
int countGoodOrderings(int a, int b, int& num_orderings, int& original_a, int& original_b);

/* countAllOrderings is passed in two integers representing the
 * number of votes for candidates A and B. It then proceeds to
 * calculate the number of possible orderings for tallying an
 * election ballot.
 */
int countAllOrderings(int a, int b) { // Wrapper Function
    int num_orderings = 0;
    if (a > 0) {
        countAllOrderings(a, b, num_orderings);
    }
    return num_orderings;
}

int countAllOrderings(int a, int b, int& num_orderings) { // Introduce a counter into the function

    // Base Case: Candidate A or B no longer have votes to be tallied and so
    //            a combination of votes has been found
    if (a == 0 || b == 0){
        num_orderings ++;
        return 0;
    }

    // Recursive Case: Subtract a vote from A or B and continue
    else{
        countAllOrderings(a - 1, b, num_orderings);
        countAllOrderings(a, b - 1, num_orderings);
    }

    return 0;
}

/* countGoodOrderings is passed in two integers representing the
 * number of votes for candidates A and B. It then calculates the
 * number of good orderings for tallying an election. A good ordering
 * is one where the eventual winner is always in the lead.
 */
int countGoodOrderings(int a, int b) { // Wrapper Function
    int original_a = a;
    int original_b = b;
    int num_orderings = 0;
    if (a > 0){
        countGoodOrderings(a, b, num_orderings, original_a, original_b);
    }
    return num_orderings;
}

int countGoodOrderings(int a, int b, int& num_orderings, int& original_a, int& original_b) {

    // Base Case: Candidate A or B no longer have votes to be tallied and so
    //            a combination of votes has been found
    if (a == 0 || b == 0){
        num_orderings ++;
        return 0;
    }

    // Recursive Case: Subtract a vote from A or B and continue
    else {
    countGoodOrderings(a - 1, b, num_orderings, original_a, original_b);
        if((original_a - a) > (original_b - b + 1)){ // Comparing how many A's have been used to how many B's + 1
                                                     // are left since we always need A to be greater than B.
            countGoodOrderings(a, b - 1, num_orderings, original_a, original_b);
        }
    }
    return 0;
}



/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("countAllOrderings, two A one B") {
   EXPECT_EQUAL(countAllOrderings(2, 1), 3);
}

PROVIDED_TEST("countGoodOrderings, two A one B") {
   EXPECT_EQUAL(countGoodOrderings(2, 1), 1);
}

// STUDENT TESTS //

STUDENT_TEST("countAllOrderings, four A three B") {
   EXPECT_EQUAL(countAllOrderings(3, 2), 10);
}

STUDENT_TEST("countAllOrderings, five A zero B (Testing immediate base case)") {
   EXPECT_EQUAL(countAllOrderings(5, 0), 1);
}

STUDENT_TEST("countAllOrderings, zero A two B (Testing immediate base case)") {
   EXPECT_EQUAL(countAllOrderings(0, 2), 0);
}

STUDENT_TEST("countAllOrderings, two A seven B") {
   EXPECT_EQUAL(countAllOrderings(2, 7), 36);
}

STUDENT_TEST("countGoodOrderings, three A two B") {
   EXPECT_EQUAL(countGoodOrderings(3, 2), 2);
}

STUDENT_TEST("countGoodOrderings, four A one B") {
   EXPECT_EQUAL(countGoodOrderings(4, 1), 3);
}

STUDENT_TEST("Bertrand's Formula"){
    for (int a = 1; a < 12; a++){
        for (int b = 0; b < a; b++){
            int expr_1 = countGoodOrderings(a, b) / countAllOrderings(a, b);
            int expr_2 = (a - b) / (a + b);
            EXPECT_EQUAL(expr_1, expr_2);
        }
    }

}
