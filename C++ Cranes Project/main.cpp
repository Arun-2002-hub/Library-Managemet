#include <iostream>
#include<mysql.h>
#include<mysqld_error.h>
#include <sstream>
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
using namespace std;

char HOST[] = "localhost";
char USER[] = "root";
char PASS[] = "Mysql@2004";
char DB[] = "library_management";
MYSQL* conn;

// Function to initialize the MySQL connection
bool initializeConnection() 
{
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, HOST, USER, PASS, DB, 0, NULL, 0)) 
	{
        cerr << "Error connecting to MySQL database: " << mysql_error(conn) << endl;
        return false;
    }
    return true;
}

// Function to add a book to the library
void addBook() 
{
    string title, author;
    cout << "Enter book title: ";
    cin.ignore();
    getline(cin, title);
    cout << "Enter author: ";
    getline(cin, author);

    string query = "INSERT INTO books (title, author, available) VALUES ('" + title + "', '" + author + "', 1)";
    
    if (mysql_query(conn, query.c_str()) != 0) 
	{
        cerr << "Error: " << mysql_error(conn) << endl;
    } 
	else 
	{
        cout << "Book added successfully." << endl;
    }
}

// Function to display all available books
void displayAvailableBooks() 
{
    string query = "SELECT * FROM books WHERE available = 1";
    
    if (mysql_query(conn, query.c_str()) != 0) 
	{
        cerr << "Error: " << mysql_error(conn) << endl;
    } 
	else 
	{
        MYSQL_RES* res = mysql_store_result(conn);
        if (res != NULL) 
		{
            cout << "Available Books:" << endl;
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res))) 
			{
                cout << "ID: " << row[0] << ", Title: " << row[1] << ", Author: " << row[2] << endl;
            }
            mysql_free_result(res);
        }
    }
}

// Function to borrow a book
void borrowBook() 
{
    int bookId;
    cout << "Enter book ID to issue: ";
    cin >> bookId;

    stringstream ss;
    ss << bookId;

    // Check if the book exists
    string checkQuery = "SELECT * FROM books WHERE id = " + ss.str();
    if (mysql_query(conn, checkQuery.c_str()) != 0) 
	{
        cerr << "Error: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES* checkRes = mysql_store_result(conn);
    if (mysql_num_rows(checkRes) == 0) 
	{
        cout << "Book with ID " << bookId << " not found." << endl;
        mysql_free_result(checkRes);
        return;
    }

    // Update the availability status
    string query = "UPDATE books SET available = 0 WHERE id = " + ss.str();
    if (mysql_query(conn, query.c_str()) != 0) 
	{
        cerr << "Error: " << mysql_error(conn) << endl;
    } 
	else 
	{
        cout << "Book issued successfully." << endl;
    }

    mysql_free_result(checkRes);
}

// Function to return a book
void returnBook() 
{
    int bookId;
    cout << "Enter book ID to return: ";
    cin >> bookId;

    stringstream ss;
    ss << bookId;

    // Check if the book exists
    string checkQuery = "SELECT * FROM books WHERE id = " + ss.str();
    if (mysql_query(conn, checkQuery.c_str()) != 0) 
	{
        cerr << "Error: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES* checkRes = mysql_store_result(conn);
    if (mysql_num_rows(checkRes) == 0) 
	{
        cout << "Book with ID " << bookId << " not found." << endl;
        mysql_free_result(checkRes);
        return;
    }

    MYSQL_ROW checkRow = mysql_fetch_row(checkRes);
    int availability = atoi(checkRow[3]);
    mysql_free_result(checkRes);

    if (availability == 1) 
	{
        cout << "Book with ID " << bookId << " is not issued. Cannot return." << endl;
        return;
    }

    // Update the availability status
    string query = "UPDATE books SET available = 1 WHERE id = " + ss.str();
    if (mysql_query(conn, query.c_str()) != 0) 
	{
        cerr << "Error: " << mysql_error(conn) << endl;
    } 
	else 
	{
        cout << "Book returned successfully." << endl;
    }
}

// Function to delete a book
void deleteBook() 
{
    int bookId;
    cout << "Enter book ID to delete: ";
    cin >> bookId;

    stringstream ss;
    ss << bookId;

    // Check if the book exists
    string checkQuery = "SELECT * FROM books WHERE id = " + ss.str();
    if (mysql_query(conn, checkQuery.c_str()) != 0) 
	{
        cerr << "Error: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES* checkRes = mysql_store_result(conn);
    if (mysql_num_rows(checkRes) == 0) 
	{
        cout << "Book with ID " << bookId << " not found." << endl;
        mysql_free_result(checkRes);
        return;
    }
    mysql_free_result(checkRes);

    // Delete the book
    string query = "DELETE FROM books WHERE id = " + ss.str();
    if (mysql_query(conn, query.c_str()) != 0) 
	{
        cerr << "Error: " << mysql_error(conn) << endl;
    } 
	else 
	{
        cout << "Book deleted successfully." << endl;
    }
}

int main() 
{
    if (!initializeConnection()) 
	{
        exit(1);
    }

    int option;
    do {
        cout << "Library Management System" << endl;
        cout << "1. Add a book" << endl;
        cout << "2. Display available books" << endl;
        cout << "3. Issue a book" << endl;
        cout << "4. Return a book" << endl;
        cout << "5. Delete a book" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> option;

        switch (option) 
		{
            case 1: addBook(); break;
            case 2: displayAvailableBooks(); break;
            case 3: borrowBook(); break;
            case 4: returnBook(); break;
            case 5: deleteBook(); break;
            case 0: cout << "Exiting Library Management System." << endl; break;
            default: cout << "Invalid option. Please try again." << endl;
        }
    } while (option != 0);

    mysql_close(conn);
    return 0;
}
