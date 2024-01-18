#pragma once

#include "IOCommon.h"
#include "Element.h"
#include "ElementIterator.h"

namespace Core::IO
{
	/// <summary>
	/// Describes a list of elements. Can be bound to an element to act as its children, or can exist standalone as storage.
	/// </summary>
	class IO_API ElementList
	{
	private:
		Element* Host = nullptr;
		Element* First = nullptr;
		Element* Last = nullptr;

		std::size_t _size = 0;

		bool CanBeTakenIn(Element* New) const;

	public:
		/// <summary>
		/// Creates the ElementList unbound to an element.
		/// </summary>
		ElementList();
		/// <summary>
		/// Creates the ElementList, but bound to an element. NOTICE: When this is called, it will AUTOMATICALLY set and delete any previous children in the element. 
		/// </summary>
		/// <param name="Host"></param>
		ElementList(Element* Host);
		~ElementList();

		using iterator = ElementIterator<Element>;
		using const_iterator = ElementIterator<const Element>;

		iterator operator[](size_t i) noexcept;
		iterator ItemAt(size_t i) noexcept;
		const_iterator operator[](size_t i) const noexcept;
		const_iterator ItemAt(size_t i) const noexcept;

		Element* getHost() const;
		void setHost(Element* NewHost);

		std::size_t count() const { return _size; }

		void resize(std::size_t newSize);
		void clear();

		void push_back(Element* New);
		void insert(Element* New, iterator after);
		void pop_back();
		void erase(iterator at);

		iterator begin();
		iterator end();
		const_iterator begin() const;
		const_iterator end() const;
	};
}