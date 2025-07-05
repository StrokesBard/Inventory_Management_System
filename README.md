# Inventory Management System

A robust C++ console application for managing inventory across multiple product categories with CSV file persistence. This system uses object-oriented programming principles including inheritance, polymorphism, and smart pointers to provide a comprehensive inventory management solution.

## Features

### Core Functionality
- **Multi-Category Support**: Manage Electronics, Food, and Medicine products
- **Product Management**: Add, update, search, and remove products
- **Category-Specific CSV Storage**: Separate CSV files for each product category
- **Stock Reporting**: Generate detailed inventory reports with low stock alerts
- **Search Capabilities**: Search by product ID or name (partial matching)
- **Data Persistence**: Automatic loading and saving of inventory data

### Product Categories

#### Electronics
- Product ID, Name, Price, Quantity
- Brand information
- Warranty period (in months)

#### Food Products
- Product ID, Name, Price, Quantity
- Expiry date
- Organic certification status

#### Medicine
- Product ID, Name, Price, Quantity
- Manufacturer information
- Expiry date
- Prescription requirement status

## Technical Architecture

### Object-Oriented Design
- **Base Class**: `Product` - Abstract base class with virtual methods
- **Derived Classes**: `Electronic`, `Food`, `Medicine` - Specialized product types
- **Manager Class**: `InventoryManager` - Handles all inventory operations
- **Application Class**: `InventoryApp` - User interface and menu system

### Key Design Patterns
- **Inheritance**: Product hierarchy with specialized classes
- **Polymorphism**: Virtual functions for category-specific behavior
- **Smart Pointers**: Memory management with `unique_ptr`
- **CSV Handling**: Robust parsing with quote and comma handling

## Getting Started

### Prerequisites
- C++ compiler with C++11 support or higher
- Standard Template Library (STL)

### Compilation
```bash
g++ -std=c++11 -o ims ims.cpp
```

### Running the Application
```bash
./ims
```

## Usage

### Main Menu Options
1. **Add Products**: Add new products to specific categories
2. **Display Products**: View all products or filter by category
3. **Search Functions**: Find products by ID or name
4. **Update Products**: Modify existing product information
5. **Remove Products**: Delete products from inventory
6. **Stock Reports**: Generate comprehensive inventory reports
7. **Export Data**: Export category-specific data to CSV files

### CSV File Structure

#### Electronics (electronics_inventory.csv)
```csv
product_id,name,price,quantity,brand,warranty_months
E002,Keyboard,599,15,HP,9
```

#### Food (food_inventory.csv)
```csv
product_id,name,price,quantity,expiry_date,is_organic
F003,Chocolates,20.00,50,07/07/2026,0
```

#### Medicine (medicine_inventory.csv)
```csv
product_id,name,price,quantity,manufacturer,expiry_date,prescription_required
M001,Paracetamol,4.00,30,Mankind,05/10/2025,0
```

## File Management

### Automatic Operations
- **Auto-Load**: Inventory data is automatically loaded on application start
- **Auto-Save**: Data is saved when exiting the application
- **Category Separation**: Each product category is stored in its own CSV file

### Manual Operations
- **Export**: Export specific categories to custom CSV files
- **Import**: Load data from existing CSV files

## Features in Detail

### Stock Management
- **Low Stock Alerts**: Automatically identifies products with quantity < 10
- **Stock Updates**: Modify product quantities
- **Total Value Calculation**: Automatic calculation of inventory value

### Search Functionality
- **ID Search**: Exact match search by product ID
- **Name Search**: Case-insensitive partial matching
- **Category Filtering**: Display products by specific categories

### Data Validation
- **Duplicate Prevention**: Prevents adding products with existing IDs
- **Input Validation**: Handles invalid input gracefully
- **CSV Parsing**: Robust handling of quoted fields and special characters

## Error Handling

- **File Operations**: Graceful handling of missing or corrupted files
- **Input Validation**: Protection against invalid user input
- **Memory Management**: Automatic cleanup with smart pointers
- **Exception Handling**: Proper error handling for data parsing

## Future Enhancements

- [ ] GUI interface using Qt or similar framework
- [ ] Database integration (MySQL/PostgreSQL)
- [ ] Barcode scanning support
- [ ] Multi-user access control
- [ ] Advanced reporting with charts and graphs
- [ ] Mobile app integration
- [ ] Cloud storage synchronization