//
// Created by alexis on 6/5/18.
//

#include "Application.h"
Application::Application() { }
const string& Application::getRelease_tag() const
{
    return release_tag;
}
void Application::setRelease_tag(const string& release_tag)
{
    Application::release_tag = release_tag;
}
const string& Application::getArchitecture() const
{
    return architecture;
}
void Application::setArchitecture(const string& architecture)
{
    Application::architecture = architecture;
}
const vector<string>& Application::getLanguages() const
{
    return languages;
}
void Application::setLanguages(const vector<string>& languages)
{
    Application::languages = languages;
}
const map<string, string>& Application::getName() const
{
    return name;
}
void Application::setName(const map<string, string>& name)
{
    Application::name = name;
}
const map<string, string>& Application::getDescription() const
{
    return description;
}
void Application::setDescription(const map<string, string>& description)
{
    Application::description = description;
}

