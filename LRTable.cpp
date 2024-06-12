#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <set>
#include "VariadicTable.h"

//https://github.com/friedmud/variadic_table/tree/master 

using namespace std;

struct Item {
    string lhs;
    string rhs;
    int dotPosition;

    Item(string l, string r, int d) : lhs(l), rhs(r), dotPosition(d) {}

    bool operator==(const Item &other) const {
        return lhs == other.lhs && rhs == other.rhs && dotPosition == other.dotPosition;
    }
};

struct State {
    vector<Item> items;

    void addItem(Item item) {
        items.push_back(item);
    }

    bool contains(Item item) {
        for (const auto &i : items) {
            if (i == item) {
                return true;
            }
        }
        return false;
    }
};

set<string> terminals = {"$", "a", "b"};
set<string> nonTerminals = {"S", "A"};
map<int, State> states;
map<pair<int, string>, string> action;
map<pair<int, string>, int> goTo;

void initializeStates() {
    // Initialize with augmented grammar
    State I0;
    I0.addItem(Item("S'", "S", 0));
    I0.addItem(Item("S", "AA", 0));
    I0.addItem(Item("A", "aA", 0));
    I0.addItem(Item("A", "b", 0));

    states[0] = I0;

    // Add further state transitions based on the rules described
    // State transitions and item sets should be constructed here.
    // Note: This is a simplified initialization. In actual parsing, you would generate these items dynamically.
}

void fillActionTable() {
    // Fill ACTION table based on state transitions and grammar reductions
    action[make_pair(0, "a")] = "S3"; // S3
    action[make_pair(0, "b")] = "S4"; // S4
    action[make_pair(1, "$")] = "ACCEPT"; // ACCEPT

    action[make_pair(2, "a")] = "S3";
    action[make_pair(2, "b")] = "S4";
    action[make_pair(2, "$")] = "r1";

    action[make_pair(3, "a")] = "S3";
    action[make_pair(3, "b")] = "S4";
    action[make_pair(3, "A")] = "6";   // I6 state

    action[make_pair(4, "a")] = "r3";
    action[make_pair(4, "b")] = "r3";
    action[make_pair(4, "$")] = "r3";

    action[make_pair(5, "a")] = "r1"; // Reduce on production 1
    action[make_pair(5, "b")] = "r1"; // Reduce on production 1
    action[make_pair(5, "$")] = "r1"; // Reduce on production 1

    action[make_pair(6, "a")] = "r2"; // Reduce on production 2
    action[make_pair(6, "b")] = "r2"; // Reduce on production 2
    action[make_pair(6, "$")] = "r2"; // Reduce on production 2
}

void fillGotoTable() {
    // Fill GOTO table based on non-terminal state transitions
    goTo[make_pair(0, "S")] = 1;
    goTo[make_pair(0, "A")] = 2;
    goTo[make_pair(2, "A")] = 5;
    goTo[make_pair(3, "A")] = 6;
}

void printSideBySideTables() {
    set<int> statesSet;
    for (const auto &entry : action) {
        statesSet.insert(entry.first.first);
    }
    for (const auto &entry : goTo) {
        statesSet.insert(entry.first.first);
    }

    // Create the table with headers
    VariadicTable<int, string, string, string, string, string> vt(
        {"State", "$", "a", "b", "A", "S"}, 10);
    
    // Add rows for ACTION table entries
    for (int state : statesSet) {
        string dollar = action.find({state, "$"}) != action.end() ? action[{state, "$"}]  : " ";
        string a = action.find({state, "a"}) != action.end() ? action[{state, "a"}] : " ";
        string b = action.find({state, "b"}) != action.end() ? action[{state, "b"}] : " ";
        string A = goTo.find({state, "A"}) != goTo.end() ? to_string(goTo[{state, "A"}]) : " ";
        string S = goTo.find({state, "S"}) != goTo.end() ? to_string(goTo[{state, "S"}]) : " ";

        vt.addRow(state, dollar, a, b, A, S);
    }
    
    // Print the table
    vt.print(cout);
}

int main() {
    initializeStates();
    fillActionTable();
    fillGotoTable();

    printSideBySideTables();

    return 0;
}
