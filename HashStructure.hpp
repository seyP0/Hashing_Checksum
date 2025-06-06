#ifndef HASHSTRUCTURE_HPP
#define HASHSTRUCTURE_HPP

#include "Block.hpp"
#include <vector>
#include <string>

class HashStructure {
private:
    int tableSize;
    bool isOpenAddressing;
    std::vector<Block*> table; // for open addressing, Array using vector; Use nullptr for empty slots in open addressing
    std::vector<std::vector<Block*>> chains; // for separate chaining

    // Primary and secondary hash functions
    int primaryHash(unsigned int key) const;
    int secondaryHash(unsigned int key) const;

    // Helper function to clear all file blocks and prevent memory leaks
    void clearTable();

public:
    // Constructor and Destructor
    HashStructure();
    ~HashStructure();

    // Hash table operations
    std::string NewHash(int size, bool useOpenAddressing);
    std::string storeFileBlock(unsigned int id, const std::string& charstring);
    std::string searchFileBlock(unsigned int id) const;
    std::string deleteFileBlock(unsigned int id);
    std::string corruptFileBlock(unsigned int id, const std::string& charstring);
    std::string validateFileBlock(unsigned int id) const;

    // Separate chaining specific
    std::string printChain(int index) const; // For separate chaining
};

#endif // HASHSTRUCTURE_HPP

