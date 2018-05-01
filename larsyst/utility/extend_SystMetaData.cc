#include "extend_SystMetaData.hh"

#include "larsyst/interface/validation.hh"
#include "printers.hh"

#include <set>

namespace larsyst {
void extend_SystMetaData(SystMetaData &md1, SystMetaData const &md2) {
  if (!validate_SystMetaData(md1, false) ||
      !validate_SystMetaData(md2, false)) {
    std::cout << "[ERROR]: Attempting to merge two parameter header sets but "
                 "one (md1 is valid:"
              << validate_SystMetaData(md1)
              << ", md2 is valid:" << validate_SystMetaData(md2)
              << ") is invalid." << std::endl;
    throw;
  }

  std::set<paramId_t> UsedIds;
  for (auto const &sph : md1.headers) {
    // No colliding ids in each parameter set already checked by
    // validate_SystMetaData
    UsedIds.insert(sph.systParamId);
  }

  for (auto const &sph : md2.headers) {
    auto inserted = UsedIds.insert(sph.systParamId);
    if (!inserted.second) {
      std::cout << "[ERROR]: Attempting to merge two parameter header sets, "
                   "but a parameter Id collison has been found for Id: "
                << std::quoted(sph.systParamId) << std::endl;
      throw;
    }
  }

  for (auto const &sph : md2.headers) {
    md1.headers.push_back(sph);
  }
}
} // namespace larsyst