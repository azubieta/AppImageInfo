# appimage-info
Provides a convenient set of functions to retrieve information
contained in the AppImage files. By inspecting and merging the 
contents of the application ".Desktop" and "appstream.xml" files
into JSON formatted document.

It has two interfaces:
 - libappimage-info (pure C interface)
 - appimage-info (cli application)
 
An example of the JSON formatted output can be found [here](https://github.com/azubieta/appimage-info/blob/master/output-example.json).