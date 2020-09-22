#include "protonPreMix/protonPreMix/interface/PUFileReader.h"

using namespace std;

PUFileReader::PUFileReader(vector<string> fileNames)
    : fileNames_(fileNames), ev_(fwlite::ChainEvent(fileNames_)) {}

void PUFileReader::PrintEvent(int i) {
  fwlite::Handle<edm::DetSetVector<CTPPSPixelRecHit>> pixelRecHits;
  pixelRecHits.getByLabel(ev_, "ctppsPixelRecHits", "");
  edm::DetSetVector<CTPPSPixelRecHit> pixelRecHitsDsv = *(pixelRecHits.ptr());
  // cout << "Size: " << pixelRecHits.ptr()->size() << endl;
  for (auto pixelRecHitsDs : pixelRecHitsDsv) {
    cout << CTPPSPixelDetId(pixelRecHitsDs.id) << endl;
    int i = 1;
    for (auto pixelRecHit : pixelRecHitsDs.data) {
      cout << " RecHit number " << i
           << ": minPixelCol = " << pixelRecHit.minPixelCol()
           << " minPixelRow = " << pixelRecHit.minPixelRow() << "\n";
    }
  }
}