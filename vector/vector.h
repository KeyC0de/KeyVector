#pragma once

#include <iostream>
#include <limits>
#include <type_traits>
#include <string>
#include <algorithm>
#include <iterator>
#include "custom_exception.h"

//template<class T>
//class Iterator;

//============================================================
//	\class	Vector<T, Alloc>
//
//	\author	KeyC0de
//	\date	19/10/2018 4:17
//
//	\brief	It is OK (and in fact preferable in C++) to use std::copy() instead of loops.
//			pop_back() must destroy a buffer[size - 1] object.
//			clear() must delete [] buffer.
//			Edit: it must destroy everything in the buffer. The buffer itself may stay.
//			All accessors (begin, end, front, back, operator[]) must have const 
//				versions as well. ie. const T& front() const; , T& front();
//			A choice of unsigned int for size parameters is questionable.
//				A natural type for size is size_t
//=============================================================
template <class T, class Alloc = std::allocator<T>>
class Vector
{
	template<class T>
	friend class Iterator;

	std::size_t m_size;
	std::size_t m_capacity;
	T* m_pdata;
public:
	//using value_type = T;
	//using size_type = std::size_t;
	//using pointer = T*;
	//using const_pointer = T* const;
	//using void_pointer = void*;
	//using const_void_pointer = void* const;
	//using difference_type = std::ptrdiff_t;
	using value_type = typename std::allocator_traits<Alloc>::value_type;
	using size_type = typename std::allocator_traits<Alloc>::size_type;
	using pointer = typename std::allocator_traits<Alloc>::pointer;
	using const_pointer = typename std::allocator_traits<Alloc>::const_pointer;
	using void_pointer = typename std::allocator_traits<Alloc>::void_pointer;
	using const_void_pointer = typename std::allocator_traits<Alloc>::const_void_pointer;
	using difference_type = typename std::allocator_traits<Alloc>::difference_type;
	////using difference_type = typename std::pointer_traits<T>::difference_type;	// doesn't work
	using reference = T&;
	using const_reference = const T&;
	using allocator_type = Alloc;

	class Iterator
	{
		Vector* v_;
		std::size_t m_index;
	public:
		using iterator_category = typename std::random_access_iterator_tag;
		//using value_type		= typename std::iterator_traits<Iterator>::value_type;
		//using pointer			= typename std::iterator_traits<Iterator>::pointer;
		//using difference_type	= typename std::iterator_traits<Iterator>::difference_type;
		//using reference		= typename std::iterator_traits<Iterator>::reference;
		using value_type = typename Vector::value_type;
		using pointer = typename Vector::pointer;
		using difference_type = typename Vector::difference_type;
		using reference = value_type&;

		Iterator()
			: v_{ nullptr },
			m_index{ 0 }
		{}
		Iterator(Vector* v_, std::size_t index)
			: v_{ v_ },
			m_index{ index }
		{}
		Iterator(Vector& v_, std::size_t index)
			: v_{ &v_ },
			m_index{ index }
		{}
		// rule-of-0
		//~Iterator() noexcept
		//{
		//	v_ = nullptr;
		//}

		operator std::size_t() noexcept {
			return m_index;
		}

		T& operator*() {
			return (*v_)[m_index];
		}
		const T& operator*() const {
			return (*v_)[m_index];
		}
		T* operator->() {
			return &((*v_)[m_index]);
		}
		const T* operator->() const {
			return &((*v_)[m_index]);
		}
		T& operator[](difference_type m) {
			return (*v_)[m_index + m];
		}
		const T& operator[](difference_type m) const {
			return (*v_)[m_index + m];
		}

		// pre-inc/dec
		Iterator& operator++() noexcept {
			++m_index;
			return *this;
		}
		Iterator& operator--() noexcept {
			--m_index;
			return *this;
		}
		// post-inc/dec
		Iterator operator++(int) {
			Iterator r{ *this };
			++m_index;
			return r;
		}
		Iterator operator--(int) {
			Iterator r{ *this };
			--m_index;
			return r;
		}

		Iterator& operator+=(difference_type n) noexcept {
			m_index += n;
			return *this;
		}
		Iterator& operator-=(difference_type n) noexcept {
			m_index -= n;
			return *this;
		}

