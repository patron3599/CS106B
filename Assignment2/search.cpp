/*
 * File: search.cpp
 * ----------------
 * By: Christopher Patron
 * Project: CS 106B Assignment 2 - Search Engine
 * Date: 10/11/2022
 *
 * This program reads in a website text file and serves as a search
 * engine for that file. The user can input querys through the command
 * window to find the location of the query in the urls.
 */
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;


/* cleanToken is passed a string as its argument. It then proceeds to loop
 * through the string, remove all non-alphabetic characters, and return the
 * string in lowercase containing the alphabetic characters and numbers,
 * if any.
 */
string cleanToken(string s) {
    s = ::toLowerCase(s);
    string result = "";
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i]) || isdigit(s[i])) {
            result += s[i];
        }
    }
    return result;
}

/* gatherTokens is passed a string as an argument. It then proceeds to
 * split the string by spaces and then call cleanToken on all elements
 * that have been splt. It then takes each clean token and adds it to a
 * set containing all the tokens from the body of text.
 */
Set<string> gatherTokens(string bodytext) {
    Set<string> tokens; 
    Vector<string> split_text = stringSplit(bodytext, " ");

    for (int i = 0; i < split_text.size(); i++){
        string temp_token = cleanToken(split_text[i]);
        if (temp_token != ""){
            tokens.add(temp_token);
        }

    }
    return tokens;
}

/* buildIndex is passed the website data file and a Map to be
 * indexed invertedely; The key values are unique tokens and the
 * values are the url's in which that token is found. It then
 * processes the website data file line by line and calls the
 * help of gatherTokens for each body of text from the url. At
 * the end, it returns the number of pages (i.e url's) that were
 * processed.
 */
int buildIndex(string dbfile, Map<string, Set<string>>& index) {
    // Reading in the file
    ifstream in;

    if (!openFile(in, dbfile))
        error("Cannot open file named " + dbfile);

    // Starting to build the inverted index
    Vector<string> lines;
    readEntireFile(in, lines);
    int npages = 0;

    for (int i = 1; i < lines.size() + 1; i += 2){
        string url = lines[i - 1];
        npages += 1;
        string text = lines[i];
        Set<string> temp_tokens = gatherTokens(text);
        Set<string> temp_value;

        for(auto elem : temp_tokens){
            if (!index.containsKey(elem)){
                temp_value.add(url);
                index.put(elem, temp_value);
            }

            else {
                Set<string> existing_value = index[elem];
                existing_value.add(url);
                index[elem] = existing_value;
            }
        }

    }
    return npages;
}

/* findQueryMatches is passed the index and searh query as arguments. It then
 * proceeds to process the query provided by the user and searches for it in
 * the index. Depending on how the user inputs the query, certain operations
 * account for the union, intersection, or difference of the queries that are
 * entered. The function then returns the results as a set.
 */
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query) {
    Set<string> result;
    Set<string> temp_set1;
    Set<string> temp_set2;

    Vector<string> query_parts = stringSplit(query, " ");

    // Creating "base" set since all operations will be based off this word
    string word1 = cleanToken(query_parts[0]);
    temp_set1 = index[word1];

    if(query_parts.size() == 1) {
        Set<string> result = index.get(word1);
        return result;
    }

    else {
        for (int i = 1; i < query_parts.size(); i++) {
            string word2 = cleanToken(query_parts[i]);
            string dirty_word2 = query_parts[i];

            char operation = dirty_word2[0];

            if(index.containsKey(word2)) {
                temp_set2 = index[word2];
            }

            if(operation == '+'){
                temp_set1.intersect(temp_set2);
            }
            else if(operation == '-'){
                temp_set1.difference(temp_set2);
            }
            else{
                 temp_set1.unionWith(temp_set2);
            }

        result = temp_set1;
        }

    }
    return result;
}

/* searchEngine takes in a website file and creates an inverted index.
 * After creating this, it then becomes an interactive program where it
 * prompts the user to type in a search query and return the url's where
 * that query can be found.
 */
