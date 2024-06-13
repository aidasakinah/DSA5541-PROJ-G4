#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <string>
#include <algorithm>

// 2 additional libraries
#include <conio.h> // for getch() function for password
#include <vector>  // for vector (found in manageMenu() function)
#include <sstream>

using namespace std;
const int MAX_ITEM = 100;

class User
{
private:
    string username;
    string email;
    string address;
    string contactNumber;
    string password;
    string cpassword;
    string inputUsername;
    string inputPassword;
    string existingUsername;
    string existingEmail;
    bool loggedIn;

    bool isValueExists(const string &filename, const string &valueToCheck);

    // Helper function to trim leading and trailing whitespaces
    string trim(const string &str);

public:
    User() : loggedIn(false) {}

    void login()
    {
        system("cls");
        cout << "User Login" << endl;
        cout << "USERNAME or EMAIL  : ";
        getline(cin, inputUsername);
        cout << "PASSWORD           : ";
        getline(cin, inputPassword);

        ifstream input("user records.txt");
        string line;
        bool loginSuccess = false;
        while (getline(input, line))
        {
            stringstream ss(line);
            ss >> username >> email;
            ss.ignore(1, '"');         // Ignore the space before the address opening quote
            getline(ss, address, '"'); // Read the address up to the closing quote
            ss >> contactNumber >> password >> cpassword;
            if ((trim(username) == trim(inputUsername) || trim(email) == trim(inputUsername)) &&
                password == inputPassword)
            {
                loginSuccess = true;
                system("cls");
                setUsername(username);
                setAddress(address);
                break;
            }
        }
        input.close(); // closes file

        if (loginSuccess)
        {
            cout << "\nHello " << getUsername() << "\n<LOGIN SUCCESSFUL>\nThanks for logging in Restaurant Fusion Fare Delights\n";
            loggedIn = true;
        }
        else
        {
            cout << "\nLOGIN ERROR\nPlease check again your username or email and password\n";
        } // end of if else
    }     // end of login

    void logout()
    {
        loggedIn = false;
    }

    void registration()
    {
        system("cls");
        cout << "User Registration\n"
             << endl;
        // Prompt user until a valid single-word username is entered
        do
        {
            cout << "Username (Single-Word) \t: ";
            getline(cin, username);

            // Check if the name contains any space
            if (username.find(' ') != string::npos)
            {
                cout << "\nInvalid username format. Please use a single word without spaces.\n";
            }
        } while (username.find(' ') != string::npos);

        // Check if the name already exists
        ifstream checkName("user records.txt");
        string line;
        while (getline(checkName, line))
        {
            stringstream ss(line);
            ss >> existingUsername;
            // trime is used to remove leading and trailing whitespaces

            if (trim(existingUsername) == trim(username))
            {
                cout << "Username already exists. Please try again and choose a different name.\n";
                // return;
                cout << "\nUsername (Single-Word) \t: ";
                getline(cin, username);

            } // end of if

        } // end of while
        checkName.close();

        bool emailExists;

        do
        {
            cout << "Email \t\t\t: ";
            getline(cin, email);

            // Ensure that the email contains the '@' symbol
            if (email.find('@') == string::npos)
            {
                cout << "Invalid email format. Please use a valid email address.\n\n";
                continue; // Go to the next iteration of the loop if the email format is invalid
            }

            // Check if the email already exists
            ifstream checkEmail("user records.txt");
            emailExists = false;
            while (getline(checkEmail, line))
            {
                stringstream ss(line);
                ss >> existingUsername >> existingEmail;
                if (trim(existingEmail) == trim(email))
                {
                    emailExists = true;
                    break;
                }
            }
            checkEmail.close();

            if (emailExists)
            {
                cout << "Email already exists. Please try again with a different email.\n\n";
            }
        } while (emailExists || email.find('@') == string::npos);

        cout << "Address \t\t: ";
        getline(cin, address);

        // Ensure that the address is stored as a single column without line breaks
        size_t found = address.find('\n');
        while (found != string::npos)
        {
            address.replace(found, 1, " ");
            found = address.find('\n');
        }

        cout << "Contact Number \t\t: ";
        getline(cin, contactNumber);

        do
        {
            cout << "Password \t\t: ";
            getline(cin, password);
            cout << "Confirm Password \t: ";
            getline(cin, cpassword);

            if (password != cpassword)
            {
                cout << "Passwords do not match. Please try again.\n\n"
                     << endl;
            }
        } while (password != cpassword);

        ofstream reg("user records.txt", ios::app);
        reg << username << ' ' << email << ' ' << '"' << address << '"' << ' ' << contactNumber << ' ' << password << ' ' << cpassword << '\n';
        reg.close();
        system("cls");
        cout << "\nRegistration Successful\n";
    }

