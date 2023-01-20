// shocurt #pragma once (includes file only once)
#ifndef _ARGUMENT_PARSER_H
#define _ARGUMENT_PARSER_H
#endif

#include<string>
#include<map>

class ArgumentParser {
  public:
    void RegisterFlag(const std::string& flag);

    void RegisterOption(const std::string& option);

    bool IsFlagRegistered(const std::string& flag) const;

    bool GetFlag(const std::string& flag) const;

    bool IsOptionRegistered(const std::string& option) const;
    
    template<typename T>
    T GetOptionAs(const std::string& option) const;

    void Parser(int argc, const char* argv[]);

    void SetMessageHelp(const std::string& helpMessage);
    
    const std::string& GetMessageHelp() const;
  
  private:
    std::map<std::string, bool> m_Flags;
    std::map<std::string, std::string> m_Options;

    const std::string& GetOption(const std::string& option) const;
    
    float GetOptionAsFloat(const std::string& option) const;
    
    int GetOptionAsInt(const std::string& option) const;

    std::string m_HelpMessage;
};