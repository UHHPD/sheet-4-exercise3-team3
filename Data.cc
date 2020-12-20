#include "Data.hh"

#include <iostream>
#include <fstream>
#include <cassert>
#include <stdexcept>

using namespace std;

double background(double x){
  double b = 0.005 - 0.00001 * x + 0.08 * exp(-0.015 * x);
  return b;
}

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
  
  // The commented section below is your old code.
  // As you can see you are trying to re-define private member variables here, which have already
  // been defined in data.hh. Don't do that! This causes funny behavior in C++. Inside the constructor,
  // you always have access to member variables defined in the header. No need to re-define! (See also
  // implementation of the original constructor)
  // After fixing this error, your printouts provide the correct values! Concgratulations!
  
  // new constructor, sets members by hand
  //std::vector<double> m_data = data;
  //std::vector<double> m_bins = bins;
  //std::vector<double> m_errors = errors;

  // This is my correction
  m_data = data;
  m_bins = bins;
  m_errors = errors;

  assertSizes();
}

void Data::assertSizes() { assert(m_data.size() + 1 == m_bins.size()); }

int Data::checkCompatibility(const Data& inData, int n){
  int counter = 0;
  for (int i = 0; i < m_data.size(); ++i){
    double y1 = m_data[i], sigmay1 = m_errors[i];
    double y2 = inData.measurement(i), sigmay2 = inData.error(i);

    double dy = abs(y1 - y2);
    double sigmay = sqrt(pow(sigmay1, 2) + pow(sigmay2, 2));
    if (dy > n * sigmay){
      ++counter;
      //std::cout << "Deviation in bin: " << i << endl;
    }
  // here the return statement used to be, which is too early!
  }
  return counter;
}

double Data::chi2ndf(){
  double chi2 = 0;
  for (int i = 0; i<m_data.size(); ++i){
    double bin_center = 0.5 * (m_bins[i] + m_bins[i+1]);
    double y_meas = m_data[i];
    double sigma = m_errors[i];
    double y_pred = background(bin_center);

    double dchi2 = pow(y_meas-y_pred, 2) / pow(sigma, 2);
    chi2 += dchi2;
  }
  return chi2 / (m_data.size() - 4);
}
