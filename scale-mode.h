#pragma once

#include "resize-mode.h"

class ScaleMode final : public ResizeMode {
  public:
    ScaleMode(const std::string& folder, const std::string& filter, const float scaleFactor);

    const std::string& GetModeName() const override;

  protected:
    void RunImpl() override;

  private:
    float m_ScaleFactor;
};