		Iterator operator+(difference_type n) const {
			Iterator r{ *this };
			return r += n;
		}
		Iterator operator-(difference_type n) const {
			Iterator r{ *this };
			return r -= n;
		}

		difference_type operator-(Iterator const& r) const noexcept {
			return m_index - r.m_index;
		}

		// comparisons
		inline constexpr bool operator<(const Iterator& it)  const noexcept { return m_index < it.m_index; }
		inline constexpr bool operator<=(const Iterator& it) const noexcept { return m_index <= it.m_index; }
		inline constexpr bool operator>(const Iterator& it)  const noexcept { return m_index > it.m_index; }
		inline constexpr bool operator>=(const Iterator& it) const noexcept { return m_index >= it.m_index; }
		inline constexpr bool operator!=(const Iterator& it) const noexcept { return m_index != it.m_index; }
		inline constexpr bool operator==(const Iterator& it) const noexcept { return m_index == it.m_index; }
	};

	class ConstIterator
	{
		const Vector* v_;
		std::size_t m_index;
	public:
		using iterator_category	= typename std::random_access_iterator_tag;
		using value_type		= const typename Vector::value_type;
		using pointer			= typename Vector::const_pointer;
		using difference_type	= typename Vector::difference_type;
		using reference			= typename Vector::const_reference;

		ConstIterator()
			: v_{ nullptr },
			m_index{ 0 }
		{}
		ConstIterator(Vector* v_, std::size_t index)
			: v_{ v_ },
			m_index{ index }
		{}
		ConstIterator(Vector& v_, std::size_t index)
			: v_{ &v_ },
			m_index{ index }
		{}
		// rule-of-0
		//~ConstIterator() noexcept
		//{
		//	v_ = nullptr;
		//}

		//operator pointer() const noexcept {
		//	return &v_[m_index];
		//}

		const T& operator*() {
			return (*v_)[m_index];
		}
		const T* operator->() {
			return &((*v_)[m_index]);
		}
		const T& operator[](difference_type m) {
			return (*v_)[m_index + m];
		}

		// pre-inc/dec
		ConstIterator& operator++() noexcept {
			++m_index;
			return *this;
		}
		ConstIterator& operator--() noexcept {
			--m_index;
			return *this;
		}
		// post-inc/dec
		ConstIterator operator++(int) {
			ConstIterator r{ *this };
			++m_index;
			return r;
		}
		ConstIterator operator--(int) {
			ConstIterator r{ *this };
			--m_index;
			return r;
		}

		ConstIterator operator+(difference_type n) {
			ConstIterator r{ *this };
			return r += n;
		}
		ConstIterator operator-(difference_type n) {
			ConstIterator r{ *this };
			return r -= n;
		}

		difference_type operator-(ConstIterator const& r) noexcept {
			return m_index - r.m_index;
		}

		// comparisons
		inline constexpr bool operator<(const ConstIterator& it)  const noexcept { return m_index < it.m_index; }
		inline constexpr bool operator<=(const ConstIterator& it) const noexcept { return m_index <= it.m_index; }
		inline constexpr bool operator>(const ConstIterator& it)  const noexcept { return m_index > it.m_index; }
		inline constexpr bool operator>=(const ConstIterator& it) const noexcept { return m_index >= it.m_index; }
		inline constexpr bool operator!=(const ConstIterator& it) const noexcept { return m_index != it.m_index; }
		inline constexpr bool operator==(const ConstIterator& it) const noexcept { return m_index == it.m_index; }
	};

	using iterator		= T*;//Iterator<T>;	// There's an error if we use my custom Iterator as the default iterator
	// Error C2664 'Vector<int,std::allocator<int>>::Vector(Vector<int,std::allocator<int>> &&) noexcept': cannot convert argument 1 from 'Vector<int,std::allocator<int>>::Iterator' to 'size_t'	vector	j : \nikosl91\documents\visual_studio_projects\vector\vector\vector.cpp	144
	using miterator		= std::move_iterator<iterator>;
	using riterator		= std::reverse_iterator<iterator>;
	using citerator		= ConstIterator;
	using mciterator	= std::move_iterator<citerator>;
	using rciterator	= std::reverse_iterator<citerator>;

private:
	std::size_t iteratorDistance(iterator first, iterator last) const noexcept
	{
		return std::abs(last - first);
	}

