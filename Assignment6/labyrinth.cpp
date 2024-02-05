/*
 * File: labyrinth.cpp
 * ----------------
 * By: Christopher Patron
 * Project: CS 106B Assignment 6 - Labyrinth
 * Date: 11/14/2022
 *
 * labyrinth.cpp is a program that has the user traverse a grid using pointers
 * to find the required items from a list.
 */
#include "labyrinth.h"
#include "demo/MazeGenerator.h"
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;

/* Change kYourName constant to your full name.
 *
 * WARNING: Once you've set set this constant and started exploring your labyrinth,
 * do NOT edit the value of kYourName. Changing kYourName will change which
 * maze you get back, which might invalidate all your hard work!
 */
const string kYourName = "christopherpatron";

/* Change this constant to the path out of your labyrinth. */
const string kPathForYourName = "SSESEEWWNEEWWNENWW";

/**
 * Given a location in a maze, returns whether the given sequence of
 * steps will let you escape the maze.
 *
 * To escape the maze, you need to collect all items in the `needed` set.
 * You can only take steps in the four cardinal directions,
 * and you can't move in directions that don't exist in the maze.
 */
bool isPathToFreedom(MazeCell* start, string moves, Set<string> needed) {
    Set<string> found;
    MazeCell* current = start;
    string item = current -> contents;

    // Checking if start location has an item
    if(needed.contains(item)){
        found.add(item);
        if (found == needed){
            return true;
        }
    }

    for (char elem : moves){
        // Moving North
        if(elem == 'N'){
            if (current -> north != nullptr){
                item = current -> north -> contents;
                if (needed.contains(item)){
                    found.add(item);
                    if (found == needed){
                        return true;
                    }
                }
                current = current -> north;
            }
            else {
                return false;
            }
        }

        // Moving South
        else if(elem == 'S'){
            if (current -> south != nullptr){
                item = current -> south -> contents;
                if (needed.contains(item)){
                    found.add(item);
                    if (found == needed){
                        return true;
                    }
                }
                current = current -> south;
            }
            else {
                return false;
            }
        }

        // Moving East
        else if(elem == 'E'){
            if (current -> east != nullptr){
                item = current -> east -> contents;
                if (needed.contains(item)){
                    found.add(item);
                    if (found == needed){
                        return true;
                    }
                }
                current = current -> east;
            }
            else {
                return false;
            }
        }

        // Moving West
        else if(elem == 'W'){
            if (current -> west != nullptr){
                item = current -> west -> contents;
                if (needed.contains(item)){
                    found.add(item);
                    if (found == needed){
                        return true;
                    }
                }
                current = current -> west;
            }
            else {
                return false;
            }
        }
        else{
            error("An invalid character was encountered in the provided solution");
        }

    }

    if (found == needed){
        return true;
    }
    else {
        return false;
    }
}


/* * * * * * Test Cases Below This Point * * * * * */

PROVIDED_TEST("isPathToFreedom: paths on example labyrinth from writeup") {
    Set<string> allThree = {"Spellbook", "Potion", "Wand"};
    auto maze = toMaze({"* *-W *",
                        "| |   |",
                        "*-* * *",
                        "  | | |",
                        "S *-*-*",
                        "|   | |",
                        "*-*-* P"});
    MazeCell *smileyStart = maze[2][2];

    /* These paths are in the writeup. Both succeed. */
    EXPECT(isPathToFreedom(smileyStart, "ESNWWNNEWSSESWWN", allThree));
    EXPECT(isPathToFreedom(smileyStart, "SWWNSEENWNNEWSSEES", allThree));

    /* This path successfully collects just spellbook */
    EXPECT(isPathToFreedom(smileyStart, "SWWN", {"Spellbook"}));

    /* This path fails, doesn't collect all three items */
    EXPECT(!isPathToFreedom(smileyStart, "WNNE", allThree));

    /* This path fails, cannot walk through nullptr link */
    EXPECT(!isPathToFreedom(smileyStart, "WNWS", allThree));
}

PROVIDED_TEST("isPathToFreedom: collects needed item from start") {
    auto maze = toMaze({"P-S-W"});

    EXPECT(isPathToFreedom(maze[0][0], "E", {"Potion"}));
    EXPECT(isPathToFreedom(maze[0][1], "W", {"Potion", "Spellbook"}));
}

PROVIDED_TEST("isPathToFreedom: errors if attempt step invalid direction char") {
    auto maze = toMaze({"P-S-W"});

    EXPECT_ERROR(isPathToFreedom(maze[0][0], "EWQ", {"Wand"})); // Q is invalid step
    EXPECT(!isPathToFreedom(maze[0][0], "EWWZ", {"Wand"})); // fails at nullptr link
    EXPECT_NO_ERROR(isPathToFreedom(maze[0][0], "EEG", {"Wand"})); // success, rest of path ignored
}

PROVIDED_TEST("Escape from your personal labyrinth!") {
    Set<string> allThree = {"Spellbook", "Potion", "Wand"};
    /* A maze for you to escape from. This maze will be personalized
     * based on the constant kYourName.
     */
    EXPECT(kYourName != "TODO");
    MazeCell* startLocation = mazeFor(kYourName);

    /* Set a breakpoint here. As a reminder, the labyrinth you
     * get will be personalized to you, so don't start exploring the labyrinth
     * unless you've edited the constant kYourName to include your name(s)!
     * Otherwise, you'll be escaping the wrong labyrinth.
     */
    EXPECT(kPathForYourName != "TODO");
    EXPECT(isPathToFreedom(startLocation, kPathForYourName, allThree));
}

