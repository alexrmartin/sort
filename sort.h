#ifndef HW3_SORT_H
#define HW3_SORT_H

#include <vector>
#include <string>

using namespace std;

struct Node {
    Node *next;
    int value;
    Node(int v) {
        value = v;
        next = NULL;
    }
};

class Sort {
private:
    int inNums[100000];
    Node *head;
    int inSize;
    vector<string> comWords;
    vector<int> fetchIndecies;
    string outFile;
    string ORDER, SORT_TYPE;
    void intervalInsertionSort(int start, int inc);
    int partition(int first, int last);
    void swap(int first, int second);
    void quickSortHelper(int first, int last);
    void makeList();
    void divideList(Node* first, Node *&second);
    void mergeSort(Node *&head);
    Node* merge(Node* first, Node* second);
    void writeToOutput(string order);

public:
    explicit Sort(string inFile, string outFile);
    void parseCommands(string commandFile);
    void executeCommands();
    void quickSort();
    void shellSort();
    void mergeSort();
    void printNums(bool arg = true);
};


#endif //HW3_SORT_H
