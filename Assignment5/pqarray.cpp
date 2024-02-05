#include "pqarray.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "SimpleTest.h"
#include "vector.h"
using namespace std;

// program constant
static const int INITIAL_CAPACITY = 10;

/*
 * The constructor initializes all of the member variables needed for
 * an instance of the PQArray class. The allocated capacity
 * is initialized to a starting constant and a dynamic array of that
 * size is allocated. The count of filled slots is initially zero.
 */
PQArray::PQArray() {
    _numAllocated = INITIAL_CAPACITY;
    _elements = new DataPoint[_numAllocated](); // add parens to zero new memory
    _numFilled = 0;
}

/* The destructor is responsible for cleaning up any resources
 * used by this instance of the PQArray class. The array
 * memory that was allocated for the PQArray is deleted here.
 */
PQArray::~PQArray() {
    delete[] _elements;
}

/* enqueue is passed in a single DataPoint structure that is to be
 * properly sorted into the array. It adds it to the end of the array
 * and then moves it swaps its location with the left index. If this
 * swap is still not a valid location, the DataPoint is swapped until
 * it is properly sorted.
 */
void PQArray::enqueue(DataPoint elem) {
    int idx = size();

    // Checking if the next enqueue will fill the array to it's capacity
    checkResize();

    // Enqueueing initial entry to end
    _elements[idx] = elem;
    _numFilled++;

    if(idx != 0){
        if(_elements[idx].priority > _elements[idx - 1].priority){ // Checking for proper ordering
            int temp_idx = idx;
            swapElements(temp_idx - 1, temp_idx); // Doing one swap
            temp_idx--;

            if (temp_idx != 0){
                // One swap was not good enough, continue swapping until it is properly located in the array
                while(_elements[temp_idx].priority > _elements[temp_idx - 1].priority){
                    swapElements(temp_idx - 1, temp_idx);
                    temp_idx--;
                    if (temp_idx == 0){ // If the enqueued entry is the largest value out of all in the array (i.e at top of array)
                        break;
                    }
                }
            }
        }
    }
}

/* checkResize is a method that resizes the array by twice its original size
 * if the current array is at its capacity. To "resize," a new array is
 * initilaized with twice the size of the previous arrays capacity and the
 * elements from the old array are copied over into the new one.
 */
