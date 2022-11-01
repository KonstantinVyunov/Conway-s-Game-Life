#include <iostream>
#include <fstream>
#include <windows.h>
#include <cstdlib>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;

void welcome_to_game() {
	cout << "\n\n\n\n\n";
	cout << "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *" << endl;
	cout << " * * * * * * * * * * * WELCOME  TO * * * * * * * * * * * * * " << endl;
	cout << "* * * * * * * * * CONWAY's GAME OF LIFE * * * * * * * * * * *" << endl;
	cout << " * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * " << endl;
	cout << "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *" << endl;
	cout << endl;
	system("pause");
	return;
}

bool** create_universe(int &rows, int &columns, int &cells) {
	bool** universe = nullptr;
	ifstream file("input.txt");
	if (!file.is_open()) {
		cout << "File \"input.txt\" has not been found!" << endl;
	} else {
		file >> rows >> columns;
		universe = new bool*[rows];
		for (int i = 0; i < rows; ++i) {
			universe[i] = new bool[columns];
			for (int j = 0; j < columns; ++j) {
				universe[i][j] = false;
			}
		}
		int x = 0, y = 0;
		while (!file.eof()) {
			file >> x >> y;
			universe[x][y] = true;
			++cells;
		}
	}
	file.close();
	return universe;
}

bool** create_temp_universe(const int rows, const int columns) {
	bool** temp_universe = new bool* [rows];
	for (int x{ 0 }; x < rows; ++x) {
		temp_universe[x] = new bool[columns];
	}
	for (int x{ 0 }; x < rows; ++x) {
		for (int y{ 0 }; y < columns; ++y) {
			temp_universe[x][y] = false;
		}
	}
	return temp_universe;
}

void print_universe(bool** universe, const int rows, const int columns, int& generation, int& alive_cells) {
	alive_cells = 0;
	for (int x{ 0 }; x < rows; ++x) {
		for (int y{ 0 }; y < columns; ++y) {
			cout.width(2);
			if (universe[x][y] == true) {
				cout << '0';
				++alive_cells;
			} else {
				cout << '~';
			}
		}
		cout << endl;
	}
	cout << "Generation: " << generation++ << ". Alive cells: " << alive_cells << endl;
	return;
}

bool** copy_universe(bool** universe, const int rows, const int columns) {
	bool** temp_universe = new bool* [rows];
	for (int i{ 0 }; i < rows; ++i) {
		temp_universe[i] = new bool[columns];
	}
	for (int x{ 0 }; x < rows; ++x) {
		for (int y{ 0 }; y < columns; ++y) {
			temp_universe[x][y] = universe[x][y];
		}
	}
	return temp_universe;
}

int count_neighbors(const int x, const int y, bool** temp_universe, const int rows, const int columns) {
	int neighbors = 0;
	for (int x_ = 0; x_ < 3; ++x_) {
		for (int y_ = 0; y_ < 3; ++y_) {
			if (x_ == 1 && y_ == 1) {
				continue;
			} else if (((x - 1 + x_) < 0) || ((x - 1 + x_) > (rows - 1)) || ((y - 1 + y_) < 0) || ((y - 1 + y_) > (columns - 1))) {
				continue;
			} else if (temp_universe[x - 1 + x_][y - 1 + y_] == true) {
				++neighbors;
			}
		}
	}
	return neighbors;
}

void spread_cells(bool** universe, bool** temp_universe, const int rows, const int columns) {
	for (int x{ 0 }; x < rows; ++x) {
		for (int y{ 0 }; y < columns; ++y) {
			if (temp_universe[x][y] == false) {
				if (count_neighbors(x, y, temp_universe, rows, columns) == 3) {
					universe[x][y] = true;
				}
			}
			else if (temp_universe[x][y] == true) {
				if (count_neighbors(x, y, temp_universe, rows, columns) == 2) {
					universe[x][y] = true;
				}
				else if (count_neighbors(x, y, temp_universe, rows, columns) == 3) {
					universe[x][y] = true;
				}
				else if (count_neighbors(x, y, temp_universe, rows, columns) < 2) {
					universe[x][y] = false;
				}
				else if (count_neighbors(x, y, temp_universe, rows, columns) > 3) {
					universe[x][y] = false;
				}
			}
		}
	}
	return;
}

bool all_cells_dead(const int alive_cells) {
	return (alive_cells == 0) ? true : false;
}

bool universe_stagnated(bool** universe, bool** temp_universe, const int rows, const int columns) {
	int cells_counter{ 0 };
	for (int x{ 0 }; x < rows; ++x) {
		for (int y{ 0 }; y < columns; ++y) {
			if (universe[x][y] != temp_universe[x][y]) {
				++cells_counter;
			}
		}
	}
	if (cells_counter) {
		return false;
	}
	else {
		return true;
	}
}

bool is_game_continue(bool** universe, bool** temp_universe, const int rows, const int columns, int generation, int alive_cells) {
	// check if there is at least one alive cell
	Sleep(2000);
	if (all_cells_dead(alive_cells)) {
		cout << endl << "\t\tAlas, all cells are dead!\n\t\t\tGAME OVER!" << endl << endl;
		return false;
	}
	// check if the universe has stagnated
	if (universe_stagnated(universe, temp_universe, rows, columns)) {
		std::system("CLS");
		print_universe(universe, rows, columns, generation, alive_cells);
		cout << endl << "\t\tAlas, the universe has stagnated!\n\t\t\tGAME OVER!" << endl << endl;
		return false;
	}
	std::system("CLS");
	return true;
}

void clean_memory(bool** array, int arr_length = 10) {
	for (int i{ 0 }; i < arr_length; ++i) {
		delete[] array[i];
	}
	delete[] array;
	return;
}

int main(int argc, char** argv) {

	//constants
	static int columns = 0;
	static int rows = 0;
	int generation = 1;
	int cells = 0;

	welcome_to_game();

	//create universe
	bool** universe = create_universe(rows, columns, cells);
	
	//create temp universe
	bool** temp_universe = create_temp_universe(rows, columns);
	
	while (is_game_continue(universe, temp_universe, rows, columns, generation, cells)) {
		//print universe state
		print_universe(universe, rows, columns, generation, cells);
		//copy universe
		temp_universe = copy_universe(universe, rows, columns);
		//spread cells
		spread_cells(universe, temp_universe, rows, columns);
	}
	//clean all dynamic arrays
	clean_memory(temp_universe, rows);
	clean_memory(universe, rows);
	
	return 0;
}