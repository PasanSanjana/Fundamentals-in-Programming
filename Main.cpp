#include <filesystem>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip> // For setw, fixed, setprecision
#include <limits>  // For numeric limits
#include <chrono>  // For date/time
#include <ctime>   // For date/time
#include <cctype>  // For isdigit

using namespace std;

// Book Details
struct BookDetails{
    int ISBN;
    string BookName;
    string Author;
    string Genres;
    double price;
    int stockCount;
};

// Sales Details
struct SaleDetails{
    string saleDate;
    int SaleID;
    int ISBN;
    string BookName;
    string BuyerName;
    long long MobileNumber;
    int Quantity; // check stockCount >= Quantity
    double TotalPrice; //  total = (one book price * Quantity)*discounted price(as a float)
};

class GeniusBookStore {
private:
    // default User Name And Password
    /*
    * User Name - Pasan
    * Password - Spasan0088@
    */
    string userName = "Pasan";
    string Password = "Spasan0088@";

public:
    // check default username and password
    bool isLoggedIn(const string& username, const string& password) const {
        return (userName == username && Password == password);
    }
};


void MainMenu();

//--------------------------------------------------------
/*
 * Manage All Sales
 * View All Sales
 * Add New Oder
 * Update Oder Details
 * delete Oders
 * View Spesicfic Oder details Via SalesID
 */
class Sales_Details {
private:
    vector<SaleDetails> sales;

    // View All Sales Details
    void ViewSales() {
        sales.clear();
        ifstream ViewSales("Sales.txt"); //open or create new Sales.txt
        if (!ViewSales.is_open()) { // error if sales.txt not open
            return;
        }
        string line;
        while (getline(ViewSales, line)) {
            stringstream S_data(line);
            SaleDetails currentSale;
            string saleDateS, saleIDS, isbnS, MNs, QuS, TotalS;

            getline(S_data >> ws, saleDateS, '|');
            getline(S_data >> ws, saleIDS, '|');
            getline(S_data >> ws, isbnS, '|');
            getline(S_data >> ws, currentSale.BookName, '|');
            getline(S_data >> ws, currentSale.BuyerName, '|');
            getline(S_data >> ws, MNs, '|');
            getline(S_data >> ws, QuS, '|');
            getline(S_data >> ws, TotalS, '|');


            currentSale.saleDate = saleDateS;
            currentSale.SaleID = stoi(saleIDS);
            currentSale.ISBN = stoi(isbnS);
            currentSale.MobileNumber = stoll(MNs);
            currentSale.Quantity = stoi(QuS);
            currentSale.TotalPrice = stod(TotalS);
            sales.push_back(currentSale);
        }
        ViewSales.close();
    }

    BookDetails FindISBN (int ISBNs) {
        ifstream Books("Books.txt");
        if (!Books.is_open()) {
            return BookDetails{-1};
        }
        string line;
        while (getline(Books, line)) {
            stringstream B_Data(line);
            BookDetails FindBookISBN;
            string isbnS, PriceS, stockS;

            getline(B_Data >> ws, isbnS, '|');
            getline(B_Data >>ws, FindBookISBN.BookName, '|');
            getline(B_Data >> ws, FindBookISBN.Author, '|');
            getline(B_Data >> ws, FindBookISBN.Genres, '|');
            getline(B_Data >> ws, PriceS, '|');
            getline(B_Data >> ws, stockS, '|');

            FindBookISBN.ISBN = stoi(isbnS);
            FindBookISBN.price = stod(PriceS);
            FindBookISBN.stockCount = stoi(stockS);

            if (FindBookISBN.ISBN == ISBNs) {
                Books.close();
                return FindBookISBN; //succesfully foun book
            }
        }
        Books.close();
        return BookDetails{-1}; //no book found
    }

    //Delete Or Update Orders
    void Updates() {
        ofstream UpdatesOders("Sales.txt", ios::out | ios::trunc);
        if (!UpdatesOders.is_open()) {
            cout << "\nError : Unable To Update Oders!!\n";
            return;
        }
        for (const auto& order : sales) {
            UpdatesOders << order.saleDate << " | "
            << order.SaleID << " | "
            << order.ISBN << " | "
            << order.BookName << " | "
            << order.BuyerName << "  | "
            << order.MobileNumber << " | "
            << order.Quantity << " | "
            << order.TotalPrice << " | " << endl;
        }
        UpdatesOders.close();
    }

