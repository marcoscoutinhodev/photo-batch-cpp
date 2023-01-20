#include <gtest/gtest.h>

#include "argument-parser.h"

TEST(ArgumentParser, RegisterFlag) {
  ArgumentParser argParser;

  EXPECT_FALSE(argParser.IsFlagRegistered("flag"));

  argParser.RegisterFlag("flag");

  EXPECT_TRUE(argParser.IsFlagRegistered("flag"));
}

TEST(ArgumentParser, RegisterFlag_Whitespace) {
  ArgumentParser argParser;

  EXPECT_FALSE(argParser.IsFlagRegistered("flag test"));

  argParser.RegisterFlag("flag test");

  EXPECT_FALSE(argParser.IsFlagRegistered("flag test"));
}

TEST(ArgumentParser, RegisterOption) {
  ArgumentParser argParser;

  EXPECT_FALSE(argParser.IsOptionRegistered("option"));

  argParser.RegisterOption("option");

  EXPECT_TRUE(argParser.IsOptionRegistered("option"));
}

TEST(ArgumentParser, RegisterOption_Whitespace) {
  ArgumentParser argParser;

  EXPECT_FALSE(argParser.IsOptionRegistered("option test"));

  argParser.RegisterOption("option test");

  EXPECT_FALSE(argParser.IsOptionRegistered("option test"));
}

TEST(ArgumentParser, Parser) {
  ArgumentParser argParser;

  EXPECT_FALSE(argParser.GetFlag("flag"));

  std::string flagString = "--flag";

  // can use std::array<const char*, SIZE> arguments = {...}
  std::vector<std::string> arguments = {
    "./build/tests/TestArgumentParser",
    "--flag",
    "--string=ABC",
    "--int=10",
    "--float=1.52",
  };
  std::vector<const char *> argvMock;

  for (auto& arg : arguments) {
    argvMock.push_back((char*)arg.data());
  }

  argParser.RegisterFlag("flag");
  argParser.RegisterOption("string");
  argParser.RegisterOption("int");
  argParser.RegisterOption("float");

  argParser.Parser(static_cast<int>(argvMock.size()), argvMock.data());

  EXPECT_FALSE(argParser.GetFlag("unregisteredFlag"));
  EXPECT_TRUE(argParser.GetFlag("flag"));
  EXPECT_EQ(argParser.GetOptionAs<const std::string&>("string"), "abc");
  EXPECT_EQ(argParser.GetOptionAs<int>("int"), 10);
  EXPECT_EQ(argParser.GetOptionAs<float>("float"), 1.52f);
}

TEST(ArgumentParser, Parse_InvalidArgs) {
  ArgumentParser argParser;

  argParser.Parser(1, nullptr);
  argParser.Parser(10, nullptr);
}