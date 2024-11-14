#ifndef DATABASE_HPP
#define DATABASE_HPP

#include "./dataset.hpp"

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <type_traits>

namespace DB
{
    template <class _Id, class _Data>
    struct database
    {
        using data_set          =   DB::dataset<_Id, _Data>;
        using id_type           =   typename data_set::id_type;
        using data_type         =   typename data_set::data_type;
        using data_storage      =   std::map<id_type, data_type>;

        data_storage data_base;
        header_set headers;
        std::fstream data_file;

        void check_file_open()
        {
            if (!data_file.is_open())
                throw std::runtime_error("No file is currently open to read from!");
        }

        [[nodiscard]] bool validate_header(header_set _headers)
        {

            if (_headers.size() < headers.size()) return false;

            for (size_t index = 0; index < headers.size(); ++index)
                if (_headers[index] != headers[index]) return false;
            
            return true;
        }
        [[nodiscard]] header_set parse_header(const std::string& _line, char separator = ',')
        {
            std::string temp;
            std::stringstream header_line(_line);
            header_set headers;
            while (std::getline(header_line, temp, separator)) headers.push_back(temp);
            return headers;
        }

        void print_header(std::ostream& output, char separator = ',')
        {
            for (size_t index = 0; index < headers.size(); ++index)
                output << headers[index] << (index == headers.size() - 1 ? '\n' : separator);
        }

        virtual header_set determine_headers_manually()
        {
            header_set headers = data_type::determine_headers();
            std::string id_header;
            std::cout << "Please type the ID header:\n";
            std::getline(std::cin, id_header);
            headers.emplace(headers.begin(), id_header);
            headers.push_back("End-of-Line Indicator");
            return headers;
        }
        virtual header_set get_headers_from_file(char separator = ',')
        {
            check_file_open();

            std::string line;
            std::getline(data_file, line);
            return parse_header(line);
        }

        database() : data_base(), headers(), data_file() {}
        database(const database&) = delete;
        database(database&&) = default;
        database& operator =(const database&) = delete;
        database& operator =(database&&) = default;

        void determine_headers(bool from_file)
        {
            headers = (from_file ? get_headers_from_file() : determine_headers_manually());
        }
        void modify_headers()
        {
            while (true)
            {
                size_t selection;
                print_header(std::cout);

                std::cout << "\nWhich header would you like to change? (1 -";
                std::cout << headers.size() << ", type 0 to exit)";
                std::cin >> selection;
                if (selection == 0) return;

                if (selection-- > headers.size())
                {
                    std::cout << "That choice was invalid.";
                    continue;
                }

                std::cout << "The header you have selected is \"" << headers[selection] << "\"\n";
                std::cout << "Please type the value you wish to replace it with:\n";
                std::cin >> headers[selection];
            }
        }
        void print(char separator = ',', char line_break = '#')
        {
            check_file_open();

            print_header(data_file, separator);

            for ( typename data_storage::value_type pair : data_base )
            {
                data_set temp(pair.first, pair.second);
                temp.print(data_file, separator);
                data_file << line_break << std::endl;
            }
        }
        void read_data(char separator = ',', char line_break = '#')
        {
            std::string line;
            try
            {
                while (std::getline(data_file, line))
                {
                    std::stringstream readable(line.substr(0, line.find(line_break)));
                    data_set temp;
                    temp.read(readable, separator);
                    data_base[temp.get_id()] = temp.get_data();
                }
            }
            catch(...)
            {
                std::cerr << "There was an error reading in your data, it appears the file is incorrectly formatted.";
            }
        }
        bool read(char separator = ',', char line_break = '#')
        {
            if (!validate_header(get_headers_from_file(data_file))) return false; // Failed parse/read, return false.

            read_data(line_break, separator);
            return true;
        }
        bool set_file(const char* filename)
        {
            data_file.open(filename);
            return data_file.is_open();
        }
    };
} // namespace DB

#endif // DB_DATABASE_HPP