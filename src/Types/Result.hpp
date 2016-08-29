
template <typename T>
struct Result
{
    bool result;
    std::vector<T> consumed;
    std::vector<T> remaining;
    
    Result(bool set_result,
                   std::vector<T> set_consumed,
                   std::vector<T> set_remaining) 
        : result(set_result),
          consumed(set_consumed),
          remaining(set_consumed)
    {}
};


