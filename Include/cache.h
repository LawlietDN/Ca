#include <iostream>
class Cache
{
private:
    void forward();
    std::string _origin;
    std::string _path;

public:
    explicit Cache(std::string const& origin, std::string const& path)
    :   _origin(origin),
        _path(path)
        {
            forward();
        }
};