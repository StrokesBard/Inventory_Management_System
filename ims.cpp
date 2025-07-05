#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <memory>

using namespace std;

// Base Product class
class Product {
protected:
    string productId;
    string name;
    double price;
    int quantity;
    string category;

public:
    // Constructor
    Product(const string& id, const string& n, double p, int q, const string& cat)
        : productId(id), name(n), price(p), quantity(q), category(cat) {}

    // Virtual destructor for proper cleanup
    virtual ~Product() = default;

    // Pure virtual functions for polymorphism
    virtual void displayDetails() const = 0;
    virtual string getProductType() const = 0;
    virtual vector<string> toCsvRow() const = 0;
    virtual string getCsvHeader() const = 0;
    virtual string getCsvFilename() const = 0;

    // Getters
    string getProductId() const { return productId; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    string getCategory() const { return category; }

    // Setters
    void setName(const string& n) { name = n; }
    void setPrice(double p) { price = p; }
    void setQuantity(int q) { quantity = q; }
    void setCategory(const string& cat) { category = cat; }

    // Utility methods
    void updateStock(int change) { quantity += change; }
    double getTotalValue() const { return price * quantity; }

    // Helper function to escape CSV values
    static string escapeCsv(const string& value) {
        if (value.find(',') != string::npos || value.find('"') != string::npos || value.find('\n') != string::npos) {
            string escaped = "\"";
            for (char c : value) {
                if (c == '"') escaped += "\"\"";
                else escaped += c;
            }
            escaped += "\"";
            return escaped;
        }
        return value;
    }
};

// Electronic Product class
class Electronic : public Product {
private:
    string brand;
    int warrantyMonths;

public:
    Electronic(const string& id, const string& n, double p, int q, 
               const string& b, int warranty)
        : Product(id, n, p, q, "Electronics"), brand(b), warrantyMonths(warranty) {}

    void displayDetails() const override {
        cout << left << setw(12) << productId 
             << setw(20) << name 
             << setw(12) << category
             << setw(10) << "$" << fixed << setprecision(2) << price
             << setw(8) << quantity
             << setw(15) << brand
             << setw(8) << warrantyMonths << " months" << endl;
    }

    string getProductType() const override { return "Electronic"; }
    string getCsvFilename() const override { return "electronics_inventory.csv"; }

    vector<string> toCsvRow() const override {
        return {
            productId,
            escapeCsv(name),
            to_string(price),
            to_string(quantity),
            escapeCsv(brand),
            to_string(warrantyMonths)
        };
    }

    string getCsvHeader() const override {
        return "product_id,name,price,quantity,brand,warranty_months";
    }

    // Getters for electronic-specific attributes
    string getBrand() const { return brand; }
    int getWarrantyMonths() const { return warrantyMonths; }

    // Setters
    void setBrand(const string& b) { brand = b; }
    void setWarrantyMonths(int w) { warrantyMonths = w; }
};

// Food Product class
class Food : public Product {
private:
    string expiryDate;
    bool isOrganic;

public:
    Food(const string& id, const string& n, double p, int q, 
         const string& expiry, bool organic)
        : Product(id, n, p, q, "Food"), expiryDate(expiry), isOrganic(organic) {}

    void displayDetails() const override {
        cout << left << setw(12) << productId 
             << setw(20) << name 
             << setw(12) << category
             << setw(10) << "$" << fixed << setprecision(2) << price
             << setw(8) << quantity
             << setw(15) << expiryDate
             << setw(8) << (isOrganic ? "Yes" : "No") << endl;
    }

    string getProductType() const override { return "Food"; }
    string getCsvFilename() const override { return "food_inventory.csv"; }

    vector<string> toCsvRow() const override {
        return {
            productId,
            escapeCsv(name),
            to_string(price),
            to_string(quantity),
            escapeCsv(expiryDate),
            (isOrganic ? "1" : "0")
        };
    }

    string getCsvHeader() const override {
        return "product_id,name,price,quantity,expiry_date,is_organic";
    }

    // Getters for food-specific attributes
    string getExpiryDate() const { return expiryDate; }
    bool getIsOrganic() const { return isOrganic; }

