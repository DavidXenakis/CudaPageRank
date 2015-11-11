#include <iostream>
#include <fstream>
#include <string>
#include "Graph.h"

using namespace std;

Graph::Graph(string fileName, bool undirected, bool invert, FILE* writeTo) {
   string fileType = fileName.substr(fileName.length() - 3, fileName.length());
   index = 0;
   if(!fileType.compare("csv")) {
      scanCSV(fileName);
   }
   else if(!fileType.compare("txt"))
      scanSNAP(fileName);
   else {
      cerr << "This file is not supported." << endl;
      exit(1);
   }

}

void Graph::scanCSV(string fileName) {
   ifstream inFile (fileName, ifstream::in);
   char oneline[512];
   char firstWord[50];
   char secondWord[50];

   int lineNdx = 0;

   inFile.getline(oneline, 512);
   while (inFile)
   {
      lineNdx = 0;

      if(oneline[0] == '#') {
         inFile.getline(oneline, 512);
         continue;
      }

      //Saving the first node
      lineNdx = parseWord(oneline + lineNdx, firstWord);
      //Skipping the numerical value, it is not necessary
      lineNdx += parseWord(oneline + lineNdx, secondWord);
      //saving the second node
      lineNdx += parseWord(oneline + lineNdx, secondWord);

      if(namesToIndex.find(firstWord) == namesToIndex.end())
         namesToIndex[firstWord] = index++;
      if(namesToIndex.find(secondWord) == namesToIndex.end())
         namesToIndex[secondWord] = index++;


      cout << "First Node: " << firstWord << " at " << namesToIndex[firstWord];
      cout << "; Second Node: " << secondWord <<  " at " << namesToIndex[secondWord] << endl;

      inFile.getline(oneline, 512);
   } 

   inFile.close();
}

void Graph::scanSNAP(string fileName) {
   ifstream inFile (fileName, ifstream::in);
   char oneline[512];
   char firstWord[50];
   char secondWord[50];

   int lineNdx = 0;

   inFile.getline(oneline, 512);
   while (inFile)
   {  
      lineNdx = 0;

      if(oneline[0] == '#') {
         inFile.getline(oneline, 512);
         continue;
      }

      lineNdx = parseWord(oneline + lineNdx, firstWord);
      lineNdx = parseWord(oneline + lineNdx, secondWord);

      if(namesToIndex.find(firstWord) == namesToIndex.end())
         namesToIndex[firstWord] = index++;
      if(namesToIndex.find(secondWord) == namesToIndex.end())
         namesToIndex[secondWord] = index++;

      cout << "First Node: " << firstWord << " at " << namesToIndex[firstWord];
      cout << "; Second Node: " << secondWord <<  " at " << namesToIndex[secondWord] << endl;

      inFile.getline(oneline, 512);
   }

   inFile.close();
}

int Graph::size() {
   return 0;
}

//return length of fromString  used up
int Graph::parseWord(char *fromString, char *toString) {
   int wordNdx = 0;
   char c = 0;
   bool contLoop = true; 
   bool keepSpace = false; 

   c = *fromString++;
   while(c == ' ' || c == ',') {
      c = *fromString++;
   }

   if(c == '"') {
      keepSpace = true;
      //Increment past the "
      c = *fromString++;
   }

   while(contLoop) {
      if(c >= '0' && c <= '9' || c >= 'a' && c <= 'z' || keepSpace && c == ' ') {
         toString[wordNdx++] = c;
      }
      else if(c >= 'A' && c <= 'Z') {
         toString[wordNdx++] = c + 32;
      }
      else {
         toString[wordNdx++] = 0;
         //Chew up the last "
         if(keepSpace) {
            c = *fromString++;
            wordNdx;
         }

         //Chew up spaces and , to prepare next char arr
         while(c == ' ' || c == ',') {
            c = *fromString++;
            wordNdx++;
         }
         return wordNdx;
      }
      c = *fromString++;
   }

   //This will never be hit since I never change contLoop
   //While loop should return on its own...
   return wordNdx;
}
