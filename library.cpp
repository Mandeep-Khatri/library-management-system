#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

// ===== Base Class: Person =====
// Abstract base class representing a generic person in the system.
class Person {
protected:
    string name;
public:
    Person() : name("") {}
    Person(string n) : name(n) {}

    // Virtual function to be overridden by derived classes
    virtual void display() const {
        cout << "Name: " << name << endl;
    }

    virtual ~Person() {}
};

// ===== Book Class =====
// Represents a single book in the library.
class Book {
public:
    string title;
    string author;
    string isbn;
    bool isAvailable;

    Book(string t, string a, string i)
        : title(t), author(a), isbn(i), isAvailable(true) {}

    // Overloaded operator to display book details
    friend ostream& operator<<(ostream& os, const Book& b) {
        os << "Title: " << b.title << ", Author: " << b.author
           << ", ISBN: " << b.isbn << ", Available: " << (b.isAvailable ? "Yes" : "No");
        return os;
    }
};

// ===== Template ArrayList =====
// Generic array-based list for storing elements of any type.
template <typename T>
class ArrayList {
private:
    T* data;
    int size;
    int capacity;

    // Resizes the internal array when capacity is reached
    void resize() {
        capacity *= 2;
        T* newData = new T[capacity];
        for(int i = 0; i < size; i++)
            newData[i] = data[i];
        delete[] data;
        data = newData;
    }

public:
    ArrayList(int cap = 10) : size(0), capacity(cap) {
        data = new T[capacity];
    }

    // Adds an item to the list
    void add(const T& item) {
        if(size == capacity) resize();
        data[size++] = item;
    }

    // Overloaded subscript operators for access
    T& operator[](int index) {
        if(index < 0 || index >= size) throw out_of_range("Index out of bounds");
        return data[index];
    }

    const T& operator[](int index) const {
        if(index < 0 || index >= size) throw out_of_range("Index out of bounds");
        return data[index];
    }

    // Returns the current number of items
    int getSize() const {
        return size;
    }

    ~ArrayList() {
        delete[] data;
    }
};

// ===== User Class =====
// Derived from Person. Represents a library user.
class User : public Person {
private:
    string userID;
    ArrayList<Book*> borrowedBooks; // List of borrowed books (pointers)

public:
    User() : Person(), userID("") {}
    User(string n, string id) : Person(n), userID(id) {}

    // Borrow a book (if available)
    void borrowBook(Book* b) {
        if (!b->isAvailable)
            throw runtime_error("Book not available.");
        b->isAvailable = false;
        borrowedBooks.add(b);
    }

    // Return a book by ISBN
    void returnBook(const string& isbn) {
        for (int i = 0; i < borrowedBooks.getSize(); ++i) {
            if (borrowedBooks[i]->isbn == isbn) {
                borrowedBooks[i]->isAvailable = true;
                cout << "Book returned.\n";
                return;
            }
        }
        throw runtime_error("You did not borrow this book.");
    }

    string getUserID() const { return userID; }

    // Overridden display method
    void display() const override {
        cout << "User ID: " << userID << ", Name: " << name << "\nBorrowed books:\n";
        for(int i = 0; i < borrowedBooks.getSize(); i++)
            cout << "  - " << *borrowedBooks[i] << endl;
    }

    // Overloaded output operator for User
    friend ostream& operator<<(ostream& os, const User& u) {
        os << "UserID: " << u.userID << ", Name: " << u.name << ", Books borrowed: " << u.borrowedBooks.getSize();
        return os;
    }
};

// ===== BookNode Structure =====
// Node structure for linked list of books.
struct BookNode {
    Book* book;
    BookNode* next;
    BookNode(Book* b) : book(b), next(nullptr) {}
};

// ===== BookList Class =====
// Singly linked list for managing all books in the library.
class BookList {
private:
    BookNode* head;

public:
    BookList() : head(nullptr) {}

    // Adds a new book to the front of the list
    void addBook(Book* b) {
        BookNode* newNode = new BookNode(b);
        newNode->next = head;
        head = newNode;
    }

    // Finds a book by ISBN
    Book* findBookByISBN(const string& isbn) {
        BookNode* current = head;
        while(current) {
            if(current->book->isbn == isbn) return current->book;
            current = current->next;
        }
        return nullptr;
    }

