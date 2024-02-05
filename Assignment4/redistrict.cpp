/*
 * File: redistrict.cpp
 * ----------------
 * By: Christopher Patron
 * Project: CS 106B Assignment 4 - Computational Redistricting
 * Date: 10/24/2022
 *
 * redistrict.cpp is a program that redistricts a given grid based on the
 * number of districts and the total number of population. This does this
 * in an unbiased way such that each district has exactly
 * total population / nDistricts number in each districit. If this can not be
 * achieved, the program will return false to indicate that redistricting is
 * not possible.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "backtracking.h"
#include "grid.h"
#include "gridgraphics.h"
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;


/* * * * FUNCTION PROTOTYPES * * * */
bool canRedistrict(Grid<int>& populationGrid, int nDistricts,int& target_size, int& found_districts);
bool checkHorizSplits(Grid<int>& populationGrid, int nDistricts, int& target_size, GridLocationRange& all, int& found_districts);
bool checkVertSplits(Grid<int>& populationGrid, int nDistricts, int& target_size, GridLocationRange& all, int& found_districts);


/* subgridSum is passed a population grid and a range within that grid. It then
 * proceeds to sum the total number of people in that grid and return the value.
 */
int subgridSum(Grid<int>& g, GridLocationRange range) {
    int sub_sum = 0;
    for (GridLocation loc : range){
        sub_sum += g[loc];
    }
    return sub_sum;
}


/* divideHorizontal is a function that is passed a set of grid location ranges with a starting point on where the
 * the grid wants to be divided horizontally. It modifies the "top" and "bottom" subgrids on where the splitting is
 * occuring.
 */
void divideHorizontal(GridLocationRange range, int topEndRow, GridLocationRange& top, GridLocationRange& bottom) {

    top = {range.startRow(), range.startCol(), topEndRow, range.endCol()};
    bottom = {topEndRow + 1, range.startCol(), range.endRow(), range.endCol()};

}


/* divideVertical is a function that is passed a set of grid location ranges with a starting point on where the
 * the grid wants to be divided vertically. It modifies the "left" and "right" subgrids on where the splitting is
 * occuring.
 */
void divideVertical(GridLocationRange range, int leftEndCol, GridLocationRange& left, GridLocationRange& right) {

    left = {range.startRow(), range.startCol(), range.endRow(), leftEndCol};
    right = {range.startRow(), leftEndCol + 1, range.endRow(), range.endCol()};

}


/* getSubGrid is a function that is passed the populationGrid and a range over which a subgrid wants to be created.
 * At first, populationGrid is the original one, but as the program continues it will be passed subgrids of the
 * original populationGrid.
 */
Grid<int> getSubGrid(Grid<int>& populationGrid, GridLocationRange range) {
    int num_rows = range.endRow() - range.startRow() + 1;
    int num_cols = range.endCol() - range.startCol() + 1;
    Grid <int> subGrid(num_rows, num_cols);

    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            subGrid[i][j] = populationGrid[range.startRow() + i][range.startCol() + j];
        }
    }
    return subGrid;
}


/* caRedistrict is a function that is passed a populationGrid and the desired number of districts
 * to be created. It then calls the helper functions checkHorizSplits and checkVertSplits who recursively
 * divide the grid and try to find the optimal district configuration.
 */
bool canRedistrict(Grid<int>& populationGrid, int nDistricts){ // Wrapper Function
    int found_districts = 0;
    int target_size = subgridSum(populationGrid, populationGrid.locations()) / nDistricts;
    if(target_size == 0){
        return false;
    }

    return canRedistrict(populationGrid, nDistricts, target_size, found_districts);
}

