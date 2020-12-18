#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <cmath>

#include "Data.hh"

// generic function comparing two values of some type T used later for int and
// double
template <class T>
bool testEqual(const std::string& name, T expected, T real) {
  if (expected != real) {
    std::cout << "(" << name << ": <" << expected << ">"
              << ", " << real << ") ";
    return false;
  }
  return true;
}

bool testReadingSize() {
  std::cout << "testReadingSize...";
  Data datA("testA");
  return testEqual<int>("size", 1, datA.size());
}

bool testReadingMeasurement() {
  std::cout << "testReadingMeasurement...";
  Data datA("testA");
  return testEqual("measurement", 10., datA.measurement(0));
}

bool testReadingBinEdges() {
  std::cout << "testReadingBinEdges...";
  Data datA("testA");
  return testEqual("bin low", 0., datA.binLow(0)) &&
         testEqual("bin high", 1., datA.binHigh(0));
}

bool testReadingErrors() {
  std::cout << "testReadingErrors...";
  Data datA("testA");
  return testEqual("error", 2., datA.error(0));
}

bool testCopyConstructor() {
  std::cout << "testCopyConstructor...";
  Data a("testA");
  Data b(a);
  Data c = a;  // equivalent to Data c(a)
  return testEqual("measurement", 10., b.measurement(0)) &&
         testEqual("measurement", 10., c.measurement(0));
}

void runTests() {
  std::cout << "running tests...\n";
  std::vector<std::function<bool()> > tests(
      {testReadingSize, testReadingMeasurement, testReadingBinEdges,
       testReadingErrors, testCopyConstructor});
  for (auto test : tests)
    std::cout << (test() ? " ok" : " FAILED!") << std::endl;
}

int main() {
  using namespace std;

  cout << "******************************************************" << endl;
  runTests();
  cout << "******************************************************" << endl;
  // create an object which holds data of experiment A
  vector<Data> data;
  Data datA("exp_A"); data.push_back(datA);
  Data datB("exp_B"); data.push_back(datB);
  Data datC("exp_C"); data.push_back(datC);
  Data datD("exp_D"); data.push_back(datD);

  std::vector<Data> data1 = {datA, datB, datC, datD};

  
  // here is the data from all experiments
  for (int i; i < 4; ++i){
    cout << "bin 27: from  experiment " << i << ": " << data[i].binLow(27) << " to " <<  data[i].binHigh(27)
       << endl;
  cout << "measurement of experiment " << i << " in bin 27: " <<  data[i].measurement(27)
       << endl;
  }
  
  double y1 = data[1].measurement(27), sigmay1 = data[1].error(27);
  double y2 = data[2].measurement(27), sigmay2 = data[2].error(27);

  double dy = y1 - y2;
  double sigmay = sqrt(pow(sigmay1, 2) + pow(sigmay2, 2));
  /*
  for (int n=1; n < 4; ++n){
    cout << n << "sigma: " << abs(dy) << " < " << n * sigmay << "?" << endl;
  }
  */
  /*
  cout << data[0].checkCompatibility(data[0], 1) << endl;
  cout << data[1].checkCompatibility(data[1], 1) << endl;
  cout << data[2].checkCompatibility(data[2], 1) << endl;
  cout << data[3].checkCompatibility(data[3], 1) << endl;

  cout << data[0].checkCompatibility(data[1], 1) << endl;
  cout << data[0].checkCompatibility(data[2], 1) << endl;
  cout << data[0].checkCompatibility(data[3], 1) << endl;
  
  cout << data[1].checkCompatibility(data[2], 1) << endl;
  cout << data[1].checkCompatibility(data[3], 1) << endl;

  cout << data[2].checkCompatibility(data[3], 1) << endl;
  */
  cout << "*************  2  a    **************" << endl;

  for (int n = 1; n < 4; ++n){
    cout << "Differ by more that " << n << " standard deviations:" << endl;
    for (int i = 0; i < data1.size(); ++i){
      for (int j = i+1; j < data1.size(); ++j){
        cout << data1[i].checkCompatibility(data1[j], n) << endl;
      }
      cout << "*************" << endl;
    }
  }
  
  cout << "**************** 2 b  *************" << endl;

  for (int i=0; i < 4; ++i){
    cout << data1[i].chi2ndf()
       << endl;
       /*
    cout << data1[i].measurement(3)
       << endl;
    cout << data1[i].measurement(3)
       << endl;
    cout << "************************" << endl;*/
  }

  cout << "*************** 2 c  **************" << endl;
  Data combData01 = data1[0] + data1[1];
  Data combData23 = data1[2] + data1[3];
  Data combData = combData01 + combData23;

  cout << combData01.chi2ndf() << endl;
  cout << combData23.chi2ndf() << endl;
  cout << combData.chi2ndf() << endl;

  return 0;
}
