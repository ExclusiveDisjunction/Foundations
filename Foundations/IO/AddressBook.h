#pragma once

#include "IOCommon.h"
#include "AddressRecord.h"

namespace Foundations::IO
{
	class CoreMember FileInstance;

	class CoreMember ForwardAddressIterator
	{
	private:
		AddressRecord* _Current;

	public:
		ForwardAddressIterator(AddressRecord* Current);

		/// <summary>
		/// Returns a reference to the current node.
		/// </summary>
		Reference* Current() const;
		/// <summary>
		/// Returns true if the iterator is currently bound to a node. 
		/// </summary>
		bool IsValid() const;

		/// <summary>
		/// Moves the iterator one forward in the list.
		/// </summary>
		/// <returns>False if there are no other items in the list or if the iterator. is not valid.</returns>
		bool Advance();
		/// <summary>
		/// Moves the iteator back one in the list.
		/// </summary>
		/// <returns>False if there are no other items in the list before this one or if the iterator is not valid.</returns>
		bool Retreat();

		friend bool operator==(const ForwardAddressIterator& a, ForwardAddressIterator& b) { return a._Current == b._Current; }
		friend bool operator!=(const ForwardAddressIterator& a, ForwardAddressIterator& b) { return a._Current != b._Current; }
	};
	class CoreMember TreeAddressIterator
	{
	private:
		AddressRecord* _Current;

	public:
		TreeAddressIterator(AddressRecord* Current);

		/// <summary>
		/// Returns a reference to the current compoennt.
		/// </summary>
		Reference* Current() const;
		/// <summary>
		/// Returns if the value of the iterator is bound to a specific part of data. 
		/// </summary>
		bool IsValid() const;

		/// <summary>
		/// Moves the iterator to the next sibling of the current node.
		/// </summary>
		/// <returns>False if there are no more siblings, or if the iterator is not valid.</returns>
		bool Advance();
		/// <summary>
		/// Moves the iterator to the previous sibling of the current node.
		/// </summary>
		/// <returns>False if there are no more siblings before it, or if the iterator is not valid.</returns>
		bool Retreat();

		/// <summary>
		/// Moves the iterator up the tree by one level, going from child to parent.
		/// </summary>
		/// <returns>False if the current node does not have a parent or if the iterator is not valid.</returns>
		bool Accend();
		/// <summary>
		/// Moves the iterator down the tree by one level, going from parent to child. It moves it to the first child of the current node.
		/// </summary>
		/// <returns>False if the current node is a leaf node or if the iterator is not valid.</returns>
		bool Decend();
	};

	enum ReductionLevel
	{
		Flat = 0,
		Cached = 1,
		Loaded = 2
	};

	class CoreMember AddressBook
	{
	private:
	public:
		AddressBook(FileInstance* Instance);
		AddressBook(const AddressBook& Obj) = delete;
		AddressBook(AddressBook&& Obj) noexcept = delete;
		~AddressBook();

		friend FileInstance;

		AddressBook& operator=(const AddressBook& Obj) = delete;
		AddressBook& operator=(AddressBook&& Obj) = delete;

		void Append(Component* Data);
		void Remove(Component* Data);
		void Remove(Address ID);

		void Reduce(Component* Object, ReductionLevel Level);

		TreeAddressIterator FirstRoot();
		TreeAddressIterator LastRoot();
		ForwardAddressIterator First();
		ForwardAddressIterator Last();
	};
}