void PQArray::checkResize(){
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

/*
 * The count of enqueued elements is tracked in the
 * member variable _numFilled.
 */
int PQArray::size() const {
    return _numFilled;
}

/*
 * The array elements are stored in decreasing order of priority value.
 * The element at index 0 is the least urgent (largest priority value)
 * and the element in the last-filled index is the most urgent
 * (minimum priority value), this element is frontmost. peek returns
 * the frontmost element (most urgent, minimum priority value).
 */
DataPoint PQArray::peek() const {
    if (isEmpty()) {
        error("Cannot access front element of empty pqueue!");
    }
    return _elements[size() - 1];
}

/*
 * This function returns the value of the frontmost element and removes
 * it from the queue.  Because the frontmost element was at the
 * last-filled index, decrementing filled count is sufficient to remove it.
 */
DataPoint PQArray::dequeue() {
    DataPoint front = peek();
    _numFilled--;
    return front;
}

/*
 * Returns true if the queue contains no elements, false otherwise
 */
bool PQArray::isEmpty() const {
    return size() == 0;
}

/*
 * Updates internal state to reflect that the queue is empty, i.e. count
 * of filled slots is reset to zero. The array memory stays allocated
 * at current capacity. The entries in the element array do not need to
 * be cleared; those slots will be overwritten when additional values
 * are enqueued.
 */
void PQArray::clear() {
    _numFilled = 0;
}

/*
 * This private member function is a helper that exchanges the element
 * at indexA with the element at indexB. In addition to being a handy
 * helper function for swapping elements, it also confirms that both
 * indexes are valid.  If you were to accidentally mishandle an index,
 * you will be so so glad this defensive protection is here to alert you!
 */
void PQArray::swapElements(int indexA, int indexB) {
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
void PQArray::validateIndex(int index) {
    if (index < 0 || index >= _numFilled) error("Invalid index " + integerToString(index));
}

/*
 * Prints the contents of internal array for debugging purposes.
 */
void PQArray::printDebugInfo(string msg) const {
    cout << msg << endl;
    for (int i = 0; i < size(); i++) {
        cout << "[" << i << "] = " << _elements[i] << endl;
    }
}

/*
 * Confirm the internal state of member variables appears valid.
 * In this case, check that the elements in the array are stored in
 * priority order. Report an error if problem iis found.
 */
void PQArray::validateInternalState() const {
    /*
     * If there are more elements than spots in the array, we have a problem.
     */
    if (_numFilled > _numAllocated) error("Too many elements in not enough space!");

    /* Loop over the array and compare priority of neighboring elements.
     * If element at index i has larger priority than at index i-1,
     * these two elements are out of order according to specification.
     * Report the problem using error.
     */
    for (int i = 1; i < size(); i++) {
        if (_elements[i].priority > _elements[i-1].priority) {
            printDebugInfo("validateInternalState");
            error("PQArray has elements out of order at index " + integerToString(i));
        }
    }
}

// Provided Helper Functions For Time Operation Check //
void fillQueue(PQArray& pq, int n) {
    pq.clear(); // start with empty queue
    for (int i = 0; i < n; i++) {
        pq.enqueue({ "", randomReal(0, 10) });
    }
}

void emptyQueue(PQArray& pq, int n) {
    for (int i = 0; i < n; i++) {
        pq.dequeue();
    }
}

/* * * * * * Test Cases Below This Point * * * * * */

/* STUDENT TESTS */
STUDENT_TEST("PQArray example with exta entry") {
    PQArray pq;

    pq.enqueue( { "Zoe", -3 } );
    pq.enqueue( { "Elmo", 10 } );
    pq.enqueue( { "Bert", 6 } );
    EXPECT_EQUAL(pq.size(), 3);
    pq.printDebugInfo("After enqueue 3 elements");

    pq.enqueue( { "Kermit", 5 } );
    pq.enqueue( { "Cookie", 8 } );
    EXPECT_EQUAL(pq.size(), 5);
    pq.printDebugInfo("After enqueue one more");

    DataPoint removed = pq.dequeue();
    DataPoint expected = { "Zoe", -3 };
    EXPECT_EQUAL(removed, expected);
    pq.printDebugInfo("After dequeue one");
}

STUDENT_TEST("Testing Enqueue/Dequeue Time Operation"){
    PQArray pq;
    Vector<int> sizes = {100,1000,10000,100000};

    for(int i = 0; i < sizes.size(); i++){
        TIME_OPERATION(sizes[i], fillQueue(pq, sizes[i]));
        TIME_OPERATION(sizes[i], emptyQueue(pq, sizes[i]));
    }

}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("PQArray example from writeup") {
    PQArray pq;

    pq.enqueue( { "Zoe", -3 } );
    pq.enqueue( { "Elmo", 10 } );
    pq.enqueue( { "Bert", 6 } );
    EXPECT_EQUAL(pq.size(), 3);
    pq.printDebugInfo("After enqueue 3 elements");

    pq.enqueue( { "Kermit", 5 } );
    EXPECT_EQUAL(pq.size(), 4);
    pq.printDebugInfo("After enqueue one more");

    DataPoint removed = pq.dequeue();
    DataPoint expected = { "Zoe", -3 };
    EXPECT_EQUAL(removed, expected);
    pq.printDebugInfo("After dequeue one");
}

PROVIDED_TEST("PQArray, enqueue only, validate at every step") {
    PQArray pq;

    pq.enqueue({ "e", 2.718 });
    pq.validateInternalState();
    pq.enqueue({ "pi", 3.14 });
    pq.validateInternalState();
    pq.enqueue({ "phi", 1.618 });
    pq.validateInternalState();
    EXPECT_EQUAL(pq.size(), 3);
}

PROVIDED_TEST("PQArray: operations size/isEmpty/clear") {
    PQArray pq;

    EXPECT(pq.isEmpty());
    pq.clear();
    EXPECT_EQUAL(pq.isEmpty(), pq.size() == 0);
    pq.enqueue({ "", 7 });
    EXPECT_EQUAL(pq.size(), 1);
    pq.enqueue({ "", 5 });
    EXPECT_EQUAL(pq.size(), 2);
    pq.enqueue({ "", 5 });
    EXPECT_EQUAL(pq.size(), 3);
    pq.clear();
    pq.validateInternalState();
    EXPECT(pq.isEmpty());
    EXPECT_EQUAL(pq.size(), 0);
}

PROVIDED_TEST("PQArray: dequeue or peek on empty pqueue raises error") {
    PQArray pq;
    DataPoint point = { "Programming Abstractions", 106 };

    EXPECT(pq.isEmpty());
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());

    pq.enqueue(point);
    pq.dequeue();
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());

    pq.enqueue(point);
    pq.clear();
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());
}

PROVIDED_TEST("PQArray, dequeue, validate at every step") {
    PQArray pq;

    pq.enqueue({ "e", 2.718 });
    pq.enqueue({ "pi", 3.14 });
    pq.enqueue({ "phi", 1.618 });

    for (int i = 0; i < 3; i++) {
        pq.dequeue();
        pq.validateInternalState();
    }
}

PROVIDED_TEST("PQArray, test enlarge array memory") {
    for (int size = 5; size <= 500; size *= 5) {
        PQArray pq;

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

PROVIDED_TEST("PQArray, sequence of mixed operations") {
    PQArray pq;
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

PROVIDED_TEST("PQArray stress test, cycle many random elements in and out") {
    PQArray pq;
    int n = 0, maxEnqueues = 1000;
    double sumEnqueued = 0, sumDequeued = 0;

    setRandomSeed(42); // make test behavior deterministic

    DataPoint mostUrgent = { "", 0 };
    pq.enqueue(mostUrgent);
    while (true) {
        if (++n < maxEnqueues && randomChance(0.9)) {
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

PROVIDED_TEST("PQArray timing test, fillQueue and emptyQueue") {
    PQArray pq;
    int n = 20000;

    TIME_OPERATION(n, fillQueue(pq, n));
    TIME_OPERATION(n, emptyQueue(pq, n));
}
