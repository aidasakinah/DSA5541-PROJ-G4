#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <cmath>
#include <cctype>
#include <sstream>
#include <vector>

using namespace std;
const int MAX_ITEM = 100;
int currentSortState = 0; // 0: unsorted, 1: price low to high, 2: price high to low, 3: alphabetical
class Restaurant;  // Forward declaration
class Cart;
// Cart cart;
struct MenuItem
{
    string name;
    float price;
    string category;
    MenuItem() : name(""), price(0.0), category("") {}
    MenuItem(string n, float p, string c) : name(n), price(p), category(c) {}
};

struct Node
{
    MenuItem data;
    Node *next;

    Node() : next(nullptr) {}
    Node(MenuItem item) : data(item), next(nullptr) {}
};

void insertMenuItem(Node *&head, MenuItem item)
{
    Node *newNode = new Node(item);
    if (!head)
    {
        head = newNode;
        return;
    }
    Node *curr = head;
    while (curr->next)
    {
        curr = curr->next;
    }
    curr->next = newNode;
}

void deleteMenuItem(Node *&head, const string &name)
{
    if (!head)
    {
        cout << "Menu is empty. Nothing to delete." << endl;
        return;
    }

    Node *curr = head;
    Node *prev = nullptr;

    // Handle the case where the head node needs to be deleted
    if (curr->data.name == name)
    {
        head = curr->next;
        delete curr;
        cout << "Item \"" << name << "\" deleted successfully." << endl;
        return;
    }

    // Traverse the linked list to find the node to delete
    while (curr != nullptr && curr->data.name != name)
    {
        prev = curr;
        curr = curr->next;
    }

    // If the item is not found
    if (curr == nullptr)
    {
        cout << "Item \"" << name << "\" not found in the menu." << endl;
        return;
    }

    // Delete the node
    prev->next = curr->next;
    delete curr;
    cout << "Item \"" << name << "\" deleted successfully." << endl;
}



void countSort(Node *&head, int pos){
    Node *temp = head;
    Node *nodeList[MAX_ITEM];
    int count = 0;

    // Store all nodes in an array
    while (temp)
    {
        nodeList[count++] = temp;
        temp = temp->next;
    }

    // Sort the array based on the character at the current position
    sort(nodeList, nodeList + count, [pos](Node *a, Node *b)
         {
        if (pos >= a->data.name.length() && pos >= b->data.name.length()) {
            return false;
        } else if (pos >= a->data.name.length()) {
            return false;
        } else if (pos >= b->data.name.length()) {
            return true;
        } else {
            return a->data.name[pos] < b->data.name[pos];
        } });

    // Rebuild the linked list from the sorted array
    Node *newHead = nullptr;
    Node *tail = nullptr;
    for (int i = 0; i < count; i++)
    {
        nodeList[i]->next = nullptr;
        if (newHead == nullptr)
        {
            newHead = nodeList[i];
        }
        else
        {
            tail->next = nodeList[i];
        }
        tail = nodeList[i];
    }

    head = newHead;
}

// alphabetically ascending
void radixSort(Node *&head)
{
    int maxLen = 0;
    Node *temp = head;
    while (temp)
    {
        maxLen = max(maxLen, static_cast<int>(temp->data.name.length()));
        temp = temp->next;
    }

    for (int pos = maxLen - 1; pos >= 0; pos--)
    {
        countSort(head, pos);
    }
}


void countSortDescending(Node *&head, int pos){
    Node *temp = head;
    Node *nodeList[MAX_ITEM];
    int count = 0;

    // Store all nodes in an array
    while (temp) {
        nodeList[count++] = temp;
        temp = temp->next;
    }

    // Sort the array based on the character at the current position in descending order
    sort(nodeList, nodeList + count, [pos](Node *a, Node *b) {
        if (pos >= a->data.name.length() && pos >= b->data.name.length()) {
            return false;
        }
        else if (pos >= a->data.name.length()) {
            return true;
        }
        else if (pos >= b->data.name.length()) {
            return false;
        }
        else {
            return a->data.name[pos] > b->data.name[pos];
        }
    });

    // Rebuild the linked list from the sorted array
    Node *newHead = nullptr;
    Node *tail = nullptr;
    for (int i = 0; i < count; i++) {
        nodeList[i]->next = nullptr;
        if (newHead == nullptr) {
            newHead = nodeList[i];
        }
        else {
            tail->next = nodeList[i];
        }
        tail = nodeList[i];
    }

    head = newHead;
}

void radixSortDescending(Node *&head) {
    int maxLen = 0;
    Node *temp = head;
    while (temp) {
        maxLen = max(maxLen, static_cast<int>(temp->data.name.length()));
        temp = temp->next;
    }

    for (int pos = maxLen - 1; pos >= 0; pos--) {
        countSortDescending(head, pos);
    }
}

