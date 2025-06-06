#include <iostream>
#include "Block.hpp"


// Constructor
Block::Block(unsigned int id, const std::string& charstring) : id(id), payload(500, 0), checksum(0) {
    // Initialize payload and checksum                               //setting payload size to be 500 and initialize with zeros
    storePayload(charstring); // fill paylaod with the data from charstring and compute checksum
}

// Getters
unsigned int Block::getId() const {
    return id; // returns ID of the BlockFile
}

int Block::getChecksum() const {
    return checksum; // computes checksum
}

// Store payload and compute checksum
void Block::storePayload(const std::string& charstring) {
// iterates over up to 500 characters, assigning each char of charstring to payload

    // reset payload to zeros
    for (unsigned int i = 0; i < payload.size(); ++i) {
        payload[i] = 0;
    } 
    // copy charstring into payload
    for (unsigned int i = 0; i < charstring.size() && i < payload.size(); ++i) {
        payload[i] = charstring[i]; // assigning each charactor of charstring into payload array
    }
    checksum = computeChecksum(); // calculates and stroes the new checksum
    // Go to computeChecksum function to compute checksum of the charstring
}

// Corrupt payload without updating checksum
void Block::corruptPayload(const std::string& charstring) {
// clears payload to 0 to prepare for corrupt data insertion
    // reset payload to zeros in the file block before writing
    for (unsigned int i = 0; i < payload.size(); ++i) {
        payload[i] = 0;
    }

    // write the new charstring into the reset payload
    for (unsigned int i = 0; i < charstring.size() && i < payload.size(); ++i) {
        payload[i] = charstring[i];
    }
}

// Compute checksum with given formula in the project document 
int Block::computeChecksum() const {
    int sum = 0;
    int payloadsize = payload.size();
    for (int i = 0; i < payloadsize; ++i) { // computation of Sigma notation 
        sum += payload[i]; // add all the values 
    }
    
    return sum % 256; // computation of the checksum
}

// Validate checksum
bool Block::validateChecksum() const {
    return computeChecksum() == checksum; // Returns true if the checksums match(meaning no corruption)
}