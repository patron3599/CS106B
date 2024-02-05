/*
 * File: merge.cpp
 * ----------------
 * By: Christopher Patron
 * Project: CS 106B Assignment 3 - Merge
 * Date: 10/10/2022
 *
 * merge.cpp is a program that implements different methods
 * to solve for the merging of two queues storing ordered
 * integers. It demonstrates how powerful recursion combined
 * with a good algorithm can be to efficiently execute a large
 * and tedious task such as the process of mergining two queues
 * and sequentially ordering their respective values.
 */
#include <iostream>    // for cout, endl
#include "queue.h"
#include "SimpleTest.h"
using namespace std;

/* binaryMerge is passed two arbitrary queues containing integers.
 * It then takes the elements within the queues and sequentially
 * stores them in a new queue that is eventually returned. It also
 * contains safeguards in place to raise an error in case the provided
 * queues are not ordered.
 */
Queue<int> binaryMerge(Queue<int> a, Queue<int> b) {
    Queue<int> result;

    int temp_elem;
    while(!a.isEmpty() && !b.isEmpty()){
        int elem1 = a.peek();
        int elem2 = b.peek();
        if(elem1 > elem2){
            temp_elem = b.dequeue();
        } else{
            temp_elem = a.dequeue();
        }
        if (!a.isEmpty() && !b.isEmpty()){ // Verifying that the queues are ordered
            if(temp_elem > a.peek() || temp_elem > b.peek()){
                error("Queue arguments are not properly sorted.");
            }
        }
        result.enqueue(temp_elem);
    }

    while(!a.isEmpty()){ // Adding rest (if any) to new queue and also making sure it is still ordered
        int temp_elem = a.dequeue();
        if (!a.isEmpty() && temp_elem > a.peek()){
            error("Queue arguments are not properly sorted.");
        }
        result.enqueue(temp_elem);

    }

    while(!b.isEmpty()){ // Adding rest (if any) to new queue and also making sure it is still ordered
        int temp_elem = b.dequeue();
        if (!b.isEmpty() && temp_elem > b.peek()){
            error("Queue arguments are not properly sorted.");
        }
        result.enqueue(temp_elem);

    }

    return result;
}

/*
 * The function naiveMultiMerge assumes you have a correctly working
 * binaryMerge function, which this function calls to iteratively
 * merge a collection of sequences, starting off with an empty sequence
 * and repeatedly merging in an additional sequence until all are merged.
 * The code below is provided to you is implemented correctly and you
 * should not need to modify it.
 */
Queue<int> naiveMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;

    for (Queue<int>& q : all) {
        result = binaryMerge(q, result);
    }
    return result;
}

/*
 * recMultiMerge takes in a vector of queues containing ordered integers
 * as arguments. It then proceeds with a binary search algorithm to and
 * calls binaryMerge to return a queue containing the integers sequentially
 * from the vector.
 */
Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    int half = all.size() / 2;
    Vector<Queue<int>> left_half = all.subList(0, half);
    Vector<Queue<int>> right_half = all.subList(half, all.size()-half);

    // Base Case: The "half" has been reduced to size 1
    if (all.size() == 1){
        return all[0];
    }
    // Recursive Case: Continue halfing the vector
    else{
        Queue<int> result1 = recMultiMerge(left_half);
        Queue<int> result2 = recMultiMerge(right_half);
        Queue<int> result = binaryMerge(result1, result2);
        return result;

    }

}

/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

PROVIDED_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

PROVIDED_TEST("naiveMultiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

PROVIDED_TEST("recMultiMerge, compare to naiveMultiMerge") {
    int n = 20;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

PROVIDED_TEST("Time binaryMerge operation") {
    int n = 1000000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
}

PROVIDED_TEST("Time naiveMultiMerge operation") {
    int n = 11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));
}
PROVIDED_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}


// Test helper to fill queue with sorted sequence //
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].enqueue(input.dequeue());
    }
}

// STUDENT TESTS //

