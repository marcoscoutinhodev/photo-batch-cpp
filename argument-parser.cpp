#include <iostream>
#include <algorithm>

#include "argument-parser.h"
#include "utils.h"

void ArgumentParser::RegisterFlag(const std::string& flag) {
  if (!flag.empty() && !Utils::HasWhitespaces(flag))
    m_Flags[flag] = false;
}

void ArgumentParser::RegisterOption(const std::string& option) {
  if (!option.empty() && !Utils::HasWhitespaces(option))
    m_Options[option] = "";
}

bool ArgumentParser::IsFlagRegistered(const std::string& flag) const {
  if (!flag.empty()) {
    auto flagIt = m_Flags.find(flag);

    if (flagIt != std::end(m_Flags))
      return true; // can use m_Flags.count(option) == 1; instead of .find()
  }

  return false;
}

bool ArgumentParser::GetFlag(const std::string& flag) const {
  if (!flag.empty()) {
    // can use 'auto' instead of std::map<std::string, bool>::const_iterator flagIt = ...
    std::map<std::string, bool>::const_iterator flagIt = m_Flags.find(flag);

    if (flagIt != std::end(m_Flags))
      return flagIt->second;  // it->first = KEY; it->second = VALUE
  }

  return false;
}

bool ArgumentParser::IsOptionRegistered(const std::string& option) const {
  if (!option.empty()) {
    auto optionIt = m_Options.find(option);

    if (optionIt != std::end(m_Options))
      return true; // can use m_Options.count(option) == 1; instead of .find()
  }

  return false;
}

const std::string& ArgumentParser::GetOption(const std::string& option) const {
  if (!option.empty()) {
    auto optionIt = m_Options.find(option);

    if (optionIt != std::end(m_Options)) {
      return optionIt->second;
    }
  }

  return NULL;
}

float ArgumentParser::GetOptionAsFloat(const std::string& option) const {
  const std::string& optionValue = this->GetOption(option);

  if (!optionValue.empty()) {
    return std::stof(optionValue);
  }

  return -1;
}

int ArgumentParser::GetOptionAsInt(const std::string& option) const {
  const std::string& optionValue = this->GetOption(option);

  if (!optionValue.empty()) {
    return std::stoi(optionValue);
  }

  return -1;
}

template<>
const std::string& ArgumentParser::GetOptionAs(const std::string& option) const {
  return GetOption(option);
}

template<>
float ArgumentParser::GetOptionAs(const std::string& option) const {
  return GetOptionAsFloat(option);
}

template<>
int ArgumentParser::GetOptionAs(const std::string& option) const {
  return GetOptionAsInt(option);
}

void ArgumentParser::Parser(int argc, const char* argv[]) {
  if (argc > 1 && argv != NULL) {
    for (int i = 1; i < argc; i++) {
      std::string arg = Utils::ToLower(argv[i]);

      if (arg.length() >= 3) {
        if (arg[0] == '-' && arg[1] == '-') {
          arg = arg.substr(2);
          const size_t equalSignPos = arg.find("=");

          if (equalSignPos != std::string::npos) {
            int equalCount = 0;
            for (int i = 0; i < arg.length(); i++) {
              if (arg[i] == '=')
                equalCount += 1;
            }

            if (equalCount > 1)
              throw std::invalid_argument("Invalid argument was provided");

            std::string optionName = arg.substr(0, equalSignPos);
            std::string optionValue = arg.substr(equalSignPos + 1);
            
            auto optionIt = m_Options.find(optionName);

            if (optionIt != std::end(m_Options)) {
              optionIt->second = optionValue;
            }
          } else {
            auto flagIt = m_Flags.find(arg); // iterator like a pointer

            if (flagIt != std::end(m_Flags))
              flagIt->second = true;
          }
        }
      }
    }
  }
}

void ArgumentParser::SetMessageHelp(const std::string& helpMessage) {
  this->m_HelpMessage = helpMessage;
}

const std::string& ArgumentParser::GetMessageHelp() const {
  return this->m_HelpMessage;
}

/**
  std::cout << "Usage: PhotoBatch [flag] [options]" << std::endl;
  std::cout << "*** Only one flag can be actived ***" << std::endl;
  std::cout << "Flags: " << std::endl;
  std::cout << "\tRename: --rename" << std::endl;
  std::cout << "\t\t Required options: --prefix=...  --startindex=..." << std::endl;
  std::cout << "\tConvert: --convert" << std::endl;
  std::cout << "\t\t Required options: --from=...  --to=... (Only JPG & PNG are available and can't be the same)" << std::endl;
  std::cout << "\tResize: --resize" << std::endl;
  std::cout << "\t\t Required options: --width=...  --height=..." << std::endl;
  std::cout << "\tScale: --scale" << std::endl;
  std::cout << "\t\t Required option: --amount=..." << std::endl;
*/
