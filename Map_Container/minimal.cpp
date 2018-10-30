#include "Map.hpp"

#include <cassert>

// basically an int wrapper
class MyKeyType {
    private:
        int val;

    public:
        //not default constructable, not copy assignable, not move assignable
        MyKeyType() = delete;
        MyKeyType& operator=(const MyKeyType&) = delete;
        MyKeyType& operator=(MyKeyType&&) = delete;

        // copy constructable and move assignable
        MyKeyType(MyKeyType&&) = default;
        MyKeyType(const MyKeyType&) = default;
        ~MyKeyType() = default;

        MyKeyType(int i) : val(i) { }

        bool operator<(const MyKeyType& other) const {
            return this->val < other.val;
        }

        bool operator==(const MyKeyType &other) const {
            return this->val == other.val;
        }

        int getKey() {
          return val;
        }
};

// same as keytype except no operator<
class MyValueType {
    private:
        int val;

    public:
        //not default constructable, not copy assignable, not move assignable
        MyValueType() = delete;
        MyValueType& operator=(const MyValueType&) = delete;
        MyValueType& operator=(MyValueType&&) = delete;

        // copy constructable and move assignable
        MyValueType(MyValueType&&) = default;
        MyValueType(const MyValueType&) = default;
        ~MyValueType() = default;

        MyValueType(int i) : val(i) { }

        bool operator==(const MyValueType &other) const {
            return this->val == other.val;
        }


};

class MyDefaultConstructible {

        friend bool operator<(const MyDefaultConstructible &o1, const MyDefaultConstructible &o2) {
            return o1.val < o2.val;
        }

    private:
        int val = 0;

    public:
        // not copy assignable, not move assignable
        MyDefaultConstructible& operator=(const MyDefaultConstructible&) = delete;
        MyDefaultConstructible& operator=(MyDefaultConstructible&&) = delete;

        // default constructable, copy constructable and move assignable
        MyDefaultConstructible() = default;
        MyDefaultConstructible(MyDefaultConstructible&&) = default;
        MyDefaultConstructible(const MyDefaultConstructible&) = default;
        ~MyDefaultConstructible() = default;

        MyDefaultConstructible(int i) : val(i) { }

        bool operator==(const MyDefaultConstructible &other) const {
            return this->val == other.val;
        }
};


class MyAssignable {
    private:
        int val = 0;

    public:
        MyAssignable() = default;
        MyAssignable(int i) : val(i) { }
        bool operator==(const MyAssignable &other) const {
            return this->val == other.val;
        }
};

// manual instantiation, instantiates every member function instead of
// just the ones called
template class cs540::Map<MyKeyType, MyDefaultConstructible>;


int main() {
    cs540::Map<MyKeyType, MyValueType> m{{3, 5}};
    m.insert({{2}, {3}});
    m.insert({{1}, {3}});
    m.insert({{5}, {3}});
    m.insert({{7}, {3}});
    m.at(2);

    auto it = m.begin();
    assert(it->first.getKey() == 1);
    auto it2 = it++;
    assert(it->first.getKey() == 2);
    assert(it2->first.getKey() == 1);

    auto it3 = m.end();
    --it3;
    assert(it3->first.getKey() == 7);

    auto start = m.begin();
    auto end = m.end();

    ++start; //points to Second
    ++start; //points to Third
    --end; //points to fifth
    --end; //points to fourth
    --end; //points to third

    assert(start == end);

    ++start; // points to fourth
    ++start; //points to fifth
    ++start; //points to one past fifth (last)

    assert(start == m.end());

    --end; //points to second
    --end; //points to first

    assert(end == m.begin());
    assert(end != start);

    auto end2 = end++;
    assert(end->first.getKey() == 2);
    assert(end2->first.getKey() == 1);

    end2 = end--;
    assert(end->first.getKey() == 1);
    assert(end2->first.getKey() == 2);





    // auto iter = m.find(2);
    // m.erase(iter);
    // auto m_copy = m;
    // assert(m_copy == m);
    //
    // cs540::Map<MyKeyType, MyDefaultConstructible> m2{{8, 9}};
    // m2[10]; // should default construct these values
    // m2[15];
    //
    // cs540::Map<MyKeyType, MyAssignable> m3{{6, 7}};
    // m3[20] = {5}; // move assign
    // MyAssignable ma{1};
    // m3[10] = ma; //copy assign

    return 0;
}