    // Setters
    void setExpiryDate(const string& expiry) { expiryDate = expiry; }
    void setIsOrganic(bool organic) { isOrganic = organic; }
};

// Medicine Product class (for pharmacy)
class Medicine : public Product {
private:
    string manufacturer;
    string expiryDate;
    bool prescriptionRequired;

public:
    Medicine(const string& id, const string& n, double p, int q,
             const string& mfg, const string& expiry, bool prescription)
        : Product(id, n, p, q, "Medicine"), manufacturer(mfg), 
          expiryDate(expiry), prescriptionRequired(prescription) {}

    void displayDetails() const override {
        cout << left << setw(12) << productId 
             << setw(20) << name 
             << setw(12) << category
             << setw(10) << "$" << fixed << setprecision(2) << price
             << setw(8) << quantity
             << setw(15) << manufacturer
             << setw(12) << expiryDate
             << setw(8) << (prescriptionRequired ? "Yes" : "No") << endl;
    }

    string getProductType() const override { return "Medicine"; }
    string getCsvFilename() const override { return "medicine_inventory.csv"; }

    vector<string> toCsvRow() const override {
        return {
            productId,
            escapeCsv(name),
            to_string(price),
            to_string(quantity),
            escapeCsv(manufacturer),
            escapeCsv(expiryDate),
            (prescriptionRequired ? "1" : "0")
        };
    }

    string getCsvHeader() const override {
        return "product_id,name,price,quantity,manufacturer,expiry_date,prescription_required";
    }

    // Getters for medicine-specific attributes
    string getManufacturer() const { return manufacturer; }
    string getExpiryDate() const { return expiryDate; }
    bool getPrescriptionRequired() const { return prescriptionRequired; }

    // Setters
    void setManufacturer(const string& mfg) { manufacturer = mfg; }
    void setExpiryDate(const string& expiry) { expiryDate = expiry; }
    void setPrescriptionRequired(bool prescription) { prescriptionRequired = prescription; }
};

// Inventory Manager class
class InventoryManager {
private:
    vector<unique_ptr<Product>> inventory;
    const vector<string> categoryFiles = {
        "electronics_inventory.csv",
        "food_inventory.csv", 
        "medicine_inventory.csv"
    };

    // Helper function to split CSV line considering quoted fields
    vector<string> parseCsvLine(const string& line) {
        vector<string> tokens;
        stringstream ss(line);
        string token;
        bool inQuotes = false;
        string currentToken;

        for (size_t i = 0; i < line.length(); i++) {
            char c = line[i];
            
            if (c == '"') {
                if (inQuotes && i + 1 < line.length() && line[i + 1] == '"') {
                    // Handle escaped quotes
                    currentToken += '"';
                    i++; // Skip next quote
                } else {
                    inQuotes = !inQuotes;
                }
            } else if (c == ',' && !inQuotes) {
                tokens.push_back(currentToken);
                currentToken.clear();
            } else {
                currentToken += c;
            }
        }
        
        tokens.push_back(currentToken); // Add the last token
        return tokens;
    }

    // Helper function to join CSV row
    string joinCsvRow(const vector<string>& row) const {
        string result;
        for (size_t i = 0; i < row.size(); i++) {
            if (i > 0) result += ",";
            result += row[i];
        }
        return result;
    }

    // Get products by category
    vector<Product*> getProductsByCategory(const string& category) const {
        vector<Product*> categoryProducts;
        for (const auto& product : inventory) {
            if (product->getCategory() == category) {
                categoryProducts.push_back(product.get());
            }
        }
        return categoryProducts;
    }

    // Save products of a specific category to their respective CSV file
    void saveCategoryToFile(const string& category, const string& filename) const {
        vector<Product*> categoryProducts = getProductsByCategory(category);
        
        if (categoryProducts.empty()) {
            return; // Don't create empty files
        }

        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not open " << filename << " for writing!\n";
            return;
        }

        // Write CSV header
        file << categoryProducts[0]->getCsvHeader() << endl;

        // Write data rows
        for (const auto& product : categoryProducts) {
            vector<string> row = product->toCsvRow();
            file << joinCsvRow(row) << endl;
        }
        file.close();
    }

