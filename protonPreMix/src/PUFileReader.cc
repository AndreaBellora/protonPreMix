#include "protonPreMix/protonPreMix/interface/PUFileReader.h"

PUFileReader::PUFileReader(std::vector<std::string> fileNames,
                           edm::InputTag pixelTag, edm::InputTag stripsTag)
    : fileNames_(fileNames), pixelTag_(pixelTag), stripsTag_(stripsTag),
      errorEventNumber_(-1) {

  for (auto &fileName : fileNames_) {
    if (fileName.find("file:") == std::string::npos &&
        fileName.find("root:") == std::string::npos) {
      fileName = "root://cms-xrd-global.cern.ch//" + fileName;
    }
  }
  ev_ = std::make_unique<fwlite::ChainEvent>(fileNames_);
}

bool PUFileReader::getAndCheckEvent(const int i) {
  try {
    if (ev_->to(i) && ev_->isValid()) {
      if (!jsonContainsEvent(*ev_))
        return false;
      else
        return true;
    } else
      return false;
  } catch (cms::Exception &e) {
    edm::LogWarning("PPS")
        << "Exception caught while getting event number " << i
        << "\nChoosing another event... Find the exception below:\n"
        << e.what();
    return false;
  }
}

bool PUFileReader::jsonContainsEvent(const edm::EventBase &event) {
  // if the jsonVec is empty, then no JSON file was provided so all
  // events should pass
  if (jsonVector_.empty())
    return true;

  bool (*funcPtr)(edm::LuminosityBlockRange const &,
                  edm::LuminosityBlockID const &) = &edm::contains;
  edm::LuminosityBlockID lumiID(event.id().run(), event.id().luminosityBlock());
  std::vector<edm::LuminosityBlockRange>::const_iterator iter = find_if(
      jsonVector_.begin(), jsonVector_.end(), boost::bind(funcPtr, _1, lumiID));
  return jsonVector_.end() != iter;
}