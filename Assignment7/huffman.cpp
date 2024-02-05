/*
 * File: huffman.cpp
 * ----------------
 * By: Christopher Patron
 * Project: CS 106B Assignment 7 - Huffman
 * Date: 12/06/2022
 *
 * huffman.cpp is a robust program that can compress and decompress
 * a file. It uses the help of structs, pointers, various containers,
 * and recursion to efficiently store the data of the passed in file.
 * It has the power to compress and decompress the given file.
 */
#include "bits.h"
#include "treenode.h"
#include "huffman.h"
#include "map.h"
#include "priorityqueue.h"
#include "vector.h"
#include "strlib.h"
#include "SimpleTest.h"  // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;

/* * * FUNCTION PROTOTYPES * * */
EncodingTreeNode* unflattenTree(Queue<Bit>& treeShape, Queue<char>& treeLeaves, EncodingTreeNode*& tree);
void buildPaths(EncodingTreeNode*& tree, Map<char, Queue<Bit>>& path, Vector<Bit> temp_path);
Map<char, Queue<Bit>> buildPaths(EncodingTreeNode*& tree);
Map<char, int> getWeights(string text);


/**
 * Given a Queue<Bit> containing the compressed message bits and the encoding tree
 * used to encode those bits, decode the bits back to the original message text.
 *
 * You can assume that tree is a well-formed non-empty encoding tree and
 * messageBits queue contains a valid sequence of encoded bits.
 *
 * Your implementation may change the messageBits queue however you like. There
 * are no requirements about what it should look like after this function
 * returns. The encoding tree should be unchanged.
 */
string decodeText(EncodingTreeNode* tree, Queue<Bit>& messageBits) {
    EncodingTreeNode* curr = tree;
    string decoded_message;
    Bit temp_bit;

    while(!messageBits.isEmpty()){
        temp_bit = messageBits.dequeue();

        if(temp_bit == 0){
            curr = curr -> zero;
            if(curr ->isLeaf()){
               string temp_letter = charToString(curr -> getChar());
               decoded_message += temp_letter;
               curr = tree;
            }
        }
        if(temp_bit == 1){
            curr = curr -> one;
            if(curr ->isLeaf()){
               string temp_letter = charToString(curr -> getChar());
               decoded_message += temp_letter;
               curr = tree;
            }
        }
    }

    return decoded_message;
}

/**
 * Reconstruct encoding tree from flattened form Queue<Bit> and Queue<char>.
 *
 * You can assume that the queues are well-formed and represent
 * a valid encoding tree.
 *
 * Your implementation may change the queue parameters however you like. There
 * are no requirements about what they should look like after this function
 * returns.
 *
 * The function uses the a wrapper function to pass in a root pointer
 * to help construct the tree.
 */

EncodingTreeNode* unflattenTree(Queue<Bit>& treeShape, Queue<char>& treeLeaves){
    Bit temp_root;
    char temp_letter;
    EncodingTreeNode* tree = nullptr;
    temp_root = treeShape.peek();

    if(temp_root == 1){
        unflattenTree(treeShape, treeLeaves, tree);
        return tree;
    }
    // It is a singleton leaf node
    else{
        temp_letter = treeLeaves.dequeue();
        tree = new EncodingTreeNode(temp_letter);
        return tree;
    }

}

EncodingTreeNode* unflattenTree(Queue<Bit>& treeShape, Queue<char>& treeLeaves, EncodingTreeNode*& tree){
    Bit temp_root;
    char temp_letter;

    // Look at next possible root node
    temp_root = treeShape.peek();

    // Base Case: We have found a leaf
    if(temp_root == 0){
        treeShape.dequeue();
        temp_letter = treeLeaves.dequeue();
        tree = new EncodingTreeNode(temp_letter);
        return tree;
    }

    // Recursive Case: Build Interior Node
    treeShape.dequeue();
    tree = new EncodingTreeNode(nullptr, nullptr);

    unflattenTree(treeShape, treeLeaves, tree -> zero);
    unflattenTree(treeShape, treeLeaves, tree -> one);
    return tree;
}

/**
 * Decompress the given EncodedData and return the original text.
 *
 * You can assume the input data is well-formed and was created by a correct
 * implementation of compress.
 *
 * Your implementation may change the data parameter however you like. There
 * are no requirements about what it should look like after this function
 * returns.
 */
