#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class table {
public:
	fstream file;
	int currentcolumn = 0;
	int currentrow = 0;
	char separator = ';';
	int columnsnumber = 1;
	int rowsnumber = 1;
private:
	/*checks cell
	returns 0 if file ends after quoted cell
	returns 1 if row ends after quoted cell
	returns 2 if new cell in the same row starts after cell ends
	returns -1 if file ends in quoted cell
	returns -2 if quoted cell formating is wrong
	returns -3 if there is quote in an unquoted cell
	*/
	int checkQuotedCell() {
		char d;
		while (true) {
			d = file.get();
			if (file.eof()) {
				return -1;
			}
			else if (d == '\"') {
				d = file.get();
				if (file.eof()) {
					return 0;
				}
				if (d == '\n') {
					return 1;
				}
				if (d == separator) {
					return 2;
				}
				if (d != '\"') {
					return -2;
				}
			}

		}
	}
	int checkNotQuotedCell() {
		char a;
		while (true) {
			a = file.get();
			if (file.eof()) {
				return 0;
			}
			else if (a == '\n') {
				return 1;
			}
			else if (a == separator) {
				return 2;
			}
			else if (a == '\"') {
				return -3;
			}
		}
	}
	int checkCell() {
		char a;
		a = file.get();
		if (file.eof()) {
			return 0;
		}
		else if (a == '\n') {
			return 1;
		}
		else if (a == separator) {
			return 2;
		}
		else if (a == '\"') {
			return checkQuotedCell();
		}
		else return checkNotQuotedCell();
	}
	string checkTable() {
		file.seekg(0);
		currentcolumn = 0;
		currentrow = 0;
		//the following while checks the first row
		while (true) {
			int b = checkCell();
			if (b == 0) {
				currentcolumn = 0;
				currentrow = 0;
				string success = "Success: table comprises 1 row and " + to_string(columnsnumber) + " columns\n";
				return success;
			}
			else if (b == 1) {
				break;
			}
			else if (b == 2) {
				columnsnumber++;
			}
			else if (b == -1) {
				string error = "Error: file ends in quoted cell: column " + to_string(columnsnumber - 1) + ", row 0\n";
				return error;
			}
			else if (b == -2) {
				string error = "Error: wrong quoted cell formating: column " + to_string(columnsnumber - 1) + ", row 0\n";
				return error;
			}
			else if (b == -3) {
				string error = "Error: there is quote in an unquoted cell: column " + to_string(columnsnumber - 1) + ", row 0\n";
				return error;
			}

		}
		//the following while checks rows other than the first
		while (true) {
			int b = checkCell();
			if (b == 0) {
				if (currentcolumn + 1 == columnsnumber) {
					string success = "Success: table comprises " + to_string(rowsnumber + 1) + " rows and " + to_string(columnsnumber) + " columns" + '\n';
					return success;
				}
				else {
					string error = "Error: wrong number of columns in row " + to_string(rowsnumber + 1) + '\n';
					return error;
				}
			}
			else if (b == 1) {
				rowsnumber++;
				if (currentcolumn + 1 != columnsnumber) {
					string error = "Error: wrong number of columns in row " + to_string(rowsnumber + 1) + '\n';
					return error;
				}
				currentcolumn = 0;
			}
			else if (b == 2) {
				currentcolumn++;
			}
			else if (b == -1) {
				string error = "Error: file ends in quoted cell: column " + to_string(currentcolumn + 1) + ", row " + to_string(rowsnumber + 1) + '\n';
				return error;
			}
			else if (b == -2) {
				string error = "Error: wrong quoted cell formating: column " + to_string(currentcolumn + 1) + ", row " + to_string(rowsnumber + 1) + '\n';
				return error;
			}
			else if (b == -3) {
				string error = "Error: there is quote in an unquoted cell: column " + to_string(currentcolumn + 1) + ", row " + to_string(rowsnumber + 1) + '\n';
				return error;
			}
		}
	}

	//returns special symbol(', ", \n) system funcion stumbles upon
	char moveForwardToSpecialSymbol() {
		char a;
		while (true) {
			a = file.get();
			if (a == '\"' || a == separator || a == '\n') {
				return a;
			}
		}
	}
	char moveForwardToLineOrQuote() {
		char a;
		while (true) {
			a = file.get();
			if (a == '\"' || a == '\n') {
				return a;
			}
		}
	}
	//nect function ends up 2 characters after ending quote and returns symbol just after quote
	char moveForwardThorughQuotedCell() {
		char a;
		while (true) {
			a = file.get();
			if (a == '\"') {
				a = file.get();
				if (a != '\"') {
					return a;
				}
			}
		}
	}
	char moveBackwardToSpecialSymbol() {
		char a;
		while (true) {
			file.seekg(file.tellg() - streampos(2));
			a = file.get();
			if (a == '\"' || a == separator || a == '\n') {
				return a;
			}
		}
	}
	char moveBackwardToLineOrQuote() {
		char a;
		while (true) {
			file.seekg(file.tellg() - streampos(2));
			a = file.get();
			if (a == '\"' || a == '\n') {
				return a;
			}
		}
	}
	char moveBackwardThorughQuotedCell() {
		char a;
		while (true) {
			file.seekg(file.tellg() - streampos(2));
			a = file.get();
			if (a == '\"') {
				file.seekg(file.tellg() - streampos(2));
				a = file.get();
				if (a != '\"') {
					return a;
				}
			}
		}
	}
	void goToNextCell() {
		if (moveForwardToSpecialSymbol() == '\"') {
			moveForwardThorughQuotedCell();
		}
	}
	void goToNextRow() {
		while (true) {
			if (moveForwardToLineOrQuote() == '\"') {
				if (moveForwardThorughQuotedCell() == '\n') {
					return;
				}
			}
			else return;
		}
	}
	void goToPreviousCell() {
		if (moveBackwardToSpecialSymbol() == '\"') {
			moveBackwardThorughQuotedCell();
		}
	}
	void goToBegginingOfCurrentRow() {
		while (true) {
			if (moveBackwardToLineOrQuote() == '\"') {
				if (moveBackwardThorughQuotedCell() == '\n') {
					return;
				}
			}
			else return;
		}
	}
	void goToPreviousRow() {
		while (true) {
			if (moveBackwardToLineOrQuote() == '\"') {
				if (moveBackwardThorughQuotedCell() == '\n') {
					return;
				}
			}
			else return;
		}
	}
	

