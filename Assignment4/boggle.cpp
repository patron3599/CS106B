/*
 * File: boggle.cpp
 * ----------------
 * By: Christopher Patron
 * Project: CS 106B Assignment 4 - Boggle
 * Date: 10/26/2022
 *
 * boggle.cpp is a program that finds all the words in the english dictionary
 * and tallies a score based on the length of the word. The program loops through
 * a grid of characters to find the combination of possible words.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "backtracking.h"
#include "gridlocation.h"
#include "grid.h"
#include "set.h"
#include "lexicon.h"
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;

/* * * FUNCTION PROTOTYPES * * */
int scoreBoard(Grid<char>& board, Lexicon& lex);
int scoreBoard(Grid<char>& board, Lexicon& lex, string& prefix, int& score, Set<GridLocation>& previous_moves, GridLocation& cur, Set<string>& words_set);
void loopThroughBoard(Grid<char>& board, Lexicon& lex, string& prefix, int& score, Set<GridLocation>& previous_moves, Set<string>& words_set);


/* generateValidMoves takes in the board and current location. After this it then
 * calculates all possible move locations in the North, South, East, West, and
 * diagonal directions. It then proceeds to check each move and if it is valid, it will
 * be added to the neighbors set. Neighbors is then returned at the end of the
 * function.
 */
Set<GridLocation> generateValidMoves(Grid<char>& board, GridLocation cur, Lexicon& lex, string& prefix) {
    Set<GridLocation> neighbors;
    string temp_cur = charToString(board[cur]);

    // Calculating possible move locations
    GridLocation north = {cur.row - 1, cur.col};
    GridLocation south = {cur.row + 1, cur.col};
    GridLocation east = {cur.row, cur.col + 1};
    GridLocation west = {cur.row, cur.col - 1};

    GridLocation up_right = {cur.row - 1, cur.col + 1};
    GridLocation up_left = {cur.row - 1, cur.col - 1};
    GridLocation down_right = {cur.row + 1, cur.col + 1};
    GridLocation down_left = {cur.row + 1, cur.col - 1};


    // Combo 1 checks if the two letters are valid
    // Combo 2 checks if the current word plus the two letters is valid

    // Check North
    if (cur.row >= 1 && north.row >= 0){
        string temp_elem = charToString(board[north]);
        string combo1 = temp_cur + temp_elem;
        string combo2 = prefix + temp_elem;
        if (lex.containsPrefix(combo1) || lex.containsPrefix(combo2)){
            neighbors.add(north);
        }
    }
    // Check South
    if (cur.row <= board.numRows() - 1 && south.row < board.numRows()){
        string temp_elem = charToString(board[south]);
        string combo1 = temp_cur + temp_elem;
        string combo2 = prefix + temp_elem;
        if (lex.containsPrefix(combo1) || lex.containsPrefix(combo2)){
            neighbors.add(south);
        }
    }
    // Check East
    if (cur.col <= board.numCols() - 1 && east.col < board.numCols()){
        string temp_elem = charToString(board[east]);
        string combo1 = temp_cur + temp_elem;
        string combo2 = prefix + temp_elem;
        if (lex.containsPrefix(combo1) || lex.containsPrefix(combo2)){
            neighbors.add(east);
        }
    }
    // Check West
    if (cur.col >= 1 && west.col >= 0){
        string temp_elem = charToString(board[west]);
        string combo1 = temp_cur + temp_elem;
        string combo2 = prefix + temp_elem;
        if (lex.containsPrefix(combo1) || lex.containsPrefix(combo2)){
            neighbors.add(west);
        }
    }
    // Check Up_Right (Combining North and East conditions)
    if (cur.row >= 1 && north.row >= 0 && cur.col <= board.numCols() - 1 && east.col < board.numCols()){
        string temp_elem = charToString(board[up_right]);
        string combo1 = temp_cur + temp_elem;
        string combo2 = prefix + temp_elem;
        if (lex.containsPrefix(combo1) || lex.containsPrefix(combo2)){
            neighbors.add(up_right);
        }
    }
    // Check Up_Left (Combining North and West conditions)
    if (cur.row >= 1 && north.row >= 0 && cur.col >= 1 && west.col >= 0){
        string temp_elem = charToString(board[up_left]);
        string combo1 = temp_cur + temp_elem;
        string combo2 = prefix + temp_elem;
        if (lex.containsPrefix(combo1) || lex.containsPrefix(combo2)){
            neighbors.add(up_left);
        }
    }
    // Check Down_Right (Combining South and East conditions)
    if (cur.row <= board.numRows() - 1 && south.row < board.numRows() && cur.col <= board.numCols() - 1 && east.col < board.numCols()){
        string temp_elem = charToString(board[down_right]);
        string combo1 = temp_cur + temp_elem;
        string combo2 = prefix + temp_elem;
        if (lex.containsPrefix(combo1) || lex.containsPrefix(combo2)){
            neighbors.add(down_right);
        }
    }
    // Check Down_Left (Combining South and West conditions)
    if (cur.row <= board.numRows() - 1 && south.row < board.numRows() && cur.col >= 1 && west.col >= 0){
        string temp_elem = charToString(board[down_left]);
        string combo1 = temp_cur + temp_elem;
        string combo2 = prefix + temp_elem;
        if (lex.containsPrefix(combo1) || lex.containsPrefix(combo2)){
            neighbors.add(down_left);
        }
    }

    return neighbors;
}