    void forgetPassword()
    {
        string input;
        cout << "Enter your username or email \t: ";
        getline(cin, input);

        string line;

        ifstream userFile("user records.txt");
        while (getline(userFile, line))
        {
            stringstream ss(line);

            // Read data from stringstream
            ss >> ws >> username >> ws >> email >> ws;

            // Read the entire line (including address) into a string
            getline(ss, line);

            // Trim leading and trailing whitespaces from username and email
            username = trim(username);
            email = trim(email);

            if (username == input || email == input)
            {
                // Find the position of the opening and closing quotes
                size_t startQuote = line.find('"');
                size_t endQuote = line.find('"', startQuote + 1);

                // Check if both quotes are found
                if (startQuote != string::npos && endQuote != string::npos)
                {
                    // Extract the content between the quotes as the address
                    address = line.substr(startQuote + 1, endQuote - startQuote - 1);

                    // Use another stringstream to extract the rest of the data
                    stringstream dataStream(line.substr(endQuote + 1));

                    // Extract remaining data
                    dataStream >> ws >> contactNumber >> ws >> password >> ws >> cpassword;

                    cout << "Your password is \t\t: " << password << endl;
                    userFile.close();
                    system("pause");
                    system("cls");
                    return;
                }
            }
        }

        cout << "User not found. Please check your username or email." << endl;
        userFile.close();
        system("pause");
        system("cls");
        return;
    }

    void setUsername(const string &name)
    {
        username = name;
    }

    string getUsername() const
    {
        return username;
    }

    void setAddress(const string &addr)
    {
        address = addr;
    }

    string getAddress() const
    {
        return address;
    }

    bool isLoggedIn() const
    {
        return loggedIn;
    }
};

// Helper function to trim leading and trailing whitespaces
string User::trim(const string &str)
{
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
};

class PaymentMethod
{
protected:
    string cardHolderName;
    string cardNumber;
    string expirationDate;
    string cvv;

public:
    // Function to validate the card number
    bool isValidCardNumber(const string &cardNumber)
    {
        // Check if the card number has 16 digits
        return cardNumber.length() == 16 && all_of(cardNumber.begin(), cardNumber.end(), ::isdigit);
    }

    // Function to validate the expiration date
    bool isValidExpirationDate(const string &expirationDate)
    {
        // Check if the expiration date has 5 characters and follows the MM/YY format
        return expirationDate.length() == 5 && isdigit(expirationDate[0]) && isdigit(expirationDate[1]) &&
               expirationDate[2] == '/' && isdigit(expirationDate[3]) && isdigit(expirationDate[4]);
    }

    // Function to validate the CVV
    bool isValidCVV(const string &cvv)
    {
        // Check if the CVV has 3 digits
        return cvv.length() == 3 && all_of(cvv.begin(), cvv.end(), ::isdigit);
    }
};

// inheritance
class Cart : public PaymentMethod
{
private:
    static const int MAX_CART_ITEMS = 100;
    string *cartItems;
    float *cartPrices;
    int *cartQuantities;
    int itemCount;
    char payOption;
    User user;

    // struct for order history
    struct OrderHistory
    {
        string itemName;
        float itemPrice;
        int quantity;
    }; // end of struct

    vector<OrderHistory> orderHistory; // clears cart after payment

    void removeItemFromCart(int index)
    {
        if (index < 0 || index >= itemCount)
        {
            cout << "Invalid index. No item removed." << endl;
            return;
        }
        for (int i = index; i < itemCount - 1; i++)
        {
            cartItems[i] = cartItems[i + 1];
            cartPrices[i] = cartPrices[i + 1];
            cartQuantities[i] = cartQuantities[i + 1];
        }
        itemCount--;
        cout << "Item removed successfully." << endl;
    } // end of removeItemFromCart

