#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <vector>
#include <string>

class Block {
private:
    unsigned int id;
    std::vector<char> payload; // vector array of characters for payload
    int checksum; // computed checksum will be stored in this variable

public:
    // Constructor
    Block(unsigned int id, const std::string& charstring);

    // Getters
    unsigned int getId() const;
    int getChecksum() const;

    // Methods to manipulate payload and checksum
    void storePayload(const std::string& charstring);
    void corruptPayload(const std::string& charstring);
    int computeChecksum() const;
    bool validateChecksum() const;
};

#endif 