#include "HashStructure.hpp"
#include <iostream>

// Constructor and Destructor
HashStructure::HashStructure() : tableSize(0), isOpenAddressing(true) {table.resize(tableSize, nullptr); chains.resize(tableSize); }
                            // resize the array of table to tableSize; this way the array size will be changed whenever the value of tableSize changes

HashStructure::~HashStructure() {
    clearTable(); // call clearTable function to free out any dynamically allocated memory to avoid memory leak
}

// Primary hash function
int HashStructure::primaryHash(unsigned int key) const {
// Calculate primary has index by taking key modulo tableSize
    return key % tableSize; // gives the primary hash index
}

// Secondary hash function (for double hashing)
int HashStructure::secondaryHash(unsigned int key) const {
    int hash = (key / tableSize) % tableSize; // taking the remainder of the computation with tableSize
    // Ensure hash is odd -> means it's needed for double hashing
    return (hash % 2 == 0) ? hash + 1 : hash;
    
}

// Clear all blocks to prevent memory leaks
void HashStructure::clearTable() {
    if (isOpenAddressing) { // for oepn addressing 
        for (int i = 0; i < table.size(); ++i) {
            if (table[i] != nullptr) {
                delete table[i]; // delete the allocated Block vector for open addressing
                table[i] = nullptr; // set the pointer to nullptr after deleting the elements
            }
        }
        table.clear(); // Clear the entire vector 

    } else { // For separate chaining, has to be cleared twice since it's double hash.
        for (int i = 0; i < chains.size(); ++i) {
            for (int j = 0; j < chains[i].size(); ++j) {
                if (chains[i][j] != nullptr) {
                    delete chains[i][j]; // delete each block in the chain to free memory
                    chains[i][j] = nullptr;
                }
            }
            chains[i].clear(); // clear the inner vector of chain at index i
        }
        chains.clear(); // clear the outer vector of chains
    }
}

// Create new hash table
std::string HashStructure::NewHash(int size, bool useOpenAddressing) 
{   tableSize = size;
    isOpenAddressing = useOpenAddressing; // set addressing mode

    if (isOpenAddressing) {
        table.resize(tableSize, nullptr); // open addressing, table is resized to size and initilized to nullptr(indicates empty slots)
    } else {
        isOpenAddressing = false;
        chains.resize(tableSize); // resize separate chaining, initializing each slot with a new vector
    }
    return "success";
}


// Store a block in the hash table
std::string HashStructure::storeFileBlock(unsigned int id, const std::string& charstring) {
    int index = primaryHash(id); // calculate the primary hash index
    if (isOpenAddressing) { // for open addressing
        int step = secondaryHash(id); // calculate the step size using secondary hashing for probing
        // interates through the hash table to find an empty slot or a duplicate
        for (int i = 0; i < tableSize; ++i) {
            int pos = (index + i * step) % tableSize; // calculate probe position
            if (table[pos] == nullptr) {
                table[pos] = new Block(id, charstring); // store block in first available slot 
                return "success";
            } else if (table[pos]->getId() == id) { // duplicate of ID is found, insertion fails
                return "failure";
            }
        }
        return "failure"; // Table is full
    } else { // for separate chaining
        for (int i = 0; i < chains[index].size(); ++i) {
            if (chains[index][i]->getId() == id) { // insertion fails because the duplicate of the ID is found
                return "failure";
            }
        }
        chains[index].push_back(new Block(id, charstring)); // adds the new Block to chains[index] in the back of the array
        return "success";
    }
}

// Search for a block by ID
std::string HashStructure::searchFileBlock(unsigned int id) const {
    int index = primaryHash(id); // Use primary hashing to get index
    if (isOpenAddressing) { // for open addressing collision occurred
        int step = secondaryHash(id); // step size for probing
        for (int i = 0; i < tableSize; ++i) {
            int pos = (index + i * step) % tableSize; // calculate probe position
            if (table[pos] != nullptr && table[pos]->getId() == id) { // Block found at the probe position so return found message
                return "found " + std::to_string(id) + " in " + std::to_string(pos) ;
            }
        }
    } else { // for separate chaining

        for (int i = 0; i < chains[index].size(); ++i) { // searches through the chain at index; chains[index]
            if (chains[index][i]->getId() == id) { //Block found in chain
                return "found " + std::to_string(id) + " in " + std::to_string(index); // return found message
            }
        }
    }
    return "not found";
}

