#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;
const int NONE = -1; // used as sentinel index

/*
 * This is the constructor of the class and it initializes some of the
 * important variables and structures that this class will need to operate.
 */
PQHeap::PQHeap() {
    _numAllocated = INITIAL_CAPACITY;
    _elements = new DataPoint[_numAllocated](); // add parens to zero new memory
    _numFilled = 0;
}

/*
 * This is the destructor of the class and cleans up any resources that the class
 * was using in order to prevent any form of memory leaking.
 */
PQHeap::~PQHeap() {
    delete [] _elements;
}

/*
 * To enqueue in a binary heap, you take the element and place it at the bottom of the tree.
 * Since this would more than likely result in an invalid binary heap, you must "bubble up"
 * the last element until it is in a position that satisfies the rules of a minimum binary heap.
 */
void PQHeap::enqueue(DataPoint elem) {
    int idx = size();

    // Checking if the next enqueue will fill the array to it's capacity
    checkResize();

    // Enqueueing initial entry to end
    _elements[idx] = elem;
    _numFilled++;
    int parent_idx, temp_parent_idx;

    if(idx != 0){
        parent_idx = getParentIndex(idx);

        if(_elements[idx].priority < _elements[parent_idx].priority){ // Checking for proper ordering
            swapElements(idx, parent_idx); // Doing one swap
            temp_parent_idx = getParentIndex(parent_idx);
            if(parent_idx != 0){
                // One swap was not good enough, continue swapping until it is properly located in the array
                // i.e "bubble up"
                while(_elements[parent_idx].priority < _elements[temp_parent_idx].priority){
                    swapElements(temp_parent_idx, parent_idx);
                    parent_idx = temp_parent_idx;
                    temp_parent_idx = getParentIndex(parent_idx);

                    if (parent_idx == 0 || temp_parent_idx == NONE){ // If the enqueued entry is the smallest
                                                                     // value out of all in the array
                                                                     // (i.e at top of array)
                        break;
                    }
                }
            }
        }
    }
}

/*
 * The array elements are stored in increasing order of priority value.
 * The element at index 0 is the most urgent (lowest priority value)
 * and the element in the last-filled index is the least urgent
 * (maximum priority value), this element is backmost. peek returns
 * the frontmost element (most urgent, minimum priority value).
 */
DataPoint PQHeap::peek() const {
    if (isEmpty()) {
        error("Cannot access front element of empty pqueue!");
    }
    return _elements[0];
}

/*
 * To dequeue in a binary heap, you remove the element with the lowest priority
 * (i.e the most urgent) which is at the top of the tree. Then, you take the element
 * at the bottom of the tree and place it at the top. Since this would more than likely
 * result in an invalid binary heap, you must "trickle down" the top element until it
 * is in a position that satisfies the rules of a minimum binary heap.
 */
DataPoint PQHeap::dequeue() {
    DataPoint top = peek();

    int parent_idx = 0; // We always start at root node
    _elements[parent_idx] = _elements[size() - 1]; // Moved last element to top of tree

    int left_child_idx, right_child_idx, min_idx;
    right_child_idx = getRightChildIndex(parent_idx);
    left_child_idx = getLeftChildIndex(parent_idx);

    // Finding which child has the lowest priority value
    if (_elements[left_child_idx].priority < _elements[right_child_idx].priority) {
        min_idx = left_child_idx;
    }
    else {
        min_idx = right_child_idx;
    }

    // Trickling down until a valid binary heap is achieved
    bool valid_min_idx = verifyIndex(min_idx);
    while ((_elements[parent_idx].priority > _elements[min_idx].priority) && valid_min_idx) {
        swapElements(min_idx, parent_idx);
        parent_idx = min_idx;  // Parent index now becomes the index that we just switched
        right_child_idx = getRightChildIndex(parent_idx);
        left_child_idx = getLeftChildIndex(parent_idx);

        // Finding which new child has the lowest priority value
        bool valid_right_child = verifyIndex(right_child_idx);
        bool valid_left_child = verifyIndex(left_child_idx);
        if (valid_right_child && valid_left_child) {
            if (_elements[left_child_idx].priority < _elements[right_child_idx].priority) {
               min_idx = left_child_idx;
            }
            else {
               min_idx = right_child_idx;
            }
        }
        else {
            _numFilled--; // Element has been "dequeued"
            return top;
        }
    }
    _numFilled--; // Element has been "dequeued"
    return top;
}

/*
 * Returns true if the queue contains no elements, false otherwise
 */
bool PQHeap::isEmpty() const {
    return size() == 0;
}

/*
 * The count of enqueued elements is tracked in the
 * member variable _numFilled.
 */
int PQHeap::size() const {
    return _numFilled;
}

