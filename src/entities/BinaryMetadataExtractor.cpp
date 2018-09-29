//
// Created by alexis on 6/8/18.
//

extern "C" {
#include <sys/stat.h>
#include <time.h>
}

#include <iostream>
#include <fstream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <appimage/appimage.h>
#include <openssl/sha.h>
#include <nlohmann/json.hpp>
#include "BinaryMetadataExtractor.h"

using namespace std;

BinaryMetadataExtractor::BinaryMetadataExtractor(const std::string& target) : target(target) {

}

nlohmann::json BinaryMetadataExtractor::getMetadata() {
    nlohmann::json data;
    data["architecture"] = getBinaryArch();
    data["sha512checksum"] = getSha512CheckSum();
    data["size"] = getFileSize();
    data["type"] = getAppImageType();
    data["date"] = getTime();

    return data;
}

time_t BinaryMetadataExtractor::getTime() const {
    struct stat t_stat;
    stat(target.c_str(), &t_stat);
    return t_stat.st_mtim.tv_sec;
}

int64_t BinaryMetadataExtractor::getFileSize() const {
    struct stat statbuf{};

    if (stat(target.c_str(), &statbuf) == -1) {
        /* check the value of errno */
    }


    return statbuf.st_size;
}

std::string BinaryMetadataExtractor::getSha512CheckSum() const {
    FILE* file = fopen(target.c_str(), "r");
    if (file) {
        SHA512_CTX sha512;
        SHA512_Init(&sha512);
        uint8_t buffer[1024];

        auto bc = fread(buffer, sizeof(uint8_t), 1024, file);
        while (bc) {
            SHA512_Update(&sha512, buffer, static_cast<size_t>(bc));
            bc = fread(buffer, sizeof(uint8_t), 1024, file);
        }

        unsigned char hash[SHA512_DIGEST_LENGTH];
        SHA512_Final(hash, &sha512);

        char mdString[SHA512_DIGEST_LENGTH * 2 + 1];
        for (int i = 0; i < SHA512_DIGEST_LENGTH; i++)
            sprintf(&mdString[i * 2], "%02x", (unsigned int) hash[i]);

        std::string out = mdString;
        return out;
    }

    return "";
}

std::string BinaryMetadataExtractor::getBinaryArch() const {
    auto e_machine = read_elf_e_machine_field(target);
    auto e_machine_str = map_e_machine_code_to_string(e_machine);
    return e_machine_str;
}

int BinaryMetadataExtractor::getAppImageType() const {
    return appimage_get_type(target.c_str(), false);
}

int16_t BinaryMetadataExtractor::read_elf_e_machine_field(std::string file_path) const {
    int16_t e_machine = 0x00;
    ifstream file(file_path, ios_base::in | ios_base::binary | ios_base::ate);
    if (file.is_open()) {
        file.seekg(0x12, ios_base::beg);
        file.read(reinterpret_cast<char*>(&e_machine), 0x02);

        file.close();

    } else throw BadFileFormat("Unable to open file.");
    return e_machine;

}

std::string BinaryMetadataExtractor::map_e_machine_code_to_string(int16_t e_machine) const {
    static std::map<int16_t, std::string> map = {
        {0x02, "SPARC"},
        {0x03, "i386"},
        {0x08, "MIPS"},
        {0x14, "PowerPC"},
        {0x16, "S390"},
        {0x28, "ARM"},
        {0x2A, "SuperH"},
        {0x32, "IA-64"},
        {0x3E, "x86_64"},
        {0xB7, "AArch64"},
        {0xF3, "RISC-V"}
    };

    if (map.find(e_machine) != map.end())
        return map[e_machine];
    else return "unknown";

}


BadFileFormat::BadFileFormat(const std::string& __arg) : runtime_error(__arg) {}
