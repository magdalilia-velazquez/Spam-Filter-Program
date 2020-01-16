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
        exit(-1);
    }
    
    string value;
    inFS >> value;  // input file value
    
    while (!inFS.eof())
    {
        if (!inFS.fail()) {
            numSpamEntries++;
            spamList.push_back(value);
        }
        inFS >> value;
    }
   
    // Done with file, so close it
    inFS.close();
    
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
bool binarySearch(string email, ourvector<string>& spamList)
{	
    int mid = 0;
    int low = 0;
    int high = spamList.size() - 1;
    int searchResult = -1;  // Stores index of email if search succeeded, else -1
    
    while (low <= high)  {
        mid = (low + high) / 2;
        searchResult = email.compare( spamList[ mid]);
        
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


// Checks a single email address to see if it is spam
// uses the binarySearch() function to check 
void check(string email, ourvector<string>& spamList) {
	if (binarySearch(email, spamList)) {
        cout << email << " is spam" << email;
    }
    else {
        cout << email << " is not spam" << endl;
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
        cin >> command; //user input for the command/function 

        if (command == "load"){
            cin >> filename; //spam list file name
            cout << "Loading '" << filename << "'" << endl;
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
/*
        if (command == "filter"){
            cin >> emailFile;
            cin >> outputFile;
            filter(emailFile, outputFile);
        }*/
    }
    
	return 0;
}
