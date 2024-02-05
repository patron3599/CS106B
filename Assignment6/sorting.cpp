/*
 * File: sorting.cpp
 * ----------------
 * By: Christopher Patron
 * Project: CS 106B Assignment 6 - Sorting
 * Date: 11/15/2022
 *
 * sorting.cpp is a program that employs the runSort and quickSort algorithms
 * to reorder the nodes of a linked list into sorted order.
 */
#include "listnode.h"
#include "vector.h"
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;

/* * * FUNCTION PROTOTYPES * * */
ListNode* splitRun(ListNode*& front, ListNode*& remainder);
ListNode* merge(ListNode*& l1_front, ListNode*& l2_front);
ListNode* partition(ListNode*& front, ListNode*& less_list, ListNode*& greater_list, ListNode*& equal_list);
void concatenate(ListNode*& front_list, ListNode*& back_list);
bool isSorted(ListNode* front);

/* runSort is an algorithm based on continuously merging
 * sorted runs, building the result from the bottom up.
 */
void runSort(ListNode*& front) {
    ListNode* result = nullptr;
    if (isSorted(front)){ // We have been passed in a sorted list
        result = front;
        return;
    }

    ListNode* remainder = front;

    while(remainder -> next != nullptr){
        front = splitRun(front, remainder);
        if (remainder -> next == nullptr){
            result = merge(result, front);
            break;
        }
        result = merge(result, front);
        front = remainder;
    }

    if(remainder -> next == nullptr){
        result = merge(result, remainder);
        front = result;
    }
}

/* splitRun detaches a sorted run from the front of a given list
 * and returns the run. A run is defined as a sorted contiguous
 * sublist.
 */
ListNode* splitRun(ListNode*& front, ListNode*& remainder){
    ListNode* run = front;
    if(front == nullptr){
        return front;
    }
    while(run -> next != nullptr){
        int prev = run -> data;
        remainder = remainder -> next;
        int curr = remainder -> data;

        if(curr < prev){
            run -> next = nullptr;
            run = front;
            break;
        }
        run = run -> next;
    }
    run = front; // If the whole list is sorted, run needs to be reassigned to front
    return run;
}

/* merge is passed in two sorted lists and produces a single sorted
 * list by comparing the front-most elements of both lists.
 */
ListNode* merge(ListNode*& l1_front, ListNode*& l2_front){
    if(l1_front == nullptr){
        ListNode* result = l2_front;
        return result;
    }
    else if(l2_front == nullptr){
        ListNode* result = l1_front;
        return result;
    }
    else{
        ListNode* result = nullptr;
        ListNode* curr_l1 = l1_front;
        ListNode* curr_l2 = l2_front;
        int l1_empty = -1; // -1 = Nodes Left , 1 = No Nodes left
        int l2_empty = -1; // -1 = Nodes Left , 1 = No Nodes left

        // Initialize the result root node to minimum value of the lists
        int val1 = curr_l1 -> data;
        int val2 = curr_l2 -> data;
        int min_val = min(val1, val2);
        if(min_val == val1){
            result = curr_l1;
            if(curr_l1 -> next != nullptr){
                curr_l1 = curr_l1 -> next;
            }
        }
        else {
            result = curr_l2;
            if(curr_l2 -> next != nullptr){
                curr_l2 = curr_l2 -> next;
            }
        }
        ListNode* tail = result;

        // While one of the lists last node is not a null pointer, build result
        while (curr_l1 -> next != nullptr && curr_l2 -> next != nullptr){
            val1 = curr_l1 -> data;
            val2 = curr_l2 -> data;
            min_val = min(val1, val2);
            if(min_val == val1){
                tail -> next = curr_l1;
                tail = tail -> next;
                curr_l1 = curr_l1 -> next;
            }
            else {
                tail -> next = curr_l2;
                tail = tail -> next;
                curr_l2 = curr_l2 -> next;
            }
        }

        // Second list has a single node and First list has nodes left, proceed with sorting
        while (curr_l1 -> next != nullptr){
            tail -> next = curr_l1;
            if(curr_l2 -> data < tail -> next -> data && l2_empty == -1){
                tail -> next = curr_l2;
                tail = tail -> next;
                tail -> next = curr_l1;
                l2_empty = 1;
            }
            tail = tail -> next;
            curr_l1 = curr_l1 -> next;
        }

        // First list has a single node and Second list has nodes left, proceed with sorting
        while (curr_l2 -> next != nullptr and l2_empty == -1){
            tail -> next = curr_l2;
            if(curr_l1 -> data < tail -> next -> data && l1_empty == -1){
                tail -> next = curr_l1;
                tail = tail -> next;
                tail -> next = curr_l2;
                l1_empty = 1;
            }
            else{
                tail = tail -> next;
                curr_l2 = curr_l2 -> next;
            }
        }

        // Add final element(s) of either list 1 or list 2 to result list
        if(l1_empty == -1 && l2_empty == -1){ // Both list 1 and list 2 have a single node
            val1 = curr_l1 -> data;
            val2 = curr_l2 -> data;
            min_val = min(val1, val2);
            if(min_val == val1){
                tail -> next = curr_l1;
                tail = tail -> next;
                tail -> next = curr_l2;
                tail = tail -> next;
                return result;
            }
            else {
                tail -> next = curr_l2;
                tail = tail -> next;
                tail -> next = curr_l1;
                tail = tail -> next;
                return result;
            }
        }
        else if(l1_empty == -1){ // List 1 has the last node
            tail -> next = curr_l1;
            tail = tail -> next;
            return result;
        }
        else if(l2_empty == -1){ // List 2 has the last node
            tail -> next = curr_l2;
            tail = tail -> next;
            return result;
        }
        return result;
    }
}

