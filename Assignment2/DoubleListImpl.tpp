#pragma once

#include "DoubleList.tpp"

template <typename T>
DoubleList<T>::DoubleList(T *a, unsigned n) : mHead{nullptr, nullptr}
{	
	Node **curr = mHead; // current node
	unsigned int i; // index

	if (n != 0)  // if there are elements in the array
	{
		for (i = 0; i < n; i++) // for each element in the array		
		{
			Node *newNode = new Node; // create new node
			newNode->mValue = a[i]; // set value
			curr[Red] = newNode; // set red next pointer
			curr[Blue] = newNode; // set blue next pointer
			curr = newNode->mNext; // set current node to new node
		}

		curr[Red] = nullptr; // set red next pointer to null
		curr[Blue] = nullptr; // set blue next pointer to null
	}
}

template <typename T>
DoubleList<T>::DoubleList(const DoubleList<T> &dl) : mHead{nullptr, nullptr}
{
	Node **curr = mHead; // Set current node to head of color list	
	Node *currdl = dl.mHead[Red]; // Current node pointer dl
	unsigned int size = 0; // Size of list
	unsigned int otherColorIndex = 0; // Index of blue color
	unsigned int i; // Loop variable

	while (currdl != nullptr) // While current node is not null
	{
		size++; // Increment size
		currdl = currdl->mNext[Red]; // Set current node to next node
	}	

	if (size != 0)
	{
		for (i = 0; i < size; i++) // Loop through size
		{
			Node *newNode = new Node; // Create new node
			newNode->mValue = dl.get(i, Red)->mValue; // Set value		
			curr[Red] = newNode; // Set red next pointer
			curr = newNode->mNext; // Set current node to new node
		}	

		curr[Red] = nullptr; // Set red next pointer to null

		Node *blue; // Node pointer for blue list
		curr = mHead; // Set current node to head of color list
		for (i = 0; i < size; i++) // Loop through size
		{
			otherColorIndex = 0; // Set other color index to 0
			blue = dl.get(i, Blue); // Set blue node to blue index
			
			currdl = dl.mHead[Red]; // Set current node to red list
			while(currdl != blue) // While current node is not the same as the blue node
			{
				otherColorIndex++; // Increment other color index
				currdl = currdl->mNext[Red]; // Set temp node to next node
			}

			curr[Blue] = get(otherColorIndex, Red); // Set blue next pointer
			curr = curr[Blue]->mNext; // Set current node to next node
		}

		curr[Blue] = nullptr; // Set blue next pointer to null
	}

}

template <typename T>
typename DoubleList<T>::Node *DoubleList<T>::get(unsigned index, Color color) const                                                 
{
	Node *curr = mHead[color]; // Set current node to head of color list
	unsigned int size = 0; // Size of list
	unsigned int i; // Counter

	while (curr != nullptr) // While current node is not null
	{
		size++; // Increment size
		curr = curr->mNext[color]; // Set current node to next node
	}

	if (index >= size) // If index is larger than or equal to size of list
	{
		throw InvalidIndexException(); // Throw exception
	}
	else
	{
		curr = mHead[color]; // Set current node to head of color list
		for (i = 0; i < index; i++) // For each element in the array (except the first)
		{
			curr = curr->mNext[color]; // Set current node to next node
		}

		return curr; // Return current node	
	}	  
}

template <typename T>
typename DoubleList<T>::Node *DoubleList<T>::insert(T value, unsigned redIndex,unsigned blueIndex) 
{
	Node *curr = mHead[Red]; // Set current node to red head
	Node *prev; // Previous node pointer
	unsigned int size = 0; // Size of list

	while (curr != nullptr) // While current node is not null
	{
		size++; // Increment size
		curr = curr->mNext[Red]; // Set current node to next node
	}	

	if (redIndex > size || blueIndex > size) // If index is larger than size of list
	{
		throw InvalidIndexException(); // Throw exception
	}
	else
	{
		Node *newNode = new Node; // Create new node
		newNode->mValue = value; // Set value

		if (redIndex == 0) // If index is 0
		{
			newNode->mNext[Red] = mHead[Red]; // Set next pointer
			mHead[Red] = newNode; // Set head pointer.
		}
		else if (redIndex == size) // If index is size of list
		{
			prev = get(redIndex - 1, Red); // Set previous node to red index - 1
			prev->mNext[Red] = newNode; // Set next pointer
			newNode->mNext[Red] = nullptr; // Set next pointer to null
		}
		else
		{
			curr = get(redIndex, Red); // Set current node to red index
			prev = get(redIndex - 1, Red); // Set previous node to red index - 1
			newNode->mNext[Red] = curr; // Set next pointer
			prev->mNext[Red] = newNode; // Set next pointer
		}

		if (blueIndex == 0) // If index is 0
		{
			newNode->mNext[Blue] = mHead[Blue]; // Set next pointer
			mHead[Blue] = newNode; // Set head pointer.
		}
		else if (blueIndex == size) // If index is size of list
		{
			prev = get(blueIndex - 1, Blue); // Set previous node to blue index - 1
			prev->mNext[Blue] = newNode; // Set next pointer
			newNode->mNext[Blue] = nullptr; // Set next pointer to null
		}
		else
		{
			curr = get(blueIndex, Blue); // Set current node to blue index
			prev = get(blueIndex - 1, Blue); // Set previous node to blue index - 1
			newNode->mNext[Blue] = curr; // Set next pointer
			prev->mNext[Blue] = newNode; // Set next pointer
		}

		return newNode; // Return new node		
	}	
}


