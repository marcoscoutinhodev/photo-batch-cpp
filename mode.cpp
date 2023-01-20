#include <iostream>
#include <filesystem>
#include <algorithm>
#include <chrono>
#include <vector>

#include "mode.h"
#include "argument-parser.h"
#include "utils.h"
#include "rename-mode.h"
#include "convert-mode.h"
#include "resize-mode.h"
#include "scale-mode.h"

/**
 * Alternative...
 * can use Mode::Mode(const std::string& folder, const std::string& filter):
 *  m_Folder{ folder },
 *  m_Filder{ filter } {}
 * */
Mode::Mode(const std::string& folder, const std::string& filter) {
  this->m_Folder = folder;
  this->m_Filter = filter;
}

const std::string& Mode::GetFolder() const {
  return this->m_Folder;
}

const std::string& Mode::GetFilter() const {
  return this->m_Filter;
}

void Mode::Run() {
  using ClockT = std::chrono::high_resolution_clock;

  ClockT::time_point startTime = ClockT::now();
  this->RunImpl();
  ClockT::time_point endTime = ClockT::now();

  ClockT::duration elapsedTime = endTime - startTime;
  std::chrono::milliseconds elapsedTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime);

  std::cout << this->GetModeName() << "Duration of operation : " << elapsedTimeMs.count() << "ms" << std::endl;
}

const std::string& GetInvalidCharacters() {
  static const std::string invalidCharacteres = "\\/*?\"<>|";
  return invalidCharacteres; 
}

bool HasInvalidCharactere(const std::string& str) {
  if (str.find_first_of(GetInvalidCharacters()) != std::string::npos)
    return true;

  return false;
}

Mode* CreateMode(const ArgumentParser& argParser) {
  const bool bRenameMode = argParser.GetFlag(Args::Flags::Rename);
  const bool bConvertMode = argParser.GetFlag(Args::Flags::Convert);
  const bool bResizeMode = argParser.GetFlag(Args::Flags::Resize);
  const bool bScaleMode = argParser.GetFlag(Args::Flags::Scale);
  const bool bHelpMode = argParser.GetFlag(Args::Flags::Help);

  const std::array<bool, 5> modes = {
    bRenameMode, bConvertMode, bResizeMode, bScaleMode, bHelpMode,
  };
  int activeModeCount = std::count(std::begin(modes), std::end(modes), true);

  // if (!(bRenameMode
  //   ^ bConvertMode
  //   ^ bResizeMode
  //   ^ bScaleMode
  // )) { alternative using XOR instead of array
  if (activeModeCount != 1) {
    if (activeModeCount == 0)
      throw std::invalid_argument("A flag must be provided");

    throw std::invalid_argument("Only one flag can be active");
  }
  
  const std::string folder = argParser.GetOptionAs<const std::string&>(Args::Options::Folder);

  if (folder.empty())
    throw std::invalid_argument("Folder must be provided");

  if (!std::filesystem::exists(folder))
    throw std::invalid_argument("The provided folder doesn't exists");

  const std::string filter = argParser.GetOptionAs<const std::string&>(Args::Options::Filter);

  if (!filter.empty()) {
    const bool prefixHasInvalidCharacter = HasInvalidCharactere(filter);
    
    if (prefixHasInvalidCharacter)
      throw std::invalid_argument("The provided filter has invalid character: " + GetInvalidCharacters());
  }

  if (bResizeMode || bScaleMode)
    if (filter.empty())
      throw std::invalid_argument("The filter must be provided in this mode");

  if (bResizeMode) {
    const int width = argParser.GetOptionAs<int>(Args::Options::Width);
    const int height = argParser.GetOptionAs<int>(Args::Options::Height);

    if (width <= 0 || height <= 0)
      throw std::invalid_argument("Width and height must be greater than or equal to 1");

    return new ResizeMode{folder, filter, width, height};
  }

  if (bScaleMode) {
    const float amount = argParser.GetOptionAs<float>(Args::Options::Amount);

    if (amount <= 0)
      throw std::invalid_argument("Amount must be greater than or equal to 1");

    return new ScaleMode{folder, filter, amount};
  }

  if (bRenameMode) {
    const std::string prefix = argParser.GetOptionAs<const std::string&>(Args::Options::Prefix);
    const bool prefixHasInvalidCharacter = HasInvalidCharactere(prefix);

    if (prefix.empty())
      throw std::invalid_argument("The prefix must be provided in 'rename' mode");

    if (prefixHasInvalidCharacter)
      throw std::invalid_argument("The provided prefix has invalid character: " + GetInvalidCharacters());

    int startIndex = argParser.GetOptionAs<int>(Args::Options::StartIndex);

    if (startIndex < 0)
      throw std::invalid_argument("The starting index must be provided and can't be negative");

    return new RenameMode{folder, filter, prefix, startIndex};
  }

  if (bConvertMode) {
    const std::string from = argParser.GetOptionAs<const std::string&>(Args::Options::From);
    const std::string to = argParser.GetOptionAs<const std::string&>(Args::Options::To);

    const std::array<std::string, 2> convertOptions = { "jpg", "png" };

    if (from.empty())
      throw std::invalid_argument("Field 'from' must be provided");
    
    if (to.empty())
      throw std::invalid_argument("Field 'to' must be provided");

    const bool bIsFromValid = std::find(std::begin(convertOptions), std::end(convertOptions), from) != std::end(convertOptions);
    const bool bIsToValid = std::find(std::begin(convertOptions), std::end(convertOptions), to) != std::end(convertOptions);

    if (!bIsFromValid || !bIsToValid)
      throw std::invalid_argument("Field 'from' and/or 'to' is invalid");

    if (from == to)
      throw std::invalid_argument("Fields 'from' and 'to' cannot be the same");

    const std::map<std::string, ConvertMode::Format> convertOptionsMap = {
      { "jpg", ConvertMode::Format::JPG },
      { "png", ConvertMode::Format::PNG },
    };

    return new ConvertMode(folder, filter, convertOptionsMap.at(from), convertOptionsMap.at(to));
  }

  return NULL;
}

std::vector<std::filesystem::path> Mode::GetFiles(const std::filesystem::path& extension) const {
  std::vector<std::filesystem::path> files;
  int totalFiles = 0;

  for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(this->GetFolder())) {
    const bool bIsFile = std::filesystem::is_regular_file(entry);
    const bool bMatchFilter = this->GetFilter().empty()
      || (entry.path().string().find(this->GetFilter()) != std::string::npos);
    const bool bMatchExtension = extension.empty()
      || (entry.path().extension() == extension);

    if (bIsFile && bMatchFilter && bMatchExtension) {
      files.push_back(entry.path());
    }

    if (bIsFile) totalFiles++;
  }

  std::cout << "Total files   : " << totalFiles << std::endl;
  std::cout << "Filtered files: " << files.size() << std::endl;

  return files;
}
