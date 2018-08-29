//
// Created by alexis on 8/14/18.
//


#include "appimage/info.h"
#include "../entities/FileMetadataExtractor.h"

void extract_desktop_file(const char *appimage_path, const char *output_path) {
    FileMetadataExtractor extractor(appimage_path);
    extractor.extractDesktopFile(output_path);
}

void extract_appstream_file(const char *appimage_path, const char *output_path) {
    FileMetadataExtractor extractor(appimage_path);
    extractor.extractAppStreamFile(output_path);
}

void extract_appinamge_icon_file(const char *appimage_path, const char *output_path, const char *size) {
    FileMetadataExtractor extractor(appimage_path);
    extractor.extractIconFile(output_path, size);
}

char *extract_appinamge_info(const char *appimage_path) {
    FileMetadataExtractor extractor(appimage_path);
    auto metadata = extractor.extractMetadata();
    char *out = strdup(metadata.dump().c_str());
    return out;
}