string decompress(EncodedData& data) {
    Queue<Bit> message_bits = data.messageBits;
    Queue<Bit> tree_shape = data.treeShape;
    Queue<char> tree_leaves = data.treeLeaves;

    EncodingTreeNode* tree = unflattenTree(tree_shape, tree_leaves);
    string message = decodeText(tree, message_bits);

    deallocateTree(tree);
    return message;
}

/**
 * Constructs an optimal Huffman coding tree for the given text, using
 * the algorithm described in lecture.
 *
 * Reports an error if the input text does not contain at least
 * two distinct characters.
 *
 * When assembling larger trees out of smaller ones, make sure to set the first
 * tree dequeued from the queue to be the zero subtree of the new tree and the
 * second tree as the one subtree.
 */
EncodingTreeNode* buildHuffmanTree(string text) {
    Map<char, int> weights = getWeights(text);
    EncodingTreeNode* left = nullptr;
    EncodingTreeNode* right = nullptr;
    EncodingTreeNode* tree = nullptr;
    PriorityQueue<EncodingTreeNode*> forrest;

    // Building the forrest
    for(char letter: weights){
        EncodingTreeNode* temp = new EncodingTreeNode(letter);
        int weight = weights[letter];
        forrest.enqueue(temp, weight);
    }

    // Building Huffman Tree
    while(forrest.size() != 1){
        // Processing left node
        int node_one_val = forrest.peekPriority();
        EncodingTreeNode* node_one = forrest.dequeue();
        left = node_one;

        // Processing right node
        int node_two_val = forrest.peekPriority();
        EncodingTreeNode* node_two = forrest.dequeue();
        right = node_two;

        // Processing root node
        int tree_priority = node_one_val + node_two_val;
        EncodingTreeNode* root = new EncodingTreeNode(left, right);
        forrest.enqueue(root, tree_priority);
    }

    tree = forrest.dequeue();
    return tree;
}

/**
 * getWeights is a helper function that is called by buildHuffmanTree. It creates
 * a map where the key is the letter from the string and the value is the frequency
 * of how many times it occurs in the string. The map is then returned.
 */
Map<char, int> getWeights(string text){
    Map<char, int> weights;
    for (char letter : text){
        if(weights.containsKey(letter) == false){
            int counter = 1;
            weights.put(letter, counter);
        }
        else{
            weights[letter]++;
        }
    }

    if(weights.size() < 2){
        error("Need atleast two distinct characters to build the Huffman Tree!");
    }

    return weights;
}


/**
 * Given a string and an encoding tree, encode the text using the tree
 * and return a Queue<Bit> of the encoded bit sequence.
 *
 * You can assume tree is a valid non-empty encoding tree and contains an
 * encoding for every character in the text.
 */
Queue<Bit> encodeText(EncodingTreeNode* tree, string text) {
    Queue<Bit> encoded;
    Queue<Bit> sequence;
    Bit temp_bit;
    Map<char, Queue<Bit>> paths = buildPaths(tree);

    //Looping through each character in the string
    for(char letter : text){
        sequence = paths.get(letter);

        //Looping through each bit in the sequence
        while(!sequence.isEmpty()){
           temp_bit = sequence.dequeue();
           encoded.enqueue(temp_bit);
        }
    }
    return encoded;
}

/**
 * buildPaths is a helper function that is called by encodeText. It is passed
 * the tree node and proceeds to build a map for the paths of each letter in
 * the string. The path is in the terms of 0's and 1's where 0 is a left branch
 * and 1 is a right branch.
 */
Map<char, Queue<Bit>> buildPaths(EncodingTreeNode*& tree){
    Map<char, Queue<Bit>> path;
    Vector<Bit> temp_path;
    buildPaths(tree, path, temp_path);
    return path;
}