    void displayCartItemsOnly()
    {
        for (int i = 0; i < itemCount; ++i)
        {
            cout << i + 1 << ". " << cartItems[i] << " x" << cartQuantities[i] << " RM" << cartPrices[i] << endl;
        }
    }

public:
    Cart() : cartItems(new string[MAX_CART_ITEMS]), cartPrices(new float[MAX_CART_ITEMS]),
             cartQuantities(new int[MAX_CART_ITEMS]), itemCount(0) {} // end of constructor

    ~Cart()
    {
        delete[] cartItems;
        delete[] cartPrices;
        delete[] cartQuantities;
    } // end of destructor

    void clearCart()
    {
        itemCount = 0; // Reset any other relevant variables in the cart
    }                  // end of clearCart

    float calculateTotalPrice() const
    {
        float total = 0.0;
        for (int i = 0; i < itemCount; ++i)
        {
            total += (cartPrices[i] * cartQuantities[i]);
        }
        return total;
    } // end of calculateTotalPrice

    void setItemDetails(const string &itemName, float itemPrice, int quantity)
    {
        if (itemCount < MAX_CART_ITEMS)
        {
            cartItems[itemCount] = itemName;
            cartPrices[itemCount] = itemPrice;
            cartQuantities[itemCount] = quantity;
            itemCount++;
        }
        else
        {
            cout << "Cart is full. Cannot add more items." << endl;
        }
    } // end of setItemDetails

    void removeItem()
    {
        if (itemCount == 0)
        {
            cout << "Your cart is empty." << endl;
            system("pause");
            system("cls");
            return;
        }

        int removeIndex, removeQuantity;
        displayCartItemsOnly();

        cout << "Enter the index of the item to remove: ";
        cin >> removeIndex;

        if (removeIndex < 1 || removeIndex > itemCount)
        {
            cout << "Invalid index." << endl;
            system("pause");
            system("cls");
            return;
        }

        cout << "Enter the quantity to remove: ";
        cin >> removeQuantity;

        if (removeQuantity <= 0)
        {
            cout << "Invalid quantity." << endl;
            system("pause");
            system("cls");
            return;
        }

        removeIndex--;
        if (cartQuantities[removeIndex] <= removeQuantity)
        {
            removeItemFromCart(removeIndex);
        }
        else
        {
            cartQuantities[removeIndex] -= removeQuantity;
            cout << "Quantity updated." << endl;
        }
    } // end of removeItem

    int getCartQuantity(int index) const
    {
        if (index >= 0 && index < itemCount)
        {
            return cartQuantities[index];
        }
        else
        {
            cout << "Invalid index. Quantity cannot be retrieved." << endl;
            return 0;
        }
    } // end of getCartQuantity

    void updateOrderHistory()
    {
        orderHistory.clear();
        for (int i = 0; i < itemCount; ++i)
        {
            OrderHistory order;
            order.itemName = cartItems[i];
            order.itemPrice = cartPrices[i];
            order.quantity = cartQuantities[i];
            orderHistory.push_back(order);
        }
    } // end of updateOrderHistory

    const vector<OrderHistory> &getOrderHistory() const
    {
        return orderHistory;
    }

    void updateSalesCount(const User &user)
    {
        ofstream salesReport("sales_report.txt", ios::app);
        if (!salesReport)
        {
            cout << "Error opening sales report file." << endl;
            return;
        }

        salesReport << "================ Sales Report ================" << endl;
        salesReport << "User: " << user.getUsername() << endl;
        salesReport << "Address: " << user.getAddress() << endl;

        for (int i = 0; i < itemCount; ++i)
        {
            salesReport << cartItems[i] << ": " << cartPrices[i] << " RM x" << cartQuantities[i] << endl;
        }

        salesReport << "Total Price: RM" << calculateTotalPrice() << endl;
        salesReport << "=============================================" << endl;

        salesReport.close();
        // clearCart();
    } // end of updateSalesCount

    void displaySalesReport()
    {
        ifstream salesReportFile("sales_report.txt");

        if (!salesReportFile.is_open())
        {
            cout << "Error opening sales report file." << endl;
            system("pause");
            system("cls");
            return;
        }

        string line;
        while (getline(salesReportFile, line))
        {
            cout << line << endl;
        }

        salesReportFile.close();
    } // end of displaySalesReport