/*
 * Updates internal state to reflect that the queue is empty, i.e. count
 * of filled slots is reset to zero. The array memory stays allocated
 * at current capacity. The entries in the element array do not need to
 * be cleared; those slots will be overwritten when additional values
 * are enqueued.
 */
void PQHeap::clear() {
    _numFilled = 0;
}

/*
 * Prints the contents of internal array for debugging purposes.
 */
void PQHeap::printDebugInfo(string msg) const {
    cout << msg << endl;
    for (int i = 0; i < size(); i++) {
        cout << "[" << i << "] = " << _elements[i] << endl;
    }
}

/*
 * This method ensures that the binary heap satifies all the condtions
 * of a true minimum binary heap. These are that it must be a valid binary
 * tree, it must be complete, and that the parent's priority is always less
 * than it's children's priorities.
 */
void PQHeap::validateInternalState() const {
    // If there are more elements than spots in the array
    if(_numFilled > _numAllocated){
        error("Too many elements in not enough space!");
    }

    /* Loop over the parents in the array and compare that the conditions of
     * a valid minimum binary heap are met.
     */
    int num_parents = (size() - 1) / 2;
    int found_parent = 0;
    int curr_parent = 0;
    int left_child, right_child;

    while(found_parent != num_parents){
        left_child = getLeftChildIndex(curr_parent);
        right_child = getRightChildIndex(curr_parent);

        if(left_child == NONE || right_child == NONE){
            error("Child index has no parent!");
        }

        if(_elements[left_child].priority < _elements[curr_parent].priority ||
                _elements[right_child].priority < _elements[curr_parent].priority){

            error("Child priority is larger than parent's!");
        }
        else{
            curr_parent++;
            found_parent++;
        }
    }
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the parent of the
 * specified child index. If this child has no parent, return
 * the sentinel value NONE.
 */
int PQHeap::getParentIndex(int child) const {
    if (child == 0){
        return NONE;
    }
    else{
       return (child - 1) / 2;
   }
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the left child of the
 * specified parent index. If this parent has no left child, return
 * the sentinel value NONE.
 */
int PQHeap::getLeftChildIndex(int parent) const {
    int left_child_idx = (2 * parent) + 1;
    if(left_child_idx >= size()){
        return NONE;
    }
    else{
     return left_child_idx;
    }
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the right child of the
 * specified parent index. If this parent has no right child, return
 * the sentinel value NONE.
 */
int PQHeap::getRightChildIndex(int parent) const {
    int right_child_idx = (2 * parent) + 2;
    if(right_child_idx >= size()){
        return NONE;
    }
    else{
     return right_child_idx;
    }
}

/*
 * This private member function is a helper that exchanges the element
 * at indexA with the element at indexB. In addition to being a handy
 * helper function for swapping elements, it also confirms that both
 * indexes are valid.  If you were to accidentally mishandle an index,
 * you will be so so glad this defensive protection is here to alert you!
 */
void PQHeap::swapElements(int indexA, int indexB) {
    validateIndex(indexA);
    validateIndex(indexB);
    DataPoint tmp = _elements[indexA];
    _elements[indexA] = _elements[indexB];
    _elements[indexB] = tmp;
}

/*
 * This private member function is a helper that confirms that index
 * is in within range of the filled portion of the element array,
 * raising an error if the index is invalid.
 */
bool PQHeap::verifyIndex(int index) {
    if (index < 0 || index >= _numFilled) {
        return false;
    }
    else {
        return true;
    }
}

/*
 * This private member function is a helper that confirms that index
 * is in within range of the filled portion of the element array,
 * raising an error if the index is invalid.
 */
void PQHeap::validateIndex(int index) {
    if (index < 0 || index >= _numFilled) error("Invalid index " + integerToString(index));
}

/* checkResize is a method that resizes the array by twice its original size
 * if the current array is at its capacity. To "resize," a new array is
 * initilaized with twice the size of the previous arrays capacity and the
 * elements from the old array are copied over into the new one.
 */
void PQHeap::checkResize(){
    int arr_size = size();
    if(arr_size == _numAllocated){
        DataPoint* bigger = new DataPoint[2 * _numAllocated]();
        for (int i = 0; i < _numAllocated; i++){
            bigger[i] = _elements[i];
        }

    delete[] _elements;
    _elements = bigger;
    _numAllocated *= 2;
    }
}


/* * * * * * Test Cases Below This Point * * * * * */
/// Left commented since get"_" methods are private ///
//STUDENT_TEST("Testing get parent/child index methods functionality"){
//    PQHeap pq;
//    pq.enqueue( { "", 4 } );
//    pq.enqueue( { "", 5 } );
//    pq.enqueue( { "", 3 } );
//    pq.enqueue( { "", 7 } );
//    pq.enqueue( { "", 2 } );
//    pq.enqueue( { "", 9 } );
//    pq.enqueue( { "", 1 } );
//    pq.enqueue( { "", 8 } );
//    pq.enqueue( { "", 6 } );

//    EXPECT_EQUAL(pq.getParentIndex(1), 0);
//    EXPECT_EQUAL(pq.getParentIndex(7), 3);
//    EXPECT_EQUAL(pq.getLeftChildIndex(2), 5);
//    EXPECT_EQUAL(pq.getRightChildIndex(3), 8);

//    EXPECT_EQUAL(pq.getParentIndex(0), NONE); // The root should not have a parent
//    EXPECT_EQUAL(pq.getLeftChildIndex(5), NONE); // Does not have a left child
//    EXPECT_EQUAL(pq.getRightChildIndex(8), NONE); // Does not have a right child
//}

STUDENT_TEST("PQHeap: Testing basic enqueue") {
    PQHeap pq;

    pq.enqueue( { "", 5 } );
    pq.enqueue( { "", 7 } );
    pq.enqueue( { "", 10 } );
    EXPECT_EQUAL(pq.size(), 3);
    pq.printDebugInfo("After enqueue 3 elements");
}

STUDENT_TEST("PQHeap: Testing basic enqueue w/ one swap") {
    PQHeap pq;

    pq.enqueue( { "", 4 } );
    pq.enqueue( { "", 5 } );
    pq.enqueue( { "", 3 } );

    EXPECT_EQUAL(pq.size(), 3);
    EXPECT_NO_ERROR(pq.validateInternalState());
    pq.printDebugInfo("After enqueue 3 elements");
}

STUDENT_TEST("PQHeap: Testing basic enqueue w/ multiple swaps") {
    PQHeap pq;
    pq.enqueue( { "", 4 } );
    pq.enqueue( { "", 5 } );
    pq.enqueue( { "", 3 } );
    pq.enqueue( { "", 7 } );
    pq.enqueue( { "", 2 } );
    pq.enqueue( { "", 9 } );
    pq.enqueue( { "", 1 } );
    pq.enqueue( { "", 8 } );
    pq.enqueue( { "", 6 } );

    EXPECT_EQUAL(pq.size(), 9);
    EXPECT_NO_ERROR(pq.validateInternalState());
    pq.printDebugInfo("After enqueue 9 elements");
}

STUDENT_TEST("PQHeap: Testing enqueue w/ multiple swaps, array size increase, and validating internal state") {
    PQHeap pq;
    pq.enqueue( { "", 4 } );
    pq.enqueue( { "", 5 } );
    pq.enqueue( { "", 3 } );
    pq.enqueue( { "", 7 } );
    pq.enqueue( { "", 2 } );
    pq.enqueue( { "", 9 } );
    pq.enqueue( { "", 1 } );
    pq.enqueue( { "", 8 } );
    pq.enqueue( { "", 6 } );
    pq.enqueue( { "", 12 } );
    pq.enqueue( { "", 11 } );
    pq.enqueue( { "", 10 } );
    pq.enqueue( { "", 14 } );
    pq.enqueue( { "", 15 } );
    pq.enqueue( { "", 17 } );

    EXPECT_EQUAL(pq.size(), 15);
    EXPECT_NO_ERROR(pq.validateInternalState());
    pq.printDebugInfo("After enqueue 16 elements");
}

STUDENT_TEST("PQHeap: Testing validate state on basic PQheap") {
    PQHeap pq;

    pq.enqueue( { "", 5 } );
    pq.enqueue( { "", 7 } );
    pq.enqueue( { "", 10 } );

    EXPECT_NO_ERROR(pq.validateInternalState());
}

STUDENT_TEST("PQHeap: Testing validate state on PQheap") {
    PQHeap pq;
    pq.enqueue( { "", 4 } );
    pq.enqueue( { "", 5 } );
    pq.enqueue( { "", 3 } );
    pq.enqueue( { "", 7 } );
    pq.enqueue( { "", 2 } );
    pq.enqueue( { "", 9 } );
    pq.enqueue( { "", 1 } );
    pq.enqueue( { "", 8 } );
    pq.enqueue( { "", 6 } );

    EXPECT_NO_ERROR(pq.validateInternalState());
}

STUDENT_TEST("PQHeap: Testing validate state on PQheap w/ unknown ordering") {
    PQHeap pq;
    pq.enqueue( { "", 1 } );
    pq.enqueue( { "", 3 } );
    pq.enqueue( { "", 6 } );
    pq.enqueue( { "", 7 } );
    pq.enqueue( { "", 2 } );
    pq.enqueue( { "", 9 } );
    pq.enqueue( { "", 5 } );
    pq.enqueue( { "", 8 } );
    pq.enqueue( { "", 11 } );

    EXPECT_NO_ERROR(pq.validateInternalState());
}

STUDENT_TEST("PQHeap: Testing basic dequeue") {
    PQHeap pq;

    pq.enqueue( { "", 2 } );
    pq.enqueue( { "", 3 } );
    pq.enqueue( { "", 4 } );
    pq.enqueue( { "", 7 } );

    pq.dequeue();
    EXPECT_EQUAL(pq.size(), 3);
    EXPECT_NO_ERROR(pq.validateInternalState());

}

STUDENT_TEST("PQHeap: Testing dequeue w/ 2 swaps") {
    PQHeap pq;

    pq.enqueue( { "", 4 } );
    pq.enqueue( { "", 5 } );
    pq.enqueue( { "", 3 } );
    pq.enqueue( { "", 7 } );
    pq.enqueue( { "", 2 } );
    pq.enqueue( { "", 9 } );
    pq.enqueue( { "", 1 } );
    pq.enqueue( { "", 8 } );
    pq.enqueue( { "", 6 } );

    pq.dequeue();
    EXPECT_EQUAL(pq.size(), 8);
    EXPECT_NO_ERROR(pq.validateInternalState());
}

STUDENT_TEST("PQHeap: Testing dequeue w/ 2 dequeues") {
    PQHeap pq;

    pq.enqueue( { "", 4 } );
    pq.enqueue( { "", 5 } );
    pq.enqueue( { "", 3 } );
    pq.enqueue( { "", 7 } );
    pq.enqueue( { "", 2 } );
    pq.enqueue( { "", 9 } );
    pq.enqueue( { "", 1 } );
    pq.enqueue( { "", 8 } );
    pq.enqueue( { "", 6 } );

    pq.dequeue();
    EXPECT_EQUAL(pq.size(), 8);
    EXPECT_NO_ERROR(pq.validateInternalState());
    pq.dequeue();
    EXPECT_EQUAL(pq.size(), 7);
    EXPECT_NO_ERROR(pq.validateInternalState());

}

STUDENT_TEST("PQHeap, test enlarge array memory") {
        PQHeap pq;
        for (int i = 1; i <= 12; i++) {
            pq.enqueue({"", double(i) });
        }
        pq.validateInternalState();

        for (int i = 1; i <= 12; i++) {
            DataPoint expected = {"", double(i) };
            EXPECT_EQUAL(pq.dequeue(), expected);
        }

}

STUDENT_TEST("PQHeap, test enlarge array memory") {
    for (int size = 5; size <= 500; size *= 5) {
        PQHeap pq;

        for (int i = 1; i <= size; i++) {
            pq.enqueue({"", double(i) });
        }
        pq.validateInternalState();

        for (int i = 1; i <= size; i++) {
            DataPoint expected = {"", double(i) };
            EXPECT_EQUAL(pq.dequeue(), expected);
        }
    }
}

STUDENT_TEST("PQHeap stress test, cycle many random elements in and out") {
    PQHeap pq;
    int exp;
    int n = 0, maxEnqueues = 1000;
    double sumEnqueued = 0, sumDequeued = 0;

    setRandomSeed(42); // make test behavior deterministic

    DataPoint mostUrgent = { "", 0 };
    pq.enqueue(mostUrgent);
    while (true) {
        exp = randomChance(0.9);
        if (++n < maxEnqueues && exp) {
            DataPoint elem = { "", randomInteger(-10, 10) + 0.5 };
            if (elem.priority < mostUrgent.priority) {
                mostUrgent = elem;
            }
            sumEnqueued += elem.priority;
            pq.enqueue(elem);
        } else {
            DataPoint elem = pq.dequeue();
            sumDequeued += elem.priority;
            EXPECT_EQUAL(elem, mostUrgent);
            if (pq.isEmpty()) break;
            mostUrgent = pq.peek();
        }
    }
    EXPECT_EQUAL(sumEnqueued, sumDequeued);
}

STUDENT_TEST("PQHeap, sequence of mixed operations") {
    PQHeap pq;
    int size = 30;
    double val = 0;

    for (int i = 0; i < size; i++) {
        pq.enqueue({"", --val });
    }
    val = 0;
    for (int i = 0; i < pq.size(); i++) {
        DataPoint front = pq.peek();
        EXPECT_EQUAL(pq.dequeue(), front);
        pq.enqueue( { "", ++val });
    }
    EXPECT_EQUAL(pq.size(), size);
    val = 0;
    while (!pq.isEmpty()) {
        DataPoint expected = { "", ++val };
        EXPECT_EQUAL(pq.dequeue(), expected);
    }
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("PQHeap example from writeup, validate each step") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    pq.validateInternalState();
    for (DataPoint dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }
    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }
}
