#pragma once

template<typename T>
class EHeap
{
    private:
        class HandleData
        {
            friend EHeap;
            private:
                unsigned index;
                T data;
        };

        unsigned mSize;
        HandleData **mHeap; 

        void swap(HandleData *a, HandleData *b)
        {
            unsigned tempIndex = a->index;
            a->index = b->index;
            b->index = tempIndex;
            
            HandleData *temp = mHeap[a->index];
            mHeap[a->index] = mHeap[b->index];
            mHeap[b->index] = temp;
        }

        unsigned percolateUp(unsigned i)
        {
            while (i > 1 and mHeap[i / 2]->data > mHeap[i]->data)
            {
                swap(mHeap[i / 2], mHeap[i]);
                i = i / 2;
            }

            return i;
        }

        void percolateDown(unsigned i)
        {
            while (2 * i <= mSize)
            {
                unsigned minChild;

                if (2 * i + 1 <= mSize and mHeap[2 * i + 1]->data < mHeap[2 * i]->data)
                {
                    minChild = 2 * i + 1;
                }
                else
                {
                    minChild = 2 * i;
                }

                if (mHeap[minChild]->data < mHeap[i]->data)
                {
                    swap(mHeap[minChild], mHeap[i]);
                    i = minChild;
                }
                else
                {
                    break;
                }
            }
        }        


    public:
        typedef HandleData * Handle;

        // Constructs an empty e-heap with a fixed capacity.
        EHeap(unsigned capacity) : mSize(0),
                                   mHeap(new Handle[capacity + 1])
        {}

        unsigned size() const 
        { 
            return mSize;
        }

        Handle push(T t) 
        { 
            mSize++;
            mHeap[mSize] = new HandleData;
            mHeap[mSize]->data = t;
            mHeap[mSize]->index = mSize;

            unsigned i = percolateUp(mSize); 
            mHeap[i]->index = i;

            return mHeap[i];
        }     

        T pop() 
        { 
            T popped = mHeap[1]->data;
            swap(mHeap[1], mHeap[mSize]);
            delete mHeap[mSize];
            mSize--;

            percolateDown(1);

            return popped;      
        }

        T erase(Handle handle) 
        { 
            T value = handle->data;
            unsigned index = handle->index;

            swap(mHeap[index], mHeap[mSize]);
            delete mHeap[mSize];
            mSize--;

            percolateUp(index);
            percolateDown(index);


            return value;
        }

        ~EHeap() 
        {
            for (unsigned i = 1; i <= mSize; delete mHeap[i], i++) {}
            delete [] mHeap;
        }
};
