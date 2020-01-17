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
//#include <ofstream>
#include <string>
#include "ourvector.h"
using namespace std;


// Loads a spam list and checks how many spam entries it has. 
ourvector<string> load(string filename, int &numSpamEntries) 
{    
	ourvector<string> spamList;
    numSpamEntries = 0;

    ifstream infile(filename); // use infile object to read from file
    
    if (!infile.good()) { // unable to open input file:
        cout << "**file not found" << endl;
    }
    else {
        string oneWord;
        infile >> oneWord;
        
        while (!infile.eof()) // until we hit the end-of-file:
        {
            if (!infile.fail()) {
                numSpamEntries++;
                spamList.push_back(oneWord);
                infile >> oneWord;
            }
        }
        infile.close();
    }
        
    return spamList;
}


// Displays the contents of a spam list 
void display(ourvector<string> spamList) 
{
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
    string domainUsername = domain + ":" + username;
    
    while (low <= high)  {
        mid = (low + high) / 2;
        searchResult = domainUsername.compare(spamList[mid]);
        
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
   long emailSize = email.size();
   int atIndex = email.find('@');
   
   username = email.substr(0, atIndex);
   domain = email.substr(atIndex+1, emailSize);
}


// Checks a single email address to see if it is spam
// uses the binarySearch() function to check 
void check(string email, ourvector<string>& spamList) 
{
    string username, domain; //, domainUsername;
    
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


// creates an output file to insert all the non-spam emails from an email file
void newEmailList(string outputFile, int msgId, string emailAddress, string subject) 
{
    ofstream outfile(outputFile); // use outfile object to write to file
    
    if (!outfile.good()) { // unable to open output file:
        cout << "**file not found" << endl;
    }
    else {
        outfile << msgId << " " << emailAddress << subject << endl;
        outfile.close(); // make sure contents are written by closing file:
    }
}


// filters an email list and outputs the resulting emails to a file 
void filter(string outputFile, ourvector<string> spamList, int msgId, string emailAddress, string subject, int& numNonSpamEmails) 
{   
    string username, domain;
    parseEmailAddress(emailAddress, username, domain);
    
    if ((!binarySearch("*", domain, spamList)) && (!binarySearch(username, domain, spamList))){
        numNonSpamEmails++; 
        newEmailList(outputFile, msgId, emailAddress, subject);
    }
}


// opens an email file and goes through the 3 different values
// to place them in separate variables 
void openEmailFile(string emailFile, string outputFile, ourvector<string> spamList, int& numEmailsProcessed, int& numNonSpamEmails)
{
    ifstream infile(emailFile); // use infile object to read from file
    
    if (!infile.good()) { // unable to open input file:
        cout << "**file not found" << endl;
    }
    else {
        int msgId;
        string emailAddress;
        string subject;
        infile >> msgId;
        infile >> emailAddress;
        getline(infile, subject);
        
        while (!infile.eof()) // until we hit the end-of-file:
        {
            if (!infile.fail()) {
                numEmailsProcessed++;
                filter(outputFile, spamList, msgId, emailAddress, subject, numNonSpamEmails);
                infile >> msgId;
                infile >> emailAddress;
                getline(infile, subject);
            }
        }
        infile.close();
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
    int numOfSpamEntries;
    int numEmailsProcessed;
    int numNonSpamEmails;
    
    cout << "** Welcome to spam filtering app **" << endl;
    cout << endl;
    
    while (command != "#") 
    {
        cout << "Enter command or # to exit> ";
        cin >> command; //user input for the command 

        if (command == "load"){
            spamList.clear();
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

        if (command == "filter"){
            numEmailsProcessed = 0;
            numNonSpamEmails = 0;
            cin >> emailFile;
            cin >> outputFile;
            openEmailFile(emailFile, outputFile, spamList, numEmailsProcessed, numNonSpamEmails);
            cout << "# emails processed: " << numEmailsProcessed << endl;
            cout << "# non-span emails: " << numNonSpamEmails << endl << endl;
        }
    }
    
    spamList._stats();
    
	return 0;
}