    void displayCart(const User &user)
    {
        float total = 0.0;
        for (int i = 0; i < itemCount; ++i)
        {
            cout << cartItems[i] << " x" << cartQuantities[i] << " RM" << cartPrices[i] << endl;
            total += (cartPrices[i] * cartQuantities[i]);
        }
        cout << "Total Price: RM" << total << endl;
        cout << "Do you want to proceed to your payment? [Y/N]: ";
        cin >> payOption;

        if (payOption == 'Y' || payOption == 'y')
        {
            processPayment(user);
            updateSalesCount(user);
            updateOrderHistory(); // Add this line to update order history
            clearCart();
        }
        else if (payOption == 'N' || payOption == 'n')
        {
            system("cls");
            return;
        }
    } // end of displayCart

    void processPayment(const User &user)
    {
        string cardHolderName, cardNumber, expirationDate, cvv;
        cin.ignore(); // Clear the newline character from the buffer

        system("cls");
        cout << "\n=================== Enter payment details ===================" << endl;
        cout << "Cardholder Name: ";
        getline(cin, cardHolderName);

        do
        {
            cout << "Card Number (16 digits): ";
            getline(cin, cardNumber);
        } while (!isValidCardNumber(cardNumber));

        do
        {
            cout << "Expiration Date (MM/YY): ";
            getline(cin, expirationDate);
        } while (!isValidExpirationDate(expirationDate));

        do
        {
            cout << "CVV (3 digits): ";
            getline(cin, cvv);
        } while (!isValidCVV(cvv));

        // Process payment logic (you can implement your own payment logic here)
        cout << "\nProcessing payment..." << endl;
        cout << "Payment successful. Thank you for your purchase!" << endl;
        system("pause");
        system("cls");
    }

    // Accessor methods
    int getItemCount() const
    {
        return itemCount;
    }

    string getCartItem(int index) const
    {
        return cartItems[index];
    }

    float getCartPrice(int index) const
    {
        return cartPrices[index];
    }

    friend class Receipt;
}; // end of class Cart

struct MenuItem
{
    string name;
    float price;

    MenuItem(string name, float price) : name(name), price(price) {}
};

class Restaurant
{
private:
    float price, itemPrice;
    string itemName;
    Cart C;

public:
    void readFile(Cart &cart)
    {
        string menuFileName = "Menu.txt";
        ifstream menuFile(menuFileName.c_str(), ios::in);
        if (!menuFile)
        {
            cout << "File does not exist";
            exit(1);
        }

        system("cls");
        cout << "Food and Beverage" << endl;

        vector<MenuItem> menuItems;
        string name;
        float price;

        while (menuFile >> name >> price)
        {
            menuItems.push_back(MenuItem(name, price));
        }

        for (size_t i = 0; i < menuItems.size(); i++)
        {
            cout << i + 1 << ") " << setw(30) << left << menuItems[i].name << " RM" << fixed << setprecision(2) << menuItems[i].price << endl;
        }

        int choice, quantity;
        float totalPrice = 0.0;

        while (true)
        {
            cout << "\nSelect a food item by entering its number: ";
            cin >> choice;

            if (choice >= 1 && choice <= menuItems.size())
            {
                cout << "You selected: " << choice << ") " << menuItems[choice - 1].name << " RM" << menuItems[choice - 1].price << endl;

                cout << "\nEnter the quantity: ";
                cin >> quantity;

                cart.setItemDetails(menuItems[choice - 1].name, menuItems[choice - 1].price, quantity);
                totalPrice += menuItems[choice - 1].price * quantity;
                cout << "Current order total: RM" << totalPrice << endl;

                char foodChoice;
                cout << "Would you like to make another order? [Y/N]: ";
                cin >> foodChoice;
                if (!(foodChoice == 'Y' || foodChoice == 'y'))
                {
                    cout << "Your current order price is RM" << totalPrice << endl;
                    cout << "\n\nPlease go to [my cart] for payment.\n"
                         << endl;
                    system("pause");
                    system("cls");
                    return; // Exit the function if the user doesn't want to make another order
                }
            }
            else
            {
                cout << "Invalid choice. Please try again.\n";
            }
        }
        menuFile.close();
    }

