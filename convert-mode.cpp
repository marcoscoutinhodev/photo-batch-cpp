#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "convert-mode.h"

std::string ToString(ConvertMode::Format format) {
  switch (format) {
    case ConvertMode::Format::JPG:
      return "JPG";
      break;
    case ConvertMode::Format::PNG:
      return "PNG";
      break;
    default:
      return "";
      break;
  }
}

std::ostream& operator<< (std::ostream& out, ConvertMode:: Format format) {
  out << ToString(format);
  return out;
}

ConvertMode::ConvertMode(
  const std::string& folder,
  const std::string& filter,
  Format fromFormat,
  Format toFormat
): Mode{folder, filter}
  , m_FromFormat{fromFormat}
  , m_ToFormat{toFormat} {}

const std::string& ConvertMode::GetModeName() const {
  static const std::string convertModeName = "[Convert]: ";
  return convertModeName;
}

void ConvertMode::RunImpl() {
  std::cout << "Mode     : " << this->GetModeName() << std::endl;
  std::cout << "Folder   : " << this->GetFolder() << std::endl;
  std::cout << "From     : " << this->m_FromFormat << std::endl;
  std::cout << "To       : " << this->m_ToFormat << std::endl;

  if (!this->GetFilter().empty()) {
    std::cout << "Filter   : " << this->GetFilter() << std::endl;
  }
  
  const std::filesystem::path& fromExtension = "." + ToString(this->m_FromFormat);

  for (const std::filesystem::path& filePath : this->GetFiles(fromExtension)) {
    std::cout << GetModeName() << filePath << std::endl;

    int width = 0;
    int height = 0;
    int comp = 0; // components (rgb)
    const int reqComp = 3;

    unsigned char* data = stbi_load(
      filePath.string().c_str(),
      &width,
      &height,
      &comp,
      reqComp
    );

    if (data != NULL) {
      std::filesystem::path newFilePath = filePath;
      newFilePath.replace_extension(ToString(this->m_ToFormat));

      int writeResult = 0;

      switch(this->m_ToFormat) {
        case Format::PNG:
          writeResult = stbi_write_png(newFilePath.string().c_str(), width, height, comp, data, 0);
          break;
        case Format::JPG:
          writeResult = stbi_write_jpg(newFilePath.string().c_str(), width, height, comp, data, 100);
          break;
        default:
          break;
      }

      if (writeResult == 0) {
        std::cout << this->GetModeName() << "Fail to convert \'" << filePath << "\'" << std::endl;
      }

      stbi_image_free(data);
    } else {
      std::cout << this->GetModeName() << "Fail to load file \'" << filePath << "\'" << std::endl;
    }
  }
}