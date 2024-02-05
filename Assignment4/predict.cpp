/*
 * File: predict.cpp
 * ----------------
 * By: Christopher Patron
 * Project: CS 106B Assignment 4 - Predict
 * Date: 10/24/2022
 *
 * predict.cpp is a program that uses the power of recursion to find
 * all valid words from a combinations of digits corresponding
 * to letters from a number keypad.
 */
#include <iostream>
#include "backtracking.h"
#include "lexicon.h"
#include "set.h"
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;


/* * * FUNCTION PROTOTYPES * * */
void predict(string digits, Set<string>& suggestions, Lexicon& lex);
void predict(string digits, Set<string>& suggestions, Lexicon& lex, string& prefix, int& counter);


// keypad is a program-wide constant that stores the Map from digit char to
// its associated set of possible letters
static const Map<char, Set<char>> keypad = {
    {'2', {'a','b','c'} }, {'3', {'d','e','f'} }, {'4', {'g','h','i'} },
    {'5', {'j','k','l'} }, {'6', {'m','n','o'} }, {'7', {'p','q','r','s'} },
    {'8', {'t','u','v'} }, {'9', {'w','x','y','z'} } };


/*
 * predict takes in the digit combination, set of suggestions, lexicon containing english words,
 * a prefix string, and a counter. The prefix string and counter are added in a wrapper function
 * to help execute the code. The prefix is used to store the current combination of letters and is
 * manipulated throughout the function; Being added to suggestions if it is a word or removing a
 * letter if it is not a viable combination. The counter is used to keep track of where in the
 * digits string the function is. This allows for the function to access the necesary letters
 * corresponding to the digit. The function then returns a set containing all the suggestions.
 */
void predict(string digits, Set<string>& suggestions, Lexicon& lex){ // Wrapper Function
    string prefix = "";
    int counter = 0;
    predict(digits, suggestions, lex, prefix, counter);
}


void predict(string digits, Set<string>& suggestions, Lexicon& lex, string& prefix, int& counter) {
    // Base Case: A word has been found or a word in progress has been pruned
    if (lex.contains(prefix) && counter == digits.size()){
        suggestions.add(prefix);
        prefix = prefix.substr(0, prefix.size() - 1);
    }
    // Base Case: A word has been unchosen and there are no other possible combinations
    else if(!lex.containsPrefix(prefix) || prefix.size() >= digits.size()){
        prefix = prefix.substr(0, prefix.size() - 1);
    }

    // Recursive Case: Choosing options
    else{
        Set <char> temp_letters = keypad.get(digits[counter]);
        counter ++;
        for(auto elem : temp_letters){
            prefix += elem;
            predict(digits, suggestions, lex, prefix, counter);
        }
    // Unchoosing once all options with that word has been explored
    counter --;
    prefix = prefix.substr(0, prefix.size() - 1);
    }
}


/* * * * * * Test Cases * * * * * */

/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
static Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}

PROVIDED_TEST("Predict intended words for digit sequence 6263, example from writeup") {
    string digits = "6263";
    Set<string> expected = {"name", "mane", "oboe"};
    Set<string> suggestions;

    predict(digits, suggestions, sharedLexicon());
    EXPECT_EQUAL(suggestions, expected);
}

PROVIDED_TEST("What words can I spell from my phone number?") {
    string digits = "3228679";  // my old phone number :-)
    Set<string> expected = {"factory"};
    Set<string> suggestions;

    predict(digits, suggestions, sharedLexicon());
    EXPECT_EQUAL(suggestions, expected);
}

// STUDENT TEST //
STUDENT_TEST("What words can I spell from 486?") {
    string digits = "486";
    Set<string> expected = {"gum", "gun", "hum", "hun"};
    Set<string> suggestions;

    predict(digits, suggestions, sharedLexicon());
    EXPECT_EQUAL(suggestions, expected);
}

STUDENT_TEST("No words can be made with 97") {
    string digits = "97";
    Set<string> expected = {};
    Set<string> suggestions;

    predict(digits, suggestions, sharedLexicon());
    EXPECT_EQUAL(suggestions, expected);
}