    void setItemDetails(string itemName, float itemPrice, int quantity)
    {
        this->itemName = itemName;
        this->itemPrice = itemPrice * quantity;
    }
};

class Receipt
{
public:
    Receipt() {}
    void generateReceipt(const User &user, const Cart &cart)
    {
        system("cls");
        cout << "========== RECEIPT ==========" << endl;
        cout << "Name \t\t: " << user.getUsername() << endl;
        cout << "Address \t: " << user.getAddress() << endl;

        float total = 0.0;
        cout << "\nOrder History:" << endl;
        for (const auto &order : cart.getOrderHistory())
        {
            float itemTotal = order.itemPrice * order.quantity;
            cout << order.itemName << " x" << order.quantity << " = RM" << itemTotal << endl;
            total += itemTotal;
        }
        cout << "\nTotal Price: RM" << total << endl;
        cout << "=============================" << endl;
        system("pause");
        system("cls");
    }
};

class Admin
{
private:
    string adminId;
    string adminPass;

public:
    Admin() : adminId("admin"), adminPass("admin123") {}
    // friend class welcomePage;

    void adminlogin()
    {
        int count = 0;
        string inputId, inputPass;

        system("cls");
        cout << "---------------------------------------------------------" << endl;
        cout << "\t>>>>>>>>>> Administrator Login <<<<<<<<<<" << endl;
        cout << "---------------------------------------------------------" << endl;
        cout << "Admin ID \t: ";
        getline(cin, inputId);
        cout << "Password \t: ";
        getline(cin, inputPass);

        if (inputId == adminId && inputPass == adminPass)
        {
            system("cls"); // clearing the screen
            cout << "<ADMIN LOGIN SUCCESSFUL>\n";
            cout << "Welcome, Admin " << endl;
            system("pause"); // pause the screen
        }
        else
        {
            do
            {
                system("cls");
                cout << "Incorrect login details. Please try again." << endl; // if the user enter wrong password, it will loop again
                cout << "\n---------------------------------------------------------" << endl;
                cout << "\t>>>>>>>>>> Administrator Login <<<<<<<<<<" << endl;
                cout << "---------------------------------------------------------" << endl;
                cout << "\nAdmin ID \t: ";
                getline(cin, inputId);
                cout << "Password \t: ";
                getline(cin, inputPass);

            } while (inputId != adminId || inputPass != adminPass); // end of do-while loop
        }                                                           // end of if else

    } // end of void adminlogin
};

