/* 
Lili Velazquez 
18 January 2020
University of Illinois at Chicago
CS 251: Data Structures 
Spring 2020

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


// Takes a file as a parameter to 
// load a spam list and check how many spam entries the list has. 
ourvector<string> load(string filename)
{    
	ourvector<string> spamList;
    int numSpamEntries = 0;

    ifstream infile(filename); // use infile object to read from file
    
    if (!infile.good()) { // unable to open input file:
        cout << "**Error, unable to open '" << filename << "'" << endl;
        cout << endl;
    }
    else {
        
        cout << "Loading '" << filename << "'" << endl;
        
        string oneWord;
        infile >> oneWord;
        
        while (!infile.eof()) // until we hit the end-of-file:
        {
            numSpamEntries++;
            spamList.push_back(oneWord);
            infile >> oneWord;
        }
        infile.close(); 
        cout << "# of spam entries: " << numSpamEntries << endl;
        cout << endl;
    }
        
    return spamList; //returns the vector with spam emails 
}


// Displays the contents of a spam list 
// Only takes in the spam list as a parameter
void display(ourvector<string>& spamList) 
{
    // loops through the spam list and displays each email 
	for (int i = 0; i < spamList.size(); ++i){
        cout << spamList[i] << endl;
    }
    cout << endl;
}


// Binary search to look up the email in the spamList vector, 
// returning true if found, false otherwise.
// Takes in 3 parameters: 
//    --> the username and domain parsed from the email,
//    --> and the spam list vector to search through it
bool binarySearch(string username, string domain, ourvector<string>& spamList)
{	
    int mid = 0;
    int low = 0;
    int high = spamList.size() - 1;
    int searchResult = -1;  // Stores index of email if search succeeded, else -1
    string domainUsername = domain + ":" + username; // concatenated to find an exact match
    
    while (low <= high)  {
        mid = (low + high) / 2;
        searchResult = domainUsername.compare(spamList[mid]);
        
        if (searchResult == 0)  {
            return true; // email IS in vector, so return the index where the email was found
        }
        else if (searchResult < 0)  {
            high = mid - 1; // email should be located before the mid location
        }
        else  {
            low = mid + 1; // email should be located after the mid location
        }
    }  
	return false; // returns email if it was not found
}


// Email is split into a domain and username
// in order to efficiently search it in the spam list.
// Takes in 3 parameters:
//    --> the email that needs to be checked if it is a spam email, 
//    --> the username which is parsed from the email
//    --> and the domain also parsed from email 
//    both the username and domain are passed by reference 
void parseEmailAddress(string email, string& username, string& domain)
{
   int emailSize = email.size();
   int atIndex = email.find('@'); 
   
   username = email.substr(0, atIndex);
   domain = email.substr(atIndex+1, emailSize);
}


// Checks a single email address to see if it is spam
// uses the binarySearch() function to search through the spam list.
// Takes in 2 parameters: email that needs to be checked if it is a spam email and the spam list 
void check(string email, ourvector<string>& spamList) 
{
    string username, domain; 
    parseEmailAddress(email, username, domain); // calls the function needed to split the email into username and domain 
    
    // First checks if the there is any domain:* form because any username associated with that domain is spam
    if (binarySearch("*", domain, spamList)){
        cout << email << " is spam" << endl << endl;
    }
    // if the email is found in the spam list then the email is spam
	else if (binarySearch(username, domain, spamList)) {
        cout << email << " is spam" << endl << endl;
    }
    // else if the email was not found then it is not spam 
    else {
        cout << email << " is not spam" << endl << endl;
    }
}


// Creates an output file to insert all the non-spam emails from an email file
// Filters an email list and outputs the resulting emails to the output file 
// Takes in 3 parameters: email file (to open it and go through every email),
// output file to open it and write in it the non-spam emails from the email file,
// and the spam list to search if an email from the email file is spam or not
void filter(string emailFile, string outputFile, ourvector<string>& spamList)
{
    int numEmailsProcessed = 0;
    int numNonSpamEmails = 0;
    
    ifstream infile(emailFile); // use infile object to read from file
    ofstream outfile(outputFile); // use outfile object to write to file
    
    if (!infile.good()) { // unable to open input file:
        cout << "**Error, unable to open '" << emailFile << "'" << endl;
        cout << endl;
    }
    else {
        int msgId;
        string emailAddress;
        string subject;
        infile >> msgId;
        infile >> emailAddress;
        getline(infile, subject);
        string username, domain;
        
        while (!infile.eof()) // until we hit the end-of-file:
        {
            numEmailsProcessed++;
            parseEmailAddress(emailAddress, username, domain);
    
            // checks if the email is not spam in order to add it to the output file 
            if ((!binarySearch("*", domain, spamList)) && (!binarySearch(username, domain, spamList))){ 
                numNonSpamEmails++; 
                if (!outfile.good()) { // unable to open output file:
                    cout << "**Error, unable to open '" << outputFile << "'" << endl;
                    cout << endl;
                }
                else {
                    // wirtes the non-spam email into the output file 
                    outfile << msgId << " " << emailAddress << " " << subject << endl;                    
                }
            }
            
            infile >> msgId;
            infile >> emailAddress;
            getline(infile, subject);
        }
        infile.close(); 
        outfile.close();
        cout << "# emails processed: " << numEmailsProcessed << endl;
        cout << "# non-spam emails:  " << numNonSpamEmails << endl << endl;
    }
}


int main()
{
    ourvector<string> spamList;
    string command; 
    string filename;
    string email;
    string emailFile;
    string outputFile;
    
    cout << "** Welcome to spam filtering app **" << endl;
    cout << endl;
    
    while (command != "#") 
    {
        cout << "Enter command or # to exit> ";
        cin >> command; //user input for the command 

        if (command == "load"){
            spamList.clear();
            cin >> filename; //spam list file name
            spamList = load(filename);
        }
        else if (command == "display"){
            display(spamList);
        }

        else if (command == "check"){
            cin >> email;
            check(email, spamList);
        }

        else if (command == "filter"){
            cin >> emailFile;
            cin >> outputFile;
            filter(emailFile, outputFile, spamList);
        }
        else if (command != "#") {
            cout << "**invalid command" << endl;
            cout << endl; 
        }
    }
    
    spamList._stats();
    
	return 0;
}
