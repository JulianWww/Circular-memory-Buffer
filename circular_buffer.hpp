#include <memory>
#include <vector>
#include <stdexcept>
#include <initializer_list>
#include "circular_buffer_iterator.hpp"

#define AT iterator iter = this->tail + pos; if (iter >= this->head) {throw std::out_of_range("Circular queue out of range");} return *iter
#define RBEGIN reverse_iterator iter(this->head); return iter++
#define REND reverse_iterator iter(this->tail); return iter++

namespace jce
{
    template<typename T, typename Allocator = std::allocator<T>>
    class CircularBuffer
    {
    public:
        using value_type             = T;
        using allocator_type         = Allocator;
        using pointer                = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer          = typename std::allocator_traits<Allocator>::const_pointer;
        using reference              = value_type&;
        using const_reference        = const value_type&;
        using size_type              = size_t;
        using difference_type        = long long;
        using iterator               = CircularIterator<T>;
        using const_iterator         = const CircularIterator<T>;
        using reverse_iterator       = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    public:
        std::unique_ptr<T[] > buffer;
        size_t _size;
        iterator head;
        iterator tail;
        allocator_type allocator;
    public:
        constexpr CircularBuffer();
        constexpr CircularBuffer(size_t size);
        constexpr explicit CircularBuffer(size_t size, Allocator alloc );
        template<typename InputIt>
        constexpr CircularBuffer(InputIt first, InputIt last, Allocator alloc = Allocator()) : allocator(alloc)
        {
            this->allocate(std::distance(first, last), alloc);
            this->copyBuffer(first, last);
        };
        constexpr CircularBuffer(const CircularBuffer& other, Allocator alloc = Allocator());
        constexpr CircularBuffer(CircularBuffer&& other, Allocator alloc = Allocator());
        constexpr CircularBuffer(std::initializer_list<T> init, Allocator alloc = Allocator());

        constexpr CircularBuffer& operator=(const CircularBuffer& other);
        constexpr CircularBuffer& operator=(std::initializer_list<T> init);

        constexpr value_type enqueue(value_type val);
        constexpr value_type dequeue();
        constexpr value_type _dequeue(); // same as dequeue but without error checking

        constexpr allocator_type get_allocator() const noexcept;

        constexpr reference operator[]( size_type pos );
        constexpr const_reference operator[]( size_type pos ) const;
        constexpr reference at( size_type pos );
        constexpr const_reference at( size_type pos ) const;
        constexpr reference front();
        constexpr const_reference front() const;
        constexpr reference back();
        constexpr const_reference back() const;
        constexpr T* data() noexcept;
        constexpr const T* data() const noexcept;

        inline constexpr iterator begin() noexcept {return this->tail;};
        inline constexpr const_iterator begin() const noexcept {return this->tail;};
        inline constexpr const_iterator cbegin() const noexcept {return this->tail;};
        inline constexpr iterator end() noexcept {return this->head;};
        inline constexpr const_iterator end() const noexcept {return this->head;};
        inline constexpr const_iterator cend() const noexcept {return this->head;};
        inline constexpr reverse_iterator rbegin() noexcept { RBEGIN; };
        inline constexpr const_reverse_iterator rbegin() const noexcept { RBEGIN; };
        inline constexpr const_reverse_iterator crbegin() const noexcept { RBEGIN; };
        inline constexpr reverse_iterator rend() noexcept { REND; };
        inline constexpr const_reverse_iterator rend() const noexcept { REND; }; 
        inline constexpr const_reverse_iterator crend() const noexcept { REND; };

        inline constexpr bool empty() const noexcept {return this->head == this->tail;};
        inline constexpr size_type size() const noexcept {return this->head.distance(this->tail);};
        inline constexpr size_type max_size() const noexcept {return this->_size;};

        constexpr void clear() noexcept;
        constexpr void resize(size_type size) noexcept;

    private:
        void allocate(size_t size, Allocator& alloc);
        void copyBuffer(const CircularBuffer& other);
        template<typename Iter>
        inline void copyBuffer(Iter begin, const Iter& end)
        {
            iterator iter(this->head);
            while (begin != end)
            {
                *iter = *begin;
                begin++; iter++;
            }
            this->head.pos = this->_size;
        }
        void moveHead();
        void moveTail();
    };
}

template<typename T, typename A>
inline constexpr jce::CircularBuffer<T, A>::CircularBuffer() : CircularBuffer(0) {};
template<typename T, typename A>
inline constexpr jce::CircularBuffer<T, A>::CircularBuffer(size_t _size) : CircularBuffer(_size, A()) {};
template<typename T, typename A>
inline constexpr jce::CircularBuffer<T, A>::CircularBuffer(size_t size, A alloc) : allocator(alloc) {this->allocate(size, alloc);};
template<typename T, typename A>
inline constexpr jce::CircularBuffer<T, A>::CircularBuffer(const CircularBuffer<T, A>& other, A alloc) : allocator(alloc)
{
    this->allocate(other._size, alloc);
    this->copyBuffer(other);
}
template<typename T, typename A>
inline constexpr jce::CircularBuffer<T, A>::CircularBuffer(std::initializer_list<T> init, A alloc)
    : CircularBuffer(init.begin(), init.end(), alloc){};

