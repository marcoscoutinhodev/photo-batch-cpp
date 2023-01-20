#pragma once

#include "mode.h"

class ResizeMode : public Mode {
  public:
    ResizeMode(
      const std::string& folder,
      const std::string& filter,
      int width = -1,
      int height = -1
    );

    const std::string& GetModeName() const override;

  protected:
    void RunImpl() override;

    void ResizeImage(
      const std::filesystem::path& filePath,
      int newWidth,
      int newHeight
    ) const;

  private:
    int m_Width;
    int m_Height;
};