/* points takes in a found string and returns the corresponding score for that
 * string as defined by the criteria in the problem statement.
 */
int points(string str) {
    return str.size() - 3;
}


/* scoreBoard uses a wrapper function to exectue the program. The wrapper function is passed
 * the board, lexicon dictionary, a string that will store the temporary word, the score, previous
 * moves made, the current location, and a set containing the words that have been found. It calls the
 * helper functions generateValidMoves and loopThroughBoard.
 */

int scoreBoard(Grid<char>& board, Lexicon& lex){ // Wrapper Function
    string prefix = "";
    int score = 0;
    Set<GridLocation> previous_moves;
    GridLocation cur;
    Set<string> words_set;
    loopThroughBoard(board, lex, prefix, score, previous_moves, words_set);

    return score;
}


int scoreBoard(Grid<char>& board, Lexicon& lex, string& prefix, int& score, Set<GridLocation>& previous_moves, GridLocation& cur, Set<string>& words_set) {

    // Base Case: A word has been found or a word in progress has been pruned
    if (lex.contains(prefix) && prefix.size() >= 4 && !words_set.contains(prefix)){
        words_set.add(prefix);
        score += points(prefix);
    }
    // Recursive Case: Choosing options

        Set <GridLocation> neighbors = generateValidMoves(board, cur, lex, prefix);

        for(auto temp_loc : neighbors){
            if(!previous_moves.contains(temp_loc)){
                previous_moves.add(temp_loc);
                prefix += charToString(board[temp_loc]);
                scoreBoard(board, lex, prefix, score, previous_moves, temp_loc, words_set);

                // Unchoosing once all options with that word has been explored
                previous_moves.remove(temp_loc);
                prefix = prefix.substr(0, prefix.size() - 1);
            }
        }
    return score;
}


/* loopThroughBoard is a helper function that is called by scoreBoard and assists in looping through the board.
 * This was introduced due to the fact that each time a recursive call is made, the loop would restart and therefore
 * sometype of escape from that was needed to ensure that all combinations of words are checked.
 */
void loopThroughBoard(Grid<char>& board, Lexicon& lex, string& prefix, int& score, Set<GridLocation>& previous_moves, Set<string>& words_set){
    for(int i = 0; i < board.numRows(); i++){
        for(int j = 0; j < board.numCols(); j++){
            GridLocation cur(i, j);
            prefix = ""; // Reset the current word each time we start with a new base letter
            prefix += charToString(board[i][j]);
            previous_moves.clear(); // Reset the location of moves we've made each time we start with a new base letter
            previous_moves.add(cur);
            scoreBoard(board, lex, prefix, score, previous_moves, cur, words_set);
        }
    }
}

/* * * * * * Test Cases * * * * * */

/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
static Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}

PROVIDED_TEST("Load shared Lexicon, confirm number of words") {
    Lexicon lex = sharedLexicon();
    EXPECT_EQUAL(lex.size(), 127145);
}

PROVIDED_TEST("Test point scoring") {
    EXPECT_EQUAL(points("and"), 0);
    EXPECT_EQUAL(points("quad"), 1);
    EXPECT_EQUAL(points("quint"), 2);
    EXPECT_EQUAL(points("sextet"), 3);
    EXPECT_EQUAL(points("seventh"), 4);
    EXPECT_EQUAL(points("supercomputer"), 10);
}

PROVIDED_TEST("Test scoreBoard, board contains no words, score of zero") {
    Grid<char> board = {{'B','C','D','F'}, //no vowels, no words
                        {'G','H','J','K'},
                        {'L','M','N','P'},
                        {'Q','R','S','T'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);
}

PROVIDED_TEST("Test scoreBoard, board contains one word, score of 1") {
    Grid<char> board = {{'C','_','_','_'},
                        {'Z','_','_','_'},
                        {'_','A','_','_'},
                        {'_','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, alternate paths for same word, still score of 1") {
    Grid<char> board = {{'C','C','_','_'},
                        {'C','Z','C','_'},
                        {'_','A','_','_'},
                        {'R','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, small number of words in corner of board") {
    Grid<char> board = {{'L','I','_','_'},
                        {'M','E','_','_'},
                        {'_','S','_','_'},
                        {'_','_','_','_'}};
    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()),  2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, small number of words") {
    Grid<char> board = {{'E','Z','R','R'},
                        {'O','H','I','O'},
                        {'N','J','I','H'},
                        {'Y','A','H','O'}};
    Set<string> words = { "HORIZON", "OHIA", "ORZO", "JOHN", "HAJI"};
    int nPoints = 0;
    for (string word: words) {
        nPoints += points(word);
    }
    EXPECT_EQUAL(nPoints, 4 + 1 + 1 + 1 + 1);
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), nPoints);
}

PROVIDED_TEST("Test scoreBoard, full board, medium number of words") {
    Grid<char> board = {{'O','T','H','X'},
                        {'T','H','T','P'},
                        {'S','S','F','E'},
                        {'N','A','L','T'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 76);
}

PROVIDED_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 234);
}

// STUDENT TESTS //

STUDENT_TEST("Test scoreBoard, board contains three words, score of 4") {
    Grid<char> board = {{'_','_','_','G'},
                        {'S','A','L','-'},
                        {'S','_','A','-'},
                        {'-','-','_','-'}};

    Set<string> words = {"GLASS", "LASS", "ALAS"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 2 + 1 + 1);
}
