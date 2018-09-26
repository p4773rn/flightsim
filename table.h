#ifndef TABLE_H
#define TABLE_H

#include <vector>
#include <utility>
#include <ostream>

class Table {
public:
    struct Entry {
        Entry(float alpha, float lift, float drag, float axis, float moment);
        float alpha;
        float lift;
        float drag;
        float moment;
    };


    // alphas must be distinct or division by zero will happen
    Table(std::vector<Entry> entries) :
        entries{entries}
    {}

    Entry get(float alpha);

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
