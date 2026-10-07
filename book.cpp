#include <sstream>
#include "book.h"

using namespace std;

//Default constructor 
Book::Book() : title(""), author(""), isbn(""), isAvailable() {}

//Parametrized constructor
Book::Book(const string& title, const string& author, const string& isbn)
    : title(title), author(author), isbn(isbn) {}

//Getters
string Book::getTitle() const { return title; }
string Book::getAuthor() const { return author; }
string Book::getISBN() const { return isbn; }
bool Book::getAvailability() const { return isAvailable; }
string Book::getBorrowerId() const { return borrowerId; }

//Setters
void Book::setTitle(const string& title) { this->title = title; }
void Book::setAuthor(const string& author) { this->author = author; }
void Book::setISBN(const string& isbn) { this->isbn = isbn; }
void Book::setAvailability(const bool isAvailable) { this->isAvailable = isAvailable; }
void Book::setBorrowerId(const string& borrowerId) { this->borrowerId = borrowerId; }

//checkout book
void Book::checkOut(const string& borrowerId) {
    isAvailable = false;
    this->borrowerId = borrowerId;
}

//Return a book
void Book::returnBook() {
    isAvailable = true;
    borrowerId = "";
}

//Book information
string Book::toString() const {
    string result = "Titre  : " + title + "\n"
                    + "Auteur : " + author + "\n"
                    + "ISBN   : " + isbn + "\n"
                    + "Disponible : ";
    if (isAvailable) {
        result += "Oui";
    } else {
        result += "Non, emprunté par " + borrowerId;
    }
    return result;
}

//Format for file 
string Book::toFileFormat() const {
    string result = title + "|" + author + "|" + isbn + "|";
    
    if (isAvailable){
        result += "1";
    }else{
        result += "0";
    } 
    result += "|" + borrowerId;
    return result;
}


void Book::fromFileFormat(const string& line){
    stringstream ss(line);
    string availableStr;

    getline(ss, title, '|');
    getline(ss, author, '|');
    getline(ss, isbn, '|');
    getline(ss, availableStr, '|');
    getline(ss, borrowerId, '|');

    isAvailable = (availableStr == "1");

}



