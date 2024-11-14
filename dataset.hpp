#pragma once
#ifndef DATASET_HPP
#define DATASET_HPP

#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

typedef std::vector<std::string> header_set;

namespace DB
{
    template <class _Id, class _Data>
    class database;
    
    template <class _Id, class _Data>
    class dataset
    {
        friend class database<_Id, _Data>;

    public:
        using id_type             =   _Id;
        using id_rvalue           =   _Id&&;
        using id_reference        =   _Id&;
        using id_pointer          =   _Id*;
        using id_const_reference  =   const _Id&;
        using id_const_pointer    =   const _Id*;

        using data_type            =   _Data;
        using data_rvalue          =   _Data&&;
        using data_reference       =   _Data&;
        using data_pointer         =   _Data*;
        using data_const_reference =   const _Data&;
        using data_const_pointer   =   const _Data*;
        
    private:
        id_type ID;
        data_type value;

        const static header_set determine_headers()
        {
            std::string id_header;
            std::cout << "What is the header for the ID?\n";
            std::cin >> id_header;
            header_set headers = data_type::determine_headers();
            headers.emplace(headers.begin(), id_header);
            return headers;
        }

    public:
        dataset() = default;
        dataset(dataset&) = delete;
        dataset(const dataset&) = delete;
        dataset(dataset&&) = default;
        dataset(id_const_reference _id) : ID(_id), value() {}
        dataset(id_rvalue _id) : ID(_id), value() {}
        dataset(id_const_reference _id, data_const_reference _value) : ID(_id), value(_value) {}
        dataset(id_rvalue _id, data_const_reference _value) : ID(_id), value(_value) {}
        dataset(id_const_reference _id, data_rvalue _value) : ID(_id), value(_value) {}
        dataset(id_rvalue _id, data_rvalue _value) : ID(_id), value(_value) {}

        id_const_reference get_id() const { return ID; }
        data_reference get_data_reference() { return value; }
        data_const_reference get_data() const { return value; }

        virtual void print(std::ostream& output, char delim = ',') const
        {
            output << ID << delim;
            value.print(output, delim);
        }
        virtual void read(std::istream& input, char delim = ',')
        {
            std::string id_str;
            std::getline(input, id_str, delim);
            std::stringstream(id_str) >> ID;
            value.read(input, delim);
        }
    };
} // namespace DB


#endif // DATASET_HPP