    // Finds a book by title
    Book* findBookByTitle(const string& title) {
        BookNode* current = head;
        while(current) {
            if(current->book->title == title) return current->book;
            current = current->next;
        }
        return nullptr;
    }

    // Updates the title and author of a book
    void updateBook(const string& isbn, const string& newTitle, const string& newAuthor) {
        Book* b = findBookByISBN(isbn);
        if (!b) throw runtime_error("Book not found.");
        b->title = newTitle;
        b->author = newAuthor;
    }

    // Displays all books in the list
    void displayBooks() const {
        BookNode* current = head;
        while(current) {
            cout << *(current->book) << endl;
            current = current->next;
        }
    }

    // Destructor - deletes all nodes and books
    ~BookList() {
        while(head) {
            BookNode* tmp = head;
            head = head->next;
            delete tmp->book;
            delete tmp;
        }
    }
};

// ===== Main Application Menu =====
// Provides console interaction for the Library Management System.
int main() {
    BookList library;
    ArrayList<User> users;

    // Sample Users (hardcoded for testing)
    users.add(User("Mandeep", "1001"));
    users.add(User("Cameron", "1002"));

    int choice;
    do {
        cout << "\n====== Library Management System ======\n";
        cout << "1. Add New Book\n";
        cout << "2. Search Book (ISBN or Title)\n";
        cout << "3. Update Book Information\n";
        cout << "4. Borrow Book\n";
        cout << "5. Return Book\n";
        cout << "6. Display All Books\n";
        cout << "7. Display All Users\n";
        cout << "0. Exit\n";
        cout << "Select an option: ";
        cin >> choice;
        cin.ignore(); // Clear newline character

        try {
            if (choice == 1) {
                // Add new book
                string title, author, isbn;
                cout << "Enter title: "; getline(cin, title);
                cout << "Enter author: "; getline(cin, author);
                cout << "Enter ISBN: "; getline(cin, isbn);
                library.addBook(new Book(title, author, isbn));
                cout << "✅ Book added.\n";
            }
            else if (choice == 2) {
                // Search book
                string query;
                cout << "Enter ISBN or Title: ";
                getline(cin, query);
                Book* b = library.findBookByISBN(query);
                if (!b) b = library.findBookByTitle(query);
                if (b) cout << *b << endl;
                else cout << "❌ Book not found.\n";
            }
            else if (choice == 3) {
                // Update book info
                string isbn, newTitle, newAuthor;
                cout << "Enter ISBN to update: ";
                getline(cin, isbn);
                cout << "New title: "; getline(cin, newTitle);
                cout << "New author: "; getline(cin, newAuthor);
                library.updateBook(isbn, newTitle, newAuthor);
                cout << "✅ Book updated.\n";
            }
            else if (choice == 4) {
                // Borrow a book
                string uid, isbn;
                cout << "Enter User ID: ";
                getline(cin, uid);
                User* currentUser = nullptr;
                for (int i = 0; i < users.getSize(); i++) {
                    if (users[i].getUserID() == uid) {
                        currentUser = &users[i];
                        break;
                    }
                }
                if (!currentUser) throw runtime_error("User not found.");
                cout << "Enter ISBN to borrow: ";
                getline(cin, isbn);
                Book* b = library.findBookByISBN(isbn);
                if (!b) throw runtime_error("Book not found.");
                currentUser->borrowBook(b);
                cout << "✅ Book borrowed.\n";
            }
            else if (choice == 5) {
                // Return a book
                string uid, isbn;
                cout << "Enter User ID: ";
                getline(cin, uid);
                User* currentUser = nullptr;
                for (int i = 0; i < users.getSize(); i++) {
                    if (users[i].getUserID() == uid) {
                        currentUser = &users[i];
                        break;
                    }
                }
                if (!currentUser) throw runtime_error("User not found.");
                cout << "Enter ISBN to return: ";
                getline(cin, isbn);
                currentUser->returnBook(isbn);
            }
            else if (choice == 6) {
                // Display all books
                library.displayBooks();
            }
            else if (choice == 7) {
                // Display all users
                for (int i = 0; i < users.getSize(); i++)
                    users[i].display();
            }
            else if (choice != 0) {
                cout << "Invalid option.\n";
            }
        } catch (exception& e) {
            cerr << "❌ Error: " << e.what() << endl;
        }

    } while (choice != 0);

    cout << "📚 Exiting Library System. Goodbye!\n";
    return 0;
}
