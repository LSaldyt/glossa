# IO

### Currently, the `IO` module only defines a `readFile` function, which returns a vector of strings (lines) given a file name.

  std::vector<std::string> readFile(std::string filename)
  {
      std::vector<std::string> toReturn;
  
      std::ifstream infile(filename);
      std::string line;
      while (std::getline(infile, line))
      {
          toReturn.push_back(line);
      }
  
      return toReturn;
  }
  
##### Eventually, this module will be used for writing either source to file, or creating object files. Most likely, the compiler will generate C++, and then compile it.