    // Load products from a specific category CSV file
    void loadCategoryFromFile(const string& filename, const string& category) {
        ifstream file(filename);
        if (!file.is_open()) {
            return; // File doesn't exist, skip
        }

        string line;
        bool isFirstLine = true;
        
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            // Skip header line
            if (isFirstLine) {
                isFirstLine = false;
                continue;
            }

            vector<string> tokens = parseCsvLine(line);
            if (tokens.size() < 4) continue; // Minimum required fields

            try {
                string id = tokens[0];
                string name = tokens[1];
                double price = stod(tokens[2]);
                int quantity = stoi(tokens[3]);

                if (category == "Electronics" && tokens.size() >= 6) {
                    string brand = tokens[4];
                    int warranty = stoi(tokens[5]);
                    inventory.push_back(make_unique<Electronic>(id, name, price, quantity, brand, warranty));
                }
                else if (category == "Food" && tokens.size() >= 6) {
                    string expiry = tokens[4];
                    bool organic = (tokens[5] == "1");
                    inventory.push_back(make_unique<Food>(id, name, price, quantity, expiry, organic));
                }
                else if (category == "Medicine" && tokens.size() >= 7) {
                    string manufacturer = tokens[4];
                    string expiry = tokens[5];
                    bool prescription = (tokens[6] == "1");
                    inventory.push_back(make_unique<Medicine>(id, name, price, quantity, manufacturer, expiry, prescription));
                }
            } catch (const exception& e) {
                cout << "Error parsing line in " << filename << ": " << line << endl;
                cout << "Error: " << e.what() << endl;
            }
        }
        file.close();
    }