/* quickSort applies a divide and conquer algorithm where it takes
 * in a list, partitions it into three sublists, sorts them recursively,
 * and then concatenates them at the end.
 */
void quickSort(ListNode*& front) {  
    ListNode* less_list = nullptr;
    ListNode* greater_list = nullptr;
    ListNode* equal_list = nullptr;

    // Base Case: The passed in list is sorted
    if(isSorted(front)){
        return;
    }

    // Recursive Case: First sort the less list and then the greater list
    partition(front, less_list, greater_list, equal_list);
    quickSort(less_list);
    quickSort(greater_list);
    concatenate(less_list, equal_list);
    concatenate(less_list, greater_list);
    front = less_list;
}

ListNode* partition(ListNode*& front, ListNode*& less_list, ListNode*& greater_list, ListNode*& equal_list){
    ListNode* curr = front;
    ListNode* temp_curr = front;
    int pivot = front -> data;
    int ll_tail = 1; // 1 = tail, -1 = will be added as a temp root
    int gl_tail = 1; // 1 = tail, -1 = will be added as a temp root
    int el_tail = 1; // 1 = tail, -1 = will be added as a temp root

    // Add pivot to equal list before starting to loop through the rest of the list
    equal_list = front;
    temp_curr = temp_curr ->next;
    curr = curr -> next;
    equal_list -> next = nullptr;
    el_tail = -1;
    less_list = nullptr;
    greater_list = nullptr;

    if(curr == nullptr){ //Singelton list case
        front = nullptr;
        return front;
    }

    // Traversing List
    while(curr -> next != nullptr){
        curr = curr -> next;
        int value = temp_curr -> data;

        // Fill in less list
        if (value < pivot){
            if (ll_tail == 1){
                less_list = temp_curr;
                temp_curr = temp_curr -> next;
                less_list -> next = nullptr;
                ll_tail = -1;
            }
            else{
                ListNode* switch_pntr = less_list;
                less_list = temp_curr;
                less_list -> next = switch_pntr;
                temp_curr = curr;
            }
        }

        // Fill in greater than list
        else if (value > pivot){
            if (gl_tail == 1){
                greater_list = temp_curr;
                temp_curr = temp_curr -> next;
                greater_list -> next = nullptr;
                gl_tail = -1;
            }
            else{
                ListNode* switch_pntr = greater_list;
                greater_list = temp_curr;
                greater_list -> next = switch_pntr;
                temp_curr = curr;
            }
        }

        // Fill in equal list
        else if (value == pivot){
            if (el_tail == 1){
                equal_list = temp_curr;
                temp_curr = temp_curr -> next;
                equal_list -> next = nullptr;
                el_tail = -1;
            }
            else{
                ListNode* switch_pntr = equal_list;
                equal_list = temp_curr;
                equal_list -> next = switch_pntr;
                temp_curr = curr;
            }
        }
    }

    // Arrange last element in the passed in list
    int value = temp_curr -> data;
    if (value < pivot){
        if (ll_tail == 1){
            less_list = temp_curr;
            temp_curr = temp_curr -> next;
            less_list -> next = nullptr;
            ll_tail = -1;
        }
        else{
            ListNode* switch_pntr = less_list;
            less_list = temp_curr;
            less_list -> next = switch_pntr;
            temp_curr = curr;
        }
    }
    else if (value > pivot){
        if (gl_tail == 1){
            greater_list = temp_curr;
            temp_curr = temp_curr -> next;
            greater_list -> next = nullptr;
            gl_tail = -1;
        }
        else{
            ListNode* switch_pntr = greater_list;
            greater_list = temp_curr;
            greater_list -> next = switch_pntr;
            temp_curr = curr;
        }
    }
    else if (value == pivot){
        if (el_tail == 1){
            equal_list = temp_curr;
            temp_curr = temp_curr -> next;
            equal_list -> next = nullptr;
            el_tail = -1;
        }
        else{
            ListNode* switch_pntr = equal_list;
            equal_list = temp_curr;
            equal_list -> next = switch_pntr;
            temp_curr = curr;
        }
    }
    front = nullptr;
    return front;
}

