#ifndef _RENAME_MODE_H
#define _RENAME_MODE_H
#endif

#include "mode.h"

class RenameMode final : public Mode {
  public:
    RenameMode(
      const std::string& folder,
      const std::string& filter,
      const std::string& prefix,
      const int startIndex
    );

    const std::string& GetModeName() const override;

  protected:
    void RunImpl() override;

  private:
    std::string m_Prefix;
    int m_StartIndex;
};