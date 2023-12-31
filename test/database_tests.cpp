/**============================================================================
Name        : database_Tests.cpp
Created on  : 31.12.2023
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description :
============================================================================ **/

#include <boost/test/unit_test.hpp>
#include "Database.h"

namespace
{
    struct TestMovie: DB::TableEntry<TestMovie>
    {
        explicit TestMovie(std::string name): TableEntry (std::move(name)) {
        }
    };

    struct TestTheater: DB::TableEntry<TestTheater>
    {
        explicit TestTheater(std::string name): TableEntry (std::move(name)) {
        }
    };

}


BOOST_AUTO_TEST_SUITE(TableEntryTests)

    BOOST_AUTO_TEST_CASE(CreateBasicEntry)
    {
        const TestMovie movie {"TestMovie"};
        BOOST_CHECK_EQUAL(1, movie.id);
    }

    BOOST_AUTO_TEST_CASE(ValidateIDCounter_1)
    {
        const TestMovie movie {"TestMovie"};
        const TestTheater theater {"TestTheater"};

        BOOST_CHECK_EQUAL(2, movie.id);
        BOOST_CHECK_EQUAL(1, theater.id);
    }

    BOOST_AUTO_TEST_CASE(EqualityCheck)
    {
        const TestMovie movie1 {"TestMovie"}, movie2 {"TestMovie"};

        BOOST_REQUIRE(movie1 != movie2);
    }

    BOOST_AUTO_TEST_CASE(EqualityCheck_IdCheckOnly)
    {
        TestMovie movie1 {"TestMovie1"}, movie2 {"TestMovie2"};
        movie1.id = movie2.id;

        BOOST_REQUIRE(movie1 == movie2);
    }

BOOST_AUTO_TEST_SUITE_END()



BOOST_AUTO_TEST_SUITE(TableTests)

    BOOST_AUTO_TEST_CASE(CreateBasicEntry)
    {
        DB::Table<TestMovie> moviesTable;
        TestMovie* ptr = moviesTable.addEntry("TestMovie1");

        BOOST_CHECK_EQUAL("TestMovie1", ptr->name);
    }

    BOOST_AUTO_TEST_CASE(AddEntry_AndFindByID)
    {
        DB::Table<TestMovie> moviesTable;
        const TestMovie* orig = moviesTable.addEntry("TestMovie2");

        const std::optional<TestMovie*> result = moviesTable.findEntryByID(orig->id);

        BOOST_REQUIRE(result.has_value());
        BOOST_CHECK_EQUAL(result.value()->id, orig->id);
        BOOST_CHECK_EQUAL(result.value()->name, orig->name);
    }

    BOOST_AUTO_TEST_CASE(AddEntry_AndFindByName)
    {
        DB::Table<TestMovie> moviesTable;
        const TestMovie* orig = moviesTable.addEntry("TestMovie2");

        const std::optional<TestMovie*> result = moviesTable.findEntryName(orig->name);

        BOOST_REQUIRE(result.has_value());
        BOOST_CHECK_EQUAL(result.value()->id, orig->id);
        BOOST_CHECK_EQUAL(result.value()->name, orig->name);
    }

    BOOST_AUTO_TEST_CASE(AddEntry_GetAllRecords)
    {
        const std::vector<std::string> names {"One", "Two", "Three", "Four", "Five"};
        DB::Table<TestMovie> moviesTable;
        for (const auto& name: names)
            moviesTable.addEntry(name);

        const std::vector<TestMovie*> result = moviesTable.getAllEntries();

        BOOST_CHECK_EQUAL(result.size(), names.size());
        for (const auto& name: names)
        {
            const bool exits = std::any_of(result.cbegin(), result.cend(), [&](const auto& movie) {
                return movie->name == name;
            });
            BOOST_CHECK_EQUAL(exits, true);
        }
    }

BOOST_AUTO_TEST_SUITE_END()
