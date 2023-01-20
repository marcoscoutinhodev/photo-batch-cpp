#pragma once

#include <string>
#include <memory>
#include <filesystem>
#include <vector>

class Mode {
  public:
    Mode(const std::string& folder, const std::string& filter);

    const std::string& GetFolder() const;
    const std::string& GetFilter() const;

    virtual const std::string& GetModeName() const = 0; // '= 0' tells to be implemented in the base class

    void Run();

  protected:
    virtual void RunImpl() = 0;
    std::vector<std::filesystem::path> GetFiles(const std::filesystem::path& extension = "") const;

  private:
    std::string m_Folder;
    std::string m_Filter;
};

class ArgumentParser;

namespace Args {
  namespace Flags {
    static const std::string Rename = "rename"; // constexpr const char*
    static const std::string Convert = "convert"; // constexpr const char*
    static const std::string Resize = "resize"; // constexpr const char*
    static const std::string Scale = "scale"; // constexpr const char*
    static const std::string Help = "help"; // constexpr const char*
  }

  namespace Options {
    static constexpr const char* Folder = "folder";
    static constexpr const char* Filter = "filter";
    static constexpr const char* Width = "width";
    static constexpr const char* Height = "height";
    static constexpr const char* Amount = "amount";
    static constexpr const char* Prefix = "prefix";
    static constexpr const char* StartIndex = "startindex";
    static constexpr const char* From = "from";
    static constexpr const char* To = "to";
  }
}

Mode* CreateMode(const ArgumentParser& argParser);