public:
    // Constructor
    InventoryManager() {
        loadFromFiles();
    }

    // Destructor
    ~InventoryManager() {
        saveToFiles();
    }

    // Add product to inventory
    void addProduct(unique_ptr<Product> product) {
        // Check if product ID already exists
        for (const auto& p : inventory) {
            if (p->getProductId() == product->getProductId()) {
                cout << "Product with ID " << product->getProductId() 
                     << " already exists! Use update function instead.\n";
                return;
            }
        }
        inventory.push_back(move(product));
        cout << "Product added successfully!\n";
    }

    // Search product by ID
    Product* searchById(const string& id) {
        auto it = find_if(inventory.begin(), inventory.end(),
            [&id](const unique_ptr<Product>& p) {
                return p->getProductId() == id;
            });
        return (it != inventory.end()) ? it->get() : nullptr;
    }

    // Update product details
    void updateProduct(const string& id) {
        Product* product = searchById(id);
        if (!product) {
            cout << "Product with ID " << id << " not found!\n";
            return;
        }

        cout << "\nCurrent product details:\n";
        cout << left << setw(12) << "ID" << setw(20) << "Name" 
             << setw(12) << "Category" << setw(10) << "Price" 
             << setw(8) << "Stock" << endl;
        cout << string(70, '-') << endl;
        product->displayDetails();

        int choice;
        cout << "\nWhat would you like to update?\n";
        cout << "1. Name\n2. Price\n3. Quantity\n4. Category\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string newName;
                cout << "Enter new name: ";
                cin.ignore();
                getline(cin, newName);
                product->setName(newName);
                break;
            }
            case 2: {
                double newPrice;
                cout << "Enter new price: ";
                cin >> newPrice;
                product->setPrice(newPrice);
                break;
            }
            case 3: {
                int newQuantity;
                cout << "Enter new quantity: ";
                cin >> newQuantity;
                product->setQuantity(newQuantity);
                break;
            }
            case 4: {
                string newCategory;
                cout << "Enter new category: ";
                cin.ignore();
                getline(cin, newCategory);
                product->setCategory(newCategory);
                break;
            }
            default:
                cout << "Invalid choice!\n";
                return;
        }
        cout << "Product updated successfully!\n";
    }

    // Display all products
    void displayAllProducts() const {
        if (inventory.empty()) {
            cout << "No products in inventory!\n";
            return;
        }

        cout << "\n" << string(100, '=') << endl;
        cout << "                          INVENTORY REPORT" << endl;
        cout << string(100, '=') << endl;

        cout << left << setw(12) << "ID" << setw(20) << "Name" 
             << setw(12) << "Category" << setw(10) << "Price" 
             << setw(8) << "Stock" << setw(15) << "Extra Info" 
             << setw(12) << "Details" << endl;
        cout << string(100, '-') << endl;

        for (const auto& product : inventory) {
            product->displayDetails();
        }
        cout << string(100, '=') << endl;
    }

    // Display products by category
    void displayProductsByCategory(const string& category) const {
        vector<Product*> categoryProducts = getProductsByCategory(category);
        
        if (categoryProducts.empty()) {
            cout << "No products found in " << category << " category!\n";
            return;
        }

        cout << "\n" << string(100, '=') << endl;
        cout << "                    " << category << " INVENTORY" << endl;
        cout << string(100, '=') << endl;

        cout << left << setw(12) << "ID" << setw(20) << "Name" 
             << setw(12) << "Category" << setw(10) << "Price" 
             << setw(8) << "Stock" << setw(15) << "Extra Info" 
             << setw(12) << "Details" << endl;
        cout << string(100, '-') << endl;

        for (const auto& product : categoryProducts) {
            product->displayDetails();
        }
        cout << string(100, '=') << endl;
    }

    // Generate stock report
    void generateStockReport() const {
        if (inventory.empty()) {
            cout << "No products in inventory!\n";
            return;
        }

        double totalValue = 0;
        int totalItems = 0;
        int lowStockItems = 0;
        const int LOW_STOCK_THRESHOLD = 10;

        cout << "\n" << string(80, '=') << endl;
        cout << "                        STOCK REPORT" << endl;
        cout << string(80, '=') << endl;

        // Category-wise breakdown
        vector<string> categories = {"Electronics", "Food", "Medicine"};
        for (const string& category : categories) {
            vector<Product*> categoryProducts = getProductsByCategory(category);
            if (!categoryProducts.empty()) {
                cout << "\n" << category << " Category:\n";
                cout << string(30, '-') << endl;
                int categoryItems = 0;
                double categoryValue = 0;
                
                for (const auto& product : categoryProducts) {
                    categoryItems += product->getQuantity();
                    categoryValue += product->getTotalValue();
                }
                
                cout << "Items: " << categoryItems << " | Value: $" 
                     << fixed << setprecision(2) << categoryValue << endl;
            }
        }

        cout << "\nLow Stock Alert (Quantity < " << LOW_STOCK_THRESHOLD << "):\n";
        cout << string(50, '-') << endl;
        
        for (const auto& product : inventory) {
            totalValue += product->getTotalValue();
            totalItems += product->getQuantity();
            
            if (product->getQuantity() < LOW_STOCK_THRESHOLD) {
                cout << "- " << product->getName() 
                     << " (ID: " << product->getProductId() 
                     << ") - Stock: " << product->getQuantity() 
                     << " [" << product->getCategory() << "]" << endl;
                lowStockItems++;
            }
        }

        if (lowStockItems == 0) {
            cout << "No items with low stock!\n";
        }

        cout << "\nOverall Summary:\n";
        cout << string(30, '-') << endl;
        cout << "Total Products: " << inventory.size() << endl;
        cout << "Total Items in Stock: " << totalItems << endl;
        cout << "Total Inventory Value: $" << fixed << setprecision(2) << totalValue << endl;
        cout << "Low Stock Items: " << lowStockItems << endl;
        cout << string(80, '=') << endl;
    }

    // Search products by name (partial match)
    void searchByName(const string& searchTerm) const {
        vector<Product*> results;
        
        for (const auto& product : inventory) {
            string productName = product->getName();
            string searchTermLower = searchTerm;
            
            // Convert to lowercase for case-insensitive search
            transform(productName.begin(), productName.end(), productName.begin(), ::tolower);
            transform(searchTermLower.begin(), searchTermLower.end(), searchTermLower.begin(), ::tolower);
            
            if (productName.find(searchTermLower) != string::npos) {
                results.push_back(product.get());
            }
        }

        if (results.empty()) {
            cout << "No products found matching '" << searchTerm << "'\n";
            return;
        }

        cout << "\nSearch Results for '" << searchTerm << "':\n";
        cout << string(80, '-') << endl;
        cout << left << setw(12) << "ID" << setw(20) << "Name" 
             << setw(12) << "Category" << setw(10) << "Price" 
             << setw(8) << "Stock" << endl;
        cout << string(80, '-') << endl;

        for (Product* product : results) {
            product->displayDetails();
        }
    }

    // Remove product
    void removeProduct(const string& id) {
        auto it = find_if(inventory.begin(), inventory.end(),
            [&id](const unique_ptr<Product>& p) {
                return p->getProductId() == id;
            });

        if (it != inventory.end()) {
            cout << "Product '" << (*it)->getName() << "' removed successfully!\n";
            inventory.erase(it);
        } else {
            cout << "Product with ID " << id << " not found!\n";
        }
    }

    // Save inventory to category-specific CSV files
    void saveToFiles() const {
        saveCategoryToFile("Electronics", "electronics_inventory.csv");
        saveCategoryToFile("Food", "food_inventory.csv");
        saveCategoryToFile("Medicine", "medicine_inventory.csv");
        
        cout << "Data saved to category-specific CSV files successfully!\n";
        cout << "Files created:\n";
        cout << "- electronics_inventory.csv\n";
        cout << "- food_inventory.csv\n";
        cout << "- medicine_inventory.csv\n";
    }

    // Load inventory from category-specific CSV files
    void loadFromFiles() {
        inventory.clear();
        
        loadCategoryFromFile("electronics_inventory.csv", "Electronics");
        loadCategoryFromFile("food_inventory.csv", "Food");
        loadCategoryFromFile("medicine_inventory.csv", "Medicine");
        
        cout << "Loaded " << inventory.size() << " products from category-specific CSV files.\n";
    }

    // Export specific category to CSV
    void exportCategoryToCsv(const string& category, const string& exportFilename) const {
        vector<Product*> categoryProducts = getProductsByCategory(category);
        
        if (categoryProducts.empty()) {
            cout << "No products found in " << category << " category!\n";
            return;
        }

        ofstream file(exportFilename);
        if (!file.is_open()) {
            cout << "Error: Could not create export file!\n";
            return;
        }

        // Write CSV header
        file << categoryProducts[0]->getCsvHeader() << endl;

        // Write data rows
        for (const auto& product : categoryProducts) {
            vector<string> row = product->toCsvRow();
            file << joinCsvRow(row) << endl;
        }
        file.close();
        cout << category << " inventory exported to " << exportFilename << " successfully!\n";
    }

    // Get available categories
    vector<string> getAvailableCategories() const {
        vector<string> categories;
        for (const auto& product : inventory) {
            string category = product->getCategory();
            if (find(categories.begin(), categories.end(), category) == categories.end()) {
                categories.push_back(category);
            }
        }
        return categories;
    }
};

