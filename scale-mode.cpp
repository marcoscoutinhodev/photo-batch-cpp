#include <iostream>

#include <stb_image.h>

#include "scale-mode.h"

ScaleMode::ScaleMode(
  const std::string& folder,
  const std::string& filter,
  float scaleFactor
): ResizeMode{ folder, filter }, m_ScaleFactor{ scaleFactor } {}

const std::string& ScaleMode::GetModeName() const {
  static const std::string scaleModeName = "[Scale]: ";
  return scaleModeName;
}

void ScaleMode::RunImpl() {
  std::cout << this->GetModeName() << "Folder      : " << this->GetFolder() << std::endl;
  std::cout << this->GetModeName() << "Scale factor: " << this->m_ScaleFactor << std::endl;

  if (!this->GetFilter().empty()) {
    std::cout << this->GetModeName() << "Filter      :" << this->GetFilter() << std::endl;
  }

  for (const std::filesystem::path& filePath : this->GetFiles()) {
    std::cout << this->GetModeName() << "Scaling: " << filePath << std::endl;

    int width = 0;
    int height = 0;
    int numComp = 0;

    stbi_info(filePath.string().c_str(), &width, &height, &numComp);

    const int newWidth = width * this->m_ScaleFactor;
    const int newHeight = height * this->m_ScaleFactor;

    this->ResizeImage(filePath, newWidth, newHeight);
  }
}