// Custom sorting algorithm: Find minimum and swap
void customSort(Node *&head){
    if (!head)
        return;

    Node *current = head;
    while (current)
    {
        Node *minNode = current;
        Node *temp = current->next;
        while (temp)
        {
            if (temp->data.price < minNode->data.price)
            {
                minNode = temp;
            }
            temp = temp->next;
        }
        if (minNode != current)
        {
            swap(current->data, minNode->data);
        }
        current = current->next;
    }
}

// Bucket sort in ascending order
void bucketSortAscending(Node *&head){
    if (!head)
    {
        cout << "The menu is empty. Nothing to sort." << endl;
        return;
    }

    // Find the maximum price in the menu
    float maxPrice = head->data.price;
    Node *temp = head->next;
    while (temp)
    {
        maxPrice = max(maxPrice, temp->data.price);
        temp = temp->next;
    }

    // Create buckets based on the maximum price
    int numBuckets = static_cast<int>(ceil(sqrt(maxPrice)));
    Node *buckets[numBuckets] = {nullptr};

    // Distribute the menu items into the buckets based on their price
    temp = head;
    while (temp)
    {
        int bucketIndex = static_cast<int>(floor(temp->data.price / maxPrice * (numBuckets - 1)));
        // Add item to the front of the bucket's linked list
        Node *newNode = new Node(temp->data);
        newNode->next = buckets[bucketIndex];
        buckets[bucketIndex] = newNode;
        temp = temp->next;
    }

    // Sort each bucket individually using custom sort
    for (int i = 0; i < numBuckets; ++i)
    {
        customSort(buckets[i]);
    }

    // Rebuild the linked list from the sorted buckets
    Node *newHead = nullptr;
    Node *tail = nullptr;
    for (int i = 0; i < numBuckets; ++i)
    {
        Node *current = buckets[i];
        while (current)
        {
            if (!newHead)
            {
                newHead = current;
                tail = current;
            }
            else
            {
                tail->next = current;
                tail = current;
            }
            current = current->next;
        }
    }

    // Set the tail's next pointer to nullptr to mark the end of the list
    if (tail)
        tail->next = nullptr;

    // Update the head pointer
    head = newHead;
}

// Custom sorting algorithm: Find maximum and swap
void customSortDescending(Node *&head){
    if (!head)
        return;

    Node *current = head;
    while (current)
    {
        Node *maxNode = current;
        Node *temp = current->next;
        while (temp)
        {
            if (temp->data.price > maxNode->data.price)
                maxNode = temp;
            temp = temp->next;
        }
        if (maxNode != current)
            swap(current->data, maxNode->data);
        current = current->next;
    }
}

// Bucket sort in descending order
void bucketSortDescending(Node *&head){
    if (!head)
    {
        cout << "The menu is empty. Nothing to sort." << endl;
        return;
    }

    // Find the maximum price in the menu
    float maxPrice = head->data.price;
    Node *temp = head->next;
    while (temp)
    {
        maxPrice = max(maxPrice, temp->data.price);
        temp = temp->next;
    }

    // Create array of linked lists for buckets
    int numBuckets = static_cast<int>(ceil(sqrt(maxPrice)));
    Node *buckets[numBuckets] = {nullptr};

    // Distribute the menu items into the buckets based on their price
    temp = head;
    while (temp)
    {
        int bucketIndex = static_cast<int>(floor(temp->data.price / maxPrice * (numBuckets - 1)));
        // Add item to the front of the bucket's linked list
        Node *newNode = new Node;
        newNode->data = temp->data;
        newNode->next = buckets[bucketIndex];
        buckets[bucketIndex] = newNode;
        temp = temp->next;
    }

    // Sort each bucket individually using custom sort in descending order
    for (int i = 0; i < numBuckets; ++i)
    {
        customSortDescending(buckets[i]);
    }

    // Rebuild the linked list from the sorted buckets in descending order
    Node *newHead = nullptr;
    Node *tail = nullptr;
    for (int i = numBuckets - 1; i >= 0; --i)
    {
        Node *current = buckets[i];
        while (current)
        {
            if (!newHead)
            {
                newHead = current;
                tail = current;
            }
            else
            {
                tail->next = current;
                tail = current;
            }
            current = current->next;
        }
    }

    // Set the tail's next pointer to nullptr to mark the end of the list
    tail->next = nullptr;

    // Update the head pointer
    head = newHead;
}

// to print menu from txt file
void printMenu(Node *head){
    if (head == nullptr) {
        cout << "The menu is empty." << endl;
        return;
    }

    cout << string(70, '-') << endl;
    cout << setw(35) << "Menu" << endl;
    cout << string(70, '-') << endl;
    cout << left << setw(5) << "No." << setw(30) << "Name" << setw(15) << "Price (RM)" << setw(20) << "Category" << endl;
    cout << string(70, '-') << endl;

    Node *temp = head;
    int itemNumber = 1;
    while (temp != nullptr) {
        cout << left << setw(5) << (to_string(itemNumber) + ")")
             << setw(30) << temp->data.name 
             << setw(15) << fixed << setprecision(2) << temp->data.price 
             << setw(20) << temp->data.category << endl;
        temp = temp->next;
        itemNumber++;
    }
}