	struct Deleter
	{// objects should be already destructed prior.
		void operator()(T* buff) const
		{
			::operator delete(buff);
		}
	};

	inline constexpr bool isInitializedIndex(std::size_t i) const noexcept
	{
		if (i < m_size) {
			return true;
		}
		return false;
	}

	inline constexpr bool isValidIndex(std::size_t i) const noexcept
	{
		if (i < m_capacity) {
			return true;
		}
		return false;
	}

	__forceinline bool needsRestructuring() const noexcept
	{
		return m_size == m_capacity;
	}

	template<typename U>
	typename std::enable_if_t<
		!(std::is_nothrow_copy_constructible_v<U> && std::is_nothrow_destructible_v<U>)
	>
		copyAssign(const Vector<U>& copy)
	{// copy and swap
		Vector<U> temp(copy);
		temp.swap(*this);
	}

	template<typename U>
	typename std::enable_if_t<
		(std::is_nothrow_copy_constructible_v<U> && std::is_nothrow_destructible_v<U>)
	>
		copyAssign(const Vector<U>& copy)
	{
		// self assignment check
		if (this == &copy) {
			return;
		}
		// if we have enough space available, reuse it
		if (m_capacity <= copy.getSize())
		{
			clear<T>();
			for (std::size_t i = 0; i < copy.getSize(); ++i)
			{
				selfMoveBack(std::move(copy[i]));
			}
		}
		// fallback to straight copying
		else
		{
			Vector<T> temp(copy);
			temp.swap(*this);
		}
	}

	// emplaces objects
	void selfPushBack(const T& val)
	{
		::new (m_pdata + m_size) T(val);
		++m_size;
	}
	void selfMoveBack(T&& val)
	{
		::new (m_pdata + m_size) T(std::move(val));
		++m_size;
	}

	template<typename U>
	typename std::enable_if_t<!std::is_nothrow_move_constructible_v<U>>
		selfCopyTo(Vector<U>& dest)
	{
		std::for_each(m_pdata, m_pdata + m_size,
			[&dest](const T& srcVal)
			{
				dest.selfPushBack(srcVal);
			}
		);
	}
	template<typename U>
	typename std::enable_if_t<std::is_nothrow_move_constructible_v<U>>
		selfMoveTo(Vector<U>& dest)
	{
		std::for_each(m_pdata, m_pdata + m_size,
			[&dest](T&& srcVal)
			{
				dest.selfMoveBack(std::move(srcVal));
			}
		);
	}
public:
	// default constructor
	Vector()
		: m_size{ 0 },
		m_capacity{ 64 },
		m_pdata{ static_cast<T*>(::operator new(sizeof(T) * m_capacity)) }
	{}

	// construct given capacity
	explicit Vector(const std::size_t capacity)
		: m_size{ 0 },
		m_capacity(capacity),
		m_pdata{ static_cast<T*>(::operator new(sizeof(T) * m_capacity)) }
	{}

	// constructor, setting default value to all elements
	//template <typename T, typename = std::enable_if_t<!IsIterator<T>>>
	explicit Vector(std::size_t capacity, const T& value)
		: m_size{ 0 },
		m_capacity(capacity),
		m_pdata{ static_cast<T*>(::operator new(sizeof(T) * m_capacity)) }
	{
		//std::for_each(m_pdata, m_pdata + m_size, [this,&value](const T& itVal)
		//	{
		//		this->selfPushBack(value);
		//	}
		//);
		for (std::size_t i = 0; i < m_capacity; ++i)
		{
			selfPushBack(value);
		}
	}

	// construct from given range of elements
	template<class Iter>
	Vector(Iter* begin, Iter* end)
		: m_size{ 0 },
		m_capacity{ iteratorDistance(begin, end) },
		m_pdata{ static_cast<T*>(::operator new(sizeof(T) * m_capacity)) }
	{
		for (auto it = begin; it < end; ++it)
		{
			selfPushBack(*it);
		}
	}