template<typename T, typename A>
inline constexpr jce::CircularBuffer<T, A>& jce::CircularBuffer<T,A>::operator=(const CircularBuffer& other)
{
    this->allocate(other.size, this->allocator);
    copyBuffer(other.tail, other.head);
    return *this;
}
template<typename T, typename A>
inline constexpr jce::CircularBuffer<T, A>& jce::CircularBuffer<T,A>::operator=(std::initializer_list<T> init)
{
    this->allocate(std::distance(init.begin(), init.end()), this->allocator);
    this->copyBuffer(init.begin(), init.end());
}

template<typename T, typename A>
inline constexpr T jce::CircularBuffer<T, A>::enqueue(value_type val)
{
    std::swap(*(this->head), val);
    this->moveHead();
    return val;
}
template<typename T, typename A>
inline constexpr T jce::CircularBuffer<T, A>::dequeue()
{
    if (this->head == this->tail)
    {
        throw std::out_of_range("CircularBuffer queue is empty");
    }
    return this->_dequeue();
};
template<typename T, typename A>
inline constexpr T jce::CircularBuffer<T, A>::_dequeue()
{
    value_type out = *(this->tail);
    this->moveTail();
    return out;
}

template<typename T, typename A>
inline constexpr A jce::CircularBuffer<T,A>::get_allocator() const noexcept
{
    return this->allocator;
}

template<typename T, typename A>
inline constexpr T& jce::CircularBuffer<T, A>::operator[]( size_t pos )
{
    return *(this->tail+pos);
}
template<typename T, typename A>
inline constexpr const T& jce::CircularBuffer<T, A>::operator[]( size_t pos ) const
{
    return *(this->tail+pos);
}
template<typename T, typename A>
constexpr T& jce::CircularBuffer<T, A>::at( size_type pos ) {AT;};
template<typename T, typename A>
constexpr const T& jce::CircularBuffer<T, A>::at( size_type pos ) const {AT;};
template<typename T, typename A>
constexpr T& jce::CircularBuffer<T, A>::front() {return *(this->tail);};
template<typename T, typename A>
constexpr const T& jce::CircularBuffer<T, A>::front() const {return *(this->tail);};
template<typename T, typename A>
constexpr T& jce::CircularBuffer<T, A>::back() {return *(this->head-1);};
template<typename T, typename A>
constexpr const T& jce::CircularBuffer<T, A>::back() const {return *(this->head-1);};
template<typename T, typename A>
constexpr T* jce::CircularBuffer<T, A>::data() noexcept {return this->buffer.get();};
template<typename T, typename A>
constexpr const T* jce::CircularBuffer<T, A>::data() const noexcept {return this->buffer.get();};

template<typename T, typename A>
constexpr void jce::CircularBuffer<T, A>::clear() noexcept
{
    this->head = CircularIterator<T>(0, _size, this->_size, this->buffer.get());
    this->tail = this->head;
};
template<typename T, typename A>
constexpr void jce::CircularBuffer<T, A>::resize(size_t size) noexcept
{
    T* buff = this->allocator.allocate(size);
    size_t min = std::min(this->size(), size);
    for (size_t i = 0; i < min; i++)
    {
        buff[i] = *(this->head - min + i);
    }
    this->_size = size;
    this->tail = iterator(0, size, 0, buff);
    this->head = iterator(min, size, 0, buff);
    this->buffer.reset(buff);
}

template<typename T, typename A>
inline void jce::CircularBuffer<T, A>::allocate(size_t _size, A& alloc) 
{
    this->_size = _size;
    this->buffer.reset(alloc.allocate(_size));
    this->clear();
};
template<typename T, typename A>
inline void jce::CircularBuffer<T, A>::copyBuffer(const CircularBuffer& other)
{
    auto iter = this->tail;
    auto otherIter = other.tail;
    while (otherIter != other.head) {
        *iter = *otherIter;
        iter++; otherIter++;
    } 
    this->head.pos = this->_size;
}
template<typename T, typename A>
inline void jce::CircularBuffer<T, A>::moveHead()
{
    if (this->head.distance(this->tail) % this->_size == 0 && this->head.distance(this->tail) > 0)
    {
        this->moveTail();
    }
    this->head++;
}
template<typename T, typename A>
inline void jce::CircularBuffer<T, A>::moveTail()
{
    this->tail++;
    if (this->tail.pos >= this->_size)
    {
        long old = this->tail.pos;
        this->tail.pos = this->tail.pos % this->_size;
        this->head.pos = this->head.pos - old + this->tail.pos;
    }
};


#undef AT