// Utility function to convert a string to lowercase
string toLowerCase(const string &str){
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

// Ternary Search
void ternarySearch(Node *head, const string &key){
    if (!head)
        return;

    // Convert key to lowercase
    string lowerKey = toLowerCase(key);

    // Display unsorted search results
    cout << "Unsorted Search Results:" << endl;
    bool foundUnsorted = false;
    Node *temp = head;
    while (temp)
    {
        string itemName = toLowerCase(temp->data.name);
        if (itemName.find(lowerKey) != string::npos)
        {
            cout << temp->data.name << " - RM" << fixed << setprecision(2) << temp->data.price << " (" << temp->data.category << ")" << endl;
            foundUnsorted = true;
        }
        temp = temp->next;
    }

    if (!foundUnsorted)
    {
        cout << "Item not found in the menu." << endl;
        return;
    }

    // Sort the linked list in ascending order using bucket sort
    bucketSortAscending(head);

    // Display sorted search results in ascending order
    cout << "\nSorted Search Results by Price (Ascending):" << endl;
    bool foundSortedAscending = false;
    temp = head;
    while (temp)
    {
        string itemName = toLowerCase(temp->data.name);
        if (itemName.find(lowerKey) != string::npos)
        {
            cout << temp->data.name << " - RM" << fixed << setprecision(2) << temp->data.price << " (" << temp->data.category << ")" << endl;
            foundSortedAscending = true;
        }
        temp = temp->next;
    }

    if (!foundSortedAscending)
    {
        cout << "Item not found in the menu." << endl;
        return;
    }

    // Sort the linked list in descending order using bucket sort
    bucketSortDescending(head);

    // Display sorted search results in descending order
    cout << "\nSorted Search Results by Price (Descending):" << endl;
    bool foundSortedDescending = false;
    temp = head;
    while (temp)
    {
        string itemName = toLowerCase(temp->data.name);
        if (itemName.find(lowerKey) != string::npos)
        {
            cout << temp->data.name << " - RM" << fixed << setprecision(2) << temp->data.price << " (" << temp->data.category << ")" << endl;
            foundSortedDescending = true;
        }
        temp = temp->next;
    }

    if (!foundSortedDescending)
    {
        cout << "Item not found in the menu." << endl;
    }
}

// Jump Search
void jumpSearch(Node *head, const string &key){
    if (!head)
        return;

    // Convert key to lowercase
    string lowerKey = toLowerCase(key);

    // Create a dummy array to hold the nodes
    Node *dummy[MAX_ITEM];
    int count = 0;

    Node *temp = head;
    while (temp)
    {
        dummy[count++] = temp;
        temp = temp->next;
    }

    // Display unsorted search results
    cout << "Unsorted Search Results:" << endl;
    bool found = false;
    for (int i = 0; i < count; ++i)
    {
        string itemName = toLowerCase(dummy[i]->data.name);
        if (itemName.find(lowerKey) != string::npos)
        {
            cout << dummy[i]->data.name << " - RM" << fixed << setprecision(2) << dummy[i]->data.price << " (" << dummy[i]->data.category << ")" << endl;
            found = true;
        }
    }

    if (!found)
    {
        cout << "Item not found in the menu." << endl;
        return;
    }

    // Sort the linked list using radix sort in ascending order
    radixSort(head);

    // Display sorted search results in ascending order
    cout << "\nSorted Search Results by Name (Ascending):" << endl;
    temp = head;
    found = false;
    while (temp)
    {
        string itemName = toLowerCase(temp->data.name);
        if (itemName.find(lowerKey) != string::npos)
        {
            cout << temp->data.name << " - RM" << fixed << setprecision(2) << temp->data.price << " (" << temp->data.category << ")" << endl;
            found = true;
        }
        temp = temp->next;
    }

    if (!found)
    {
        cout << "Item not found in the menu." << endl;
    }

    // Sort the linked list using radix sort in descending order
    radixSortDescending(head);

    // Display sorted search results in descending order
    cout << "\nSorted Search Results by Name (Descending):" << endl;
    temp = head;
    found = false;
    while (temp)
    {
        string itemName = toLowerCase(temp->data.name);
        if (itemName.find(lowerKey) != string::npos)
        {
            cout << temp->data.name << " - RM" << fixed << setprecision(2) << temp->data.price << " (" << temp->data.category << ")" << endl;
            found = true;
        }
        temp = temp->next;
    }

    if (!found)
    {
        cout << "Item not found in the menu." << endl;
    }
}

void searchResults(Node *head, const string &itemName, int searchType){
    if (searchType == 1)
    {
        cout << "----------------------------------------" << endl;
        cout << "         Ternary Search Results         " << endl;
        cout << "----------------------------------------" << endl;
        ternarySearch(head, itemName);
    }
    else if (searchType == 2)
    {
        cout << "----------------------------------------" << endl;
        cout << "           Jump Search Results          " << endl;
        cout << "----------------------------------------" << endl;
        jumpSearch(head, itemName);
    }
}

// Overloaded function to search by price range
void searchResults(Node *head, float minPrice, float maxPrice) {
    cout << "----------------------------------------" << endl;
    cout << "      Price Range Search Results        " << endl;
    cout << "----------------------------------------" << endl;

    Node *temp = head;
    bool found = false;
    while (temp) {
        if (temp->data.price >= minPrice && temp->data.price <= maxPrice) {
            cout << temp->data.name << " - RM" << fixed << setprecision(2) << temp->data.price << " (" << temp->data.category << ")" << endl;
            found = true;
        }
        temp = temp->next;
    }

    if (!found) {
        cout << "No items found in the specified price range." << endl;
    }
}

void goBackToMenu(Node *&head, Cart &cart, Restaurant &R);
void showMenuOptions(Node *&head, Cart &cart, Restaurant &R);

void addMenuItem(Node *&head, Cart &cart, Restaurant &R){
    string itemName;

    system("cls");
    fflush(stdin);
    cout << "----------------------------------------" << endl;
    cout << "\tEntering new item" << endl;
    cout << "----------------------------------------" << endl;

    cout << "Enter the name of the new item (no spaces) : ";
    getline(cin, itemName);

    float itemPrice;
    cout << "Enter the price of the new item : RM ";
    cin >> itemPrice;

    string itemCategory;
    cout << "Enter the category of the new item (FOOD/BEVERAGE): ";
    cin.ignore();
    getline(cin, itemCategory);
    transform(itemCategory.begin(), itemCategory.end(), itemCategory.begin(), ::toupper);

    fstream menuFile("Menu.txt", ios::in | ios::out | ios::app);
    if (menuFile.is_open())
    {
        menuFile << itemName << " " << fixed << setprecision(2) << itemPrice << " " << itemCategory << endl;
        cout << "Item added successfully.\n\n";
        menuFile.close();

        // Update the linked list with the new item
        insertMenuItem(head, MenuItem(itemName, itemPrice, itemCategory));

        // Display the updated menu
        printMenu(head);
        goBackToMenu(head,cart,R);
    }
    else
    {
        cout << "Error opening Menu.txt for writing.\n";
    }
}

// menu sorting algorithm
// finished
void algorithmSortMenu(Node *&head, Cart &cart, Restaurant &R){
    int sortalgo, radixChoice, bucketChoice;
    system("cls");
    cout << "----------------------------------------" << endl;
    cout << "            Sorting Algorithms          " << endl;
    cout << "----------------------------------------" << endl;
    cout << "1. Alphabetical Sort" << endl;
    cout << "2. Price Sort" << endl;
    cout << "3. Return to homepage" << endl;
    cout << "\nEnter your choice: ";
    cin >> sortalgo;

    if (sortalgo == 1)
    {
        system("cls");
        cout << "----------------------------------------" << endl;
        cout << "               Radix Sort               " << endl;
        cout << "----------------------------------------" << endl;
        cout << "1. Alphabetically (A-Z)" << endl;
        cout << "2. Alphabetically (Z-A)" << endl;
        cout << "3. Return to previous page" << endl;
        cout << "\nEnter your choice: ";
        cin >> radixChoice;

        if (radixChoice == 1)
        {
            system("cls");
            radixSort(head);
            printMenu(head);
            goBackToMenu(head,cart,R);
            algorithmSortMenu(head,cart,R);
        }
        else if (radixChoice == 2)
        {
            system("cls");
            radixSortDescending(head);
            printMenu(head);
            goBackToMenu(head,cart,R);
        }
        else if (radixChoice == 3)
        {
            
        }
    }
    else if (sortalgo == 2)
    {
        system("cls");
        cout << "----------------------------------------" << endl;
        cout << "               Bucket Sort              " << endl;
        cout << "----------------------------------------" << endl;
        cout << "1. Price Range (Lowest-Highest)" << endl;
        cout << "2. Price Range (Highest to Lowest)" << endl;
        cout << "3. Return to previous page" << endl;
        cout << "\nEnter your choice: ";
        cin >> bucketChoice;

        if (bucketChoice == 1)
        {
            system("cls");
            bucketSortAscending(head);
            printMenu(head);
            goBackToMenu(head,cart,R);
        }
        else if (bucketChoice == 2)
        {
            system("cls");
            bucketSortDescending(head);
            printMenu(head);
            goBackToMenu(head,cart,R);
        }
        else if (bucketChoice == 3)
        {
            algorithmSortMenu(head,cart,R);
        }
    }
    else if (sortalgo == 3)
    {
        system("cls");
        printMenu(head);
        goBackToMenu(head,cart,R);
    }
    else if (sortalgo == 4)
    {
        showMenuOptions(head, cart, R);
    }
    else
    {
        cout << "Invalid choice. Please select again." << endl;
        algorithmSortMenu(head,cart,R);
    }
}

// menu search algorithm
void algorithmSearchMenu(Node *&head, Cart &cart, Restaurant &R){
    int searchType;
    system("cls");
    cout << "----------------------------------------" << endl;
    cout << "            Search Algorithms           " << endl;
    cout << "----------------------------------------" << endl;
    cout << "1. Ternary Search" << endl;
    cout << "2. Jump Search" << endl;
    cout << "3. Price Range Search" << endl;
    cout << "4. Return to homepage" << endl;
    cout << "\nEnter your choice: ";
    cin >> searchType;

    if (searchType == 1)
    {
        system("cls");
        cout << "Enter the name of the item to search: ";
        string itemName;
        cin.ignore();
        getline(cin, itemName);
        searchResults(head, itemName, searchType);
        goBackToMenu(head,cart,R);
    }
    else if (searchType == 2)
    {
        system("cls");
        cout << "Enter the name of the item to search: ";
        string itemName;
        cin.ignore();
        getline(cin, itemName);
        searchResults(head, itemName, searchType);
        goBackToMenu(head,cart,R);
    }
    else if (searchType == 3) {
        system("cls");
        float minPrice, maxPrice;
        cout << "Enter the minimum price: RM ";
        cin >> minPrice;
        cout << "Enter the maximum price: RM ";
        cin >> maxPrice;
        searchResults(head, minPrice, maxPrice);
        goBackToMenu(head,cart,R);
    }
    else if (searchType == 4)
    {
        showMenuOptions(head,cart, R);
    }
    else
    {
        cout << "Invalid choice. Please select again." << endl;
        algorithmSearchMenu(head,cart,R);
    }
}

void printSortedMenu(Node *head, int displayChoice) {
    if (head == nullptr) {
        cout << "The menu is empty." << endl;
        return;
    }

    cout << string(70, '-') << endl;
    cout << setw(35) << "Menu";
    if (displayChoice == 1) {
        cout << " (Sorted by Price: Low to High)";
    } else if (displayChoice == 2) {
        cout << " (Sorted by Price: High to Low)";
    } else if (displayChoice == 3) {
        cout << " (Sorted Alphabetically: A-Z)";
    } else if (displayChoice == 4) {
        cout << " (Sorted Alphabetically: Z-A)";
    }
    cout << endl;
    cout << string(70, '-') << endl;
    cout << left << setw(5) << "No." << setw(30) << "Name" << setw(15) << "Price (RM)" << setw(20) << "Category" << endl;
    cout << string(70, '-') << endl;

    Node *temp = head;
    int itemNumber = 1;
    while (temp != nullptr) {
        cout << left << setw(5) << (to_string(itemNumber) + ")")
             << setw(30) << temp->data.name 
             << setw(15) << fixed << setprecision(2) << temp->data.price 
             << setw(20) << temp->data.category << endl;
        temp = temp->next;
        itemNumber++;
    }
}

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
class Cart : public PaymentMethod{
public:
    vector<Node*> items;
    float totalCost = 0.0f;

    
void addToCart(Node* head, bool isSorted) {
    cout << "\nSelect a food item by entering its number: ";
    int itemNumber;
    cin >> itemNumber;
    Node* temp = head;
    int counter = 1;
    while (temp != nullptr) {
        if (counter == itemNumber) {
            cout << "You selected: " << counter << ") " << temp->data.name << " RM" << fixed << setprecision(2) << temp->data.price << endl;
            cout << "Enter the quantity: ";
            int quantity;
            cin >> quantity;

            // Create a new Node for the cart
            Node* cartItem = new Node(temp->data);
            float originalPrice = cartItem->data.price;
            cartItem->data.price *= quantity; // Adjust price for quantity

            // Check if the item is already in the cart
            bool itemFound = false;
            for (auto& item : items) {
                if (item->data.name == cartItem->data.name) {
                    // Update existing item
                    int existingQuantity = item->data.price / originalPrice;
                    int newQuantity = existingQuantity + quantity;
                    item->data.price = originalPrice * newQuantity;
                    delete cartItem; // Delete the newly created node as we don't need it
                    itemFound = true;
                    break;
                }
            }

            if (!itemFound) {
                items.push_back(cartItem);
            }

            totalCost += originalPrice * quantity;
            cout << "Item added to cart. Current order total: RM" << fixed << setprecision(2) << totalCost << endl;

            char choice;
            cout << "Would you like to add another item? [Y/N]: ";
            cin >> choice;
            if (toupper(choice) == 'Y') {
                addToCart(head, isSorted); // Recursive call to add another item
            }
            return;
        }
        temp = temp->next;
        counter++;
    }
    cout << "Invalid item number." << endl;
}

 void displayCart() {
        if (items.empty()) {
            cout << "Your cart is empty." << endl;
        } else {
            cout << "My Cart:" << endl;
            cout << string(50, '-') << endl;
            cout << left << setw(5) << "No." 
                 << setw(20) << "Name" 
                 << setw(10) << "Quantity" 
                 << setw(15) << "Price (RM)" << endl;
            cout << string(50, '-') << endl;

            for (size_t i = 0; i < items.size(); ++i) {
                float originalPrice = items[i]->data.price / (items[i]->data.price / items[i]->data.price);
                int quantity = items[i]->data.price / originalPrice;
                cout << left << setw(5) << (i + 1)
                     << setw(20) << items[i]->data.name 
                     << setw(10) << quantity
                     << setw(15) << fixed << setprecision(2) << items[i]->data.price << endl;
            }
            cout << string(50, '-') << endl;
            cout << "Total Cost: RM" << fixed << setprecision(2) << totalCost << endl;
        }
    }

    void removeFromCart() {
        if (items.empty()) {
            cout << "Your cart is empty. Nothing to remove." << endl;
            return;
        }

        displayCart();

        int itemNumber;
        cout << "\nEnter the number of the item you want to remove: ";
        cin >> itemNumber;

        if (itemNumber < 1 || itemNumber > items.size()) {
            cout << "Invalid item number. Please try again." << endl;
            return;
        }

        Node* removedItem = items[itemNumber - 1];
        float originalPrice = removedItem->data.price / (removedItem->data.price / removedItem->data.price);
        int quantity = removedItem->data.price / originalPrice;

        totalCost -= removedItem->data.price;
        
        delete removedItem;
        items.erase(items.begin() + itemNumber - 1);

        cout << "Item removed from cart. Updated total: RM" << fixed << setprecision(2) << totalCost << endl;
    }
};


class Restaurant { 
private:
    float price, itemPrice;
    string itemName;
	Cart& cart;
    Node* originalHead;
    Node* sortedHead;

public:

Restaurant(Cart& c) : cart(c), originalHead(nullptr), sortedHead(nullptr) {}

    void readFile(int displayChoice) {
        if (originalHead == nullptr) {
            // Clear the existing list if any
            while (originalHead != nullptr) {
                Node* temp = originalHead;
                originalHead = originalHead->next;
                delete temp;
            }

            string menuFileName = "Menu.txt";
            ifstream menuFile(menuFileName.c_str(), ios::in);
            if (!menuFile) {
                cout << "File does not exist";
                exit(1);
            }

            string name, category;
            float price;

            while (menuFile >> name >> price >> category) {
                insertMenuItem(originalHead, MenuItem(name, price, category));
            }

            menuFile.close();
        }

        createSortedList(displayChoice);

        system("cls");
        cout << "Food and Beverage" << endl;

        if (displayChoice == 0) {
            printMenu(originalHead);
        } else {
            printSortedMenu(sortedHead, displayChoice);
        }

        char addcartchoice;
        cout << "\nWould you like to add items to the cart? [Y/N]: ";
        cin >> addcartchoice;
         if (toupper(addcartchoice) == 'Y') {
        if (displayChoice == 0) {
            cart.addToCart(originalHead, false);
        } else {
            cart.addToCart(sortedHead, true);
        }
    }
}
        void createSortedList(int displayChoice) {
        // Delete previous sortedHead if it exists
        while (sortedHead != nullptr) {
            Node* temp = sortedHead;
            sortedHead = sortedHead->next;
            delete temp;
        }

        // Create a new list by copying data from originalHead
        Node* current = originalHead;
        while (current != nullptr) {
            insertMenuItem(sortedHead, current->data);
            current = current->next;
        }

        // Sort the new list based on displayChoice
        if (displayChoice == 1) {
            bucketSortAscending(sortedHead);
        } else if (displayChoice == 2) {
            bucketSortDescending(sortedHead);
        } else if (displayChoice == 3) {
            radixSort(sortedHead);
        } else if (displayChoice == 4) {
            radixSortDescending(sortedHead);
        }
    }
};//end of class restaurant

void showMenuOptions(Node *&head, Cart &cart, Restaurant &R){
	//Restaurant R;
	//Cart cart;
    int choice;
    string itemName, line, itemCategory, itemToDelete;
    float itemPrice;
    char addcartchoice;
    fstream menuFile;
    
    while (true) {
    system("cls");
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
    cin.ignore();
    cout << endl;

    Node *temp = nullptr;
    int i = 1;

    switch (choice)
    {
    case 1:
    int displayChoice;
    do {
        system("cls");
        cout << "----------------------------------------" << endl;
        cout << "              Menu Options         " << endl;
        cout << "----------------------------------------" << endl;
        cout << "0. View Unsorted Menu" << endl;
        cout << "1. Sort by Price (Low to High)" << endl;
        cout << "2. Sort by Price (High to Low)" << endl;
        cout << "3. Sort Alphabetically (A-Z)" << endl;
        cout << "4. Sort Alphabetically (Z-A)" << endl;
        cout << "5. Search Menu Items" << endl;
        cout << "6. Back to Main Menu" << endl;
        cout << "\nEnter your choice : ";
        cin >> displayChoice;

        if (displayChoice >= 0 && displayChoice <= 4) {
            R.readFile(displayChoice);
            cout << endl;
            cout << "Press Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        } else if (displayChoice == 5) {
        int searchChoice;
        cout << "----------------------------------------" << endl;
        cout << "              Search Options            " << endl;
        cout << "----------------------------------------" << endl;
        cout << "1. Search by Name" << endl;
        cout << "2. Search by Price Range" << endl;
        cout << "\nEnter your choice : ";
        cin >> searchChoice;

        if (searchChoice == 1) {
            string itemName;
            cout << "Enter the name of the item to search: ";
            cin.ignore();
            getline(cin, itemName);
            searchResults(head, itemName, 1); // Assuming 1 is for Ternary Search
        } else if (searchChoice == 2) {
            float minPrice, maxPrice;
            cout << "Enter the minimum price: RM ";
            cin >> minPrice;
            cout << "Enter the maximum price: RM ";
            cin >> maxPrice;
            searchResults(head, minPrice, maxPrice);
        } else {
            cout << "Invalid search option." << endl;
        }
        cout << "\nPress Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    } else if (displayChoice == 6) {
        break;
    } else {
        cout << endl << "Invalid choice. Please press enter to select again.";
        // Pause for a moment to let the user read the message
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }
} while (displayChoice != 6);
break;

case 2: // My Cart
    cart.displayCart();
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    break;

    case 3:
        cart.removeFromCart();
        cout << "\nPress Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        break;   
    

    case 4:
        cout << "Thank you for visiting Restaurant Fusion Fare Delights." << endl;
        exit(0);
        break;

    default:
        system("cls");
        cout << "Invalid choice. Please select again." << endl;
        showMenuOptions(head,cart,R);
        break;
    	} // end of switch
    } // end of while loop
     goBackToMenu(head, cart, R); // Pass the cart instance to goBackToMenu
} // end function

void goBackToMenu(Node *&head, Cart& cart, Restaurant &R) {
     // Create a static Cart object
    //static Cart cart;
    cout << "\nEnter 1 to go back to the homepage, 2 to Sorting Menu or 3 to Search Menu: ";
    int backChoice;
    cin >> backChoice;
    if (backChoice == 1)
    {
        showMenuOptions(head, cart, R);
    }
    else if (backChoice == 2)
    {
        algorithmSortMenu(head, cart, R);
    }
    else if (backChoice == 3)
    {
        algorithmSearchMenu(head, cart, R);
    }
    else
    {
        cout << "Invalid choice. Please select again." << endl;
        goBackToMenu(head, cart,R); // Recursive call with correct parameters
    }
}



struct UserRecord {
	
    string username;
    string email;
    string address;
    string contactNumber;
    string password;
    UserRecord* next; // Pointer to the next node in the linked list

	// containing all the user information stored in its member variables (means store in one node)
    UserRecord(const string& username, const string& email, const string& address, const string& contactNumber, const string& password)
        : username(username), email(email), address(address), contactNumber(contactNumber), password(password), next(nullptr) {}
};

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
    string trim(const string &str) {
        size_t first = str.find_first_not_of(' ');
        if (string::npos == first) {
            return "";
        }
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, (last - first + 1));
    }
    
    // Helper function to find a user record in the linked list
    UserRecord* findUserRecord(const string& usernameOrEmail) {
        UserRecord* current = userRecordHead;
        while (current != nullptr) {
            if (current->username == usernameOrEmail || current->email == usernameOrEmail) {
                return current;
            }
            current = current->next;
        }
        return nullptr; // User record not found
    }
    
        // Helper function to find a user record with a given username
    UserRecord* findUserRecordByUsername(const string& username) {
        UserRecord* current = userRecordHead;
        while (current != nullptr) {
            if (current->username == username) {
                return current;
            }
            current = current->next;
        }
        return nullptr; // User record not found
    }

    // Helper function to find a user record with a given email
    UserRecord* findUserRecordByEmail(const string& email) {
        UserRecord* current = userRecordHead;
        while (current != nullptr) {
            if (current->email == email) {
                return current;
            }
            current = current->next;
        }
        return nullptr; // User record not found
    }
    