	// delegating ctor
	// construct from std::initializer_list
	explicit Vector(std::initializer_list<T>& list)
		: Vector(std::begin(list), std::end(list))
	{}

	// destructor
	~Vector()
	{
#ifdef _DEBUG
		std::cout << "dtor" << '\n';
#endif // _DEBUG
		clear<T>();
		Deleter deleter{};
		std::unique_ptr<T, Deleter> deletesAtEndOfScope{ m_pdata, std::move(deleter) };
		//m_pdata = nullptr;
	}

	// copy constructor
	Vector(const Vector& rhs)
		: m_size{ 0 },
		m_capacity{ rhs.m_capacity },
		m_pdata{ static_cast<T*>(::operator new(sizeof(T) * m_capacity)) }
	{
#ifdef _DEBUG
		std::cout << "cctor" << '\n';
#endif // _DEBUG
		try
		{
			for (std::size_t i = 0; i < rhs.getCapacity(); ++i)
			{
				selfPushBack(rhs.m_pdata[i]);
			}
		}
		catch (...) {
			clear<T>();
			std::unique_ptr<T, Deleter> deletesAtEndOfScope{ m_pdata, Deleter{} };
			// or:
			//::operator delete(m_pdata);
			throw;	// continue propagating the caught exception outside
		}
	}

	// copy assignment operator
	Vector& operator=(const Vector& rhs)
	{
		copyAssign(rhs);
		return *this;
	}

	// move ctor
	Vector(Vector&& rhs) noexcept
		: m_size{ 0 },
		m_capacity{ 0 },
		m_pdata{ nullptr }
	{
#ifdef _DEBUG
		std::cout << "mctor" << '\n';
#endif
		rhs.swap(*this);
	}

	// move assignment operator
	Vector& operator=(Vector&& rhs) noexcept
	{
		rhs.swap(*this);
		return *this;
	}

	void reserve(const std::size_t newCapacity)
	{
		if (newCapacity > m_capacity) {
			Vector<T> tmp(newCapacity);
			tmp.swap(*this);
		}
		// don't shrink otherwise
	}

	void swap(Vector& rhs) noexcept
	{
		std::swap(m_size, rhs.m_size);
		std::swap(m_capacity, rhs.m_capacity);
		std::swap(m_pdata, rhs.m_pdata);
	}

	void pushBack(T&& val)
	{
		if (needsRestructuring())
		{
			resize(m_capacity << 1ull);
		}
		selfMoveBack(std::move(val));
	}
	void pushBack(const T& val)
	{
		if (needsRestructuring())
		{
			resize(m_capacity << 1ull);
		}
		selfPushBack(val);
	}

	template<typename... TArgs>
	void emplaceBack(TArgs&&... args)
	{
		if (needsRestructuring())
		{
			resize(m_capacity << 1ull);
		}
		::new (m_pdata + m_size) T(std::forward<TArgs>(args)...);
		++m_size;
	}

	void popBack() noexcept
	{
		--m_size;
		m_pdata[m_size].~T();
	}

	// restructuring / replacing vector in memory with a new one of different capacity
	//	old values are retained
	//	Complexity: O(n^2): worst case, O(n): average case
	void resize(std::size_t newCapacity)
	{
		Vector<T> tmp(newCapacity);
		if (newCapacity > m_size)
		{
			std::for_each(m_pdata, m_pdata + m_size,
				[&tmp](const T& arg)
				{
					tmp.selfPushBack(arg);
				}
			);
		}
		else if (newCapacity < m_size)
		{
			std::for_each(m_pdata, m_pdata + newCapacity,
				[&tmp](const T& arg)
				{
					tmp.selfPushBack(arg);
				}
			);
		}
		else {
			return;
		}
		tmp.swap(*this);
	}

	template<typename U>
	typename std::enable_if_t<!std::is_trivially_constructible_v<U>>
		clear() noexcept
	{
		// destroy the elements in reverse order
		for (std::size_t i = 0; i < m_size; ++i) {
			m_pdata[m_size - i - 1].~T();
		}
	}
	// SFINAE optimization - no need to reallocate for builtins/pods - just copy over them
	template<typename U>
	typename std::enable_if_t<std::is_trivially_constructible_v<U>>
		clear() noexcept
	{
		// trivially destructible objects can used without calling destructors
	}

