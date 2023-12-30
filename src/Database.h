/**
 * @file       Database.h
 * @date       29.12.2023
 * @author     Andrei Tokmakov
 * @version    1.0
 * @copyright  Your copyright notice
 * @brief      Database modules implementation
 */

#ifndef BOOKINGSERVICE_DATABASE_H
#define BOOKINGSERVICE_DATABASE_H

#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
#include <optional>
#include <concepts>

//! Contains the definition and implementation of the basic building blocks of database behavior emulation
namespace DB
{
    struct NonCopyable
    {
        NonCopyable() = default;

        NonCopyable(const NonCopyable&) = delete;
        NonCopyable& operator=(const NonCopyable&) = delete;

        NonCopyable(const NonCopyable&&) noexcept = delete;
        NonCopyable& operator=(const NonCopyable&&) noexcept = delete;
    };

    /**
     * @brief It is a basic template class for entities stored in databases
     * @tparam T the TableEntry specialization
     */
    template<typename T>
    struct TableEntry: NonCopyable
    {
        std::string name;
        size_t id { 0 };

        explicit TableEntry(std::string name) : name{std::move(name)} {
            // First entry shall have ID = 1 not zero
            id = ++uniqueEntryId;
        }

        template<typename Type>
        friend bool operator==(const TableEntry<Type> &, const TableEntry<Type> &);

    private:

        static inline size_t uniqueEntryId {0};
    };


    template <typename Ty>
    concept TableEntryType = std::derived_from<Ty, TableEntry<Ty>>;


    /**
     * @brief The DataBase Table emulation class<br>
     * Under the hood, it is implemented in the form of two Hash Tables for searching by ID and name of the corresponding entity
     * @tparam T type of the Entry stored in Table
     */
    template<TableEntryType T>
    class Table
    {
        using EntryType = T;
        using EntryPointer = EntryType*;
        using SharedEntry = std::shared_ptr<EntryType>;

        // static_assert(!std::is_same_v<EntryType, void>, "ERROR: EntryType type can not be void");

        std::unordered_map<size_t, SharedEntry> entryTable{};
        std::unordered_map<std::string, SharedEntry> tableByName{};

    public:

        /**
          * Add corresponding DataBase EntryType record to the table <br>
          * the record is added to two tables at once, for searching by keys: std::string and size_t
          * @param name the entry name
         */
        void addEntry(const std::string &name)
        {
            SharedEntry objPtr = std::make_shared<EntryType>(name);
            entryTable.emplace(objPtr->id, objPtr);
            tableByName.emplace(objPtr->name, std::move(objPtr));
        }

        /**
          * Searches for an entry in the table by its name
          *
          * @param name the entry name
          * @return std::optional<T*> (std::nullopt in case if no entry was found in table)
          * @note Clients (callers) of the function shall not participate in lifecycle managing of <br>
          * the stored object - therefore returning <b>*T</b> instead of <b>std::shared_ptr<T></b>
         */
        std::optional<EntryPointer> findEntryName(const std::string &name) const noexcept {
            if (const auto it = tableByName.find(name); tableByName.end() != it)
                return std::make_optional<EntryPointer>(it->second.get());
            return std::nullopt;
        }

        /**
          * Searches for an entry in the table by its ID
          *
          * @param id the entry ID
          * @return std::optional<T*> (std::nullopt in case if no entry was found in table)
          * @note Clients (callers) of the function shall not participate in lifecycle managing of <br>
          * the stored object - therefore returning <b>*T</b> instead of <b>std::shared_ptr<T></b>
         */
        std::optional<EntryPointer> findEntryByID(size_t id) const noexcept {
            if (const auto it = entryTable.find(id); entryTable.end() != it)
                return std::make_optional<EntryPointer>(it->second.get());
            return std::nullopt;
        }

        /**
          * Return the list of records in table
          *
          * @param id the entry ID
          * @return the list (vector) of all data in table [std::vector<T*>]
          * @note Clients (callers) of the function shall not participate in lifecycle managing of <br>
          * the stored object - therefore returning <b>*T</b> instead of <b>std::shared_ptr<T></b>
         */
        [[nodiscard]]
        std::vector<EntryPointer> getAllEntries() const {
            std::vector<EntryPointer> allRecords;
            allRecords.reserve(entryTable.size());
            for (const auto &[id, obj]: entryTable)
                allRecords.push_back(obj.get());
            return allRecords;
        }
    };
}

#endif //BOOKINGSERVICE_DATABASE_H
