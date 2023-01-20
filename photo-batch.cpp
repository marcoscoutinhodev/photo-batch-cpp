#include <iostream>

#include "argument-parser.h"
#include "mode.h"

int main(int argc, const char* argv[]) {
  ArgumentParser argParser;

  argParser.RegisterFlag(Args::Flags::Rename);
  argParser.RegisterFlag(Args::Flags::Convert);
  argParser.RegisterFlag(Args::Flags::Resize);
  argParser.RegisterFlag(Args::Flags::Scale);
  argParser.RegisterFlag(Args::Flags::Help);
  
  argParser.RegisterOption(Args::Options::Folder);
  argParser.RegisterOption(Args::Options::Filter);
  argParser.RegisterOption(Args::Options::Width);
  argParser.RegisterOption(Args::Options::Height);
  argParser.RegisterOption(Args::Options::Amount);
  argParser.RegisterOption(Args::Options::Prefix);
  argParser.RegisterOption(Args::Options::StartIndex);
  argParser.RegisterOption(Args::Options::From);
  argParser.RegisterOption(Args::Options::To);

  // cpp 11+
  argParser.SetMessageHelp(R"(Usage: PhotoBatch --[flag] --[options] * Only one flag can be actived *
  *** Options folder and filter are required for any flag ***
  Flags: 
    Rename: --rename
      Required options: --prefix=...  --startindex=...
    Convert: --convert
      Required options: --from=<jpg|png>  --to=<jpg|png>
    Resize: --resize
      Required options: --width=...  --height=...
    Scale: --scale
      Required option: --amount=...)"
  );

  argParser.Parser(argc, argv);

  if (argParser.GetFlag(Args::Flags::Help)) {
    const std::string& helpMessage = argParser.GetMessageHelp();
    std::cout << helpMessage << std::endl;
    
    return 0;
  }
  
  try {
    Mode* photoBatchMode = CreateMode(argParser);

    if (photoBatchMode) {
      photoBatchMode->Run();
    }
  } catch (const std::exception& e) {
    std::cerr << "Err: " << e.what() << std::endl;
    std::cerr << "Type --help to show PhotoBatch usage" << std::endl;
    return -1;
  }

  return 0;
}