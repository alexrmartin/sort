#include "Sort.h"
#include "ArgumentManager.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

Sort::Sort(string inFile, string outFile) {
    this->outFile = move(outFile);
    ifstream infile(inFile);
    int x; int i = 0;
    while (infile >> x) {
        inNums[i++] = x;
    }

    inSize = i;
    infile.close();
}

void Sort::intervalInsertionSort(int start, int inc) {
    int numToSort, j;
    for (int i = start+inc; i < inSize; i += inc) {
        numToSort = inNums[i];
        j = i - inc;
        while (j >= start && numToSort < inNums[j]) {
            inNums[j+inc] = inNums[j];
            j -= inc;
        }
        inNums[j+inc] = numToSort;
    }
}

void Sort::shellSort() {
    int i;
    for (i = 1; i < inSize/9; i = 3*i+1);
    do {
        for (int start = 0; start < i; start++) {
            intervalInsertionSort(start, i);
        }
        i /= 3;
    }
    while (i > 0);
}

void Sort::quickSortHelper(int first, int last) {
    int pLoc;
    if (first < last) {
        pLoc = partition(first, last);
        quickSortHelper(first, pLoc - 1);
        quickSortHelper(pLoc + 1, last);
    }
}

void Sort::quickSort() {
    quickSortHelper(0,  inSize - 1);
}

int Sort::partition(int first, int last) {
    int pivot, index, smallInd;
    swap(first, (last - first)/2 + first);
    pivot = inNums[first];
    smallInd = first;

    for (index = first + 1; index <= last; index++) {
        if (inNums[index] < pivot) {
            smallInd++;
            swap(smallInd, index);
        }
    }
    swap(first, smallInd);
    return smallInd;
}

void Sort::swap(int first, int second) {
    int temp = inNums[first];
    inNums[first] = inNums[second];
    inNums[second] = temp;
}

void Sort::makeList() {
    head = new Node(inNums[0]);
    Node *present = head;
    for (int i = 1; i < inSize; i++) {
        present->next = new Node(inNums[i]);
        present = present->next;
    }
}

void Sort::divideList(Node *first1, Node *&first2) {
    Node *mid, *pres;
    if (first1 == NULL) {
        first2 = NULL;
    }
    else if (first1->next == NULL) {
        first2 = NULL;
    }
    else {
        mid = first1;
        pres = first1->next;

        if (pres != NULL) {
            pres = pres->next;
        }

        while (pres != NULL) {
            mid = mid->next;
            pres = pres->next;
            if (pres != NULL) {pres = pres->next;}
        }

        first2 = mid->next;
        mid->next = NULL;
    }
}

Node* Sort::merge(Node* first1, Node* first2) {
    Node *lastSmall;
    Node *newHead;

    if (first1 == NULL) {
        return first2;
    }

    else if (first2 == NULL) {
        return first1;
    }

    else {
        if (first1->value < first2->value) {
            newHead = first1;
            first1 = first1->next;
            lastSmall = newHead;
        }

        else {
            newHead = first2;
            first2 = first2->next;
            lastSmall = newHead;
        }

        while (first2 != NULL && first1 != NULL) {
            if (first2->value > first1->value) {
                lastSmall->next = first1;
                lastSmall = lastSmall->next;
                first1 = first1->next;
            }

            else {
                lastSmall->next = first2;
                lastSmall = lastSmall->next;
                first2 = first2->next;
            }
        }

        if (first1 == NULL) {
            lastSmall->next = first2;
        }

        else {
            lastSmall->next = first1;
        }

        return newHead;
    }
}

void Sort::mergeSort() {
    makeList();
    mergeSort(head);
}

void Sort::mergeSort(Node *&head) {
    Node *head2;
    if (head != NULL) {
        if (head->next != NULL) {
            divideList(head, head2);
            mergeSort(head);
            mergeSort(head2);
            head = merge(head, head2);
        }
    }
}

void Sort::printNums(bool arg) {
    if (arg) {
        for (int i = 0; i < inSize; i++) {
            cout << inNums[i] << endl;
        }
    }

    else {
        Node *pres = head;
        while (pres != NULL) {
            cout << pres->value << endl;
            pres = pres->next;
        }
    }
}

void Sort::parseCommands(string comFile) {
    ifstream commands(comFile);
    string line, presWord;
    int index;

    while (getline(commands, line)) {
        if (!line.empty()) {
            break;
        }
    }

    if (line.empty()) {
        return;
    }


    stringstream lineStream(line);
    
    for (int i = 0; i < 2; i++) {
        lineStream >> presWord;
        comWords.push_back(presWord);
    }

    lineStream >> presWord;
    if (presWord.size() > 1) {
        comWords.push_back(presWord);
    }
    else {
        fetchIndecies.push_back(stoi(presWord));
    }

    while (lineStream) {
        if (lineStream >> index) {
            fetchIndecies.push_back(index);
        }
    }

    if (!commands.eof()) {
        while (getline(commands, line)) {
            stringstream extraLine(line);
            while (extraLine >> index) {
                fetchIndecies.push_back(index);
            }
        }
    }

    if (comWords.size() < 3) {
        comWords.emplace_back("ascending");
    }

    SORT_TYPE = comWords[0];
    ORDER = comWords[2];


}

void Sort::executeCommands() {
    if (SORT_TYPE == "merge") {
        mergeSort();
    }

    else if (SORT_TYPE == "quick") {
        quickSort();
    }

    else if (SORT_TYPE == "shell") {
        shellSort();
    }

    else {
        return;
    }

    if (ORDER == "descending" || ORDER == "ascending")
        writeToOutput(ORDER);

    else {
        return;
    }
}

void Sort::writeToOutput(string order) {
    ofstream output(outFile);

    if (SORT_TYPE != "merge") {
        if (order == "ascending") {
            for (int i = 0; i < fetchIndecies.size(); i++) {
                if (fetchIndecies[i] >= inSize || fetchIndecies[i] < 0) {
                    continue;
                }

                output << inNums[fetchIndecies[i]];
                output << " ";
            }
        }

        else {
            for (int i = 0; i < fetchIndecies.size(); i++) {
                if (fetchIndecies[i] >= inSize || fetchIndecies[i] < 0) {
                    continue;
                }
                output << inNums[inSize - 1 - fetchIndecies[i]];
                output << " ";
            }
        }
        output.close();
    }

    else {
        Node *pres;
        int iterNum, tar;
        if (order == "ascending") {
            for (int i = 0; i < fetchIndecies.size(); i++) {
                if (fetchIndecies[i] >= inSize || fetchIndecies[i] < 0) {
                    continue;
                }
                iterNum = 0;
                tar = fetchIndecies[i];
                pres = head;
                while (iterNum < tar) {
                    pres = pres->next;
                    iterNum++;
                }
                output << pres->value << " ";
            }
        }
        else {
            for (int i : fetchIndecies) {
                if (fetchIndecies[i] >= inSize || fetchIndecies[i] < 0) {
                    continue;
                }
                iterNum = 0;
                tar = 9999 - fetchIndecies[i];
                pres = head;
                
                while (iterNum < tar) {
                    pres = pres->next;
                    iterNum++;
                }
                output << pres->value << " ";
            }
        }
    }
}

int main(int argc, char* argv[]) {
    ArgumentManager am(argc, argv);
    string inFileName = am.get("input");
    string comFileName = am.get("command");
    string outFileName = am.get("output");
    Sort s(inFileName, outFileName);
    s.parseCommands(comFileName);
    s.executeCommands();

    return 0;

}
