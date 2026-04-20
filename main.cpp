#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <climits>
using namespace std;

// class 1 - holds one row from the csv
class SaleRecord {
private:
    int    year;
    string season;
    int    day;
    string item;
    string category;
    int    quantity;
    int    gold;

public:
    SaleRecord(int y, string s, int d, string i, string c, int q, int g)
        : year(y), season(s), day(d), item(i), category(c), quantity(q), gold(g) {}

    int    getYear()     const { return year; }
    string getSeason()   const { return season; }
    int    getDay()      const { return day; }
    string getItem()     const { return item; }
    string getCategory() const { return category; }
    int    getQuantity() const { return quantity; }
    int    getGold()     const { return gold; }
};


// c-2 reads the file and runs reports
class FarmAnalyzer {
private:
    vector<SaleRecord> records;

    string trim(const string& s) {
        int a = 0, b = (int)s.size() - 1;
        while (a <= b && s[a] == ' ') a++;
        while (b >= a && s[b] == ' ') b--;
        return s.substr(a, b - a + 1);
    }

    // split one csv lineby commas D-2
    vector<string> splitLine(const string& line) {
        vector<string> parts;
        stringstream ss(line);
        string cell;
        while (getline(ss, cell, ','))
            parts.push_back(trim(cell));
        return parts;
    }

public:
    bool loadData(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Could not open file." << endl;
            return false;
        }

        string line;
        getline(file, line); 

        while (getline(file, line)) {
            if (line.empty()) continue;
            vector<string> cols = splitLine(line);
            if (cols.size() < 7) continue;

            try {
                int    year     = stoi(cols[0]);
                string season   = cols[1];
                int    day      = stoi(cols[2]);
                string item     = cols[3];
                string category = cols[4];
                int    qty      = stoi(cols[5]);
                int    gold     = stoi(cols[6]);
                records.push_back(SaleRecord(year, season, day, item, category, qty, gold));
            } catch (...) {
                continue; 
            }
        }

        cout << "Loaded " << records.size() << " records." << endl;
        return !records.empty();
    }

    // report 1 - total gold per sezn  D-3
    void reportBySeason() {
        map<string, int> totals;
        for (const SaleRecord& r : records)
            totals[r.getSeason()] += r.getGold();

        cout << "\n--- Profit by Season ---" << endl;
        string best; int bestGold = 0;
        for (auto& p : totals) {
            cout << p.first << ": " << p.second << "g" << endl;
            if (p.second > bestGold) { bestGold = p.second; best = p.first; }
        }
        cout << "Best season: " << best << " (" << bestGold << "g)" << endl;
    }

    // report 2 - total gold per category
    void reportByCategory() {
        map<string, int> totals;
        for (const SaleRecord& r : records)
            totals[r.getCategory()] += r.getGold();

        cout << "\n--- Profit by Category ---" << endl;
        string best; int bestGold = 0;
        for (auto& p : totals) {
            cout << p.first << ": " << p.second << "g" << endl;
            if (p.second > bestGold) { bestGold = p.second; best = p.first; }
        }
        cout << "Top category: " << best << " (" << bestGold << "g)" << endl;
    }

    // recommendation based on the data 
    // Continue D-4 
    void recommend() {
        map<string, int> bySeason, byCat;
        for (const SaleRecord& r : records) {
            bySeason[r.getSeason()]  += r.getGold();
            byCat[r.getCategory()]   += r.getGold();
        }

        // nice and bad season
        string bestSeason, worstSeason;
        int top = 0, low = INT_MAX;
        for (auto& p : bySeason) {
            if (p.second > top) { top = p.second; bestSeason  = p.first; }
            if (p.second < low) { low = p.second; worstSeason = p.first; }
        }

        // best category
        string bestCat; int topCat = 0;
        for (auto& p : byCat)
            if (p.second > topCat) { topCat = p.second; bestCat = p.first; }

        cout << "\n--- Farm Recommendation ---" << endl;
        cout << "Best season is " << bestSeason << " -> plant more in " << bestSeason << endl;
        cout << "Top category is " << bestCat   << " -> focus on " << bestCat << " items" << endl;
        cout << "Slowest season is " << worstSeason << " -> try fishing or mining then" << endl;
    }
};


int main() {
    FarmAnalyzer farm;

    cout << "Stardew Valley Farm Analyzer " << endl;
    if (!farm.loadData("stardew_dataset.csv"))
        return 1;

    int choice = 0;
    while (choice != 4) {
        cout << "\n1. Profit by Season"   << endl;
        cout << "2. Profit by Category"  << endl;
        cout << "3. Get Recommendation"  << endl;
        cout << "4. Quit"                << endl;
        cout << "Pick: ";
        cin >> choice;

        if      (choice == 1) farm.reportBySeason();
        else if (choice == 2) farm.reportByCategory();
        else if (choice == 3) farm.recommend();
        else if (choice == 4) cout << "Goodbye!" << endl;
        else                  cout << "Invalid choice." << endl;
    }

    return 0;
}