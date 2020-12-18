#ifndef DATA_HH
#define DATA_HH

#include <iostream>
#include <vector>
#include <string>
#include <cmath>

class Data {
 public:
  Data(const std::string& filename);
  Data(std::vector<double> data, std::vector<double> bins, std::vector<double> errors);

  unsigned int size() const { return m_data.size(); }
  double measurement(int i) const { return m_data[i]; }
  double binCenter(int i) const { return 0.5 * (m_bins[i+1]+m_bins[i]) ; }
  double binLow(int i) const { return m_bins[i]; }
  double binHigh(int i) const { return m_bins[i+1]; }
  double error(int i) const { return m_errors[i]; }
  int checkCompatibility(const Data& inData, int n);
  double chi2ndf();

  Data operator+(const Data& inData){
    /// Define addition for data sets by error weighted averaging
    if(checkCompatibility(inData, 1) < 20){;
    // Define inputs for return data set, will be inputs to additional constructor
      std::vector<double> data;
      std::vector<double> bins = m_bins;
      std::vector<double> errors;
      // Loop over entreis, compute weighted sum, apply error propagation
      for (int i=0; i < m_data.size(); ++i){
        double y0 = m_data[i];
        double y1 = inData.measurement(i);
        double w0 = pow(m_errors[i], -2);
        double w1 = pow(inData.error(i), -2);
        double y = (w0 * y0 + w1 * y1) / (w0 + w1);
        double sigma = sqrt(1/(w0 + w1));
        // save averaged values to vectors
        data.push_back(y);
        errors.push_back(sigma);
      }
      // retrun average Data sample
      Data sumData(data, bins, errors);
      return sumData;

    } else{
      std::cout << "Error" << std::endl;
      std::vector<double> zeros = {0};
      std::vector<double> emptyBin = {0, 1};
      Data emptyData(zeros, emptyBin, zeros);
      return emptyData;
    }
    
  }


 private:
  Data() {}  // disallow empty data set
  void assertSizes();
  std::vector<double> m_data;
  std::vector<double> m_bins;
  std::vector<double> m_errors;
};

#endif
