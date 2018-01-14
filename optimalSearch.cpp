/*
 * Author: Tyler Griffith
 * Date: 10/08/16
 * Class: CSC-2710
 * File Location: ~/CSC2710/proj4/optimalSearch.cpp
 *
 * This program finds the optimal binary search tree then outputs the words in
 * pre/in/post order, and outputs the average seek time.
 */

#include <iostream>
#include <algorithm>
#include "binary.h"
using namespace std;

// globals
int count = 0;

template<class T>
void ct(BinaryTreeNode<T> *t)
{
   extern int count;
   count++;
}

//MAX
#define MAX 100

//function prototypes
void load(string [], string &, int &, float []);
void print(string, BinaryTree<string>);
void optsearchtree(int, float [], float &, int [][MAX]);
float minimum(float [][MAX], int, int, int &);
float sum(int, int, float []);
BinaryTree<string> tree(int, int, int [][MAX], string []);

//driver
int main(void){
   //declaring all variables
   int n, R[MAX][MAX];
   string words[MAX], seekTime;
   float p[MAX], minavg;
   //load in the keys, seekTime, number of keys, and probabilities
   load(words, seekTime, n, p);
   //uses probabilities to to build R[][]
   optsearchtree(n, p, minavg, R);
   //creates the empty binary tree
   BinaryTree<string> x;
   //fills the tree
   x = tree(1, n, R, words);
   //print the output
   print(seekTime, x);

   return 0;
}

/* load function:
 *     Read from standard input to load the words
 *     into an array. Load in the seek time. Load in the
 *     number of words and load in the probabilities.
 *
 *  precondition:
 *     words[] is an array of strings.
 *     seekTime is the seek time of the tree.
 *     n is the number of words.
 *     p[] is the probabilities.
 *     words[] is empty.
 *     p[] is empty.
 *
 *  postcondition:
 *     words[] will be loaded with keys.
 *     seekTime will be loaded with the seek time.
 *     n will contain the number of words.
 *     p[] will be loaded with the probabilities.
 */
void load(string words[], string &seekTime, int &n, float p[]){
   //get n
   cin >> n;
   //get seekTime
   string temp;
   cin >> temp >> temp >> temp >> seekTime;
   //get words
   for(int i = 0; i < n; i++){
      cin >> words[i];
   }
   //get probabilities
   for(int i = 0; i < n; i++){
      cin >> p[i];
   }
}

/* print function:
 *     Prints the average seek time and
 *     the pre/in/post order.
 *
 *  precondition:
 *     seekTime is the seek time of the tree.
 *     x is the tree.
 *
 *  postcondition:
 *     the seek time is printed along with the
 *     pre/in/post order.
 */
void print(string seekTime, BinaryTree<string> x){
   //print seek time
   cout << "Average Seek Time: " << seekTime << endl;
   cout << endl;
   //print preorder
   cout << "preorder - ";
   x.PreOutput();
   cout << endl;
   //print inorder
   cout << "inorder - ";
   x.InOutput();
   cout << endl;
   //print postorder
   cout << "postorder - ";
   x.PostOutput();
}

/* optsearchtree function:
 *     uses the probabilities in p[] to
 *     build R[][];
 *
 *  precondition:
 *     n is the amount of keys.
 *     p[] is array of probabilities.
 *     minavg is the minimum average.
 *     R[][] is the location of the values.
 *     p[] is loaded with the probabilities.
 *     R[][] is empty.
 *
 *  postcondition:
 *     R is loaded with the tree values.
 *     minavg is loaded with the minimum average.
 */
void optsearchtree(int n, float p[], float& minavg, int R[][MAX]){
   //creating functions
	int i, j, k, diagonal;
	float A[MAX][MAX];
   //adding values to A[][] and R[][]
	for (int i = 1; i <= n; i++){
		A[i][i-1] = 0;
		A[i][i] = p[i];
		R[i][i] = i;
		R[i][i-1] = 0;
	}
   //set diagonals equal to zero
	A[n+1][n] = 0;
	R[n+1][n] = 0;
   //get the min avg
	for(diagonal = 1; diagonal <= n-1; diagonal++){
      for(i=1; i<= n-diagonal; i++){
         j = i + diagonal;
         A[i][j] = minimum(A, i, j, k) + sum(i, j, p);
         R[i][j] = k;
		}
   }
   minavg = A[1][n];
}

/* minimum function:
 *     gets the minimum of the A[][] array from
 *     values i to j.
 *
 *  precondition:
 *     A[][] is the location of the min averages.
 *     i is the minimum value.
 *     j is the maximum value.
 *     k is the location of the value.
 *
 *  postcondition:
 *     k is filled with the location of the value.
 *     minimum is returned.
 */
float minimum(float A[][MAX], int i, int j, int &k){
   //make it so the new min will always be lower
   float min = 9999999999, t;
   //find the minimum and return it
   for(int a = i; a <= j; a++){
      t = A[i][a-1]+A[a+1][j];
      if(t <= min){
         min = t;
         k = a;
      }
   }
   return min;
}

/* sum function:
 *     gets the total of the probabilities from
 *     i to j.
 *
 *  precondition:
 *     p[] is filled with the probabilities.
 *     i is the minimum value.
 *     j is the maximum value.
 *
 *  postcondition:
 *     the sum of the probabilities is returned.
 */
float sum(int i, int j, float p[]){
   float total = 0;
   //add up all the probabilities from i to j
   for(int a = i; a <= j; a++){
      total += p[a];
   }
   return total;
}

/* tree function:
 *     Uses R[][] and words[] to create an optimal
 *     binary search tree.
 *
 *  precondition:
 *     i is the minimum value.
 *     j is the maximum value.
 *     R[][] is the location of the values.
 *     words[] is an array that contains the keys.
 *
 *  postcondition:
 *     the optimal binary search tree is created!
 */
BinaryTree<string> tree(int i, int j, int R[][MAX], string words[]){
   int k;
   BinaryTree<string> p;
   k = R[i][j];
   //make sure key is not empty
   if(k == 0){
      return p;
   }
   //create tree
   else{
      BinaryTree<string> lt = tree(i, k-1, R, words);
      BinaryTree<string> rt = tree(k+1, j, R, words);
      p.MakeTree(words[k], lt, rt);
      return p;
   }
}