    //Stock Count Chnage with Oders
    void StockMange(int ISBN_S, int QuantitySold) {
        vector<BookDetails> AllBooks;
        ifstream books("Books.txt");
        string line;
        bool  StockUpdate = false;

        while (getline(books, line)) {
            stringstream S_data(line);
            BookDetails Books;
            string priceS, isbnS, StockS;

            getline(S_data >>  ws, isbnS, '|');
            getline(S_data >> ws, Books.BookName, '|');
            getline(S_data >> ws, Books.Author, '|');
            getline(S_data >> ws, Books.Genres, '|');
            getline(S_data >> ws, priceS, '|');
            getline(S_data >> ws, StockS, '|');

            Books.ISBN = stoi(isbnS);
            Books.price = stod(priceS);
            Books.stockCount = stoi(StockS);

            if (Books.ISBN == ISBN_S) {
                Books.stockCount -= QuantitySold;
                StockUpdate = true;
            }
            AllBooks.push_back(Books);
        }
        books.close();

        if (!StockUpdate) {
            cout << "Warning: Could not find book to update stock for ISBN: " << ISBN_S << endl;
            return;
        }
        ofstream BooksFile("Books.txt", ios::trunc | ios::out);
        if (!BooksFile.is_open()) {
            cout << "Could not open books file for Updates" << endl;
            return;
        }
        for (const auto& Book : AllBooks) {
            BooksFile  << Book.ISBN << " | "
                    << Book.BookName << " | "
                    << Book.Author << " | "
                    << Book.Genres << " | "
                    << Book.price << " | "
                    << Book.stockCount << endl;
        }BooksFile.close();
    }
    //Add New order
    void NewOrder(const SaleDetails& order) {
        ofstream newBookorder("Sales.txt", ios::app);
        if (!newBookorder.is_open()) {
            cout << "Error opening file " << endl;
            return;
        }
        newBookorder << order.saleDate << " | "
        << order.SaleID << " | "
        << order.ISBN << " | "
        << order.BookName << " | "
        << order.BuyerName << " | "
        << order.MobileNumber << " | "
        << order.Quantity << " | "
        <<order.TotalPrice << endl;
        newBookorder.close();
    }


public:
    //View All Sales Details
    void ViewAllSales() {
        ViewSales();

        cout << "\n         - All Sales -           \n";

        if (sales.empty()) {
            cout << "\nNo sales records found.\n";
            return;
        }
        for ( const auto& ViewSale : sales ) {
            cout << "\n";
            cout << "Sales Date         : " << ViewSale.saleDate << endl;
            cout << "Sale ID            : " << ViewSale.SaleID << endl;
            cout << "ISBN               : " << ViewSale.ISBN << endl;
            cout << "Book Name          : " << ViewSale.BookName << endl;
            cout << "Buyer Name         : " << ViewSale.BuyerName << endl;
            cout << "Mobile Number      : " << ViewSale.MobileNumber << endl;
            cout << "Quantity           : " << ViewSale.Quantity << endl;
            cout << "Total Price        : $" << ViewSale.TotalPrice << endl;
            cout << "-----------------------" << endl;
        }
    }