void buildPaths(EncodingTreeNode*& tree, Map<char, Queue<Bit>>& path, Vector<Bit> temp_path){
    char letter;
    Bit temp_bit, loc;
    Queue<Bit> sequence;
    int last_idx;

    // Base Case: We have a leaf and will enqueue its path
    if(tree -> isLeaf()){
        letter = tree -> getChar();
        for(int i = 0; i < temp_path.size(); i++){
            loc = temp_path[i];
            sequence.enqueue(loc);
        }
        path.put(letter, sequence);
        return;
    }
    // Recursive Case: Traverse the tree
    temp_bit = Bit(0);
    temp_path.add(temp_bit);
    buildPaths(tree -> zero, path, temp_path);

    last_idx = temp_path.size() - 1;
    temp_path.remove(last_idx); // Removing last location to continue where the recursive call left off
    temp_bit = Bit(1);
    temp_path.add(temp_bit);
    buildPaths(tree -> one, path, temp_path);

    return;
}


/**
 * Flatten the given tree into a Queue<Bit> and Queue<char> in the manner
 * specified in the assignment writeup.
 *
 * You can assume the input queues are empty on entry to this function.
 *
 * You can assume tree is a valid well-formed encoding tree.
 */
void flattenTree(EncodingTreeNode* tree, Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    Bit loc;
    char letter;

    // Base Case: We have reached a leaf node
    if(tree ->isLeaf()){
        loc = Bit(0);
        treeShape.enqueue(loc);
        letter = tree -> getChar();
        treeLeaves.enqueue(letter);
        return;
    }

    // Recursive Case: Traverse the tree
    loc = Bit(1);
    treeShape.enqueue(loc);
    flattenTree(tree -> zero, treeShape, treeLeaves);
    flattenTree(tree -> one, treeShape, treeLeaves);

}


/**
 * Compress the input text using Huffman coding, producing as output
 * an EncodedData containing the encoded message and flattened
 * encoding tree used.
 *
 * Reports an error if the message text does not contain at least
 * two distinct characters.
 */
EncodedData compress(string messageText) {
    EncodingTreeNode* tree = buildHuffmanTree(messageText);
    Queue<Bit> message_bits = encodeText(tree, messageText);

    Queue<Bit> tree_shape;
    Queue<char> tree_leaves;
    flattenTree(tree, tree_shape, tree_leaves);

    EncodedData compressed;
    compressed.messageBits = message_bits;
    compressed.treeShape = tree_shape;
    compressed.treeLeaves = tree_leaves;

    deallocateTree(tree);
    return compressed;
}


/* * * * * * Testing Helper Functions Below This Point * * * * * */
/**
 * createExampleTree creates the example tree listed below for testing
 * and debugging purposes.
 */
EncodingTreeNode* createExampleTree() {
    /* Example encoding tree used in multiple test cases:
     *                *
     *              /   \
     *             T     *
     *                  / \
     *                 *   E
     *                / \
     *               R   S
     */

    EncodingTreeNode* R = new EncodingTreeNode('R');
    EncodingTreeNode* S = new EncodingTreeNode('S');
    EncodingTreeNode* RS_root = new EncodingTreeNode(R, S);


    EncodingTreeNode* E = new EncodingTreeNode('E');
    EncodingTreeNode* E_0 = RS_root;
    EncodingTreeNode* E_root = new EncodingTreeNode(E_0, E);

    EncodingTreeNode* T = new EncodingTreeNode('T');
    EncodingTreeNode* root = new EncodingTreeNode(T, E_root);

    EncodingTreeNode* tree = root;

    return tree;
}

/**
 * deallocateTree is passed in the root of the tree and
 * then proceeds to delete all the structs and pointers in the
 * tree to prevent memory leaks.
 */
void deallocateTree(EncodingTreeNode* t) {
    // Base Case: The node is a leaf or nullptr
    if (t == nullptr){
        delete t;
        return;
    }
    if (t -> isLeaf()){
        delete t;
        return;
    }

    // Recursive Case: Explore both left and right side of the tree
    deallocateTree(t -> zero);
    deallocateTree(t -> one);
    delete t;
}

/**
 * areEqual is a function that compares two trees and validates
 * if they are equal or not.
 */