template <typename T>
void DoubleList<T>::remove(unsigned index, Color color) 
{
	Color otherColor = (color == Red) ? Blue : Red; // Set other color
	Node *curr = mHead[color]; // Set current node to head of color list
	Node *prev; // Previous node pointer
	Node *curr2 = mHead[otherColor]; // Current node pointer 2
	Node *prev2; // Previous node pointer 2
	unsigned int size = 0; // Size of list
	unsigned int otherColorIndex = 0; // Index of other color

	while (curr != nullptr) // While current node is not null
	{
		size++; // Increment size
		curr = curr->mNext[color]; // Set current node to next node
	}	

	if (index >= size) // If index is larger than size of list
	{
		throw InvalidIndexException(); // Throw exception
	}
	else
	{
		curr = get(index, color); // Set current node to index

		Node *temp = curr; // Set temp node to current node
		while(temp != nullptr) // While temp node is not null
		{
			otherColorIndex++; // Increment other color index
			temp = temp->mNext[otherColor]; // Set temp node to next node
		}
	
		otherColorIndex = size - otherColorIndex; // Set other color index to size - other color index
		curr2 = get(otherColorIndex, otherColor); // Set current node 2 to other color index
		
		if (index == 0) // If index is 0
		{
			mHead[color] = mHead[color]->mNext[color]; // Set head pointer.
		}
		else if (index == size - 1) // If index is size of list - 1
		{
			prev = get(index - 1, color); // Set previous node to index - 1
			prev->mNext[color] = nullptr; // Set next pointer
		}
		else
		{
			prev = get(index - 1, color); // Set previous node to index - 1
			prev->mNext[color] = curr->mNext[color]; // Set next pointer
		}

		if (otherColorIndex == 0) // If other color index is 0
		{
			mHead[otherColor] = mHead[otherColor]->mNext[otherColor]; // Set head pointer.
		}
		else if (otherColorIndex == size - 1) // If other color index is size of list - 1
		{
			prev2 = get(otherColorIndex - 1, otherColor); // Set previous node to other color index - 1
			prev2->mNext[otherColor] = nullptr; // Set next pointer
		}
		else
		{
			prev2 = get(otherColorIndex - 1, otherColor); // Set previous node to other color index - 1
			prev2->mNext[otherColor] = curr2->mNext[otherColor]; // Set next pointer
		}

		delete curr; // Delete current node
	}
}

template <typename T>
void DoubleList<T>::append(DoubleList<T> &dl) 
{
	Node *curr = mHead[Red]; // Set current node to head of color list
	unsigned int size = 0; // Size of list

	while (curr != nullptr) // While current node is not null
	{
		size++; // Increment size
		curr = curr->mNext[Red]; // Set current node to next node
	}

	if (size == 0)
	{
		mHead[Red] = dl.mHead[Red];
		mHead[Blue] = dl.mHead[Blue];
	}
	else
	{
		Node *redLast = get(size - 1, Red); // Set red last node to size - 1
		Node *blueLast = get(size - 1, Blue); // Set blue last node to size - 1
		redLast->mNext[Red] = dl.mHead[Red]; // Set red last node to dl head
		blueLast->mNext[Blue] = dl.mHead[Blue]; // Set blue last node to dl head
	}

	dl.mHead[Red] = nullptr; // Set dl head to null
	dl.mHead[Blue] = nullptr; // Set dl head to null
}

template <typename T>
DoubleList<T>::~DoubleList() 
{
	Node *curr = mHead[Red]; // Set current node to head of color list
	unsigned int i; // Index
	unsigned int size = 0; // Size of list

	while (curr != nullptr) // While current node is not null
	{
		size++; // Increment size
		curr = curr->mNext[Red]; // Set current node to next node
	}	

    for (i = 0; i < size; i++) // For each node in double list
    {
        remove(0, Red); // Remove node from double list
    }
}