    // Add New Oder To Oders.txt
    void AddOrder() {
        ViewSales();
        SaleDetails AddNewOrder;
        BookDetails FindBook;

        // Get current date
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        char date_buffer[11];
        std::strftime(date_buffer, sizeof(date_buffer), "%Y.%m.%d", std::localtime(&now_c));
        AddNewOrder.saleDate = date_buffer;


        while (true) {
            cout << "\nEnter Book ISBN : ";
            if (cin >> AddNewOrder.ISBN) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                FindBook = FindISBN(AddNewOrder.ISBN);
                if (FindBook.ISBN == -1) {
                    cout << "\nNo Book With That ISBN Found. Please try again.\n";
                } else {
                    break;
                }
            } else {
                cout << "\nInvalid input. Please enter a number for the ISBN.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        //Display book details
        cout << "\nBook Details With " << AddNewOrder.ISBN << "\n";
        cout << "\nBook Name : " << FindBook.BookName << endl;
        AddNewOrder.BookName = FindBook.BookName;
        cout << "Book Price : " << FindBook.price << endl;
        cout << "Current Stock Count : " << FindBook.stockCount << endl;

        // get buyer information
        cout << "\nBuyer Name : ";
        getline(cin, AddNewOrder.BuyerName);

        string mobile_str; // Use a temporary string for mobile number input
        while (true) {
            cout << "\nMobile Number (10 digits) : ";
            getline(cin, mobile_str);

            if (mobile_str.length() == 10) { //Conform Mobile Number Have Only 10 Numbers
                bool is_all_digits = true;
                for (char m : mobile_str) {
                    if (!isdigit(m)) {
                        is_all_digits = false;
                        break;
                    }
                }
                if (is_all_digits) {
                    AddNewOrder.MobileNumber = stoll(mobile_str);
                    break;
                }
            }
            \
            cout << "\nPlease enter a correct number without country code.\n";
        }


        while (true) {
            cout << "\nQuantity " << "(Maximum Quantity Is " << FindBook.stockCount << " ) : ";
            if (cin >> AddNewOrder.Quantity) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (AddNewOrder.Quantity > FindBook.stockCount) {
                    cout << "\nNot Enough Stocks To Place The Oder!!\n";
                } else {
                    break;
                }
            } else {
                cout << "\nInvalid input. Please enter a number for the quantity.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        // Get Total Amount
        AddNewOrder.TotalPrice = AddNewOrder.Quantity * FindBook.price;
        cout << "\nTotal Price : " << AddNewOrder.TotalPrice << "\n\n";

        AddNewOrder.SaleID = sales.empty() ? 1 : sales.back().SaleID + 1;
        NewOrder(AddNewOrder);
        cout << "\nOrder Complete...\n";
        StockMange(AddNewOrder.ISBN, AddNewOrder.Quantity);
    } // End of AddOrder function

    //Update the Order Details
   void DeleteOrders() {
    ViewSales();
    int delSaleID;
    int option;
    cout << "\nEnter Sale ID To Continue Delete : ";
    cin >> delSaleID;
    bool ISDelete = false;
    for (auto del = sales.begin(); del != sales.end(); ++del) {
        if (del->SaleID == delSaleID) {
            cout << "\nFound Oder With "<< del->BuyerName << "\n";
            cout << "\n01. Conform Delete\n";
            cout << "\n02. Back To Main Menu";

            cout <<"\n\nEnter Option : ";
            cin >> option;

            switch (option) {
                case 1:
                    cout << "\nDeleting: " << del->SaleID << "\n";
                    sales.erase(del);
                    ISDelete = true;
                    break;
                case 2:
                    MainMenu();
                    return;
                default:
                    cout << "\nInvalid Option\n";
                    break;
            }
            if (ISDelete) {
                break;
            }
        }
    }
    if (!ISDelete) {
        cout << "\nNo Oders With Sale ID " << delSaleID << " Not Found...\n";
    } else {
        Updates();
        cout << "Delete Successful....\n";
           }
    }

    //Veiw Spesific Oder Details
    void ViewSpecSale(){
        ViewSales();
        int ViewID;
        cout << "\nEnter Sale ID To View : ";
        cin >> ViewID;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        bool IsViewFind = false;
        for (const auto& sale : sales) {
            if (sale.SaleID == ViewID) {
                cout << "\n            - Order Details -    \n";
                cout << "-----------------------\n";
                cout << "Sales Date         : " << sale.saleDate << endl;
                cout << "Sale ID            : " << sale.SaleID << endl;
                cout << "ISBN               : " << sale.ISBN << endl;
                cout << "Book Name          : " << sale.BookName << endl;
                cout << "Buyer Name         : " << sale.BuyerName << endl;
                cout << "Mobile Number      : " << sale.MobileNumber << endl;
                cout << "Quantity           : " << sale.Quantity << endl;
                cout << "Total Price        : $" << sale.TotalPrice << endl;
                cout << "-----------------------" << endl;
                IsViewFind = true;
                break; // Exit the loop once the correct sale is found
            }
        }
        if (!IsViewFind) {
            cout << "\nNo Oders found With Sale ID " << ViewID << " Please Re Check Sale ID \n";
        }
    }


    // Manage Sales Menu
    void MangeSalesMenu() {
        int Choice;

        while (true) { //Loop Main Menu untill logout or exit
            cout << "\n                -   Manage Sales    -           \n\n";
            cout << "               01. Place A New Order\n";
            cout << "               02. Delete Orders From The Store\n";
            cout << "               03. View Specific Order Details\n";
            cout << "               04. Back To Main Menu\n";
            cout << "\nPlease Enter Your Choice: ";
            cin >> Choice;

            if (cin.fail()) {
                cout << "\nInvalid input. Please enter a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (Choice) {
                case 1:
                    cout << "\n           -   Place A New Order  -           \n";
                    cout << "\n       Please fill the details to add a new book to the store \n\n";
                    AddOrder();
                    break;
                case 2:
                    cout << "\n           -   Delete Orders From The Store -             \n";
                    DeleteOrders();
                    break;
                case 3:
                    cout << "\n           -   View Specific Order Details -             \n";
                    ViewSpecSale();
                    break;
                case 4:
                    cout << "\nReturning to Main Menu...\n";
                    return;
                    break;

                default:
                    cout << "\nInvalid Choice. Please try again.\n";
            }
        }
    }

};


//--------------------------------------------------------
/*
 * Manage All Books
 * View and CRUD Operations
 */
class ManageBooks {
private:
    vector<BookDetails> books;

    // View All Books in Books.txt
    void ViewAllBooks() {
        books.clear();
        ifstream View("Books.txt");
        if (!View.is_open()) {
            return;
        }

        string line;
        while (getline(View, line)) {
            stringstream B_data(line);
            BookDetails ViewBook;
            string priceS, stockCountS, isbnS;


            getline(B_data >> ws, isbnS, '|');
            getline(B_data >> ws, ViewBook.BookName, '|');
            getline(B_data >> ws, ViewBook.Author, '|');
            getline(B_data >> ws, ViewBook.Genres, '|');
            getline(B_data >> ws, priceS, '|');
            getline(B_data >> ws, stockCountS, '|');

            ViewBook.ISBN = stoi(isbnS); // string to integer
            ViewBook.price = stod(priceS); //string to double
            ViewBook.stockCount = stoi(stockCountS);
            books.push_back(ViewBook);
        }
        View.close();
    }

    // Add New Books To Books.txt
    void AddNewBook(const BookDetails& book) {
        ofstream AddNew("Books.txt", ios::app);
        if (!AddNew.is_open()) {
            cout << "Error: Unable to open file....\n";
            return;
        }
        AddNew << book.ISBN << " | "
                << book.BookName << " | "
                << book.Author << " | "
                << book.Genres << " | "
                << book.price << " | "
                << book.stockCount << endl;
        AddNew.close();
    }

    // Update Or Delete Book in Books.txt
    void Updates() {
        ofstream UpdateBooks("Books.txt", ios::out | ios::trunc); // Overwrite file, trunc clears content
        if (!UpdateBooks.is_open()) {
            cout << "Error: Unable to open file.....\n";
            return;
        }
        for (const auto& book : books) {
            UpdateBooks << book.ISBN << " | "
                    << book.BookName << " | "
                    << book.Author << " | "
                    << book.Genres << " | "
                    << book.price << " | "
                    << book.stockCount << endl;
        }
        UpdateBooks.close();
    }

public:
    ManageBooks() {
        ViewAllBooks();
    }

    //--------------------------------------------
    /*
     * View All Books
     * Get Book Details from Books.txt
     * Display As A Table Using setw
     */
    void ViewBooks() {
        ViewAllBooks();

        if (books.empty()) {
            cout << "\n No Books Available Yet....\n";
            return;
        }

        cout << left << setw(10) << "ISBN"
             << setw(30) << "Book Name"
             << setw(20) << "Author"
             << setw(15) << "Genres"
             << setw(10) << "Price $"
             << setw(10) << "Stock" << endl;
        cout << string(100, '-') << endl;

        for (const auto& book : books) {
            cout << left << setw(10) << book.ISBN
                 << setw(30) << book.BookName
                 << setw(20) << book.Author
                 << setw(15) << book.Genres
                 << setw(10) << book.price
                 << setw(10) << book.stockCount << endl;
        }
    }

    //--------------------------------------------
    /*
     * Add New Books to Books.txt
     */
    void AddBooks() {
        BookDetails NewBook;
        cout<< "\nEnter Book ISBN : "<< endl;
        cin >> NewBook.ISBN;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Check duplicate ISBN
        for (const auto& book : books) {
            if (book.ISBN == NewBook.ISBN) {
                cout << "A book with this ISBN already exists.\n";
                return;
            }
        }


        cout << "\nEnter Book Name : \n";
        getline(cin, NewBook.BookName);
        cout << "\nEnter Book Author Name : \n";
        getline(cin, NewBook.Author);
        cout << "\nEnter Book Genres (Separate by using ' , ') : \n";
        getline(cin, NewBook.Genres);
        cout << "\nEnter Book Price : $\n";
        cin >> NewBook.price;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nEnter Book Stock Count : \n";
        cin >> NewBook.stockCount;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        books.push_back(NewBook);
        AddNewBook(NewBook);

    }

    //--------------------------------------------------------
    // Delete Books From Books.txt
    void DeleteBooks() {
        int delISBN;
        cout << "Enter ISBN Of The Book To Delete : ";
        cin >> delISBN;

        // Book delete Part
        bool IsDelete = false;
        for (auto del = books.begin(); del != books.end(); ++del) {
            if (del->ISBN == delISBN) {
                cout << "\nDeleting: " << del->BookName << "\n";
                books.erase(del);
                IsDelete = true;
                break;
            }
        }
        //Error message if No Books with enterd ISBN
        if (!IsDelete) {
            cout << "\nBook With ISBN " << delISBN << " Not Found...\n";
        } else {
            Updates();
            cout << "Delete Successful....\n";
        }
    }

    //--------------------------------------------------------
    //Update Book Data in Books.txt
    void UpdateBooks() {
        int UpISBN;
        cout << "Enter ISBN Of The Book To Update : ";
        cin >> UpISBN;

        bool BookFound = false;
        for (auto& book : books) { // Use reference to modify directly
            if (book.ISBN == UpISBN) {
                BookFound = true;
                cout << "\nBook Found: " << book.BookName <<"\n\n";

                cout << "Enter New Book Name (current: " << book.BookName << ") : ";
                getline(cin, book.BookName);
                cout << "Enter New Author (current: " << book.Author << ") : ";
                getline(cin, book.Author);
                cout << "Enter New Genres (current: " << book.Genres << ") : ";
                getline(cin, book.Genres);
                cout << "Enter New Price (current: $" <<book.price << ") : ";
                cin >> book.price;
                cout << "Enter New Stock Count (current: " << book.stockCount << ") : ";
                cin >> book.stockCount;

                // Update old data In to New data in ISBN
                Updates();
                cout << "\nBook details updated successfully.\n";
                break;
            }
        }
        // Show Error Invalid ISBN
        if (!BookFound) {
            cout << "\nBook with ISBN " << BookFound << " not found.\n";
        }
    }

    //--------------------------------------------------------
    // Manage All Books
    /*
     * Add New Books  - AddBooks();
     * Delete Books
     * Update Book Details
     */
    void ManageBookMenu() {
        int Choice;

        while (true) {
            cout << "\n                -   Manage All Books    -           \n\n";
            cout << "               01. Add New Book To Store\n";
            cout << "               02. Update Selected Books\n";
            cout << "               03. Delete Book From Store\n";
            cout << "               04. Back To Main Menu\n";
            cout << "\nPlease Enter Your Choice: ";
            cin >> Choice;

            //Validate Integer
            if (cin.fail()) {
                cout << "\nInvalid input. Please enter a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue; //Agin Show Manage Book Menu
            }

            switch (Choice) {
                case 1:
                    cout << "\n           -   Add New Books To The Store  -           \n";
                    cout << "\n       Please fill the details to add a new book to the store \n\n";
                    AddBooks();
                    if (cin.fail()) {

                    } else {
                        cout << "\nComplete... Book Successfully Added.\n";
                    }
                    break;
                case 2:
                    cout << "\n           -   Update Book Details In The Store -            \n";
                    UpdateBooks();
                    break;
                case 3:
                    cout << "\n           -   Delete Books From The Store -             \n";
                    DeleteBooks();
                    break;
                case 4:
                    cout << "\nReturning to Main Menu...\n";
                    return; // Exit the loop and return to where ManageBookMenu was called
                default:
                    cout << "\nInvalid Choice. Please try again.\n";
            }
        }
    }
};

//--------------------------------------------------------
/*
 * Login Page
 * User Name  - Pasan
 * Password - Spasan0088@
 */
void LogPage() {
    GeniusBookStore BookStore;
    string username, password;

    cout << "\n           |   Welcome To  |       \n";
    cout << "\n          -   Genius Books    -           \n";
    cout << "\n";
    cout << "                LogIn            \n";
    cout << "\n";
    cout << "\n";
    cout << " Enter Your User Name: ";
    cin >> username;
    cout << "\n";
    cout << " Enter Your Password: ";
    cin >> password;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Consume remaining newline

    // check the credentials
    if (BookStore.isLoggedIn(username, password)) {
        cout << "Logged In Successfully!\n";
    } else {
        cout << "Invalid credentials. Please Try Again.\n";
        LogPage(); // if the credentials Wrong, recall LogPage
    }
}

//Company Deatails
void AboutUs() {
    cout << "\n             |   About Us | \n\n";
    cout << "Name       : GENIUS BOOKS\n";
    cout << "Type       : Bookshop Management System\n";
    cout << "Services   : Wholesaler supplying bulk books\n";
    cout << "Location   : Mehewara Piyesa, 15th Floor, Narahenpita, Colombo 5\n";
    cout << "Contact    : 0112000222 / +9476000011332\n";
    cout << "Email      : publishers@geniusbooks.com\n ";
}
//--------------------------------------------------------
/*
 * Main Menu Page
 *
 * View Books
 * Manage Books
 * View Sales
 * Manage Sales
 * LogOut
 */
void MainMenu() {
    Sales_Details Sale;
    ManageBooks Manage; // Create an instance of ManageBooks
    int choice;
    string confirmation;

    while (true) { // Loop to keep showing menu until user exits or logs out
        cout << "\n         | Main Menu |           \n";
        cout << "       01. View Available Books\n";
        cout << "       02. Manage Books\n";
        cout << "       03. View Sales\n";
        cout << "       04. Manage Orders\n";
        cout << "       05. About Company\n";
        cout << "       06. Logout\n";
        cout << "       07. Exit\n";

        cout << " Please Enter Your Choice:  ";
        cin >> choice;

        if (cin.fail()) { // Handle non-integer input
            cout << "\nInvalid input. Please enter a number.\n";
            cin.clear(); // Clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            continue; // Restart loop
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Consume remaining newline


        switch (choice) {
            case 1:
                Manage.ViewBooks();
                break;
            case 2:
                Manage.ManageBookMenu();
                break;
            case 3:
                Sale.ViewAllSales();
                break;
            case 4:
                Sale.MangeSalesMenu();
                break;
            case 5:
                AboutUs();
                break;
            case 6:
                cout << "\nConfirm Before Log Out : ";
                cout << "\nType [yes] to Logout. Press Enter to return to Main Menu \n";
                getline(cin, confirmation); // Use getline for confirmation
                if (confirmation == "yes") {
                    cout << "Logging out...\n";
                    LogPage();
                    MainMenu();
                    return;
                }
                break; // If not "yes", just break and show MainMenu again
            case 7:
                cout << "\nExiting....\n";
                exit(0);
            default:
                cout << "\nInvalid Choice. Please Enter a Valid Choice.\n";
        }
    }
}

//--------------------------------------------------------
int main() {
    LogPage();
    MainMenu();

    return 0;
}