	// forward
	inline iterator begin() noexcept {
		return &m_pdata[0];
	}
	inline iterator end() noexcept {
		return &m_pdata[m_size];	// or m_pdata + m_size
	}
	inline citerator cbegin() const noexcept {
		return &m_pdata[0];
	}
	inline citerator cend() const noexcept {
		return &m_pdata[m_size];
	}
	// reverse
	inline riterator rbegin() noexcept {
		return riterator{ end() };
	}
	inline riterator rend() noexcept {
		return riterator{ begin() };
	}
	inline rciterator crbegin() const noexcept {
		return rciterator{ cend() };		// or rbegin()
	}
	inline rciterator crend() const noexcept {
		return rciterator{ cbegin() };	// or rend();
	}

	T& front() noexcept {
		return m_pdata[0];
	}
	const T& cfront() const noexcept {
		return m_pdata[0];
	}
	T& back() noexcept {
		return m_pdata[m_size - 1];
	}
	const T& cback() const noexcept {
		return m_pdata[m_size - 1];
	}

	// Does it have any elements?
	explicit operator bool() const noexcept
	{
		return m_size > 0;
	}
	
	inline bool operator==(const Vector& rhs) const noexcept {
		return this->m_pdata == rhs.m_pdata;
	}
	inline bool operator!=(const Vector& rhs) const noexcept {
		return this->m_pdata != rhs.m_pdata;
	}
	inline bool operator==(const Vector* rhs) const noexcept {
		return this->m_pdata == rhs->m_pdata;
	}
	inline bool operator!=(const Vector* rhs) const noexcept {
		return this->m_pdata != rhs->m_pdata;
	}

	T& operator[](std::size_t index) const {
		return m_pdata[index];
	}
	T& operator[](std::size_t index) {
		return m_pdata[index];
	}

	// const & non-const versions are needed to satisfy every intent (get & set)
	T& at(std::size_t index) const
	{
		if (isValidIndex(index)) {
			return m_pdata[index];
		}
		throwException("Array out of bounds exception.");
	}
	T& at(std::size_t index)
	{
		if (isValidIndex(index)) {
			return m_pdata[index];
		}
		throwException("Array out of bounds exception.");
	}

	bool isEmpty() const noexcept {
		return m_size == 0;
	}

	bool isFull() const noexcept {
		return m_size == std::numeric_limits<std::size_t>::max() - 1 ? true : false;
	}

	std::size_t getSize() const noexcept {
		return m_size;
	}

	std::size_t getCapacity() const noexcept {
		return m_capacity;
	}

	// prints all elements
#if defined(_UNICODE) || defined(UNICODE)
	template<typename J>
	friend std::wostream& operator<<(std::wostream& stream, const Vector<J>& v) noexcept;

	void print(std::wostream& stream = std::wcout) const noexcept
	{
		for (std::size_t i = 0; i < m_size; ++i)
		{
			stream << m_pdata[i] << L' ';
		}
	}
#else
	template<typename J>
	friend std::ostream& operator<<(std::ostream& stream, const Vector<J>& v) noexcept;

	void print(std::ostream& stream = std::cout) const noexcept
	{
		for (std::size_t i = 0; i < m_size; ++i)
		{
			stream << m_pdata[i] << ' ';
		}
	}
#endif

	friend void swap(Vector<T>& lhs, Vector<T>& rhs) noexcept;
};

#if defined(_UNICODE) || defined(UNICODE)
template<typename J>
std::wostream& operator<<(std::wostream& stream, const Vector<J>& v) noexcept
{
	for (std::size_t i = 0; i < v.m_size; ++i)
	{
		stream << v.m_pdata[i] << L' ';
	}
}
#else
template<typename J>
std::ostream& operator<<(std::ostream& stream, const Vector<J>& v) noexcept
{
	for (std::size_t i = 0; i < v.m_size; ++i)
	{
		stream << v.m_pdata[i] << ' ';
	}
}
#endif

// specialization of std::swap for the Vector class
template <typename T>
void swap(Vector<T>& lhs, Vector<T>& rhs) noexcept
{
	lhs.swap(rhs);
}