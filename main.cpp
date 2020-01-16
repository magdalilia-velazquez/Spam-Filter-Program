/*
Lili Velazquez 
University of Illinois at Chicago
CS 251: Spring 2020

Project 01: Spam Filter Program
A C++ program that is able to load a spam list, display its contents, 
check a single email address to see if it’s spam, 
and filter an email list and output the resulting emails to an output file
*/

#include <iostream>
#include <fstream>
#include <string>
#include "ourvector.h"
using namespace std;


// Loads a spam list and checks how many spam entries it has. 
ourvector<string> load(string filename, int &numSpamEntries) 
{
	ourvector<string> spamList;
    numSpamEntries = 0;
	
	ifstream inFS;
    inFS.open(filename);
    
    if (!inFS.is_open()) {
        cout << "**file not found" << endl;
    }
    else {
        string oneWord;
        inFS >> oneWord;  

        while (!inFS.eof())
        {
            if (!inFS.fail()) {
                numSpamEntries++;
                spamList.push_back(oneWord);
                inFS >> oneWord;
            }
        }
        inFS.close();
    }
        
    return spamList;
}


// Displays the contents of a spam list 
void display(ourvector<string> spamList) {
	for (int i = 0; i < spamList.size(); ++i){
        cout << spamList[i] << endl;
    }
    cout << endl;
}


// Binary search to look up the email in the spamList vector, 
// returning true if found, false otherwise.
bool binarySearch(string username, string domain, ourvector<string>& spamList)
{	
    int mid = 0;
    int low = 0;
    int high = spamList.size() - 1;
    int searchResult = -1;  // Stores index of email if search succeeded, else -1
    string parsedEmail = domain + ":" + username;
    
    while (low <= high)  {
        mid = (low + high) / 2;
        searchResult = parsedEmail.compare(spamList[mid]);
        
        if (searchResult == 0)  {
            // email IS in vector, so return the index where the email was found
            return true;
        }
        else if (searchResult < 0)  {
            high = mid - 1; // email should be located before the mid location
        }
        else  {
            low = mid + 1; // email should be located after the mid location
        }
    }
    
    // email was not found
	return false;
}


// Email is split into a domain and username
// in order to efficiently search it in the spam list 
void parseEmailAddress(string email, string& username, string& domain)
{
   username = "";
   domain = "";
   int counter = 0;
   long emailSize = email.size();
   
   for (int i = 0; i < emailSize; ++i) {
      counter++;
      if (email[i] == '@'){
         break;
      }
   }
   
   username = email.substr(0, counter-1);
   domain = email.substr(counter, email.size());
   
   return;
}


// Checks a single email address to see if it is spam
// uses the binarySearch() function to check 
void check(string email, ourvector<string> spamList) {
    string username, domain, parsedEmail;
    
    parseEmailAddress(email, username, domain);
    
    if (binarySearch("*", domain, spamList)){
        cout << email << " is spam" << endl << endl;
    }
	else if (binarySearch(username, domain, spamList)) {
        cout << email << " is spam" << endl << endl;
    }
    else {
        cout << email << " is not spam" << endl << endl;
    }
}


// filters an email list and outputs the resulting emails to a file 
void filter() {
	
}


int main()
{
    ourvector<string> spamList;
    string command; 
    string filename;
    string email;
    string emailFile;
    string outputFile;
    int numOfSpamEntries;
    
    cout << "** Welcome to spam filtering app **" << endl;
    cout << endl;
    
    while (command != "#") 
    {
        cout << "Enter command or # to exit> ";
        cin >> command; //user input for the command 

        if (command == "load"){
            cin >> filename; //spam list file name
            cout << "Loading '" << filename << "'" << endl;
            spamList.clear();
            spamList = load(filename, numOfSpamEntries);
            cout << "# of spam entries: " << numOfSpamEntries << endl;
            cout << endl;
        }

        if (command == "display"){
            display(spamList);
        }

        if (command == "check"){
            cin >> email;
            check(email, spamList);
        }

        if (command == "filter"){
            cin >> emailFile;
            cin >> outputFile;
            filter(emailFile, outputFile);
        }
    }
    
    spamList._stats();
    
	return 0;
}