//    Restaurant R;
//    Node* head = nullptr;
    //static void welcomePage(User& user, Restaurant& R, Node*& head);
	UserRecord* userRecordHead;
	
public:
	void initializeUserRecords() {
    readUserRecordsFromFile(); // Load user records from the file
}
     User() : loggedIn(false), userRecordHead(nullptr) { readUserRecordsFromFile();}

	 void addUserRecord(const string& username, const string& email, const string& address, const string& contactNumber, const string& password) {
     UserRecord* newRecord = new UserRecord(username, email, address, contactNumber, password);

        if (userRecordHead == nullptr) {
            userRecordHead = newRecord;
        } else {
            UserRecord* current = userRecordHead;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newRecord;
        }
    }

    void readUserRecordsFromFile() {
        ifstream file("user records.txt");
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string username, email, address, contactNumber, password, cpassword;
            ss >> username >> email;
            ss.ignore(1, '"');
            getline(ss, address, '"');
            ss >> contactNumber >> password >> cpassword;
            addUserRecord(username, email, address, contactNumber, password);
        }
        file.close();
    }
	
    void login() {
    system("cls");
    cout << "User Login" << endl;
    cout << "USERNAME or EMAIL  : ";
    getline(cin, inputUsername);
    cout << "PASSWORD           : ";
    getline(cin, inputPassword);

    UserRecord* userRecord = findUserRecord(trim(inputUsername));
    if (userRecord != nullptr && userRecord->password == inputPassword) {
        system("cls");
        username = userRecord->username;
        address = userRecord->address;
        cout << "\nHello " << username << "\n<LOGIN SUCCESSFUL>\nThanks for logging in Restaurant Fusion Fare Delights\n";
        loggedIn = true;
        //welcomePage(*this, R, head);
    } else {
        cout << "\nLOGIN ERROR\nPlease check again your username or email and password\n";
    }
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
        do {
        cout << "Username (Single-Word) \t: ";
        getline(cin, username);

        // Check if the name contains any space
        if (username.find(' ') != string::npos) {
            cout << "\nInvalid username format. Please use a single word without spaces.\n";
        } else if (findUserRecordByUsername(trim(username)) != nullptr) { // Check if the username already exists in the linked list
            cout << "Username already exists. Please try again and choose a different name.\n";
        }
    } while (username.find(' ') != string::npos || findUserRecordByUsername(trim(username)) != nullptr);

    bool emailExists;
    do {
        cout << "Email \t\t\t: ";
        getline(cin, email);

        // Ensure that the email contains the '@' symbol
        if (email.find('@') == string::npos) {
            cout << "Invalid email format. Please use a valid email address.\n\n";
            continue; // Go to the next iteration of the loop if the email format is invalid
        }

        // Check if the email already exists in the linked list
        emailExists = (findUserRecordByEmail(trim(email)) != nullptr);

        if (emailExists) {
            cout << "Email already exists. Please try again with a different email.\n\n";
        }
    } while (email.find('@') == string::npos || emailExists);
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
        
        // Save user data to linked list
    	addUserRecord(username, email, address, contactNumber, password);
        system("cls");
        cout << "\nRegistration Successful\n";
    }

    void forgetPassword() {
    string input;
    cout << "Enter your username or email \t: ";
    getline(cin, input);

    UserRecord* userRecord = findUserRecord(trim(input));
    if (userRecord != nullptr) {
        cout << "Your password is \t\t: " << userRecord->password << endl;
    } else {
        cout << "User not found. Please check your username or email." << endl;
    }

    system("pause");
    system("cls");
}

    bool isLoggedIn() const
    {
        return loggedIn;
    }
};

