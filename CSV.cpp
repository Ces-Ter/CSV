#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    fstream table;
    table.open("C:/Users/User/Desktop/SP.txt");
    if (table.is_open()) {
        while (!table.eof()) {
            cout << "1: " << table.tellg() << " 2: " << table.tellp() << '\n';
            if (table.get() == ',') {
                cout << "3: " << table.tellg() << " 4: " << table.tellp() << '\n';
                table.seekp(table.tellp() - static_cast<streampos>(1));
                cout << "5: " << table.tellg() << " 6: " << table.tellp() << '\n';
                table.put(';');
                cout << "7: " << table.tellg() << " 8: " << table.tellp() << '\n';
                //table.seekp(table.tellp());
                //cout << "9: " << table.tellg() << " 10: " << table.tellp() << '\n';
            }
        }
    }
    else {
        cout << "failed to open file" << '\n';

    table.close();
}