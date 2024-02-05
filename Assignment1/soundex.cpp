/*
 * File: soundex.cpp
 * ----------------
 * By: Christopher Patron
 * Project: CS 106B Assignment 1 - Soundex
 * Date: 10/02/2022
 *
 * This program implements the Soundex algorithm to assist with phonetic matching.
 * When ran, this program will prompt the user to input a surname. After this, it will
 * proceed to code this surname using the Soundex algorithm and display all other surnames
 * from a database that contain the same Soundex code.
 */
#include <cctype>
#include <fstream>
#include <string>
#include "console.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include "vector.h"
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;

/* This function is intended to return a string which
 * includes only the letter characters from the original
 * (all non-letter characters are excluded).
 *
 * The original issue that this function had was that it was
 * automatically adding the first character to the result which
 * would yield errors if you passed an empty string. Therefore,
 * the issue was to update the index at where the loop started.
 */
string lettersOnly(string s) {
    string result = "";
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}

/* encodeString takes in a string argument, encodes it according to
 * the Soundex code digit table, and then returns the encoded string.
 */
string encodeString(string s){
    string encoded = "";
    transform(s.begin(), s.end(), s.begin(), ::tolower); // Converting all the characters to lowercase

    for (int i = 0; i < s.length(); i++) {
        string ch = charToString(s[i]);

        // Encode the string as perscribed by the soundex digit to letter table
        if (ch == "a" || ch == "e" || ch == "i" || ch == "o" || ch == "u" || ch == "h" || ch == "w" || ch == "y") {
            encoded += "0";
        }
        else if (ch == "b" || ch == "f" || ch == "p" || ch == "v") {
            encoded += "1";
        }
        else if (ch == "c" || ch == "g" || ch == "j" || ch == "k" || ch == "q" || ch == "s" || ch == "x" || ch == "z") {
            encoded += "2";
        }
        else if (ch == "d" || ch == "t") {
            encoded += "3";
        }
        else if (ch == "l") {
            encoded += "4";
        }
        else if (ch == "m" || ch == "n") {
            encoded += "5";
        }
        else if (ch == "r") {
            encoded += "6";
        }
    }
    return encoded;
}

/* removeDuplicates takes in the encoded string, loops through the string,
 * removes adjacent duplicate values, and returns the encoded string with no
 * duplicates.
 */
string removeDuplicates(string enc_str){
    string no_dup = "";

    for (int i = 1; i < enc_str.length() + 1; i++) {
        if (enc_str[i] != enc_str[i-1]){
            no_dup += enc_str[i-1];
        }
    }
    return no_dup;
}

/* finalizeCode is passed the original string with only letters and the string
 * with no duplicates. It then proceeds to complete the coding process by
 * concatenating the capital of the first letter to a new string, removing
 * all zeros from the no duplicates string, and then either buffering or truncating
 * characters as necessary to meet the 4 character length requrirement. It returns
 * the final coded string.
 */
string finalizeCode(string no_dup, string result){
    string coded = "";
    coded += toUpperCase(charToString(result[0]));
    const int LENGTH = 4;

    // Removing all the zeros
    for (int i = 1; i < no_dup.length(); i++) {
        if (charToString(no_dup[i]) != "0"){
            coded += no_dup[i];
        }
    }
    // Either buffering or truncating the code as necessary
    if (coded.length() < LENGTH){
        while (coded.length() < LENGTH){
            coded += "0";
        }
    }
    else if (coded.length() > LENGTH){
        coded = coded.substr(0, LENGTH);
        }

    return coded;
}

/* soundex is passed a string that is to be encoded. This function
 * then calls all its helper functions sequentially to eventually return
 * the string coded to the Soundex standards.
 */
string soundex(string s) {
    string letters_only = lettersOnly(s);
    string encoded_string = encodeString(letters_only);
    string no_dup = removeDuplicates(encoded_string);
    string encoded = finalizeCode(no_dup, letters_only);

    return encoded;
}

/* soundexSearch is the main workhorse of this program. It first
 * reads the surname database into a vector. After this, it will
 * prompt the user for a surname and will then begin the procees
 * of encoding the surname and all the other surnames in the database.
 * Then, it will print a vector with the surnames from the database who
 * have the same soundex code as the inputted surname.
 */
void soundexSearch(string filepath) {
    // This provided code opens the specified file
    // and reads the lines into a vector of strings
    ifstream in;
    Vector<string> allNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, allNames);
    }
    cout << "Read file " << filepath << ", "
         << allNames.size() << " names found." << endl;

    // The names read from file are now stored in Vector allNames

    string surname = getLine("Enter a surname (RETURN to quit): ");
    Vector <string> matching_codes;

    string curr_code = soundex(surname);
    if (surname != ""){
        cout << "Soundex code is: " << curr_code << endl; // To prevent erroneous print if the user wants to quit
    }

    while (surname != ""){
        // Creating the vector that contains the surname codes that match the inputted one
        for (int i = 0; i < allNames.size(); i++) {
            string temp_code = soundex(allNames[i]);
            if (temp_code == curr_code){
                matching_codes.add(allNames[i]);
            }
        }
        matching_codes.sort();

        cout << "Matches from database: " << matching_codes << endl << endl;
        matching_codes.clear();

        // Repeating the procees of asking for a new surname
        surname = getLine("Enter a surname (RETURN to quit): ");
        curr_code = soundex(surname);
        if (surname != ""){
            cout << "Soundex code is: " << curr_code << endl; // To prevent erroneous print if the user wants to quit
        }
    }
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Test exclude of punctuation, digits, and spaces") {
    string s = "O'Hara";
    string result = lettersOnly(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = lettersOnly(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = lettersOnly(s);
    EXPECT_EQUAL(result, "tldr");
}

PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// STUDENT_TESTS
STUDENT_TEST("Finding bug in lettersOnly"){
    string s = "-2- HiP 3Ty0k ";
    string result = lettersOnly(s);
    EXPECT_EQUAL(result, "HiPTyk");

    s = "";
    result = lettersOnly(s);
    EXPECT_EQUAL(result, "");
}

STUDENT_TEST("Testing encodeString functionality"){
    EXPECT_EQUAL(encodeString("Curie"), "20600");
    EXPECT_EQUAL(encodeString("OConner"), "0205506");
    EXPECT_EQUAL(encodeString("AnGeLoU"), "0520400");
    EXPECT_EQUAL(encodeString("PATRON"), "103605");
}

STUDENT_TEST("Testing removeDuplicates functionality"){
    EXPECT_EQUAL(removeDuplicates("222025"), "2025");
    EXPECT_EQUAL(removeDuplicates("2021"), "2021");
    EXPECT_EQUAL(removeDuplicates("1999"), "19");
    EXPECT_EQUAL(removeDuplicates("112355433367"), "12354367");
}

STUDENT_TEST("Testing finalizeCode functionality"){
    EXPECT_EQUAL(finalizeCode("2060", "Curie"), "C600");
    EXPECT_EQUAL(finalizeCode("020506", "OConner"), "O256");
    EXPECT_EQUAL(finalizeCode("302064010250", "TessierLavigne"), "T264");


}
