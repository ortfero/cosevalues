# cosevalues

C++17 single header library to read and write CSV files


## Info

* Lazy, zero allocation parser
* Quotes support
* No dependencies


## Snippets

### Read csv file

```cpp
#include <iostream>
#include <system_error>
#include <cosevalues/cosevalues.hpp>


int main() {
    std::error_code ec;
    auto maybe_reader = cosevalues::reader::from_file("test.csv", ec);
    if(!maybe_reader) {
        std::cerr << ec.message();
        return -1;
    }
    auto reader = std::move(*maybe_reader);
    return 0;
}

```


### Get lines count

```cpp
std::size_t lines_count = 0;
for(auto& row: reader.first_to_last_rows())
    ++lines_count;
```


### Get file header 

```cpp
std::string column_x, column_y;
if(!reader.first_row().parse(column_x, column_y)) {
    std::cerr << "Invalid data\n";
} else {
    std::cout << column_x << ' ' << column_y << '\n';
}
```

### Read file without header

```cpp
#include <iostream>
#include <system_error>
#include <cosevalues/cosevalues.hpp>

struct point { double x, y; };

int main() {
    std::vector<point> points;
    std::error_code ec;
    
    auto maybe_reader = cosevalues::reader::from_file("points.csv", ec);
    if(!maybe_reader) {
        std::cerr << ec.message() << '\n';
        return -1;
    }
    
    constexpr std::size_t average_line_size_estimation = 32;
    points.reserve(maybe_reader->text_size() / average_line_size_estimation + 1);
    
    point p;
    for(auto& row: maybe_reader->first_to_last_rows()) {
        if(!row.parse(p.x, p.y)) {
            std::cerr << "Invalid data\n";
            return -1;
        }
        points.push_back(p);
    }
    
    return 0;
}

```


### Skip header

```cpp
#include <iostream>
#include <system_error>
#include <cosevalues/cosevalues.hpp>

struct point { double x, y; };

int main() {
    std::vector<point> points;
    std::error_code ec;
    
    auto maybe_reader = cosevalues::reader::from_file("points.csv", ec);
    if(!maybe_reader) {
        std::cerr << ec.message() << '\n';
        return -1;
    }
    
    constexpr std::size_t average_line_size_estimation = 32;
    points.reserve(maybe_reader->file_size() / average_line_size_estimation);
    point p;
    
    for(auto& row: maybe_reader->second_to_last_rows()) {
        if(!row.parse(p.x, p.y)) {
            std::cerr << "Invalid data\n";
            return -1;
        }
        points.push_back(p);
    }
    
    return 0;
}
```


### Write file

```cpp
#include <iostream>
#include <system_error>
#include <cosevalues/cosevalues.hpp>

struct point { double x, y; };

int main() {
    std::vector<point> points = {{1.0, 2.0}, {3.0, 4.0}};
    constexpr std::size_t average_line_size_estimation = 32;
    
    cosevalues::writer w;
    w.reserve(average_line_size_estimation * points.size());
    
    w.format("x", "y");
    
    for(auto const& point: points)
        w.format(point.x, point.y);
    
    std::error_code ec;
    if(!w.to_file("points.csv", ec)) {
        std::cerr << ec.message() << '\n';
        return -1;
    }
    
    return 0;
}
```