// class Adminpage
// this class is for admin to manage the menu :D
class Adminpage : public Admin
{
private:
    int choice;
    int back;

protected:
public:
    // friend class Restaurant;
    void displayAdmin()
    {
        Receipt Rp;
        User user;
        Cart C;
        Restaurant R;
        Admin admin;
        Adminpage ap;

        system("cls");
        cout << "---------------------------------------------------------" << endl;
        cout << "\t>>>>>>>>>> Admin Page <<<<<<<<<<" << endl;
        cout << "---------------------------------------------------------" << endl;
        cout << "1. Manage Menu" << endl;
        cout << "2. Manage User" << endl;
        cout << "3. Manage Order" << endl;
        cout << "4. Exit" << endl;
        cout << "\nEnter your choice : ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            manageMenu();
            break;
        case 2:
            manageUser();
            break;
        case 3:
            system("cls");
            C.displaySalesReport();
            returntopage();
            break;
        case 4:
            cout << "Exiting, Returning to Main Menu" << endl;
            // welcomePage(user, admin, ap, R, C);
            // returntoWelcomePage();
            // back = 2;
            system("pause");
            system("cls");
            return;
            break;
        }
    }

    void manageMenu()
    {

        string itemName, line;
        float itemPrice;
        int pick;
        char searchAgain;
        string a[MAX_ITEM];
        float b[MAX_ITEM];
        int itemNumber = 0;

        system("cls"); // clearing the screen
        cout << "Enter [1] to add item to menu OR [2] to remove item from menu OR [3] to search for item from menu : ";
        cin >> pick;

        if (pick == 1)
        {
            fflush(stdin); // Clear the newline character from the buffer
            cout << "Enter the name of the new item : ";
            getline(cin, itemName);

            cout << "Enter the price of the new item : RM ";
            cin >> itemPrice;

            fstream menuFile("Menu.txt", ios::in | ios::out | ios::app); // Open the file in append mode
            if (menuFile.is_open())
            {
                menuFile << itemName << " " << fixed << setprecision(2) << itemPrice << endl; // adds item into a new line
                cout << "Item added successfully.\n\n";
                menuFile.close(); // Close the file after appending

                // Reopen the file in read mode to display the updated menu
                menuFile.open("Menu.txt", ios::in);

                if (menuFile.is_open())
                {
                    menuFile.seekg(0, ios::beg); // Move the file pointer to the beginning of the file (to read the file

                    cout << "Updated Menu" << endl;
                    while (menuFile >> a[itemNumber] >> b[itemNumber])
                    {
                        itemNumber++;
                    }

                    for (int i = 0; i < itemNumber; i++)
                    {
                        cout << i + 1 << ") " << setw(30) << left << a[i] << " RM" << fixed << setprecision(2) << b[i] << endl;
                    }
                } // end if

                menuFile.close(); // Close the file after reading
            }
            else
            {
                cout << "Error opening Menu.txt for writing.\n";
            }

        } // end of if pick == 1
        else if (pick == 2)
        {
            cin.ignore(); // Clear the newline character from the buffer
            cout << "Enter the name of the item to delete \t: ";
            getline(cin, itemName);

            ifstream inFile("Menu.txt");
            ofstream outFile("temp.txt");

            bool found = false;

            while (getline(inFile, line))
            {
                if (line.find(itemName) != string::npos)
                {
                    // Ask user whether to delete this item
                    cout << "Found item : " << line << endl;
                    cout << "Do you want to delete this item? (Y/N) : ";
                    char choice;
                    cin >> choice;

                    if (toupper(choice) == 'Y')
                    {
                        found = true;
                        continue; // Skip this line (delete the item)
                    }
                }
                outFile << line << endl;
            }

            inFile.close();
            outFile.close();

            if (found)
            {
                remove("Menu.txt");
                rename("temp.txt", "Menu.txt");
                cout << "Item deleted successfully." << endl;
            }
            else
            {
                cout << "Item not found in the menu." << endl;
                remove("temp.txt");
            }

            // Reopen the file in read mode to display the updated menu
            fstream menuFile("Menu.txt", ios::in);

            if (menuFile.is_open())
            {
                menuFile.seekg(0, ios::beg); // Move the file pointer to the beginning of the file (to read the file

                cout << "\nUpdated Menu" << endl;
                while (menuFile >> a[itemNumber] >> b[itemNumber])
                {
                    itemNumber++;
                }

                for (int i = 0; i < itemNumber; i++)
                {
                    cout << i + 1 << ") " << setw(30) << left << a[i] << " RM" << fixed << setprecision(2) << b[i] << endl;
                }
            } // end if

        } // end of else if pick == 2
        else if (pick == 3)
        {
            do
            {
                cin.ignore(); // Clear the newline character from the buffer
                cout << "Enter the name of the item to search \t: ";
                getline(cin, itemName);
                searchItem(itemName);

                cout << "Do you want to search again? [Y/N] \t: ";
                cin >> searchAgain;

            } while (toupper(searchAgain) == 'Y'); // end of do while

        } // end of else if pick == 3
        else
        {
            cout << "Invalid choice. Please try again.\n";
        } // end of else

        returntopage();

    } // end of manageMenu

    void manageUser()
    {
        string userName;
        cout << "Enter the username of the user to remove: ";
        cin.ignore(); // Clear the newline character from the buffer
        getline(cin, userName);

        ifstream inFile("user records.txt");
        ofstream outFile("temp_users.txt");

        string line;
        bool found = false;

        while (getline(inFile, line))
        {
            stringstream ss(line);
            string existingUsername;
            ss >> existingUsername;

            if (existingUsername == userName)
            {
                // Skip the line to delete the user
                found = true;
                continue;
            }
            outFile << line << endl;
        }

        // closes files
        inFile.close();
        outFile.close();

        if (found)
        {
            remove("user records.txt");
            rename("temp_users.txt", "user records.txt");
            cout << "User removed successfully.\n";
        }
        else
        {
            cout << "User not found.\n";
            remove("temp_users.txt");
        }

        returntopage();
    }

    // Function to convert a string to lowercase
    string toLowercase(const string &str)
    {
        string result = str;
        transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }

    bool searchItem(const string &itemName)
    {
        string line;

        ifstream menuFile("Menu.txt");
        if (!menuFile.is_open())
        {
            cout << "Error opening Menu.txt for reading.\n";
            return false;
        }

        string itemNameLower = toLowercase(itemName);
        bool found = false;

        while (getline(menuFile, line))
        {
            string lineLower = toLowercase(line);

            if (lineLower.find(itemNameLower) != string::npos)
            {
                cout << "Item found in the menu: " << line << endl;
                found = true;
            }
        }

        if (!found)
        {
            cout << "Item not found in the menu.\n";
        }

        // Close files
        menuFile.close();
        return found;
    }

    void returntopage()
    {
        system("pause");
        system("cls");
        cout << "Please press [1] to return to Admin Page OR [2] to return to Welcome Page : ";
        cin >> back;

        if (back == 1)
        {
            displayAdmin();
        }
        else if (back == 2)
        {
            return;
        }
        else
        {
            cout << "Invalid choice. Please try again.\n";
        }
    }

}; // end of class Adminpage

