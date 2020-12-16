#include "Data.hh"

#include <iostream>
#include <fstream>
#include <cassert>
#include <stdexcept>

using namespace std;

Data::Data(const std::string& filename) {
  ifstream file(filename);

  // check if file opened correctly
  if (!file.is_open()) {
    throw std::runtime_error("Error could not open file with name: " +
                             filename);
  }

  int size = -1;
  file >> size;

  // read in data from file: m_siz+1 bin edges
  for (int i = 0; i < size + 1; ++i) {
    double edge;
    file >> edge;
    m_bins.push_back(edge);
  }
  // read in data from file: m_siz bin contents
  for (int i = 0; i < size; ++i) {
    double entries;
    file >> entries;
    m_data.push_back(entries);
  }
  // read in data from file: m_siz bin errors
  for (int i = 0; i < size; ++i) {
    double errors;
    file >> errors;
    m_errors.push_back(errors);
  }

  // done! close the file
  file.close();

  assertSizes();
};

// New constructor to enable adding of Data objects
Data::Data(std::vector<double> data, std::vector<double> bins, std::vector<double> errors){
  // new constructor, sets memebers by hand
  std::vector<double> m_data = data;
  std::vector<double> m_bins = bins;
  std::vector<double> m_errors = errors;
}

void Data::assertSizes() { assert(m_data.size() + 1 == m_bins.size()); }

int Data::checkCompatibility(const Data& inData, int n){
  int counter = 0;
  for (int i; i < m_data.size(); ++i){
    double y1 = m_data[i], sigmay1 = m_errors[i];
    double y2 = inData.measurement(i), sigmay2 = inData.error(i);

    double dy = abs(y1 - y2);
    double sigmay = sqrt(pow(sigmay1, 2) + pow(sigmay2, 2));
    if (dy > n * sigmay){
      counter++;
    }
  return counter;
  }
}

