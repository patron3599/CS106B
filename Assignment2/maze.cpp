/*
 * File: maze.cpp
 * ----------------
 * By: Christopher Patron
 * Project: CS 106B Assignment 2 - Mazes
 * Date: 10/10/2022
 *
 * maze.cpp is a program that implements both a breadth-first search
 * and a depth-first search to solve a maze. This maze is constructued
 * of a grid of boolean values where "false" represents an obstruction
 * and "true" a clear path.
 */
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;


/* generateValidMoves takes in the maze and current location. After this it then
 * calculates all possible move locations in the North, South, East, and West
 * directions. It then proceeds to check each move and if it is valid, it will
 * be added to the neighbors set. Neighbors is then returned at the end of the
 * function.
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;

    GridLocation north = {cur.row - 1, cur.col};
    GridLocation south = {cur.row + 1, cur.col};
    GridLocation east = {cur.row, cur.col + 1};
    GridLocation west = {cur.row, cur.col - 1};


    // Check North
    if (cur.row >= 1 && north.row >= 0){
        if (maze[north] == true){
            neighbors.add(north);
        }
    }
    // Check South
    if (cur.row <= maze.numRows() - 1 && south.row < maze.numRows()){
        if (maze[south] == true){
            neighbors.add(south);
        }
    }
    // Check East
    if (cur.col <= maze.numCols() - 1 && east.col < maze.numCols()){
        if(maze[east] == true){
            neighbors.add(east);
        }
    }
    // Check West
    if (cur.col >= 1 && west.col >= 0){
        if(maze[west] == true){
            neighbors.add(west);
        }
    }

    return neighbors;
}

/* validatePath is passed a maze and path as arguments. Then, it proceeds
 * check each index of the path and validate that moving through it is
 * possible by calling the helper function generateValidMoves for each
 * index in the path. If the path is valid, nothing will happen. If
 * it is invalid, an error will be raised and notifies the user.
 */
void validatePath(Grid<bool>& maze, Vector<GridLocation>& path) {
    if (path.isEmpty()) {
        error("Path is empty!");
    }
    GridLocation entry;
    GridLocation exit = {maze.numRows() - 1, maze.numCols() - 1};
    Set<GridLocation> true_path;
    int last_index = path.size() - 1;

    for (int i = 1; i < path.size(); i++){
        if(path[0] == entry && path[last_index] == exit){
            true_path.add(path[0]);
            true_path.add(path[last_index]);

            Set <GridLocation> temp_moves = generateValidMoves(maze, path[i]);
            if (temp_moves.contains(path[i - 1])){
                true_path.add(path[i - 1]); // Can add i-1 and won't raise an error at the end of the loop
                                            // since the last index of the path has already been added to
                                            // the set.
            }
        }

        else{
           error("Here is my message about what has gone wrong");
        }
    }

    if (path.size() != true_path.size()){ // A valid path will have the same length as the temporary
                                          // path set created in this function.
        error("Here is my message about what has gone wrong");
    }
}

/* solveMazeBFS is passed in a maze as the argument. It then proceeds to
 * solve the maze using a breadth-first search approach where it considers
 * paths outwards from an entry point in a radial fashion. This function
 * accomplishes this with the help of stacks, queues, and vectors to
 * ultimately solve the maze.
 */
Vector<GridLocation> solveMazeBFS(Grid<bool>& maze) {
    Vector<GridLocation> path;
    Queue<Vector<GridLocation>> allPaths;
    drawMaze(maze);
    GridLocation entry;
    GridLocation exit = {maze.numRows() - 1, maze.numCols() - 1};
    Set<GridLocation>previous_valid_moves; // To prevent a cyclic path and to not allow
                                           // the algorithm to visit a previously visited location

    path.add(entry);
    allPaths.enqueue(path);
    previous_valid_moves.add(entry);

    while(allPaths.size() != 0){

    path = allPaths.dequeue();
    highlightPath(path, "blue", 15);
    int last_index = path.size() - 1;

            if (path[last_index] == exit){
                return path;
            }

            else{
                Set<GridLocation> neighbors = generateValidMoves(maze, path[last_index]);
                for (auto elem : neighbors){
                    if (!previous_valid_moves.contains(elem)){ // Preventing cyclic path or location revisiting
                        path.add(elem);
                        int temp_last_index = path.size() - 1; // Introduced because the size of the path changes within
                                                               // the loop
                        allPaths.enqueue(path);
                        previous_valid_moves.add(path[temp_last_index]);
                        path.remove(temp_last_index);          // To allow for the next neighbor to be added from the
                                                               // current position
                    }
                }
            path.clear(); // To initialize the new path for the next loop
            }
    }
    return path;
}

/* solveMazeDFS is passed in a maze as the argument. It then proceeds to
 * solve the maze using a depth-first search approach where it considers
 * one path at a time until it reaches the exit or is an invalid path.
 * This function accomplishes this with the help of stacks and vectors to
 * ultimately solve the maze.
 */