bool areEqual(EncodingTreeNode* a, EncodingTreeNode* b) {
    // Base Case: Have been passed in empty trees or both trees have been completely traversed
    if (a == nullptr && b == nullptr){
        return true;
    }
    // Base Case: We are at the bottom of a sub-tree (or tree) in one, but not the other
    else if ((a == nullptr && b != nullptr) || (a != nullptr && b == nullptr)){
        return false;
    }

    // Base Case: We have reached a leaf node for both trees
    else if(a -> isLeaf() && b -> isLeaf()){
        return (a -> getChar() == b -> getChar());
    }
    else{
        // Recursive Case: Comparing roots and both children
        bool a_node_status = a -> isLeaf();
        bool b_node_status = b -> isLeaf();

        return(a_node_status == b_node_status && areEqual(a -> zero, b -> zero) && areEqual(a -> one, b -> one));
    }
}

/* * * * * * Test Cases (Student) Below This Point * * * * * */
STUDENT_TEST("Testing deallocateTree functionality") {
    EncodingTreeNode* tree = createExampleTree();
    EXPECT(tree != nullptr);
    deallocateTree(tree);
}

STUDENT_TEST("Testing areEqual functionality on simple tree and nullptr") {
    EncodingTreeNode* R = new EncodingTreeNode('R');
    EncodingTreeNode* S = new EncodingTreeNode('S');
    EncodingTreeNode* a = new EncodingTreeNode(R, S);

    EncodingTreeNode* b = nullptr;
    EXPECT(!areEqual(a, b));
    deallocateTree(a);
    deallocateTree(b);
}

STUDENT_TEST("Testing areEqual functionality on equal simple trees") {
    EncodingTreeNode* R = new EncodingTreeNode('R');
    EncodingTreeNode* S = new EncodingTreeNode('S');
    EncodingTreeNode* a = new EncodingTreeNode(R, S);

    EncodingTreeNode* R_cop = new EncodingTreeNode('R');
    EncodingTreeNode* S_cop = new EncodingTreeNode('S');
    EncodingTreeNode* b = new EncodingTreeNode(R_cop, S_cop);
    EXPECT(areEqual(a, b));
    deallocateTree(a);
    deallocateTree(b);
}

STUDENT_TEST("Testing areEqual functionality on un-equal simple trees") {
    EncodingTreeNode* R = new EncodingTreeNode('R');
    EncodingTreeNode* S = new EncodingTreeNode('S');
    EncodingTreeNode* a = new EncodingTreeNode(R, S);

    EncodingTreeNode* R_cop = new EncodingTreeNode('R');
    EncodingTreeNode* S_cop = new EncodingTreeNode('S');
    EncodingTreeNode* b = new EncodingTreeNode(S_cop, R_cop);
    EXPECT(!areEqual(a, b));
    deallocateTree(a);
    deallocateTree(b);
}

STUDENT_TEST("Testing areEqual functionality on simple tree and createExampleTree") {
    EncodingTreeNode* R = new EncodingTreeNode('R');
    EncodingTreeNode* S = new EncodingTreeNode('S');
    EncodingTreeNode* a = new EncodingTreeNode(R, S);

    EncodingTreeNode* tree = createExampleTree();
    EXPECT(!areEqual(a, tree));
    deallocateTree(a);
    deallocateTree(tree);
}

STUDENT_TEST("Testing areEqual functionality on equal createExampleTree") {
    EncodingTreeNode* tree_a = createExampleTree();
    EncodingTreeNode* tree_b = createExampleTree();
    EXPECT(areEqual(tree_a, tree_b));
    deallocateTree(tree_a);
    deallocateTree(tree_b);
}

STUDENT_TEST("decodeText, small example encoding tree") {
    EncodingTreeNode* tree = createExampleTree();
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(decodeText(tree, messageBits), "E");

    Queue<Bit> messageBits_one = { 0, 1, 0, 0, 1, 1, 1, 1}; // TREE
    EXPECT_EQUAL(decodeText(tree, messageBits_one), "TREE");

    deallocateTree(tree);
}

STUDENT_TEST("decodeText, small example encoding tree") {
    EncodingTreeNode* tree = createExampleTree();
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = { 1, 0, 0, 1, 1, 1, 0, 1, 0}; // REST
    EXPECT_EQUAL(decodeText(tree, messageBits), "REST");


    Queue<Bit> messageBits_1 = {0, 1, 0, 0, 1, 1, 1, 0, 1}; // TRES is three in spanish
    EXPECT_EQUAL(decodeText(tree, messageBits_1), "TRES");

    deallocateTree(tree);
}