// Main application class
class InventoryApp {
private:
    InventoryManager manager;

    void displayMenu() const {
        cout << "\n" << string(60, '=') << endl;
        cout << "         INVENTORY MANAGEMENT SYSTEM" << endl;
        cout << "           (Category-Specific CSVs)" << endl;
        cout << string(60, '=') << endl;
        cout << "1. Add Electronic Product\n";
        cout << "2. Add Food Product\n";
        cout << "3. Add Medicine Product\n";
        cout << "4. Display All Products\n";
        cout << "5. Display Products by Category\n";
        cout << "6. Search Product by ID\n";
        cout << "7. Search Product by Name\n";
        cout << "8. Update Product\n";
        cout << "9. Remove Product\n";
        cout << "10. Generate Stock Report\n";
        cout << "11. Export Category to CSV\n";
        cout << "12. Save and Exit\n";
        cout << string(60, '-') << endl;
        cout << "Enter your choice: ";
    }

    void addElectronicProduct() {
        string id, name, brand;
        double price;
        int quantity, warranty;

        cout << "\nEnter Electronic Product Details:\n";
        cout << "Product ID: ";
        cin >> id;
        cout << "Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Price: ";
        cin >> price;
        cout << "Quantity: ";
        cin >> quantity;
        cout << "Brand: ";
        cin.ignore();
        getline(cin, brand);
        cout << "Warranty (months): ";
        cin >> warranty;

        manager.addProduct(make_unique<Electronic>(id, name, price, quantity, brand, warranty));
    }

    void addFoodProduct() {
        string id, name, expiry;
        double price;
        int quantity;
        char organicChoice;

        cout << "\nEnter Food Product Details:\n";
        cout << "Product ID: ";
        cin >> id;
        cout << "Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Price: ";
        cin >> price;
        cout << "Quantity: ";
        cin >> quantity;
        cout << "Expiry Date (DD/MM/YYYY): ";
        cin >> expiry;
        cout << "Is Organic? (y/n): ";
        cin >> organicChoice;

        bool isOrganic = (organicChoice == 'y' || organicChoice == 'Y');
        manager.addProduct(make_unique<Food>(id, name, price, quantity, expiry, isOrganic));
    }