STUDENT_TEST("binaryMerge, uneven sequences") {
    Queue<int> a = {2, 4, 5, 7, 10};
    Queue<int> b = {3, 6, 11};
    Queue<int> expected = {2, 3, 4, 5, 6, 7, 10, 11};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

STUDENT_TEST("binaryMerge, with one empty queue") {
    Queue<int> a = {};
    Queue<int> b = {3, 6, 11};
    Queue<int> expected = {3, 6, 11};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

STUDENT_TEST("binaryMerge, with queue of size 1") {
    Queue<int> a = {2, 3, 4, 5, 6, 7, 8};
    Queue<int> b = {5};
    Queue<int> expected = {2, 3, 4, 5, 5, 6, 7, 8};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

STUDENT_TEST("binaryMerge, unordered sequences") {
    Queue<int> a = {2, 5, 4};
    Queue<int> b = {3, 6, 11};
    EXPECT_ERROR(binaryMerge(a, b));
}

STUDENT_TEST("Time binaryMerge operation") {
    int n = 20000000;
    for(int i = 0; i < 4; i++){
        Queue<int> a = createSequence(n);
        Queue<int> b = createSequence(n);
        TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
        n *= 2;
    }
}

STUDENT_TEST("naiveMultiMerge, on empty queues") {
    Vector<Queue<int>> all = {{},
                             {},
                             {},
                             };
    Queue<int> expected = {};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

STUDENT_TEST("naiveMultiMerge, on empty vector") {
    Vector<Queue<int>> all = {};
    Queue<int> expected = {};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

STUDENT_TEST("Time naiveMultiMerge operation with n fixed") {
    int n = 200000;
    int k = 10;
    for (int i = 1; i < 6; i++){
        k = k * i;
        Queue<int> input = createSequence(n);
        Vector<Queue<int>> all(k);
        distribute(input, all);
        TIME_OPERATION(input.size(), naiveMultiMerge(all));
    }
}

STUDENT_TEST("Time naiveMultiMerge operation with k fixed") {
    int n = 200000;
    int k = 10;
    for (int i = 1; i < 6; i++){
        n = n * 3;
        Queue<int> input = createSequence(n);
        Vector<Queue<int>> all(k);
        distribute(input, all);
        TIME_OPERATION(input.size(), naiveMultiMerge(all));
    }
}

STUDENT_TEST("recMultiMerge, on simple set") {
    Vector<Queue<int>> all = {{4},
                             {3, 4, 5},
                             {1},
                             {8, 8},
                             {5},
                             {9},
                             {1, 2},
                            };
    Queue<int> expected = {1, 1, 2, 3, 4, 4, 5, 5, 8, 8, 9};
    EXPECT_EQUAL(recMultiMerge(all), expected);
}

STUDENT_TEST("Comparing recMultiMerge and naiveMultiMerge on empty queues") {
    Vector<Queue<int>> all = {{},
                             {},
                             {},
                             };
    EXPECT_EQUAL(naiveMultiMerge(all), recMultiMerge(all));
}

STUDENT_TEST("Comparing recMultiMerge and naiveMultiMerge on short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    EXPECT_EQUAL(naiveMultiMerge(all), recMultiMerge(all));
}

STUDENT_TEST("Time recMultiMerge operation with n fixed") {
    int n = 200000;
    int k = 10;
    for (int i = 1; i < 6; i++){
        k = k * i * 4;
        Queue<int> input = createSequence(n);
        Vector<Queue<int>> all(k);
        distribute(input, all);
        TIME_OPERATION(input.size(), recMultiMerge(all));
    }
}

STUDENT_TEST("Time recMultiMerge operation with k fixed") {
    int n = 200000;
    int k = 10;
    for (int i = 1; i < 6; i++){
        n = n * 3;
        Queue<int> input = createSequence(n);
        Vector<Queue<int>> all(k);
        distribute(input, all);
        TIME_OPERATION(input.size(), recMultiMerge(all));
    }
}

STUDENT_TEST("recMultiMerge on sequence of 1,000,000") {
    int n = 1000000;
    Vector<Queue<int>> test;
    Queue<int> a = createSequence(n);
    test.add(a);
    TIME_OPERATION(a.size(), recMultiMerge(test));

}

