#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

class BitcoinExchange
{
private:
    std::map<std::string, double> _database; // map to store date->price pairs
    
    // private helper methods
    bool isValidDate(const std::string& date) const;
    bool isValidValue(const std::string& value) const;
    double parseValue(const std::string& value) const;
    std::string findClosestDate(const std::string& date) const;
    void loadDatabase(const std::string& filename);
    void processInputFile(const std::string& filename);
    void processLine(const std::string& line);

public:
    // orthodox canonical form
    BitcoinExchange();
    BitcoinExchange(const BitcoinExchange& other);
    BitcoinExchange& operator=(const BitcoinExchange& other);
    ~BitcoinExchange();
    
    // main method to run the program
    void run(const std::string& inputFile);
};

#endif
