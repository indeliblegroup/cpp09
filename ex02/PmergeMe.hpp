#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <ctime>
#include <iomanip>

class PmergeMe
{
private:
    std::vector<int> _vector; // first container for sorting
    std::deque<int> _deque;   // second container for sorting
    
    // ford-johnson algorithm implementation for vector
    void fordJohnsonVector(std::vector<int>& arr);
    void mergeInsertVector(std::vector<int>& arr, int left, int right);
    void insertionSortVector(std::vector<int>& arr, int left, int right);
    void mergeVector(std::vector<int>& arr, int left, int mid, int right);
    
    // ford-johnson algorithm implementation for deque
    void fordJohnsonDeque(std::deque<int>& arr);
    void mergeInsertDeque(std::deque<int>& arr, int left, int right);
    void insertionSortDeque(std::deque<int>& arr, int left, int right);
    void mergeDeque(std::deque<int>& arr, int left, int mid, int right);
    
    // helper methods
    bool isValidNumber(const std::string& str) const;
    void parseArguments(int argc, char* argv[]);
    void printArray(const std::vector<int>& arr, const std::string& prefix) const;
    void printArray(const std::deque<int>& arr, const std::string& prefix) const;

public:
    // orthodox canonical form
    PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();
    
    // main method to run the program
    void run(int argc, char* argv[]);
};

#endif