bool canRedistrict(Grid<int>& populationGrid, int nDistricts, int& target_size, int& found_districts){
    GridLocationRange all = populationGrid.locations();
    // Base Case: One of the splits is valid and therefore a district has been found
    if (subgridSum(populationGrid, all) == target_size){ // In this check, populationGrid is a subgrid which has been passed
                                                        // either by checkHorizSplits or checkVertSplits.
        found_districts ++;
        if (found_districts == nDistricts){
            return true;
        }
    }
    // Recursive Case: call the helper functions to find a valid horizontal or vertical split.
    if (checkHorizSplits(populationGrid, nDistricts, target_size, all, found_districts)) {
        return true;
    }

    if(checkVertSplits(populationGrid, nDistricts, target_size, all, found_districts)) {
        return true;
    }

     return false;
}


/* checkHorizSplits is a helper function that is called by canRedistrict. It proceeds to try and horizontally split the
 * passed in grid and recursively check whether the split it made was valid or not.
 */
bool checkHorizSplits(Grid<int>& populationGrid, int nDistricts, int& target_size, GridLocationRange& all, int& found_districts){
    for (int i = 0; i < populationGrid.numRows() - 1; i++){
        GridLocationRange top, bottom;
        divideHorizontal(all, i, top, bottom);
        int valid_H_split = subgridSum(populationGrid, top) % target_size;

        if(valid_H_split == 0){
            Grid <int> top_part = getSubGrid(populationGrid, top);
            Grid <int> bottom_part = getSubGrid(populationGrid, bottom);

            bool check_top_part = canRedistrict(top_part, nDistricts, target_size, found_districts);
            if(check_top_part){
                return true;
            }
            bool check_bottom_part = canRedistrict(bottom_part, nDistricts, target_size, found_districts);
            if(check_bottom_part){
                return true;
            }
        }
    }
    return false;
}


/* checkVertSplits is a helper function that is called by canRedistrict. It proceeds to try and vertically split the
 * passed in grid and recursively check whether the split it made was valid or not.
 */
