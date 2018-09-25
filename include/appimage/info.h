#ifndef APPIMAGE_INFO_APPIMAGE_INFO_H
#define APPIMAGE_INFO_APPIMAGE_INFO_H

#ifdef __cplusplus
extern "C" {
#endif
#include <appimage/info-config.h>

void appimage_extract_desktop_file(const char *appimage_path, const char *output_path);

void appimage_extract_appstream_file(const char *appimage_path, const char *output_path);

/* Extracts the AppImage Icon.
 *
 * IMPORTANT: No image manipulation will be performed. This will only output the icon in the required
 * size if available at usr/share/icon/hicolor/<size> or it will fallback to the .DirIcon
 * */
void appimage_extract_icon_file(const char* appimage_path, const char* output_path, const char* size = "256");

/* Combines contents from the application '.Desktop' file, the 'appstream.xml' file and information
 * about the binary file.
 *
 * Returns: JSon formatted string
 * */
char *appimage_extract_info(const char *appimage_path);

#ifdef __cplusplus
}
#endif

#endif //APPIMAGE_INFO_APPIMAGE_INFO_H