void searchEngine(string dbfile) {
    Map<string, Set<string>> index;

    cout << "Stand by while building index..." << endl;
    int npages = buildIndex(dbfile, index);
    cout << "Indexed " << npages << " pages while containing " << index.size() << " unique terms." << endl;

    string query;
    cout << endl;
    cout << "Enter query sentence (RETURN/ENTER to quit): ";
    getline(cin, query);

    while (query != ""){
        Set<string> result;
        result = findQueryMatches(index, query);

        cout << "Found " << result.size() << " matching pages" << endl;
        cout << result;
        cout << endl << endl;

        cout << "Enter query sentence (RETURN/ENTER to quit): ";
        getline(cin, query);
    }
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings of letters and digits") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS106B"), "cs106b");
}

PROVIDED_TEST("cleanToken on strings containing punctuation") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
    EXPECT_EQUAL(cleanToken("they're"), "theyre");
}

PROVIDED_TEST("cleanToken on string of only punctuation") {
    EXPECT_EQUAL(cleanToken("#$^@@.;"), "");
}


PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 6 unique tokens, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 6);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 12 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 12);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


// STUDENT TESTS //
STUDENT_TEST("cleanToken on strings of letters, digits, and spaces") {
    EXPECT_EQUAL(cleanToken("mer ha ba"), "merhaba");
    EXPECT_EQUAL(cleanToken("S`T~a*n?FoR@D 2022"), "stanford2022");
    EXPECT_EQUAL(cleanToken("http://cs106b.stanford.edu/about_assessments"), "httpcs106bstanfordeduaboutassessments");
    EXPECT_EQUAL(cleanToken("!@#$%^&*()"), "");
}

STUDENT_TEST("gatherTokens from simple string") {
    Set<string> expected = {"cs106b", "is", "so", "fun"};
    EXPECT_EQUAL(gatherTokens("CS-106b I$s sO %% F&&&&&&&&u&&&&&n!!!!!!!!"), expected);
}

STUDENT_TEST("gatherTokens from a sentence in website.txt") {
    Set<string> tokens = gatherTokens("The problem isn't with the square() function, but with the buggy test case that produces a false negative.");
    EXPECT_EQUAL(tokens.size(), 15);
}

STUDENT_TEST("gatherTokens from simple string") {
    Set<string> tokens = gatherTokens("6738 %^$* 9*6%");
    EXPECT_EQUAL(tokens.size(), 2);
}

STUDENT_TEST("gatherTokens from simple string") {
    Set<string> tokens = gatherTokens("(*^%&) %^$* $#)&%");
    EXPECT_EQUAL(tokens.size(), 0);
}


STUDENT_TEST("buildIndex from website.txt, 72/2 pages") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/website.txt", index);
    EXPECT_EQUAL(nPages, 36);
    EXPECT(index.containsKey("durand"));
    EXPECT(index.containsKey("cs107"));
    EXPECT(index.containsKey("cs106"));
    EXPECT(index.containsKey("timeoperation")); // Originally TIME_OPERATION
    EXPECT(index.containsKey("2529")); // Originally 2.5-2.9
}

STUDENT_TEST("Running TIME_OPERATION on buildIndex for the website file"){
    Map<string, Set<string>> index;
    TIME_OPERATION(0, buildIndex("res/website.txt", index));
}

STUDENT_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesMilkandGreen = findQueryMatches(index, "milk +green");
    EXPECT_EQUAL(matchesMilkandGreen.size(), 0);

    Set<string> matchesFishwithoutiorBread = findQueryMatches(index, "fish -i bread");
    EXPECT_EQUAL(matchesFishwithoutiorBread.size(), 2);

    Set<string> matchesRedandBlueorFishwithout10 = findQueryMatches(index, "red +blue fish -10");
    EXPECT_EQUAL(matchesRedandBlueorFishwithout10.size(), 3);
}
