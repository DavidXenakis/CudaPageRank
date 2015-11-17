#include "Graph.h"
#include "Node.h"
#include <ctype.h>

using namespace std;

unordered_map<string, Node*> namesToIndex;
vector<string> indexToName;
bool csvFile = false;
int numEdges = 0;

string trim(string str)
{
   size_t first = str.find_first_not_of(' ');
   size_t last = str.find_last_not_of(' ');
   return str.substr(first, (last-first+1));
}

bool Graph::findInVector(string str) {

   for(int i = 0; i < indexToName.size(); i++) {
      if(indexToName[i].compare(str) == 0) {
         return true;
      }
   }
   return false;
}

void Graph::printMatrix(SparseMatrix sm) {
   for(int i = 0; i < sm.nnz; i++) {
      cout << sm.cooValA[i] << " ";
   }
   cout << endl;
   for(int i = 0; i< sm.nnz;i ++) {
      cout << sm.cooRowIndA[i] << " ";
   }
   cout << endl;
   for (int i = 0; i < sm.nnz; i ++) {
      cout << sm.cooColIndA[i] << " ";
   }
   cout << endl;
}

SparseMatrix Graph::createSparseMatrix() {
   int *rowIndex = (int *)malloc(sizeof(int) * numEdges);
   int *colIndex = (int *)malloc(sizeof(int) * numEdges);
   float *vals = (float *)malloc(sizeof(float) * numEdges);
   int valIndex = 0;
   int undirected = true;
   for(int i = 0; i < indexToName.size(); i++) {
      string nodeName = indexToName[i];
      Node *tempNode = namesToIndex[nodeName];
      while(!tempNode->edges.empty()) {
         vals[valIndex] = 1.0 / ((namesToIndex[indexToName[tempNode->edges.top()]])->numOutlinks);
         colIndex[valIndex] = tempNode->edges.top();
         tempNode->edges.pop();
         rowIndex[valIndex] = i;
         valIndex++;
      }
   }
   return SparseMatrix(vals, rowIndex, colIndex, indexToName.size(), numEdges, &indexToName[0]);
}

Graph::Graph(string fileName, bool directed) {
   string fileType = fileName.substr(fileName.length() - 3, fileName.length());
   if(!fileType.compare("csv")) {
      scanFile(fileName, true, directed);
      csvFile = true;
   }
   else if(!fileType.compare("txt"))
      scanFile(fileName, false, directed);
   else {
      cerr << "This file is not supported." << endl;
      exit(1);
   }

}

void Graph::scanFile(string fileName, bool csvFile, bool directed) {
   ifstream inFile (fileName, ifstream::in);
   int index = 0;
   int count = 0;
   char oneline[512];
   char *firstWord;
   char *secondWord;
   int firstIndex = -1;
   int secondIndex = -1;
   unordered_map<std::string,Node*>::const_iterator gotFirst;
   unordered_map<std::string,Node*>::const_iterator gotSecond;



   char* lineNdx = oneline;

   inFile.getline(oneline, 512);
   while (inFile)
   {
      firstWord = (char *)calloc(1, 50);
      secondWord = (char *)calloc(1, 50);
      lineNdx = 0;

      if(oneline[0] == '#') {
         inFile.getline(oneline, 512);
         continue;
      }

      //Saving the first node
      lineNdx = parseWord(oneline, firstWord);

      //If csv file, skip the numerical value, it is not necessary
      if(csvFile)
         lineNdx = parseWord(lineNdx, secondWord);

      //saving the second node
      lineNdx = parseWord(lineNdx, secondWord);
      string firstWordString = trim(string(firstWord));
      string secondWordString = trim(string(secondWord));
      if(namesToIndex.find(string(firstWordString)) == namesToIndex.end()) { //first word not in hashmap
         count++;
         firstIndex = index++;
         indexToName.push_back(firstWordString);
         Node *tempFirst = new Node(firstIndex);
         namesToIndex[firstWordString] = tempFirst;
      } else { //in hashmap
         firstIndex = namesToIndex.find(firstWordString)->second->index;
        // firstIndex = namesToIndex[string(firstWord)]->index;/*gotFirst->second->index;*/
      }

      if(namesToIndex.find(secondWordString) == namesToIndex.end()) { //second word not in hashmap
         count++;
         secondIndex = index++;
         indexToName.push_back(secondWordString);
         Node *tempSecond = new Node(secondIndex);
//         tempSecond->index = secondIndex;

         namesToIndex[secondWordString] = tempSecond;

      } else { //in hashmap
         secondIndex = namesToIndex.find(secondWordString)->second->index;
         //secondIndex = namesToIndex[string(secondWord)]->index;/*gotSecond->second->index;*/
      }
      numEdges++;
      Node *secondNode = namesToIndex[secondWordString];
      Node *firstNode = namesToIndex[firstWordString];
      secondNode->edges.push(firstIndex);
      secondNode->edgeMap[firstIndex] = 1;
      firstNode->numOutlinks++;

      if(directed == false && firstNode->edgeMap.find(secondIndex) == firstNode->edgeMap.end()) {
         numEdges++;
         firstNode->edges.push(secondIndex);
         firstNode->edgeMap[secondIndex] = 1;
         secondNode->numOutlinks++;
      }

//      cout << "First Node: " << firstWord << " at " << namesToIndex[firstWord]->index;
//      cout << "; Second Node: " << secondWord <<  " at " << namesToIndex[secondWord]->index << endl;

      inFile.getline(oneline, 512);
   }
   inFile.close();
}

int Graph::size() {
   return 0;
}

//return length of fromString  used up
char* Graph::parseWord(char *fromString, char *toString) {
   int wordNdx = 0;
   char c = 0;
   bool contLoop = true; 
   bool keepSpace = false; 

   c = *fromString;
   while(c == ' ' || c == ',' || c == '\t') {
      c = *++fromString;
   }

   if(c == '"') {
      keepSpace = true;
      //Increment past the "
      c = *++fromString;
   }

   while(c != 0) {
      /*if((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (keepSpace && c == ' ') || c == '(' || c == ')' || c == '&'
              || c == '-') {
         toString[wordNdx++] = c;
      }*/
      if(c >= 'A' && c <= 'Z') {
         toString[wordNdx++] = c + 32;
      } else if((c != '"' && c != 0 && c != ' ' && c != ',' && c != '\t' && c != '\r' && c != '\n') || (keepSpace && c == ' ')) {
         toString[wordNdx++] = c;
      }
      else {
         toString[wordNdx]  = 0;
         //Chew up the last "
         if(keepSpace) {
            c = *++fromString;
         }

         //Chew up spaces and , to prepare next char arr
         /*while(c == ' ' || c == ',' || c == '\t') {
            c = *++fromString;
         }*/
         return fromString;
      }
      c = *++fromString;
   }
   toString[wordNdx]  = 0;

   //This will never be hit since I never change contLoop
   //While loop should return on its own...
   return fromString;
}