//function declare
//void welcomePage(User& user, Restaurant R, Node*& head);

void welcomePage(User& user, Restaurant& R, Node*& head, Cart &cart)
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
                    // Read menu data from file and populate linked list
				    //R.readFile(head, 0); // Pass 0 as displayChoice to skip sorting
				    showMenuOptions(head,cart, R);
                }
                break;
            case 2:
                user.registration();
                break;
            case 3:
                user.forgetPassword();
                break;
            case 4:
//                admin.adminlogin();
//                ap.displayAdmin();
                break;
            case 5:
                cout << "Thank you for visiting Restaurant Fusion Fare Delights." << endl;
                exit(0);

            default:
                cout << "\nPlease select a valid option\n"
                     << endl;
            } // end of switch
        }     // end of if
        //else
        //{
            //showMenuOptions(user, admin, ap, R, C);
        //} // end of else

    } while (choice != 5); // end of do
} // end of welcome

int main()
{
    Node *head = nullptr;
    Cart cart;  
    Restaurant R(cart);  
    User user;
    user.initializeUserRecords();
    welcomePage(user, R, head, cart);


    // Clean up dynamically allocated memory
    Node *temp = head;
    while (temp != nullptr)
    {
        Node *next = temp->next;
        delete temp;
        temp = next;
    }

    return 0;
}
