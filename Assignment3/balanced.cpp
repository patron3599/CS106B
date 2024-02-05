/*
 * File: balanced.cpp
 * ----------------
 * By: Christopher Patron
 * Project: CS 106B Assignment 3 - Balanced
 * Date: 10/17/2022
 *
 * balanced.cpp is a program that can take in an arbitrary string
 * and if there are any delimiting brackets such as "()", "{}", or
 * "[]", it then proceeds to see if those brackets are "balanced"
 * (i.e if they both have an opening and closing one)
 */

#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "SimpleTest.h"
#include "set.h"

using namespace std;

/*
 * operatorsFrom takes in a string as the argument. It then processes the string
 * and extracts only the bracketing characters from the string and returns in the
 * order in which they are found in the original string.
 */
string operatorsFrom(string str) {
    Set<char> operators = {'(', ')', '[', ']', '{', '}'};

    // Base Case: All the elements of the string have been processed
    if (str.empty()) {
        return "";
    }

    // Recursive Case
    else{
        if (operators.contains(str[0])){
            string found_op = str[0] + operatorsFrom(str.substr(1)); // Formatted to return the operators
                                                                     // the way in which they originally were
                                                                     // in the string.
            return found_op;
        }
        else{
            return  operatorsFrom(str.substr(1));
        }
    }
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
bool operatorsAreMatched(string ops) {

    // Base Case: All the pairs in the string have been removed
    if (ops.empty()) {
        return true;
    }

    // Recursive Case: Find where balancing occurs, if any
    else if(ops.find("[]") != string::npos){
        int found_index = ops.find("[]");
        ops.erase(found_index, 2);
        return operatorsAreMatched(ops);
    }
    else if(ops.find("()") != string::npos){
        int found_index = ops.find("()");
        ops.erase(found_index, 2);
        return operatorsAreMatched(ops);
    }
    else if(ops.find("{}") != string::npos){
        int found_index = ops.find("{}");
        ops.erase(found_index, 2);
        return operatorsAreMatched(ops);
    }
    else{
        return false;
    }
}

/*
 * The isBalanced function assumes correct implementation of
 * the above two functions operatorsFrom and operatorsMatch.
 * It uses operatorsFrom to extract the operator characters
 * from the input string and then confirms that those
 * operators are balanced by using operatorsMatch.
 * You should not modify the provided code in the isBalanced
 * function.  If the previous two functions have been implemented
 * correctly, the provided isBalanced will correctly report whether
 * the input string has balanced bracketing operators.
 */
bool isBalanced(string str) {
    string ops = operatorsFrom(str);
    return operatorsAreMatched(ops);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
}

PROVIDED_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("{}"));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on non-balanced examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}

// STUDENT TESTS //
STUDENT_TEST("operatorsFrom on some examples") {
    EXPECT_EQUAL(operatorsFrom("{test[x+(y/2)]"), "{[()]");
    EXPECT_EQUAL(operatorsFrom("no_operators"), "");
    EXPECT_EQUAL(operatorsFrom(")mix&((match]{)"), ")((]{)");

}

STUDENT_TEST("operatorsAreMatched on balanced example") {
    EXPECT(operatorsAreMatched("[(){}]"));
}

STUDENT_TEST("operatorsAreMatched on unbalanced example") {
    EXPECT(!operatorsAreMatched("{}]"));
}

STUDENT_TEST("operatorsAreMatched on unbalanced example") {
    EXPECT(!operatorsAreMatched("[(){{}]"));
}

STUDENT_TEST("operatorsAreMatched on empty string") {
    EXPECT(operatorsAreMatched(""));
}


