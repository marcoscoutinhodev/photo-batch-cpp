#include <iostream>

//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize.h>

#include "resize-mode.h"

ResizeMode::ResizeMode(
  const std::string& folder,
  const std::string& filter,
  int width,
  int height
): Mode{ folder, filter }, m_Width{ width }, m_Height{ height } {};

const std::string& ResizeMode::GetModeName() const {
  static const std::string resizeModeName = "[Resize]: ";
  return resizeModeName;
};

void ResizeMode::ResizeImage(
  const std::filesystem::path& filePath,
  int newWidth,
  int newHeight
) const {
  /**
   * 1º - Read image
   * 2º - Resize image in memory
   * 3º - Write image
  */

  int inputWidth = 0;
  int inputHeight = 0;
  int inputNumComp = 0;
  const int reqComp = 4;

  unsigned char* inputData = stbi_load(filePath.string().c_str(), &inputWidth, &inputHeight, &inputNumComp, reqComp);

  if (inputData) {
    const int numOutputPixels = newWidth * newHeight * reqComp;
    std::vector<unsigned char> outputData(numOutputPixels, 0);

    const int resizeResult = stbir_resize_uint8(
      inputData,
      inputWidth,
      inputHeight,
      0,
      outputData.data(),
      newWidth,
      newHeight,
      0,
      reqComp
    );

    if (resizeResult == 1) {
      int writeResult = 1;
      const std::filesystem::path extension = filePath.extension();

      if (extension == ".jpg") {
        writeResult = stbi_write_jpg(filePath.string().c_str(), newWidth, newHeight, reqComp, outputData.data(), 100);
      } else if (extension == ".png") {
        writeResult = stbi_write_png(filePath.string().c_str(), newWidth, newHeight, reqComp, outputData.data(), 0);
      } else {
        std::cout << this->GetModeName() << "Invalid file format : " << filePath << std::endl;
      }

      if (writeResult == 0) {
        std::cout << this->GetModeName() << "Error to write file : " << filePath << std::endl;
      }
    } else {
      std::cout << this->GetModeName() << "Error to resize file: " << filePath << std::endl;
    }

    stbi_image_free(inputData);
  } else {
    std::cout << this->GetModeName() << "Error to load file  : " << filePath << std::endl;
  }
};

void ResizeMode::RunImpl() {
  std::cout << this->GetModeName() << "Folder: " << this->GetFolder() << std::endl;
  std::cout << this->GetModeName() << "Width : " << this->m_Width << std::endl;
  std::cout << this->GetModeName() << "Height: " << this->m_Height << std::endl;

  if (!this->GetFilter().empty()) {
    std::cout << this->GetModeName() << "Filter:" << this->GetFilter() << std::endl;
  }

  if (this->m_Width > 0 && this->m_Height) {
    for (const std::filesystem::path& filePath : this->GetFiles()) {
      std::cout << this->GetModeName() << "file: " << filePath << std::endl;
      this->ResizeImage(filePath, this->m_Width, this->m_Height);
    }
  }
};