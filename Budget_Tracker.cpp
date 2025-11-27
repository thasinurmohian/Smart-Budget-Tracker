#include <bits/stdc++.h>
#include <fstream>
#include <iomanip>
#include <map>
#include <cmath>
using namespace std;

// ----------- COLOR CODES (ADDED) -------------
namespace Color {
    const string RESET   = "\033[0m";
    const string RED     = "\033[31m";
    const string GREEN   = "\033[32m";
    const string YELLOW  = "\033[33m";
    const string BLUE    = "\033[34m";
    const string MAGENTA = "\033[35m";
    const string CYAN    = "\033[36m";
    const string BOLD    = "\033[1m";
}

// ----------- CLEAR SCREEN FUNCTION -------------
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// ----------- FORMAT DATE FUNCTION -------------
string formatDate(int d, int m, int y) {
    ostringstream out;
    out << setw(2) << setfill('0') << d << "-"
        << setw(2) << setfill('0') << m << "-"
        << setw(4) << setfill('0') << y;
    return out.str();
}

// ----------- DATA STRUCTURE -------------
struct Transaction {
    int id;
    string date;   // stored in DD-MM-YYYY format
    string category;
    double amount;
};

// ----------- BUDGET MANAGER CLASS -------------
class BudgetManager {
private:
    vector<Transaction> transactions;
    const string filename = "budget_data.txt";

    int getNextId() {
        if (transactions.empty()) return 1;
        return transactions.back().id + 1;
    }

public:
    BudgetManager() {
        loadFromFile();
    }

    // ------------ ADD TRANSACTION ------------
    void addTransaction() {
        Transaction t;
        t.id = getNextId();

        cout << "\n" << Color::BOLD << Color::CYAN << "========== ADD NEW TRANSACTION ==========" << Color::RESET << "\n";

        int d, m, y;
        cout << Color::YELLOW << "Enter date (DD MM YYYY): " << Color::RESET;
        cin >> d >> m >> y;
        t.date = formatDate(d, m, y);

        cout << Color::YELLOW << "Category (Use_Underscores): " << Color::RESET;
        cin >> t.category;

        cout << Color::YELLOW << "Amount (+ income, - expense): " << Color::RESET;
        cin >> t.amount;

        transactions.push_back(t);
        saveToFile();

        cout << "\n" << Color::GREEN << ">> Successfully added transaction ID: " << t.id << Color::RESET << "\n";
    }

    // ------------ VIEW HISTORY ------------
    void viewHistory() {
        cout << "\n" << Color::BOLD << Color::BLUE << "============== TRANSACTION HISTORY ==============" << Color::RESET << "\n\n";

        cout << Color::BOLD << left << setw(5) << "ID"
             << setw(15) << "Date"
             << setw(20) << "Category"
             << right << setw(12) << "Amount" << Color::RESET << endl;

        cout << Color::BLUE << "--------------------------------------------------------" << Color::RESET << "\n";

        for (const auto &t : transactions) {
            // Pick color based on amount (Green for +, Red for -)
            string amountColor = (t.amount >= 0) ? Color::GREEN : Color::RED;

            cout << left << setw(5) << t.id
                 << setw(15) << t.date
                 << setw(20) << t.category
                 << amountColor << right << setw(12) << fixed << setprecision(2) << t.amount << Color::RESET
                 << endl;
        }
    }

    // ------------ DELETE TRANSACTION ------------
    void deleteTransaction() {
        int targetId;
        cout << "\n" << Color::YELLOW << "Enter ID to delete: " << Color::RESET;
        cin >> targetId;

        for (auto it = transactions.begin(); it != transactions.end(); ++it) {
            if (it->id == targetId) {
                transactions.erase(it);
                saveToFile();
                cout << Color::GREEN << ">> Transaction deleted.\n" << Color::RESET;
                return;
            }
        }
        cout << Color::RED << ">> Error: ID not found.\n" << Color::RESET;
    }

