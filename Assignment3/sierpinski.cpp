/*
 * File: sierpinksi.cpp
 * ----------------
 * By: Christopher Patron
 * Project: CS 106B Assignment 3 - Sierpinski Triangle
 * Date: 10/10/2022
 *
 * sierpinski.cpp is a program that creates a GUI for the user to
 * toggle an order "n" sierpinksi triangle. This triangle is constructed
 * recursively off the initial points of a single triangle that is provided.
 */
#include <iostream>    // for cout, endl
#include "recursion.h"
#include "SimpleTest.h"
using namespace std;

/*
 * This function draws a filled, black triangle on the provided GWindow
 * that is defined by the corner GPoints one, two, and three. This
 * function has been provided for you and does not need to be modified.
 */
void fillBlackTriangle(GWindow& window, GPoint one, GPoint two, GPoint three) {
    window.setColor("black");
    window.fillPolygon( {one, two, three} );
}

/*
 * drawSierpinskiTriangle takes in the window and three points of a triangle as arguments.
 * It then creates a GUI where the user can toggle an order "n" Sierpinski triangle which
 * is constructed recursively.
 */
int drawSierpinskiTriangle(GWindow& window, GPoint one, GPoint two, GPoint three, int order) {
    // Base Case: Create an order 0 triangle
    if (order == 0){
        fillBlackTriangle(window, one, two, three);
        return 1;
    }
    else{
    // Recursive Case: Create a smaller triangle to eventually be drawn by fillBlackTriangle
    order -= 1;
    GPoint pt_1_2 = GPoint ((one.x + two.x) / 2 , (one.y + two.y) / 2 );
    GPoint pt_1_3 = GPoint ((one.x + three.x) / 2, (one.y + three.y) / 2);
    GPoint pt_2_3 = GPoint ((two.x + three.x) / 2, (two.y + three.y) / 2);

    int count_1 = drawSierpinskiTriangle(window, one, pt_1_2, pt_1_3, order);
    int count_2 = drawSierpinskiTriangle(window, two, pt_1_2, pt_2_3, order);
    int count_3 = drawSierpinskiTriangle(window, three, pt_1_3, pt_2_3, order);
    int tot_count = count_1 + count_2 + count_3;

    return tot_count;
    }
}


/* * * * * * Test Cases * * * * * */

/*
 * Do not modify the code beyond this line! There are no
 * unit tests for this problem. You should do all your testing
 * via the interactive demo.
 */

PROVIDED_TEST("Test fractal drawing interactively using graphical demo") {
    runInteractiveDemo();
}

