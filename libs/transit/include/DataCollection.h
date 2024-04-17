#ifndef CSCI3081W_TEAM28_LIBS_TRANSIT_INCLUDE_DATACOLLECTION_H_
#define CSCI3081W_TEAM28_LIBS_TRANSIT_INCLUDE_DATACOLLECTION_H_

#include <fstream>
#include <iostream>
#include <map>

#include "DataCollection.h"
using namespace std;

/**
 * @brief A struct to hold the different data
 * to be collected from the drone
 */
struct DroneTotals {
  /**
   * @brief name of the drone
   */
  std::string name;

  /**
   * @brief trip number of the drone
   */
  int tripNum;

  /**
   * @brief distance traveled of the drone
   */
  float distTrav;

  /**
   * @brief battery lost of the drone
   * over a trip
   */
  float batteryLost;

  /**
   * @brief number of recharges of the drone
   */
  int rechargeNum;

  /**
   * @brief time for a trip of the drone
   */
  double tripTime;

  /**
   * @brief struct definition
   */
  DroneTotals()
      : name(),
        tripNum(0),
        distTrav(0),
        batteryLost(0),
        rechargeNum(0),
        tripTime(0) {}
};

/**
 * @brief Singleton class that stores the instance for data collection. Allows
 * for writing to the csv file for both a history of the simulation and the
 * final amounts for the data
 */
class DataCollection {
 public:
  /**
   * @brief Gets the singleton instance of this class
   *
   * @return the instance of the DataCollection singleton
   */
  static DataCollection *getInstance();

  /**
   * @brief Gets the filestream for writing to the data collection
   *
   * @return the filestream for writing to the csv file
   */
  static fstream *getFstream();

  /**
   * @brief Delete the copy constructor so the singleton instance
   * @param other a different instance of the dataCollection, this is not used
   */
  DataCollection(DataCollection &other) = delete;

  /**
   * @brief Writes the trip data for constructing the history table in the csv
   * @param dats variable that holds the data to be written to the csv
   */
  void writeTrip(DroneTotals dats);

  /**
   * @brief Reads the csv file to compile the totals for each drone and
   * create the final results table
   */
  void writeResults();

 private:
  DataCollection() {}
  ~DataCollection() = default;

  static DataCollection *dataCollection;

  static fstream *csvBOI;
  /**
   * Collects trip aggregates for each drone
   */
  map<string, DroneTotals> finalResults;
};

#endif  // CSCI3081W_TEAM28_LIBS_TRANSIT_INCLUDE_DATACOLLECTION_H_
