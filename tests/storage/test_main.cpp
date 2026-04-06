#define BOOST_TEST_MODULE "Vault Server Database Tests"

#include <filesystem>
#include <iostream>

#include <boost/test/unit_test.hpp>

// Global test fixture for database setup/cleanup
struct GlobalTestFixture
{
    GlobalTestFixture()
    {
        std::cout << "=== Starting Database Tests ===" << std::endl;

        // Create test directory if it doesn't exist
        std::error_code ec;
        std::filesystem::create_directories("./test_data", ec);
        if (ec)
        {
            std::cerr << "Warning: Could not create test_data directory: " << ec.message() << std::endl;
        }
        else
        {
            std::cout << "Test directory created/verified: "
                      << std::filesystem::absolute("./test_data") << std::endl;
        }

        // Also create directory using absolute path to be safe
        auto abs_path = std::filesystem::absolute("./test_data");
        std::filesystem::create_directories(abs_path, ec);
    }

    ~GlobalTestFixture()
    {
        std::cout << "=== Cleaning up test files ===" << std::endl;
        // Clean up test database files
        try
        {
            std::error_code ec;
            std::filesystem::remove("./test_data/test.db", ec);
            std::filesystem::remove("./test_data/test2.db", ec);
            std::filesystem::remove("./test_data/transaction_test.db", ec);
            std::filesystem::remove("./test_data/connection_test.db", ec);
            std::filesystem::remove("./test_data/statement_test.db", ec);
            std::filesystem::remove("./test_data/resultset_test.db", ec);
            std::filesystem::remove("./test_data/integration_test.db", ec);
            std::filesystem::remove("./test_data/concurrent_test.db", ec);
            std::filesystem::remove("./test_data/error_test.db", ec);
            std::filesystem::remove("./test_data/test_init.db", ec);
            std::filesystem::remove("./test_data/test_init_twice.db", ec);
            std::filesystem::remove("./test_data/test_execute.db", ec);
            std::filesystem::remove("./test_data/test_invalid.db", ec);
            std::filesystem::remove("./test_data/test_query.db", ec);
            std::filesystem::remove("./test_data/test_empty_query.db", ec);
            std::filesystem::remove("./test_data/test_transaction_success.db", ec);
            std::filesystem::remove("./test_data/test_transaction_rollback.db", ec);
            std::filesystem::remove("./test_data/test_get_connection.db", ec);
            std::filesystem::remove("./test_data/test_fk.db", ec);
            std::filesystem::remove("./test_data/test_factory.db", ec);
            std::filesystem::remove("./test_data/test_conn_string.db", ec);

            // Remove directory if empty
            std::filesystem::remove_all("./test_data", ec);
        }
        catch (...)
        {
            // Ignore cleanup errors
        }
    }
};

BOOST_GLOBAL_FIXTURE(GlobalTestFixture);
