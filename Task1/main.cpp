#include <iostream>
#include <limits>
#include <locale>
#include "my_vector.h"

int main() {
    std::setlocale(LC_ALL, "Russian");

    int rows;
    int cols;

    std::cout << "Введите число строк и столбцов: ";
    if (!(std::cin >> rows >> cols) || rows <= 0 || cols <= 0) {
        std::cerr << "Некорректные размеры матрицы";
        return 1;
    }

    int global_max = std::numeric_limits<int>::min();
    MyVector<std::size_t> rows_with_max;

    std::cout << "Введите все " << rows * cols << " элементов матрицы: ";

    for (std::size_t r = 0; r < rows; ++r) {
        bool row_has_max = false;
        for (std::size_t c = 0; c < cols; ++c) {
            int value = 0;
            if (!(std::cin >> value)) {
                std::cerr << "Ошибка чтения элементов матрицы" << std::endl;
                return 1;
            }

            if (value > global_max) {
                global_max = value;
                rows_with_max.clear();
                row_has_max = true;
            } else if (value == global_max) {
                row_has_max = true;
            }
        }
        if (row_has_max) rows_with_max.push_back(r);
    }

    std::cout << "Максимальный элемент - " << global_max << "\n" << "Строки с максимальным элементом: ";
    
    for (size_t i = 0; i < rows_with_max.size(); ++i) {
        std::cout << rows_with_max[i] << " ";
    }
}