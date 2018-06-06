//
// Created by alexis on 6/5/18.
//

#ifndef APPIMAGE_RELEASES_INDEXER_APPLICATION_H
#define APPIMAGE_RELEASES_INDEXER_APPLICATION_H

#include <string>
#include <map>
#include <vector>

using namespace std;
class Application {
    string id;
    string release_tag;
    string architecture;
    vector<string> languages;
    std::map<string, string> name;
    std::map<string, string> description;

public:
    Application();
    const string& getRelease_tag() const;
    void setRelease_tag(const string& release_tag);
    const string& getArchitecture() const;
    void setArchitecture(const string& architecture);
    const vector<string>& getLanguages() const;
    void setLanguages(const vector<string>& languages);
    const map<string, string>& getName() const;
    void setName(const map<string, string>& name);
    const map<string, string>& getDescription() const;
    void setDescription(const map<string, string>& description);
};

#endif //APPIMAGE_RELEASES_INDEXER_APPLICATION_H
