# appimage-info
Provides a convenient set of functions to retrieve information
about the application inside an AppImage file. By inspecting 
and merging the contents of the ".Desktop" and 
".appstream.xml" files into JSON formatted document.

It has two interfaces:
 - libappimage-info (pure [C interface](https://github.com/azubieta/appimage-info/blob/master/include/appimage/appimage-info.h))
 - appimage-info (cli application)
 
An example of the JSON formatted output can be found [here](https://github.com/azubieta/appimage-info/blob/master/output-example.json).

## Target
It's meant to be used by third party tools like software centers, 
file managers or others to improve the user experience when dealing
with AppImages.

## Requires/Depends
 - libstd++
 - boost algorithms 
 - boost filesystem 
 - boost program_options
 - libxml2 
 - libopenssl
 - binutils
 - libappimage 