# IO

### Currently, the `IO` module only defines `readFile` and `writeFile` functions

`readFile`: returns a vector of strings (lines) given a file name.

```cpp
    std::vector<std::string> readFile(std::string filename)
    {
        // Read an entire file, line by line into a vector
        std::vector<std::string> content;

        std::ifstream infile(filename);
        std::string line;

        while (std::getline(infile, line))
        {
            content.push_back(line);
        }

        return content;
    }
```
  
`writeFile`: given a vector of strings and a filename, creates a file with the given lines

```cpp
void writeFile(std::vector<std::string> content, std::string filename)
{
    // Write a vector of lines into a file
    std::ofstream file;
    file.open(filename);
    for (auto line : content)
    {
        file << (line + "\n");
    }
    file.close();
}
```