public:
	string setTable(string filepath, char setseparator) {
		separator = setseparator;
		file.open(filepath);
		if (file.is_open()) {
			//string a = checkTable();
			string a = "sus\n";
			file.clear();
			file.seekg(0);
			currentcolumn = 0;
			currentrow = 0;
			return a;
		}
		else return "Error: failed to open file";
	}
	int getCurrentColumn() {
		return currentcolumn;
	}
	int getCurrentRow() {
		return currentrow;
	}
	int getColumnsNumber() {
		return columnsnumber;
	}
	int getRowsNumber() {
		return rowsnumber;
	}
	char getSeparator() {
		return separator;
	}

	string getCell() {
		if (currentcolumn == columnsnumber - 1) {
			currentcolumn = 0;
			currentrow++;
		}
		else {
			currentcolumn++;
		}
 		char a = file.get();
		string b;
		if (a == separator || a == '\n') {
			return b;
		}
		else if (file.eof()) {
			currentrow = 0;
			currentcolumn = 0;
			file.clear();
			file.seekg(0);
			return b;
		}
		else if (a == '\"') {
			while (true) {
				a = file.get();
				if (a == '\"') {
					a = file.get();
					if (a != '\"') return b;
				}
				b += a;
			}
		}
		else b += a;
		while (true) {
			a = file.get();
			if (a == '\n' || a == separator) {
				return b;
			}
			else if (file.eof()) {
				file.clear();
				currentrow = 0;
				currentcolumn = 0;
				file.seekg(0);
				return b;
			}
			b += a;
		}
	}
	void goToCellRelative(int columnOffset, int rowOffset) {
		int a = currentcolumn;
		if (rowOffset > 0) {
			for (int i = 0; i < rowOffset; i++) {
				goToNextRow();
				currentrow++;
			}
			currentcolumn = 0;
		}
		else if (rowOffset < 0) {
			for (int i = 0; i < rowOffset; i++) {
				goToPreviousRow();
				currentrow--;
			}
			currentcolumn = 0;
		}
		else if (rowOffset == 0) {
			if (columnOffset > 0) {
				for (int i = 0; i < columnOffset; i++) {
					goToNextCell();
					currentrow++;
				}
			}
			else if (columnOffset < 0) {
				for (int i = 0; i < -columnOffset; i++) {
					goToPreviousCell();
					currentrow--;
				}
			}
			return;
		}
		for (int i = 0; i < a + columnOffset; i++) {
				goToNextCell();
				currentrow++;
		}
	}
	void goToCellAbsolute(int columnNumber, int rowNumber) {
		currentcolumn = 0;
		currentrow = 0;
		file.seekg(0);
		while (currentrow < rowNumber) {
			goToNextRow();
			currentrow++;
		}
		while (currentcolumn < columnNumber) {
			goToNextCell();
			currentcolumn++;
		}
		return;
	}
};

int main()
{
	table table;
	cout << table.setTable("C:\\Users\\User\\Desktop\\SP.csv", ',');
	table.goToCellAbsolute(1, 9332781);
	cout << "column: " << table.getCell() << ", row: " << table.getCurrentRow() << ", content of the cell: " << table.getCell() << "\n";
}