/* concatenate takes in two linked lists designated as front and back.
 * It traverses the front list to find its last node and then points
 * that node to the start of back.
 */
void concatenate(ListNode*& front_list, ListNode*& back_list){
    ListNode* curr = front_list;

    if(front_list == nullptr){ // Front list is empty check
        front_list = back_list;
        return;
    }
    if(back_list == nullptr){ // Back list is empty check
        return;
    }

    while(curr -> next != nullptr){
        curr = curr -> next;
    }
    curr -> next = back_list;
}

/* * * * * * Test Code Below This Point * * * * * */

/* TODO: Write your own student tests below to test your sorting helper
 * functions and overall sorting algorithm correctness. We have provided
 * the skeleton of some tests below to help you get started with this
 * process.
 */

/*
 * We have provided this utility function as a convenience. This
 * function traverses the linked list represented by the provided
 * front pointer and prints out the data stored in the list along
 * the way in a convenient format. You may find it to be helpful to
 * use this function as a debugging aid.
 */
void printList(ListNode* front) {
   cout << "{";
   for (ListNode *cur = front; cur != nullptr; cur = cur->next) {
       cout << cur->data;
       if (cur->next != nullptr) {
           cout << ", ";
       }
   }
   cout << "}" << endl;
}

/*
 * This utility function deallocates the memory for all the nodes in a
 * given linked list. It can be used to recycle the memory allocated
 * during a test case and avoid memory leaks.
 */
void deallocateList(ListNode* front) {
    if (front == nullptr){
        delete front;
    }
    else{
        ListNode* current = nullptr;
        while(front -> next != nullptr){
            current = front;
            front = front -> next;
            delete current;
        }
        delete front;
    }
}

/*
 * This utility function is given a vector of values and constructs
 * an equivalent linked list containing the same values in the same order.
 * It can be used to construct a linked list to use as an input to
 * a function being tested.
 */
ListNode* createList(Vector<int> values) {
    if(values.size() == 0){ // Empty list case
        return nullptr;
    }

    ListNode* front = new ListNode(values[0], nullptr);
    ListNode* current = front;

    for (int i = 1; i < values.size(); i++){
        current -> next = new ListNode(values[i], nullptr);
        current = current -> next;
    }
    return front;
}

/*
 * This utility function compares a linked list and a vector for
 * equivalence and returns true if both contain the same values in the
 * same order. It can be used to confirm a linked list's contents match
 * the expected.
 */
bool areEquivalent(ListNode* front, Vector<int> v) {
    ListNode* current = front;
    if(front == nullptr and v.size() == 0){ // Empty list case
        return true;
    }

    if(front == nullptr and v.size() != 0){ // Empty list case
        return false;
    }

    int ll_size = 1;

    // Checking that the size of the linked list and vector are the same
    while(current -> next != nullptr){
        ll_size++;
        current = current -> next;
    }
    if(ll_size != v.size()){
        return false;
    }

    // Resetting current pointer
    current = front;
    for(int vec_elem : v){
        int ll_elem = current -> data;
        if (vec_elem != ll_elem){
            return false;
        }
       current = current -> next;
    }
    return true;
}

/*
 * This utility function is given a linked list and verifies if
 * it is sorted or not. It is borrowed from the CS106B Section 6:
 * Question 6 solution - All out of Sorts.
 */
