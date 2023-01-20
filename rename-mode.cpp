#include <iostream>
#include <filesystem>
#include <vector>

#include "rename-mode.h"

RenameMode::RenameMode(
  const std::string& folder,
  const std::string& filter,
  const std::string& prefix,
  const int startIndex
): Mode{ folder, filter }, m_Prefix{ prefix }, m_StartIndex{ startIndex } {}

const std::string& RenameMode::GetModeName() const {
  static const std::string RenameModeName = "[Rename]: ";
  return RenameModeName;
}

void RenameMode::RunImpl() {
  const std::string filter = this->GetFilter();

  std::cout << this->GetModeName() << "Mode       : " << this->GetModeName() << std::endl;
  std::cout << this->GetModeName() << "Folder     : " << this->GetFolder() << std::endl;
  std::cout << this->GetModeName() << "Prefix     : " << this->m_Prefix << std::endl;
  std::cout << this->GetModeName() << "Start index: " << this->m_StartIndex << std::endl;

  if (!filter.empty())
    std::cout << this->GetModeName() << "Filter     : " << this->GetFilter() << std::endl;

  std::cout << '\n';

  int index = m_StartIndex;
  int successCount = 0;
  int errorCount = 0;
  
  for (const std::filesystem::path& filePath : this->GetFiles()) {
    const std::filesystem::path extension = filePath.extension();
    const std::string newFileName = m_Prefix + std::to_string(index) + extension.string();

    std::filesystem::path newFilePath = filePath;
    newFilePath.replace_filename(newFileName);

    try {
      std::filesystem::rename(filePath, newFilePath);

      std::cout << this->GetModeName() << filePath << " -> " << newFilePath << std::endl;
      successCount++;
    } catch (std::exception& exeception) {
      std::cout << "Error on rename file \'" << filePath << "\' to \'" << newFilePath << "\'" << std::endl;
      errorCount++;
    }

    index++;
  }

  // std::cout << "\n";
  // std::cout << this->GetModeName() << "Total files to update : " << filesToRename.size() << "/" << filesCount << std::endl;
  // std::cout << this->GetModeName() << "Total files updated   : " << successCount << std::endl;
  // std::cout << this->GetModeName() << "Total files with error: " << errorCount << std::endl;
}