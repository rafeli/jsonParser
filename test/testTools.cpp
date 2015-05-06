#include "testTools.hpp"

std::string * TestTools::logFileName =0;
std::ofstream * TestTools::logStream = 0;

  void TestTools::prepare(std::string fn) {
    // singletons scheinen in cpp nicht "gut"zu sein:
    // stackoverflow.com/questions/86582/singleton-how-should-it-be-used
    // static classes sind u.U nicht viel besser ...
    TestTools::logFileName = new std::string(fn)     ;
    TestTools::logStream = new std::ofstream((TESTDIR + (*logFileName)).c_str());
  }

  void TestTools::testingComplete() {
    if (logStream->is_open()) {
      logStream->close();
    }
    delete TestTools::logFileName;
  }

  void TestTools::logMessage(std::string message) {
    (*(logStream)) << message;
  }

void TestTools::report(std::string actual_, std::string expected_, std::string test_) {

    if ( actual_ == expected_ ) {
      (*logStream) << test_ << "tested and confirmed:\n\t" 
		   << actual_ << std::endl;
    } else {
      (*logStream) << test_ << "FAILED: \n\t(" <<  expected_
    		   << ") <> \n\t(" << actual_ << ")" << std::endl;
      std::cout << "FAILURE in " << test_ << std::endl;
    };
}

std::ofstream * TestTools::getLogStream(){
  return TestTools::logStream;
}

  std::string * TestTools::writeFile(std::string fn, std::string content) {

    // opens a file in testdirectory with specified filename=fn
    // writes content into this file, then closes and returns filepath

     std::string *fpath = new std::string(TESTDIR + fn);
    std::ofstream ofs(fpath->c_str());
    if (!ofs.is_open()) throw std::runtime_error("couldnt open file for writing: " + (*fpath));
    ofs << content << std::endl;
    if (ofs.is_open()) ofs.close();
    return fpath;
  }


// trim from start
std::string TestTools::ltrim(std::string s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
std::string TestTools::rtrim(std::string s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
std::string TestTools::trim(std::string s) {
        return ltrim(rtrim(s));
}
