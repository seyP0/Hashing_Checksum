#include "HashStructure.hpp"
#include <iostream>
#include <vector>
#include <string>

int main() {
    
    HashStructure hashtable;  // Initialize the hash table structure

    std::string cmd;  // Variable to hold user commands

    // Main loop to process commands until "EXIT" is entered
    while (std::cin >> cmd) {
        
        // Command to create a new hash table with a specified size and addressing method
        if (cmd == "NEW") {
            int N; // Size of the hash table
            int T; // Addressing method: 0 for open addressing, 1 for separate chaining
            std::cin >> N >> T;
            bool useOpenAddressing = (T == 0);  // Determine addressing method
            std::cout << hashtable.NewHash(N, useOpenAddressing) << std::endl;

        } 
        // Command to store a new block in the hash table
        else if (cmd == "STORE") {
            unsigned int id;       // ID of the block to be stored
            std::string payload;   // Payload for the block
            std::cin >> id >> payload;
            
            // Remove trailing '!' from the payload if it exists
            if (!payload.empty() && payload.back() == '!') {
                payload.pop_back();
            }
            
            // Store the block in the hash table and output the result ("success" or "failure")
            std::cout << hashtable.storeFileBlock(id, payload) << std::endl;

        } 
        // Command to search for a block by ID
        else if (cmd == "SEARCH") {
            unsigned int id;  // ID of the block to search for
            std::cin >> id;
            
            // Search the block in the hash table and output the result (found location or "not found")
            std::cout << hashtable.searchFileBlock(id) << std::endl;

        } 
        // Command to delete a block by ID
        else if (cmd == "DELETE") {
            unsigned int id;  // ID of the block to delete
            std::cin >> id;
            
            // Delete the block from the hash table and output the result ("success" or "failure")
            std::cout << hashtable.deleteFileBlock(id) << std::endl;

        } 
        // Command to corrupt a block by overwriting its payload without recalculating the checksum
        else if (cmd == "CORRUPT") {
            unsigned int id;       // ID of the block to corrupt
            std::string payload;   // New corrupted payload for the block
            std::cin >> id >> payload;
            
            // Remove trailing '!' from the corrupted payload if it exists
            if (!payload.empty() && payload.back() == '!') {
                payload.pop_back();
            }
            
            // Corrupt the block's payload and output the result ("success" or "failure")
            std::cout << hashtable.corruptFileBlock(id, payload) << std::endl;

        } 
        // Command to validate the checksum of a block by ID
        else if (cmd == "VALIDATE") {
            unsigned int id;  // ID of the block to validate
            std::cin >> id;
            
            // Validate the block's checksum and output the result ("valid" or "invalid")
            std::cout << hashtable.validateFileBlock(id) << std::endl;

        } 
        // Command to print all block IDs in a chain at a given index (for separate chaining only)
        else if (cmd == "PRINT") {
            int index;  // Index of the chain to print
            std::cin >> index;
            
            // Print the IDs in the specified chain or "chain is empty" if the chain has no elements
            std::cout << hashtable.printChain(index) << std::endl;

        } 
        // Command to exit the program
        else if (cmd == "EXIT") {
            break;  // Exit the main loop and end the program
        }
    }

    return 0;
}