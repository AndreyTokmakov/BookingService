/**
 * @file       Database.cpp
 * @date       29.12.2023
 * @author     Andrei Tokmakov
 * @version    1.0
 * @copyright  Your copyright notice
 * @brief      Database modules implementation
 */

#include "Database.h"

namespace DB
{
    template<typename T>
    bool operator==(const TableEntry<T> &left, const TableEntry<T> &right) {
        return left.id == right.id;
    }
}