STUDENT_TEST("buildPaths helper function, functionality on small example encoding tree") {
    EncodingTreeNode* tree = createExampleTree();
    EXPECT(tree != nullptr);

    Queue<Bit> T_Bits = {0};
    Queue<Bit> R_Bits = {1, 0, 0};
    Queue<Bit> S_Bits = {1, 0, 1};
    Queue<Bit> E_Bits = {1, 1};

    Map<char, Queue<Bit>> answer = {{'T', T_Bits}, {'R', R_Bits},  {'S', S_Bits},  {'E', E_Bits}};
    Map<char, Queue<Bit>> paths = buildPaths(tree);

    EXPECT_EQUAL(answer, paths);

    deallocateTree(tree);
}

STUDENT_TEST("buildHuffman on happy hip hop (Verified by hand by calculating entropy)"){
    string text = "happy hip hop";
    EncodingTreeNode* tree = buildHuffmanTree(text);
    deallocateTree(tree);
}

STUDENT_TEST("buildHuffman on bookkeeper (Verified by hand by calculating entropy)"){
    string text = "bookkeeper";
    EncodingTreeNode* tree = buildHuffmanTree(text);
    deallocateTree(tree);
}

STUDENT_TEST("Testing error call in getWeights"){
    string text = "AAAAAAAAAAA";
    EXPECT_ERROR(getWeights(text));

    text = "w";
    EXPECT_ERROR(getWeights(text));

}


/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("decodeText, small example encoding tree") {
    EncodingTreeNode* tree = createExampleTree();
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(decodeText(tree, messageBits), "E");

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(decodeText(tree, messageBits), "SET");

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1}; // STREETS
    EXPECT_EQUAL(decodeText(tree, messageBits), "STREETS");

    deallocateTree(tree);
}

PROVIDED_TEST("unflattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  treeShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeLeaves = { 'T', 'R', 'S', 'E' };
    EncodingTreeNode* tree = unflattenTree(treeShape, treeLeaves);

    EXPECT(areEqual(tree, reference));

    deallocateTree(tree);
    deallocateTree(reference);
}

PROVIDED_TEST("decompress, small example input") {
    EncodedData data = {
        { 1, 0, 1, 1, 0, 0, 0 }, // treeShape
        { 'T', 'R', 'S', 'E' },  // treeLeaves
        { 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1 } // messageBits
    };

    EXPECT_EQUAL(decompress(data), "TRESS");
}

PROVIDED_TEST("buildHuffmanTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    EncodingTreeNode* tree = buildHuffmanTree("STREETTEST");
    EXPECT(areEqual(tree, reference));

    deallocateTree(reference);
    deallocateTree(tree);
}

PROVIDED_TEST("encodeText, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(encodeText(reference, "E"), messageBits);

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(encodeText(reference, "SET"), messageBits);

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1 }; // STREETS
    EXPECT_EQUAL(encodeText(reference, "STREETS"), messageBits);

    deallocateTree(reference);
}

PROVIDED_TEST("flattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  expectedShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> expectedLeaves = { 'T', 'R', 'S', 'E' };

    Queue<Bit>  treeShape;
    Queue<char> treeLeaves;
    flattenTree(reference, treeShape, treeLeaves);

    EXPECT_EQUAL(treeShape,  expectedShape);
    EXPECT_EQUAL(treeLeaves, expectedLeaves);

    deallocateTree(reference);
}

PROVIDED_TEST("compress, small example input") {
    EncodedData data = compress("STREETTEST");
    Queue<Bit>  treeShape   = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeChars   = { 'T', 'R', 'S', 'E' };
    Queue<Bit>  messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0 };

    EXPECT_EQUAL(data.treeShape, treeShape);
    EXPECT_EQUAL(data.treeLeaves, treeChars);
    EXPECT_EQUAL(data.messageBits, messageBits);
}

PROVIDED_TEST("Test end-to-end compress -> decompress") {
    Vector<string> inputs = {
        "HAPPY HIP HOP",
        "Nana Nana Nana Nana Nana Nana Nana Nana Batman",
        "Research is formalized curiosity. It is poking and prying with a purpose. – Zora Neale Hurston",
    };

    for (string input: inputs) {
        EncodedData data = compress(input);
        string output = decompress(data);

        EXPECT_EQUAL(input, output);
    }
}