// Delete a block by ID
std::string HashStructure::deleteFileBlock(unsigned int id) {
    int index = primaryHash(id); // find index using primary hashing 
    if (isOpenAddressing) { // for open addressing
        int step = secondaryHash(id); // step size for probing
        for (int i = 0; i < tableSize; ++i) { // searches for the block
            int pos = (index + i * step) % tableSize;
            if (table[pos] != nullptr && table[pos]->getId() == id) { 
                delete table[pos];// if found, delete block and free memory 
                table[pos] = nullptr; // set the position to nullptr
                return "success";
            }
            
        }
    } else { // for separate chaining

        for(int i = 0; i < chains[index].size(); ++i) {
            if (chains[index][i]->getId() == id) { // find the block by ID
                delete chains[index][i]; // delete the block to free memory

                for (int j = i; j < chains[index].size() -1; ++j) { // shift elements to remove the found block
                    chains[index][j] = chains[index][j+1]; // setting the next element as the current one to shift elements to the front
                }    
                // remove the last element in the chain after shifting
                chains[index].pop_back();
                return "success";
            }
        }
    }
    return "failure"; // Block not found
}

// Corrupt a block by ID ; overwriting its payload without recalculating the checksum
std::string HashStructure::corruptFileBlock(unsigned int id, const std::string& charstring) {
    int index = primaryHash(id);
    if (isOpenAddressing) { // for open addressing
        int step = secondaryHash(id);
        for (int i = 0; i < tableSize; ++i) {
            int pos = (index + i * step) % tableSize;
            if (table[pos] != nullptr && table[pos]->getId() == id) { // searches for the block
                table[pos]->corruptPayload(charstring); // call corruptPayload function from Block file
                                                        // to overwrite its data without updating the checksum
                return "success";
            }
            
        }
    } else {// for separate chaining
    
        for (int i = 0; i < chains[index].size(); ++i) {
            if (chains[index][i] -> getId() == id) {  // Block found in chain
                chains[index][i] -> corruptPayload(charstring);// call corruptPayload function from Block.cpp
                                                                // to overwrite its data without updating the checksum
                return "success";
            }
        }
    }
    return "failure"; // Block not found
}

// Validate a block's checksum by ID
std::string HashStructure::validateFileBlock(unsigned int id) const {
    int index = primaryHash(id);
    if (isOpenAddressing) { // for open addressing
        int step = secondaryHash(id);
        for (int i = 0; i < tableSize; ++i) {
            int pos = (index + i * step) % tableSize;
            if (table[pos] != nullptr && table[pos]->getId() == id) {
                // return the appropriate statement based on the checksum comparison
                return table[pos]->validateChecksum() ? "valid" : "invalid";
            }
            
        }
    } else { // for separate chaining
        
        for (int i = 0; i < chains[index].size(); ++i) {
            if (chains[index][i]->getId() == id) { // Block found in chain
                return chains[index][i]->validateChecksum() ? "valid" : "invalid";
                // calling validateChecksum from the Block file
                // will resturn valie if there's no corruption and invalid when there's corruption
            }
        }
    }
    return "failure"; // Block not found
}

// Print chain (for separate chaining only)
// print all block IDs in the cain at a given index
std::string HashStructure::printChain(int index) const {
    if (index >= chains.size() || chains[index].empty()) {
        return "chain is empty"; // chain is empty or index out of bounds
    }
    std::string result;
    for (int i = 0; i < chains[index].size(); ++i) {
        result += std::to_string(chains[index][i]->getId()) + " ";
    }
    result.pop_back(); // Remove trailing space
    return result;
}