Vector<GridLocation> solveMazeDFS(Grid<bool>& maze) {
    Vector<GridLocation> path;
    Stack<Vector<GridLocation>> allPaths;
    drawMaze(maze);

    GridLocation entry;
    GridLocation exit = {maze.numRows() - 1, maze.numCols() - 1};
    Set<GridLocation>previous_valid_moves; // To prevent a cyclic path and to not allow
                                           // the algorithm to visit a previously visited location

    path.add(entry);
    allPaths.push(path);
    previous_valid_moves.add(entry);

    while(allPaths.size() != 0){

    path = allPaths.pop();
    highlightPath(path, "blue", 15);
    int last_index = path.size() - 1;

            if (path[last_index] == exit){
                return path;
            }

            else{
                Set<GridLocation> neighbors = generateValidMoves(maze, path[last_index]);
                for (auto elem : neighbors){
                    if (!previous_valid_moves.contains(elem)){ // Preventing cyclic path or location revisiting
                        path.add(elem);
                        int temp_last_index = path.size() - 1; // Introduced because the size of the path changes within
                                                               // the loop
                        allPaths.push(path);
                        previous_valid_moves.add(path[temp_last_index]);
                        path.remove(temp_last_index);          // To allow for the next neighbor to be added from the
                                                               // current position
                    }
                }
            path.clear(); // To initialize the new path for the next loop
            }
    }
    return path;
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Vector<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0, 0}, {0, 2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

PROVIDED_TEST("validatePath on correct solution, hand-constructed maze") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Vector<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution read from file, medium maze") {
    Grid<bool> maze;
    Vector<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true},
                       {true, true}};
    Vector<GridLocation> not_end_at_exit = { {0, 0}, {1, 0}, {2, 0} };
    Vector<GridLocation> not_begin_at_entry = { {1, 1}, {2, 1} };
    Vector<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1}, {2, 1} };

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
}

PROVIDED_TEST("solveMazeBFS on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Vector<GridLocation> soln = solveMazeBFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMazeDFS on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Vector<GridLocation> soln = solveMazeDFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

// STUDENT TESTS //

STUDENT_TEST("generateValidMoves on corner of 2x2 grid with walls on both sides") {
    Grid<bool> maze = {{true, false},
                       {false, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

STUDENT_TEST("generateValidMoves in middle of 4x4 grid with walls on 3 sides") {
    Grid<bool> maze = {{true, false, true, true},
                       {true, true, false, false},
                       {false, false, true, true},
                       {true, true, true, true}};
    GridLocation corner = {1, 1};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

STUDENT_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {1, 2};
    Set<GridLocation> expected = {{0, 2}, {2, 2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}


STUDENT_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false, true},
                       {true, true, false},
                       {true, true, true}};
    Vector<GridLocation> start_in_middle = { {1, 1}, {2, 1}, {2, 2} };

    EXPECT_ERROR(validatePath(maze, start_in_middle));
}

STUDENT_TEST("validatePath on valid path should raise no error") {
    Grid<bool> maze = {{true, false, true},
                       {true, true, false},
                       {true, true, true}};
    Vector<GridLocation> valid_path_1 = { {0, 0}, {1, 0}, {1, 1}, {2, 1}, {2, 2} };
    Vector<GridLocation> valid_path_2 = { {0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2} };

    EXPECT_NO_ERROR(validatePath(maze, valid_path_1));
    EXPECT_NO_ERROR(validatePath(maze, valid_path_2));
}

STUDENT_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {false, true},
                       {true, true}};
    Vector<GridLocation> nowhere_to_go = { {0, 0}, {1, 0}, {2, 0}, {2, 1} };

    EXPECT_ERROR(validatePath(maze, nowhere_to_go));
}

STUDENT_TEST("solveMazeBFS of 4x4 grid") {
    Grid<bool> maze = {{true, false, true, false},
                       {true, true, true, false},
                       {true, false, true, true},
                       {false, false, false, true}};

    Vector<GridLocation> soln = solveMazeBFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMazeBFS of 5x5 grid") {
    Grid<bool> maze = {{true, true, false, false, false},
                       {true, true, true, true, true},
                       {true, false, false, true, false},
                       {true, true, false, true, true},
                       {true, true, false, false, true}};

    Vector<GridLocation> soln = solveMazeBFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMazeBFS of 5x6 grid") {
    Grid<bool> maze = {{true, true, false, true, true, true},
                       {true, true, true, true, false, true},
                       {true, false, false, true, false, true},
                       {true, true, false, false, false, true},
                       {true, true, true, true, false, true}};

    Vector<GridLocation> soln = solveMazeBFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMazeBFS on all maze files") {
    Grid<bool> maze;
    Vector<string> maze_files = {"res/2x2.maze", "res/5x7.maze", "res/13x39.maze", "res/17x37.maze", "res/19x35.maze"
                                , "res/21x23.maze", "res/21x25.maze", "res/21x35.maze", "res/25x15.maze", "res/25x33.maze"
                                , "res/33x41.maze"};
    for (int i = 0; i < maze_files.size(); i++) {
        readMazeFile(maze_files[i], maze);
        Vector<GridLocation> soln = solveMazeBFS(maze);

        EXPECT_NO_ERROR(validatePath(maze, soln));
    }
}

STUDENT_TEST("solveMazeDFS on all maze files") {
    Grid<bool> maze;
    Vector<string> maze_files = {"res/2x2.maze", "res/5x7.maze", "res/13x39.maze", "res/17x37.maze", "res/19x35.maze"
                                , "res/21x23.maze", "res/21x25.maze", "res/21x35.maze", "res/25x15.maze", "res/25x33.maze"
                                , "res/33x41.maze"};
    for (int i = 0; i < maze_files.size(); i++) {
        readMazeFile(maze_files[i], maze);
        Vector<GridLocation> soln = solveMazeDFS(maze);

        EXPECT_NO_ERROR(validatePath(maze, soln));
    }
}