    // ------------ ANALYZE SPENDING ------------
    void analyzeSpending() {
        map<string,double> categoryTotals;
        double totalExpense = 0;

        for (const auto &t : transactions) {
            if (t.amount < 0) {
                double val = abs(t.amount);
                categoryTotals[t.category] += val;
                totalExpense += val;
            }
        }

        cout << "\n" << Color::BOLD << Color::MAGENTA << "============ SPENDING ANALYSIS ============" << Color::RESET << "\n";

        if (totalExpense == 0) {
            cout << "No expenses recorded yet.\n";
            return;
        }

        for (auto &[cat, amt] : categoryTotals) {
            double percent = (amt / totalExpense) * 100;
            int bars = percent / 5;

            cout << left << setw(15) << cat
                 << " | BDT" << setw(8) << amt
                 << " | " << Color::RED;

            for (int i = 0; i < bars; i++) cout << "#";

            cout << Color::RESET << " (" << (int)percent << "%)\n";
        }
    }

    // ------------ EXPORT TO CSV ------------
    void exportToCSV() {
        ofstream file("budget_export.csv");
        file << "ID,Date,Category,Amount\n";

        for (const auto &t : transactions) {
            file << t.id << "," << t.date << "," 
                 << t.category << "," << t.amount << "\n";
        }

        cout << "\n" << Color::GREEN << ">> Exported to 'budget_export.csv'.\n" << Color::RESET;
    }

    // ------------ BALANCE ------------
    void showBalance() {
        double balance = 0;
        for (const auto &t : transactions) balance += t.amount;

        string balColor = (balance >= 0) ? Color::GREEN : Color::RED;

        cout << "\n" << Color::BOLD << Color::CYAN << "============ CURRENT BALANCE ============" << Color::RESET << "\n";
        cout << "Balance = " << balColor << "BDT" << fixed << setprecision(2) << balance << Color::RESET << endl;
    }

    // ------------ FILE SAVE ------------
    void saveToFile() {
        ofstream file(filename);
        for (const auto &t : transactions) {
            file << t.id << " " << t.date << " "
                 << t.category << " " << t.amount << endl;
        }
    }

    // ------------ FILE LOAD ------------
    void loadFromFile() {
        ifstream file(filename);
        Transaction t;

        while (file >> t.id >> t.date >> t.category >> t.amount) {
            transactions.push_back(t);
        }
    }
};

// =============== MAIN MENU ==================
int main() {
    // Enable colors in Windows CMD
    #ifdef _WIN32
        system("color");
    #endif

    BudgetManager manager;
    int choice;

    while (true) {
        clearScreen();

        cout << "\n" << Color::BOLD << Color::BLUE << "=====================================\n";
        cout << "        SMART BUDGET TRACKER\n";
        cout << "=====================================" << Color::RESET << "\n";
        cout << Color::YELLOW << "1." << Color::RESET << " Add Income / Expense\n";
        cout << Color::YELLOW << "2." << Color::RESET << " View History\n";
        cout << Color::YELLOW << "3." << Color::RESET << " Delete Transaction\n";
        cout << Color::YELLOW << "4." << Color::RESET << " Analyze Spending\n";
        cout << Color::YELLOW << "5." << Color::RESET << " Export to CSV (Excel)\n";
        cout << Color::YELLOW << "6." << Color::RESET << " Check Balance\n";
        cout << Color::RED    << "7. Exit" << Color::RESET << "\n";
        cout << Color::BLUE << "-------------------------------------" << Color::RESET << "\n";
        cout << "Choose an option: ";
        cin >> choice;

        clearScreen();

        switch (choice) {
            case 1: manager.addTransaction(); break;
            case 2: manager.viewHistory(); break;
            case 3: manager.deleteTransaction(); break;
            case 4: manager.analyzeSpending(); break;
            case 5: manager.exportToCSV(); break;
            case 6: manager.showBalance(); break;
            case 7:
                cout << Color::CYAN << "Exiting... Data saved.\n" << Color::RESET;
                return 0;
            default:
                cout << Color::RED << "Invalid choice.\n" << Color::RESET;
        }

        cout << "\n\nPress ENTER to continue...";
        cin.ignore();
        cin.get();
    }
}