void showMenuOptions(User &user, Admin &admin, Adminpage &ap, Restaurant &R, Cart &C)
{
    int choice;
    Receipt receipt;
    do
    {
        cout << "\n---------------------------------------------------------" << endl;
        cout << "\tWelcome to Restaurant Fusion Fare Delight!" << endl;
        cout << "---------------------------------------------------------" << endl;
        cout << "1. Menu" << endl;
        cout << "2. My Cart" << endl;
        cout << "3. Delete item from [My Cart]" << endl;
        cout << "4. Logout" << endl;
        cout << "5. EXIT" << endl;
        cout << "\nEnter your choice :";
        cin >> choice;
        cout << endl;

        cin.ignore();

        switch (choice)
        {
        case 1:
            R.readFile(C);
            break;
        case 2:
            C.displayCart(user); // Pass the User object to displayCart
            receipt.generateReceipt(user, C);
            break;
        case 3:
            C.removeItem();
            break;
        case 4:
            // Logout Logic
            system("cls");
            user.logout();
            break;
        case 5:
            cout << "Thank you for visiting Restaurant Fusion Fare Delights." << endl;
            exit(0);

        default:
            system("cls");
            cout << "Invalid choice. Please select again.\n"
                 << endl;
        }
    } while (choice != 5 && user.isLoggedIn());
}

void welcomePage(User &user, Admin &admin, Adminpage &ap, Restaurant &R, Cart &C)
{
    int choice;
    do
    {
        if (!user.isLoggedIn())
        {
            cout << "\n---------------------------------------------------------" << endl;
            cout << "\tWelcome to Restaurant Fusion Fare Delight!" << endl;
            cout << "---------------------------------------------------------" << endl;
            cout << "1. USER LOGIN" << endl;
            cout << "2. USER REGISTER" << endl;
            cout << "3. FORGET PASSWORD" << endl;
            cout << "4. ADMINISTRATOR LOGIN" << endl;
            cout << "5. EXIT" << endl;
            cout << "\nEnter your choice :";
            cin >> choice;
            cin.ignore(); // To consume the '\n' character after cin >>

            switch (choice)
            {
            case 1:
                user.login();
                if (user.isLoggedIn())
                {
                    showMenuOptions(user, admin, ap, R, C);
                }
                break;
            case 2:
                user.registration();
                break;
            case 3:
                user.forgetPassword();
                break;
            case 4:
                admin.adminlogin();
                ap.displayAdmin();
                break;
            case 5:
                cout << "Thank you for visiting Restaurant Fusion Fare Delights." << endl;
                exit(0);

            default:
                cout << "\nPlease select a valid option\n"
                     << endl;
            } // end of switch
        }     // end of if
        else
        {
            showMenuOptions(user, admin, ap, R, C);
        } // end of else

    } while (choice != 5); // end of do
} // end of welcome

int main()
{
    int choice;
    string selectedItem;
    float selectedPrice;
    User user;
    Admin admin;
    Adminpage ap;
    Receipt Rp;
    Restaurant R;
    Cart C;

    bool userAuthenticated = false;

    welcomePage(user, admin, ap, R, C);

    return 0;
}