bool isSorted(ListNode* front) {
    if (front != nullptr) {
        ListNode* current = front;
        while (current->next != nullptr) {
            if (current->data > current->next->data) {
                return false;
            }
            current = current->next;
        }
    }
    return true;
}

///* * * PROVIDED TESTS * * *///
STUDENT_TEST("This is a skeleton to demonstrate an end-to-end student test.") {
    /* Initialize vector to sequence of values */
    Vector<int> values = {1, 2, 3, 4};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    runSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

PROVIDED_TEST("Confirm correctness of test utility functions.") {
    /* Manually construct testList 1 -> 2 -> 3 */
    ListNode* testList = nullptr;
    testList = new ListNode(3, testList);
    testList = new ListNode(2, testList);
    testList = new ListNode(1, testList);

    /* Test equality comparison function. */
    EXPECT(areEquivalent(testList, {1, 2, 3}));
    EXPECT(!areEquivalent(testList, {1, 2}));
    EXPECT(!areEquivalent(testList, {1, 2, 3, 4}));
    EXPECT(!areEquivalent(testList, {1, 3, 2}));

    /* Test create list function. */
    ListNode* studentList = createList({1, 2, 3, 4});
    printList(studentList);
    ListNode* cur = studentList;
    for (int i = 1; i <= 4; i++) {
        EXPECT(cur != nullptr);
        EXPECT_EQUAL(cur->data, i);
        cur = cur->next;
    }

    /* Test deallocate list functions. There should be no memory leaks from this test. */
    deallocateList(studentList);
    deallocateList(testList);
}

///*
// * The provided test case demonstrates a comparsion between the same
// * algorithm (quicksort) operating on a linked list versus a vector.
// */
PROVIDED_TEST("Time linked list quicksort vs vector quicksort") {
    int startSize = 50000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n-1; i >= 0; i--) {
            v[i] = randomInteger(-10000, 10000);
            list = new ListNode(v[i], list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(n, quickSort(list));
        TIME_OPERATION(n, v.sort());        /* Standard vector sort operation is backed
                                               with quicksort algorithm. */

        deallocateList(list);
    }
}

///* * * STUDENT TESTS * * *///
STUDENT_TEST("Testing splitRun functionality on basic list"){
    ListNode* front = createList({3, 5, 4});
    ListNode* run = front;
    ListNode* remainder = front;

    Vector<Vector<int>> run_values = {{3, 5}, {4}};

    for (int i = 0; i < run_values.size(); i++){
        splitRun(front, remainder);
        EXPECT(areEquivalent(run, run_values[i]));

        deallocateList(front);
        front = remainder;
        run = front;
    }


}

STUDENT_TEST("Testing splitRun functionality on list in assignment"){
    ListNode* front = createList({3, 5, 5, 4, 10, 4});
    ListNode* remainder = front;

    Vector<Vector<int>> run_values = {{3, 5, 5}, {4, 10}, {4}};

    for (int i = 0; i < run_values.size(); i++){
        splitRun(front, remainder);
        EXPECT(areEquivalent(front, run_values[i]));

        deallocateList(front);
        front = remainder;
    }

}

STUDENT_TEST("Testing splitRun functionality on a sorted list"){
    ListNode* front = createList({1, 2, 3, 4, 5, 6});
    ListNode* remainder = front;

    Vector<Vector<int>> run_values = {{1, 2, 3, 4, 5 ,6}};

    for (int i = 0; i < run_values.size(); i++){
        splitRun(front, remainder);
        EXPECT(areEquivalent(front, run_values[i]));

        deallocateList(front);
        front = remainder;
    }

}

STUDENT_TEST("Testing splitRun functionality on a reverse sorted list"){
    ListNode* front = createList({5, 4, 3, 2, 1});
    ListNode* remainder = front;

    Vector<Vector<int>> run_values = {{5}, {4}, {3}, {2}, {1}};

    for (int i = 0; i < run_values.size(); i++){
        splitRun(front, remainder);
        EXPECT(areEquivalent(front, run_values[i]));

        deallocateList(front);
        front = remainder;
    }

}

STUDENT_TEST("Testing splitRun functionality on a singleton list"){
    ListNode* front = createList({5});
    ListNode* remainder = front;

    Vector<Vector<int>> run_values = {{5}};

    for (int i = 0; i < run_values.size(); i++){
        splitRun(front, remainder);
        EXPECT(areEquivalent(front, run_values[i]));

        deallocateList(front);
        front = remainder;
    }

}

STUDENT_TEST("Testing splitRun functionality on a duplicated list"){
    ListNode* front = createList({1, 1, 1, 1});
    ListNode* remainder = front;

    Vector<Vector<int>> run_values = {{1, 1, 1, 1}};

    for (int i = 0; i < run_values.size(); i++){
        splitRun(front, remainder);
        EXPECT(areEquivalent(front, run_values[i]));

        deallocateList(front);
        front = remainder;
    }

}

STUDENT_TEST("Testing splitRun functionality on an empty list"){
    ListNode* front = createList({});
    ListNode* remainder = front;

    Vector<int> answer = {};

    splitRun(front, remainder);
    EXPECT(areEquivalent(front, answer));

    deallocateList(front);
}

STUDENT_TEST("Testing merge functionality on basic lists"){
    ListNode* l1_front = createList({1, 2, 3});
    ListNode* l2_front = createList({4, 5, 6});
    ListNode* result;


    Vector<int> result_values = {1, 2, 3, 4, 5, 6};

    result = merge(l1_front, l2_front);
    EXPECT(areEquivalent(result, result_values));

    deallocateList(result);
}

STUDENT_TEST("Testing merge functionality on uneven lists"){
    ListNode* l1_front = createList({1, 4, 8});
    ListNode* l2_front = createList({2, 6, 9, 10, 11, 12});
    ListNode* result;


    Vector<int> result_values = {1, 2, 4, 6, 8, 9, 10, 11, 12};

    result = merge(l1_front, l2_front);
    EXPECT(areEquivalent(result, result_values));

    deallocateList(result);
}

STUDENT_TEST("Testing merge functionality on uneven lists"){
    ListNode* l2_front = createList({1, 4, 8});
    ListNode* l1_front = createList({2, 6, 9, 10, 11, 12});
    ListNode* result;


    Vector<int> result_values = {1, 2, 4, 6, 8, 9, 10, 11, 12};

    result = merge(l1_front, l2_front);
    EXPECT(areEquivalent(result, result_values));

    deallocateList(result);
}


STUDENT_TEST("Testing merge functionality on lists w/ even and odd values"){
    ListNode* l1_front = createList({-5, 3, 6});
    ListNode* l2_front = createList({-3, -1, 8});
    ListNode* result;


    Vector<int> result_values = {-5, -3, -1, 3, 6, 8};

    result = merge(l1_front, l2_front);
    EXPECT(areEquivalent(result, result_values));

    deallocateList(result);
}

STUDENT_TEST("Testing merge functionality on lists w/ even and odd values"){
    ListNode* l2_front = createList({-5, 3, 6});
    ListNode* l1_front = createList({-3, -1, 8});
    ListNode* result;

    Vector<int> result_values = {-5, -3, -1, 3, 6, 8};

    result = merge(l1_front, l2_front);
    EXPECT(areEquivalent(result, result_values));

    deallocateList(result);
}

STUDENT_TEST("Testing merge functionality on lists w/ even, odd values, and unbalanced list"){
    ListNode* l1_front = createList({-5, 3, 6, 13});
    ListNode* l2_front = createList({-3, -1, 8, 9, 12});
    ListNode* result;

    Vector<int> result_values = {-5, -3, -1, 3, 6, 8, 9, 12, 13};

    result = merge(l1_front, l2_front);
    EXPECT(areEquivalent(result, result_values));

    deallocateList(result);
}

STUDENT_TEST("Testing merge functionality on lists w/ even, odd values, and unbalanced list"){
    ListNode* l1_front = createList({-1, 8, 11, 12, 15});
    ListNode* l2_front = createList({-6});
    ListNode* result;

    Vector<int> result_values = {-6, -1, 8, 11, 12, 15};

    result = merge(l1_front, l2_front);
    EXPECT(areEquivalent(result, result_values));

    deallocateList(result);
}

STUDENT_TEST("Testing runSort functionality on basic list"){
    ListNode* front = createList({1, 2, 5, 4, 3});
    Vector<int> result_values = {1, 2, 3, 4, 5};

    runSort(front);
    EXPECT(areEquivalent(front, result_values));

    deallocateList(front);
}

STUDENT_TEST("Testing runSort functionality on basic list"){
    ListNode* front = createList({15, -1, 8, 11, 12, -6});
    Vector<int> result_values = {-6, -1, 8, 11, 12, 15};

    runSort(front);
    EXPECT(areEquivalent(front, result_values));

    deallocateList(front);
}

STUDENT_TEST("Testing runSort functionality on basic list"){
    ListNode* front = createList({15, -1, 8, -7, 12, -8, 0, 8});
    Vector<int> result_values = {-8, -7, -1, 0, 8, 8, 12, 15};

    runSort(front);
    EXPECT(areEquivalent(front, result_values));

    deallocateList(front);
}

STUDENT_TEST("Testing partition functionality on list"){
    ListNode* front = createList({15, -1, 20, 14, 15, 36});
    Vector<int> less_values = {14, -1};
    Vector<int> greater_values = {36, 20};
    Vector<int> equal_values = {15, 15};

    ListNode* less_list = nullptr;
    ListNode* greater_list = nullptr;
    ListNode* equal_list = nullptr;

    partition(front, less_list, greater_list, equal_list);

    EXPECT(areEquivalent(less_list, less_values));
    EXPECT(areEquivalent(greater_list, greater_values));
    EXPECT(areEquivalent(equal_list, equal_values));

    deallocateList(front);
    deallocateList(less_list);
    deallocateList(greater_list);
    deallocateList(equal_list);
}

STUDENT_TEST("Testing partition functionality on 2 node list"){
    ListNode* front = createList({16, 1});
    Vector<int> less_values = {1};
    Vector<int> greater_values = {};
    Vector<int> equal_values = {16};

    ListNode* less_list = nullptr;
    ListNode* greater_list = nullptr;
    ListNode* equal_list = nullptr;

    partition(front, less_list, greater_list, equal_list);

    EXPECT(areEquivalent(less_list, less_values));
    EXPECT(areEquivalent(greater_list, greater_values));
    EXPECT(areEquivalent(equal_list, equal_values));

    deallocateList(front);
    deallocateList(less_list);
    deallocateList(greater_list);
    deallocateList(equal_list);
}

STUDENT_TEST("Testing partition functionality on singleton node list"){
    ListNode* front = createList({16});
    Vector<int> less_values = {};
    Vector<int> greater_values = {};
    Vector<int> equal_values = {16};

    ListNode* less_list = nullptr;
    ListNode* greater_list = nullptr;
    ListNode* equal_list = nullptr;

    partition(front, less_list, greater_list, equal_list);

    EXPECT(areEquivalent(less_list, less_values));
    EXPECT(areEquivalent(greater_list, greater_values));
    EXPECT(areEquivalent(equal_list, equal_values));

    deallocateList(front);
    deallocateList(less_list);
    deallocateList(greater_list);
    deallocateList(equal_list);
}

STUDENT_TEST("Testing concatenate functionality on simple lists"){
    ListNode* list_1 = createList({1, 2, 3});
    ListNode* list_2 = createList({4, 5, 6});
    ListNode* list_3 = createList({7, 8, 9});

    concatenate(list_1, list_2);
    concatenate(list_1, list_3);
    Vector<int> list_values = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    EXPECT(areEquivalent(list_1, list_values));
    deallocateList(list_1);
}

STUDENT_TEST("Testing quickSort functionality on simple lists"){
    ListNode* list = createList({15, -1, 20, 14, 15, 36});


    Vector<int> list_values = {-1, 14, 15, 15, 20, 36};
    quickSort(list);

    EXPECT(areEquivalent(list, list_values));

    deallocateList(list);

}

STUDENT_TEST("Timing runSort"){
    int startSize = 10000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n-1; i >= 0; i--) {
            v[i] = randomInteger(-10, 10);
            list = new ListNode(v[i], list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(n, runSort(list));

        deallocateList(list);
    }
}

STUDENT_TEST("Timing runSort on sorted lists: Best case"){
    int startSize = 10000000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same sorted sequence. */
        for (int i = n - 1; i >= 0; i--) {
            list = new ListNode(i, list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(n, runSort(list));

        deallocateList(list);
    }
}

STUDENT_TEST("Timing quickSort"){
    int startSize = 500000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n - 1; i >= 0; i--) {
            v[i] = randomInteger(-10, 10);
            list = new ListNode(v[i], list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(n, quickSort(list));


        deallocateList(list);
    }
}

STUDENT_TEST("Timing quickSort: Worst case"){
    int startSize = 10000000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v(n);
        ListNode* list = nullptr;

                /* Create linked list and vector with the same sorted sequence. */
                for (int i = 0; i < v.size(); i++) {
                    list = new ListNode(i, list);
                }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(n, quickSort(list));

        deallocateList(list);
    }
}
