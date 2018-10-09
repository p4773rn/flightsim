#ifndef TABLE_H
#define TABLE_H

#include <vector>
#include <utility>
#include <ostream>

class Table {
public:
    struct Entry {
        Entry(double alpha, double lift, double drag, double axis, double moment);
        double alpha;
        double lift;
        double drag;
        double moment;
    };


    // alphas must be distinct or division by zero will happen
    Table(const std::vector<Entry>& entries) :
        entries{entries}
    {}

    Table(const Table& other) = default;

    Table(Table&& other) : entries{std::move(other.entries)}
    {}

    Entry get(double alpha) const;

    friend std::ostream& operator<<(std::ostream& stream, const Table& table);
private:
    std::vector<Entry> entries;
};

inline std::ostream& 
operator<<(std::ostream& stream, const Table::Entry& e) {
    stream << "(" << e.alpha << ", " << e.lift << ", " << e.drag << ", " << e.moment << ")";
    return stream;
}

inline std::ostream&
operator<<(std::ostream& stream, const Table& table) {
    stream << "{\n";
    
    for (auto& e : table.entries)
        stream << e << "\n";

    stream << "}";
    return stream;
}


#endif
