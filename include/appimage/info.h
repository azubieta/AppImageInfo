#ifndef APPIMAGE_INFO_APPIMAGE_INFO_H
#define APPIMAGE_INFO_APPIMAGE_INFO_H

#ifdef __cplusplus
extern "C" {
#endif

void extract_desktop_file(const char *appimage_path, const char *output_path);

void extract_appstream_file(const char *appimage_path, const char *output_path);

/* Extracts the AppImage Icon.
 *
 * IMPORTANT: No image manipulation will be performed. This will only output the icon in the required
 * size if available at usr/share/icon/hicolor/<size> or it will fallback to the .DirIcon
 * */
void extract_appinamge_icon_file(const char *appimage_path, const char *output_path, const char *size = "256");

/* Combines contents from the application '.Desktop' file and the 'appstream.xml' file
 *
 * Returns: JSon formatted string
 * */
char *extract_appinamge_info(const char *appimage_path);

#ifdef __cplusplus
}
#endif

#endif //APPIMAGE_INFO_APPIMAGE_INFO_H
