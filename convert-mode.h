#ifndef _CONVERT_MODE
#define _CONVERT_MODE
#endif

#include "mode.h"

class ConvertMode final : public Mode {
  public:
    enum class Format {
      JPG,
      PNG,
    };

    ConvertMode(
      const std::string& folder,
      const std::string& filter,
      Format fromFormat,
      Format toFormat
    );

    const std::string& GetModeName() const override;

  protected:
    void RunImpl() override;

  private:
    Format m_FromFormat;
    Format m_ToFormat;
};