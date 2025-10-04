#include "PmergeMe.hpp"

PmergeMe::PmergeMe()
{
    // constructor - containers are automatically initialized
}

PmergeMe::PmergeMe(const PmergeMe& other)
{
    _vector = other._vector;
    _deque = other._deque;
}

PmergeMe& PmergeMe::operator=(const PmergeMe& other)
{
    if (this != &other)
    {
        _vector = other._vector;
        _deque = other._deque;
    }
    return *this;
}

PmergeMe::~PmergeMe()
{
    // destructor - containers are automatically cleaned up
}

void PmergeMe::run(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Error" << std::endl;
        return;
    }
    
    try
    {
        parseArguments(argc, argv);
        
        // copy data to both containers
        _deque = std::deque<int>(_vector.begin(), _vector.end());
        
        // print before sorting
        printArray(_vector, "Before");
        
        // sort with vector and measure time
        std::clock_t start = std::clock();
        fordJohnsonVector(_vector);
        std::clock_t end = std::clock();
        double vectorTime = double(end - start) / CLOCKS_PER_SEC * 1000000; // convert to microseconds
        
        // sort with deque and measure time
        start = std::clock();
        fordJohnsonDeque(_deque);
        end = std::clock();
        double dequeTime = double(end - start) / CLOCKS_PER_SEC * 1000000; // convert to microseconds
        
        // print after sorting
        printArray(_vector, "After");
        
        // print timing results
        std::cout << "Time to process a range of " << _vector.size() 
                  << " elements with std::vector : " << std::fixed << std::setprecision(5) 
                  << vectorTime << " us" << std::endl;
        std::cout << "Time to process a range of " << _deque.size() 
                  << " elements with std::deque : " << std::fixed << std::setprecision(5) 
                  << dequeTime << " us" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error" << std::endl;
    }
}

void PmergeMe::parseArguments(int argc, char* argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        if (!isValidNumber(argv[i]))
        {
            throw std::runtime_error("Invalid number");
        }
        
        int num = std::atoi(argv[i]);
        if (num < 0)
        {
            throw std::runtime_error("Negative number");
        }
        
        _vector.push_back(num);
    }
}

bool PmergeMe::isValidNumber(const std::string& str) const
{
    if (str.empty())
        return false;
    
    // check if all characters are digits
    for (size_t i = 0; i < str.length(); ++i)
    {
        if (!std::isdigit(str[i]))
            return false;
    }
    
    return true;
}

void PmergeMe::printArray(const std::vector<int>& arr, const std::string& prefix) const
{
    std::cout << prefix << ": ";
    for (size_t i = 0; i < arr.size(); ++i)
    {
        std::cout << arr[i];
        if (i < arr.size() - 1)
            std::cout << " ";
    }
    std::cout << std::endl;
}

void PmergeMe::printArray(const std::deque<int>& arr, const std::string& prefix) const
{
    std::cout << prefix << ": ";
    for (size_t i = 0; i < arr.size(); ++i)
    {
        std::cout << arr[i];
        if (i < arr.size() - 1)
            std::cout << " ";
    }
    std::cout << std::endl;
}

// Ford-Johnson algorithm implementation for vector
void PmergeMe::fordJohnsonVector(std::vector<int>& arr)
{
    if (arr.size() <= 1)
        return;
    
    mergeInsertVector(arr, 0, arr.size() - 1);
}

void PmergeMe::mergeInsertVector(std::vector<int>& arr, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        
        // use insertion sort for small arrays
        if (right - left + 1 <= 10)
        {
            insertionSortVector(arr, left, right);
        }
        else
        {
            // recursively sort both halves
            mergeInsertVector(arr, left, mid);
            mergeInsertVector(arr, mid + 1, right);
            
            // merge the sorted halves
            mergeVector(arr, left, mid, right);
        }
    }
}

void PmergeMe::insertionSortVector(std::vector<int>& arr, int left, int right)
{
    for (int i = left + 1; i <= right; ++i)
    {
        int key = arr[i];
        int j = i - 1;
        
        while (j >= left && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void PmergeMe::mergeVector(std::vector<int>& arr, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    // create temporary arrays
    std::vector<int> leftArr(n1);
    std::vector<int> rightArr(n2);
    
    // copy data to temporary arrays
    for (int i = 0; i < n1; ++i)
        leftArr[i] = arr[left + i];
    for (int j = 0; j < n2; ++j)
        rightArr[j] = arr[mid + 1 + j];
    
    // merge the temporary arrays back
    int i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2)
    {
        if (leftArr[i] <= rightArr[j])
        {
            arr[k] = leftArr[i];
            i++;
        }
        else
        {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }
    
    // copy remaining elements
    while (i < n1)
    {
        arr[k] = leftArr[i];
        i++;
        k++;
    }
    
    while (j < n2)
    {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
}

// Ford-Johnson algorithm implementation for deque
void PmergeMe::fordJohnsonDeque(std::deque<int>& arr)
{
    if (arr.size() <= 1)
        return;
    
    mergeInsertDeque(arr, 0, arr.size() - 1);
}

void PmergeMe::mergeInsertDeque(std::deque<int>& arr, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        
        // use insertion sort for small arrays
        if (right - left + 1 <= 10)
        {
            insertionSortDeque(arr, left, right);
        }
        else
        {
            // recursively sort both halves
            mergeInsertDeque(arr, left, mid);
            mergeInsertDeque(arr, mid + 1, right);
            
            // merge the sorted halves
            mergeDeque(arr, left, mid, right);
        }
    }
}

void PmergeMe::insertionSortDeque(std::deque<int>& arr, int left, int right)
{
    for (int i = left + 1; i <= right; ++i)
    {
        int key = arr[i];
        int j = i - 1;
        
        while (j >= left && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void PmergeMe::mergeDeque(std::deque<int>& arr, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    // create temporary arrays
    std::deque<int> leftArr(n1);
    std::deque<int> rightArr(n2);
    
    // copy data to temporary arrays
    for (int i = 0; i < n1; ++i)
        leftArr[i] = arr[left + i];
    for (int j = 0; j < n2; ++j)
        rightArr[j] = arr[mid + 1 + j];
    
    // merge the temporary arrays back
    int i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2)
    {
        if (leftArr[i] <= rightArr[j])
        {
            arr[k] = leftArr[i];
            i++;
        }
        else
        {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }
    
    // copy remaining elements
    while (i < n1)
    {
        arr[k] = leftArr[i];
        i++;
        k++;
    }
    
    while (j < n2)
    {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
}