    void addMedicineProduct() {
        string id, name, manufacturer, expiry;
        double price;
        int quantity;
        char prescriptionChoice;

        cout << "\nEnter Medicine Product Details:\n";
        cout << "Product ID: ";
        cin >> id;
        cout << "Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Price: ";
        cin >> price;
        cout << "Quantity: ";
        cin >> quantity;
        cout << "Manufacturer: ";
        cin.ignore();
        getline(cin, manufacturer);
        cout << "Expiry Date (DD/MM/YYYY): ";
        cin >> expiry;
        cout << "Prescription Required? (y/n): ";
        cin >> prescriptionChoice;

        bool prescriptionRequired = (prescriptionChoice == 'y' || prescriptionChoice == 'Y');
        manager.addProduct(make_unique<Medicine>(id, name, price, quantity, manufacturer, expiry, prescriptionRequired));
    }

    void displayByCategory() {
        cout << "\nSelect Category:\n";
        cout << "1. Electronics\n";
        cout << "2. Food\n";
        cout << "3. Medicine\n";
        cout << "Enter choice: ";
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1:
                manager.displayProductsByCategory("Electronics");
                break;
            case 2:
                manager.displayProductsByCategory("Food");
                break;
            case 3:
                manager.displayProductsByCategory("Medicine");
                break;
            default:
                cout << "Invalid choice!\n";
        }
    }

    void exportCategoryToCsv() {
        cout << "\nSelect Category to Export:\n";
        cout << "1. Electronics\n";
        cout << "2. Food\n";
        cout << "3. Medicine\n";
        cout << "Enter choice: ";
        
        int choice;
        cin >> choice;
        
        string category;
        switch (choice) {
            case 1:
                category = "Electronics";
                break;
            case 2:
                category = "Food";
                break;
            case 3:
                category = "Medicine";
                break;
            default:
                cout << "Invalid choice!\n";
                return;
        }
        
        string exportFilename;
        cout << "Enter export filename (with .csv extension): ";
        cin >> exportFilename;
        manager.exportCategoryToCsv(category, exportFilename);
    }

public:
    void run() {
        int choice;
        
        cout << "Welcome to Category-Specific Inventory Management System!\n";
        
        do {
            displayMenu();
            cin >> choice;

            switch (choice) {
                case 1:
                    addElectronicProduct();
                    break;
                case 2:
                    addFoodProduct();
                    break;
                case 3:
                    addMedicineProduct();
                    break;
                case 4:
                    manager.displayAllProducts();
                    break;
                case 5:
                    displayByCategory();
                    break;
                case 6: {
                    string id;
                    cout << "Enter Product ID: ";
                    cin >> id;
                    Product* product = manager.searchById(id);
                    if (product) {
                        cout << "\nProduct Found:\n";
                        cout << left << setw(12) << "ID" << setw(20) << "Name" 
                             << setw(12) << "Category" << setw(10) << "Price" 
                             << setw(8) << "Stock" << endl;
                        cout << string(70, '-') << endl;
                        product->displayDetails();
                    } else {
                        cout << "Product not found!\n";
                    }
                    break;
                }
                case 7: {
                    string searchTerm;
                    cout << "Enter product name to search: ";
                    cin.ignore();
                    getline(cin, searchTerm);
                    manager.searchByName(searchTerm);
                    break;
                }
                case 8: {
                    string id;
                    cout << "Enter Product ID to update: ";
                    cin >> id;
                    manager.updateProduct(id);
                    break;
                }
                case 9: {
                    string id;
                    cout << "Enter Product ID to remove: ";
                    cin >> id;
                    manager.removeProduct(id);
                    break;
                }
                case 10:
                    manager.generateStockReport();
                    break;
                case 11:
                    exportCategoryToCsv();
                    break;
                case 12:
                    cout << "Saving data and exiting...\n";
                    manager.saveToFiles();
                    cout << "Thank you for using Category-Specific Inventory Management System!\n";
                    break;
                default:
                    cout << "Invalid choice! Please try again.\n";
            }
        } while (choice != 12);
    }
};

// Main function
int main() {
    InventoryApp app;
    app.run();
    return 0;
}