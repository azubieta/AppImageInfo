#pragma once

#include <nlohmann/json.hpp>

class BadFileFormat : public std::runtime_error {
public:
    explicit BadFileFormat(const std::string& __arg);
};

class BinaryMetadataExtractor {
    std::string target;
public:
    explicit BinaryMetadataExtractor(const std::string& target);

    nlohmann::json getMetadata();

protected:
    std::string getSha512CheckSum() const;

    std::string getBinaryArch() const;

    int64_t getFileSize() const;

    int getAppImageType() const;

    time_t getTime() const;

private:

    int16_t read_elf_e_machine_field(std::string file_path) const;

    std::string map_e_machine_code_to_string(int16_t e_machine) const;
};