bool checkVertSplits(Grid<int>& populationGrid, int nDistricts, int& target_size, GridLocationRange& all, int& found_districts){
    for (int i = 0; i < populationGrid.numCols() - 1; i++){
        GridLocationRange left, right;
        divideVertical(all, i, left, right);
        int valid_V_split = subgridSum(populationGrid, left) % target_size;

        if(valid_V_split == 0){
            Grid <int> left_part = getSubGrid(populationGrid, left);
            Grid <int> right_part = getSubGrid(populationGrid, right);

            bool check_left_part = canRedistrict(left_part, nDistricts, target_size, found_districts);
            if(check_left_part){
                return true;
            }
            bool check_right_part = canRedistrict(right_part, nDistricts, target_size, found_districts);
            if(check_right_part){
                return true;
            }
        }
    }
    return false;
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("subgridSum sums values in grid range") {
    Grid<int> grid = { {1, 2, 3},
                       {4, 5, 6},
                       {7, 8, 9},
                       {10, 10, 10} };
    GridLocationRange all = grid.locations();
    GridLocationRange firstColumn = GridLocationRange(0, 0, 3, 0);
    GridLocationRange bottomCorner = GridLocationRange(3, 2, 3, 2);

    EXPECT_EQUAL(subgridSum(grid, firstColumn), 22);
    EXPECT_EQUAL(subgridSum(grid, bottomCorner), 10);
    EXPECT_EQUAL(subgridSum(grid, all), 75);
}

PROVIDED_TEST("divide grid range horizontally, test using subgridSum") {
    Grid<int> grid = { {1, 2, 3},
                       {4, 5, 6},
                       {7, 8, 9},
                       {10, 10, 10} };
    GridLocationRange all = grid.locations();
    GridLocationRange subrange1, subrange2;

    divideHorizontal(all, 0, subrange1, subrange2); // split first row from rest
    EXPECT_EQUAL(subgridSum(grid, subrange1), 6);
    EXPECT_EQUAL(subgridSum(grid, subrange2), 69);
}

PROVIDED_TEST("nevada example from writeup can be divided into 4 districts") {
    Grid<int> nevada = { {20, 31, 20},
                         { 5, 19, 30},
                         {25, 25, 25} };

    EXPECT(canRedistrict(nevada, 4));
}

PROVIDED_TEST("wyoming example from writeup cannot be divided") {
    Grid<int> wyoming = { {10, 15},
                         { 5, 10} };

    EXPECT(!canRedistrict(wyoming, 2));
}


PROVIDED_TEST("medium-size animated example from writeup can be divided into 12 districts") {
    Grid<int> medium = { {10, 1,  2, 5, 1, 4},
                          {1, 1,  8, 2, 1, 7},
                          {9, 8, 10, 3, 3, 4},
                          {5, 2,  2, 8, 7, 3},
                          {2, 1,  1, 7, 1, 1} };

    EXPECT(canRedistrict(medium, 12));
}

PROVIDED_TEST("large example can be divided into 25 districts") {
    Grid<int> large = { { 5,  2, 2,  1,  6, 4, 8, 2},
                        {10, 10, 2,  1,  3, 4, 2, 3},
                        { 1,  1, 4,  5,  1, 9, 1, 4},
                        { 9,  5, 5, 10, 10, 3, 3, 4},
                        { 3,  1, 1,  1,  2, 8, 6, 2},
                        { 2,  1, 1,  7,  3, 5, 1, 1},
                        { 1,  1, 2,  2,  7, 5, 1, 9},
                        { 4,  1, 2, 10,  1, 9, 5, 5} };

    EXPECT(canRedistrict(large, 25));
}

// STUDENT TESTS //
STUDENT_TEST("Testing subgridsum on all of nevada example.") {
    Grid<int> nevada = { {20, 31, 20},
                         { 5, 19, 30},
                         {25, 25, 25} };

    GridLocationRange range(0, 0, 2, 2);
    int sum = subgridSum(nevada, range);
    EXPECT_EQUAL(sum, 200);

}

STUDENT_TEST("divide grid range horizontally from a vertically divided grid, test using subgridSum") {
    Grid<int> grid = { {10, 1, 2, 5, 1, 4},
                       {1, 1, 8, 2, 1, 7},
                       {9, 8, 10, 3, 3, 4},
                       {5, 2, 2, 8, 7, 3},
                       {2, 1, 1, 7, 1, 1} };

    GridLocationRange all = {0, 2, 2, 5};
    GridLocationRange subrange1, subrange2;

    divideHorizontal(all, 0, subrange1, subrange2); // split first row in a subgrid
    EXPECT_EQUAL(subgridSum(grid, subrange1), 12);
    EXPECT_EQUAL(subgridSum(grid, subrange2), 38);
}

STUDENT_TEST("Divide grid vertically and verify using subgridSum") {
    Grid<int> nevada = { {20, 31, 20},
                         { 5, 19, 30},
                         {25, 25, 25} };

    GridLocationRange all = nevada.locations();
    GridLocationRange subrange1, subrange2;

    divideVertical(all, 0, subrange1, subrange2); // split first row in a subgrid
    EXPECT_EQUAL(subgridSum(nevada, subrange1), 50);
    EXPECT_EQUAL(subgridSum(nevada, subrange2), 150);

}

STUDENT_TEST("divide grid range vertically from a horizontally divided grid, test using subgridSum") {
    Grid<int> grid = { {10, 1, 2, 5, 1, 4},
                       {1, 1, 8, 2, 1, 7},
                       {9, 8, 10, 3, 3, 4},
                       {5, 2, 2, 8, 7, 3},
                       {2, 1, 1, 7, 1, 1} };

    GridLocationRange all = {0, 2, 2, 5};
    GridLocationRange subrange1, subrange2;

    divideVertical(all, 2, subrange1, subrange2); // split first row in a subgrid
    EXPECT_EQUAL(subgridSum(grid, subrange1), 20);
    EXPECT_EQUAL(subgridSum(grid, subrange2), 30);
}

