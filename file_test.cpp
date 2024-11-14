
#include "..\UTILS\ANSI.hpp"

#include <fstream>
#include <iostream>

using byte      = unsigned char;
using byte_pair = unsigned short;

#define TYPE_BIT 0b10000000 // Tells int vs string

std::string interaction( std::istream& input = std::cin, std::ostream& output = std::cout )
{
    std::string temp;

    output << ANSI::TEXT::YELLOW << "(MyDB) " << ANSI::RESET;
    std::getline( input, temp );

    return temp;
}

int main()
{
    std::ofstream output( "File.txt" );

    output << std::string( 1000, '\0' );

    while ( interaction() != "quit" );

    return 1;
}