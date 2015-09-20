/*
  bf15: A cross-platform brainfuck interpreter written in C++ with a few extra features
  Licensed under the GNU General Public License v3 or any later version
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

typedef unsigned char cell_t;
typedef unsigned long long cellptr_t;

int main(int argc,char** argv)
{
  if (argc < 2 || argc > 3) {
    std::cout << argv[0] << " <file> (-u)" << std::endl
              << "-u: allow unsafe behavior" << std::endl;
    return 1;
  }

  bool safe = true;

  if (argc == 3) {
    if (std::string(argv[2]) == "-u") {
      safe = false;
    } else {
      std::cerr << "Unknown parameter " << argv[2] << std::endl;
      return 1;
    }
  }

  std::vector<cell_t> cells;
  cellptr_t cell_ptr = 0;

  std::ifstream ifs(argv[1]);
  std::vector<char> program;

  if (!ifs.good()) {
    std::cerr << "Error: Can't open " << argv[1] << std::endl;
    return 1;
  }

  //Read brainfuck program from file
  while (ifs.good()) {
    char c = ifs.get();

    //Ignore all non-brainfuck characters
    if (std::string("<>+-.,[]").find(c) != std::string::npos)
      program.push_back(c);
  }
  ifs.close(); //Close input stream

  for (size_t pc = 0;pc < program.size();pc++) {
    if (cells.size() == cell_ptr) cells.push_back(0); //Reserve a new cell
    switch (program[pc]) {
      //Increase cell pointer
      case '>':
        ++cell_ptr;
        break;
      //Decrease cell pointer
      case '<':
        //Unsafe behavior
        if (cell_ptr == 0 && safe) {
          std::cerr << "Error: Cell pointer smaller than zero (Use -u to allow unsafe behavior)" << std::endl;
          return 1;
        }
        --cell_ptr;
        break;
      //Increase cell value
      case '+': ++cells[cell_ptr]; break;
      //Decrease cell value
      case '-': --cells[cell_ptr]; break;
      //Print cell value
      case '.': std::cout << cells[cell_ptr]; break;
      //Read cell value
      case ',': std::cin >> cells[cell_ptr]; break;
      //If cell value is zero jump to ] (-1)
      case '[': while (cells[cell_ptr] == 0 && program[++pc] != ']') {} break;
      //If cell value is not zero jump to [ (+1)
      case ']': while (cells[cell_ptr] != 0 && program[--pc] != '[') {} break;
    }
  }

  return 0;
}
