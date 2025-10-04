#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange()
{
    // load the default database file
    loadDatabase("data.csv");
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other)
{
    _database = other._database;
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
    if (this != &other)
    {
        _database = other._database;
    }
    return *this;
}

BitcoinExchange::~BitcoinExchange()
{
    // destructor - no dynamic memory to clean up
}

void BitcoinExchange::run(const std::string& inputFile)
{
    processInputFile(inputFile);
}

void BitcoinExchange::loadDatabase(const std::string& filename)
{
    std::ifstream file(filename.c_str());
    if (!file.is_open())
    {
        throw std::runtime_error("Error: could not open database file");
    }
    
    std::string line;
    // skip the header line
    std::getline(file, line);
    
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string date, price;
        
        if (std::getline(iss, date, ',') && std::getline(iss, price))
        {
            _database[date] = parseValue(price);
        }
    }
    file.close();
}

void BitcoinExchange::processInputFile(const std::string& filename)
{
    std::ifstream file(filename.c_str());
    if (!file.is_open())
    {
        std::cout << "Error: could not open file." << std::endl;
        return;
    }
    
    std::string line;
    // skip the header line
    std::getline(file, line);
    
    while (std::getline(file, line))
    {
        processLine(line);
    }
    file.close();
}

void BitcoinExchange::processLine(const std::string& line)
{
    std::istringstream iss(line);
    std::string date, value;
    
    if (std::getline(iss, date, '|') && std::getline(iss, value))
    {
        // trim whitespace from date and value
        date.erase(0, date.find_first_not_of(" \t"));
        date.erase(date.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);
        
        if (!isValidDate(date))
        {
            std::cout << "Error: bad input => " << date << std::endl;
            return;
        }
        
        if (!isValidValue(value))
        {
            return; // error message already printed in isValidValue
        }
        
        double amount = parseValue(value);
        std::string closestDate = findClosestDate(date);
        double rate = _database[closestDate];
        double result = amount * rate;
        
        std::cout << date << " => " << amount << " = " << result << std::endl;
    }
    else
    {
        std::cout << "Error: bad input => " << line << std::endl;
    }
}

bool BitcoinExchange::isValidDate(const std::string& date) const
{
    if (date.length() != 10)
        return false;
    
    // check format: YYYY-MM-DD
    if (date[4] != '-' || date[7] != '-')
        return false;
    
    // check if all characters except dashes are digits
    for (size_t i = 0; i < date.length(); ++i)
    {
        if (i != 4 && i != 7 && !std::isdigit(date[i]))
            return false;
    }
    
    // parse year, month, day
    int year = std::atoi(date.substr(0, 4).c_str());
    int month = std::atoi(date.substr(5, 2).c_str());
    int day = std::atoi(date.substr(8, 2).c_str());
    
    // basic validation
    if (year < 2009 || year > 2023) // bitcoin started in 2009
        return false;
    if (month < 1 || month > 12)
        return false;
    if (day < 1 || day > 31)
        return false;
    
    return true;
}

bool BitcoinExchange::isValidValue(const std::string& value) const
{
    if (value.empty())
    {
        std::cout << "Error: not a positive number." << std::endl;
        return false;
    }
    
    // check for negative numbers
    if (value[0] == '-')
    {
        std::cout << "Error: not a positive number." << std::endl;
        return false;
    }
    
    // check if it's a valid number
    char* end;
    double num = std::strtod(value.c_str(), &end);
    
    if (*end != '\0')
    {
        std::cout << "Error: not a positive number." << std::endl;
        return false;
    }
    
    if (num < 0)
    {
        std::cout << "Error: not a positive number." << std::endl;
        return false;
    }
    
    if (num > 1000)
    {
        std::cout << "Error: too large a number." << std::endl;
        return false;
    }
    
    return true;
}

double BitcoinExchange::parseValue(const std::string& value) const
{
    return std::strtod(value.c_str(), NULL);
}

std::string BitcoinExchange::findClosestDate(const std::string& date) const
{
    // if exact date exists, return it
    if (_database.find(date) != _database.end())
    {
        return date;
    }
    
    // find the closest lower date
    std::map<std::string, double>::const_iterator it = _database.lower_bound(date);
    
    // if lower_bound returns end, use the last element
    if (it == _database.end())
    {
        return (--it)->first;
    }
    
    // if lower_bound returns the first element and it's greater than date,
    // we can't find a valid date
    if (it == _database.begin())
    {
        return it->first; // use the first available date
    }
    
    // return the previous element (closest lower date)
    return (--it)->first;
}
