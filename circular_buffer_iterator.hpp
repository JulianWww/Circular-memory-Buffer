#include <iterator>

#define increment(x) this->add(x); return *this
#define addValue(x) auto other = CircularIterator<T>(this); other.add(x); return other
#define compare(x) return (this->distance() x other.distance())

namespace jce
{
    template<typename T>
    class CircularIterator: public std::iterator<
                        std::random_access_iterator_tag,    // iterator_category
                        T,                                  // value_type
                        long ,                              // difference_type
                        const T*,               // pointer
                        T&                       // reference
                    >{
        template<typename U, typename V>
        friend class CircularBuffer;
    private: 
        long pos;
        long max_size;
        long head;
        T* buffer;
        T* ptr;

    public:
        CircularIterator();
        explicit CircularIterator(long pos, long max_size, long head, T* buffer);
        CircularIterator(const CircularIterator& other);
        CircularIterator(const CircularIterator*other);
        ~CircularIterator();
        void operator=(const CircularIterator& other);

        bool operator==(const CircularIterator& other) const;
        bool operator!=(const CircularIterator& other) const;

        T& operator*();
        const T& operator*() const;
        T* operator->();
        const T* operator->() const;

        CircularIterator& operator++();
        CircularIterator& operator--();
        CircularIterator operator++(int);
        CircularIterator operator--(int);

        CircularIterator operator+(const long& n) const;
        CircularIterator operator-(const long& n) const;

        CircularIterator& operator+=(const long& n);
        CircularIterator& operator-=(const long& n);

        bool operator<(const CircularIterator& other) const;
        bool operator>(const CircularIterator& other) const;
        bool operator<=(const CircularIterator& other) const;
        bool operator>=(const CircularIterator& other) const;

        void swap(CircularIterator& other);

        long distance(const CircularIterator& other) const;
    
    public:
        void assignPtr();
        void add(const long& other);
        long distance() const;
    };
}
namespace std 
{
    template<typename T>
    void swap(jce::CircularIterator<T>&, jce::CircularIterator<T>&);
}

template<typename T>
inline jce::CircularIterator<T>::CircularIterator(){};
template<typename T>
inline jce::CircularIterator<T>::CircularIterator(long _pos, long _max_size, long _head, T* _buffer) :
    pos(_pos), max_size(_max_size), head(_head), buffer(_buffer) {this->assignPtr();}
template<typename T>
inline jce::CircularIterator<T>::CircularIterator(const CircularIterator& other){*this = other;}
template<typename T>
inline jce::CircularIterator<T>::CircularIterator(const CircularIterator* other){*this = *other;}
template<typename T>
inline jce::CircularIterator<T>::~CircularIterator(){}
template<typename T>
inline void jce::CircularIterator<T>::operator=(const CircularIterator& other)
{
    this->pos = other.pos;
    this->max_size = other.max_size;
    this->buffer = other.buffer;
    this->head = other.head;
    this->assignPtr();
}

template<typename T>
inline bool jce::CircularIterator<T>::operator==(const CircularIterator& other) const
{
    return (this->pos == other.pos);
}
template<typename T>
inline bool jce::CircularIterator<T>::operator!=(const CircularIterator& other) const
{
    return !(*this == other);
}

template<typename T>
T& jce::CircularIterator<T>::operator*(){return *(this->ptr);}
template<typename T>
const T& jce::CircularIterator<T>::operator*() const {return *(this->ptr);}
template<typename T>
T* jce::CircularIterator<T>::operator->() {return this->ptr;}
template<typename T>
const T* jce::CircularIterator<T>::operator->() const {return this->ptr;}

template<typename T>
jce::CircularIterator<T>& jce::CircularIterator<T>::operator++(){increment(1);}
template<typename T>
jce::CircularIterator<T>& jce::CircularIterator<T>::operator--(){increment(-1);}
template<typename T>
jce::CircularIterator<T> jce::CircularIterator<T>::operator++(int){increment(1);}
template<typename T>
jce::CircularIterator<T> jce::CircularIterator<T>::operator--(int){increment(-1);}

template<typename T>
jce::CircularIterator<T> jce::CircularIterator<T>::operator+(const long& n) const {addValue(n);}
template<typename T>
jce::CircularIterator<T> jce::CircularIterator<T>::operator-(const long& n) const {addValue(-n);}

template<typename T>
jce::CircularIterator<T>& jce::CircularIterator<T>::operator+=(const long& n){increment(n);}
template<typename T>
jce::CircularIterator<T>& jce::CircularIterator<T>::operator-=(const long& n){increment(-n);}

template<typename T>
bool jce::CircularIterator<T>::operator<(const CircularIterator& other) const {compare(<);}
template<typename T>
bool jce::CircularIterator<T>::operator>(const CircularIterator& other) const {compare(>);}
template<typename T>
bool jce::CircularIterator<T>::operator<=(const CircularIterator& other) const {compare(<=);}
template<typename T>
bool jce::CircularIterator<T>::operator>=(const CircularIterator& other) const {compare(>=);}

template<typename T>
void jce::CircularIterator<T>::swap(CircularIterator<T>& other)
{
    std::swap(this->pos, other.pos);
    std::swap(this->max_size, other.max_size);
    std::swap(this->buffer, other.buffer);
    std::swap(this->ptr, other.ptr);
    std::swap(this->head, other.head);
}

template<typename T>
long jce::CircularIterator<T>::distance(const CircularIterator<T>& other) const
{
    return this->pos - other.pos;
}

template<typename T>
void jce::CircularIterator<T>::assignPtr()
{
    if (max_size)
        this->ptr = &(this->buffer[this->pos % this->max_size]);
    else
        this->ptr = nullptr;
}
template<typename T>
void jce::CircularIterator<T>::add(const long & value)
{
    this->pos = (this->pos + value);
    this->assignPtr();
}
template<typename T>
long jce::CircularIterator<T>::distance() const
{
    return this->pos - this->head;
}

//std
template<typename T>
void std::swap(jce::CircularIterator<T>& a, jce::CircularIterator<T>& b)
{
    a.swap(b);
}