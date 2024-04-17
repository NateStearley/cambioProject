#include "../include/DataCollection.h"

#include <sstream>

DataCollection *DataCollection::dataCollection = nullptr;

DataCollection *DataCollection::getInstance() {
  if (dataCollection == nullptr) {
    dataCollection = new DataCollection();
  }

  return dataCollection;
}

fstream *DataCollection::csvBOI = nullptr;

fstream *DataCollection::getFstream() {
  if (!csvBOI) {
    // new file written for every simulation
    csvBOI = new fstream("datacollect.csv", ios::out | ios::trunc);
    *csvBOI << "name, tripNum, distTravel, batteryLost, numRecharges, tripTime"
            << endl;
  }

  return csvBOI;
}

void DataCollection::writeTrip(DroneTotals dats) {
  DataCollection *dataCollection = DataCollection::getInstance();
  fstream *fileStream = dataCollection->getFstream();

  *fileStream << dats.name << ", " << dats.tripNum << ", " << dats.distTrav
              << ", " << dats.batteryLost << ", " << dats.rechargeNum << ", "
              << dats.tripTime << endl;

  // collect aggregate trip data for each drone
  finalResults[dats.name].name = dats.name;
  finalResults[dats.name].tripNum += 1;
  finalResults[dats.name].distTrav += dats.distTrav;
  finalResults[dats.name].batteryLost += dats.batteryLost;
  finalResults[dats.name].rechargeNum += 1;
  finalResults[dats.name].tripTime += dats.tripTime;
}

void DataCollection::writeResults() {
  std::cout << "Writing final data...\n";
  DataCollection *instance = DataCollection::getInstance();
  fstream *fileStream = instance->getFstream();

  *fileStream << endl << "Final Results" << endl;
  *fileStream << "Drone Name, Total Trips, Total Distance, Total Battery Used, "
                 "Total Times Recharged, TotalTripTime"
              << endl;

  for (auto &finalResult : finalResults) {
    *fileStream << finalResult.second.name << ", " << finalResult.second.tripNum
                << ", " << finalResult.second.distTrav << ", "
                << finalResult.second.batteryLost << ", "
                << finalResult.second.rechargeNum << ", "
                << finalResult.second.tripTime << std::endl;
  }
  fileStream->close();
}
