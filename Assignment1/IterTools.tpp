#pragma once
#include "Iterable.tpp"

// Write your implementations here.
template<typename T>
class RepeatIterator : public IIterator<T>
{
    private:
        T mValue;

    public:
        RepeatIterator(T v) : mValue(v)
        {
        }

        virtual T next()
        {
            return mValue;
        }

        virtual ~RepeatIterator()
        {
        }
};

template<typename T>
class RepeatIterable : public IIterable<T>
{
    private:
        T mValue;

    public:
        RepeatIterable(T v) : mValue(v)
        {
        }

        virtual IIterator<T> * iter() const
        {
            return new RepeatIterator<T>(mValue);
        }

        virtual ~RepeatIterable()
        {
        }
};

template<typename T>
IIterable<T> * repeat(T v)
{
    return new RepeatIterable<T>(v);
}


template<typename T>
class CountIterator : public IIterator<T>
{
    private:
        T mStart;
        T mDelta;
        T mAccumulation;
        bool mHasIterationStarted;


    public:
        CountIterator(T start , T delta) :  mStart(start),
                                            mDelta(delta),
                                            mAccumulation(),
                                            mHasIterationStarted(false)
        {
        }

        virtual T next()
        {
            if (!mHasIterationStarted)
            {
                mAccumulation = mStart;
                mHasIterationStarted = true;
            }
            else
            {
                mAccumulation += mDelta;
            }

            return mAccumulation;
        }

        virtual ~CountIterator()
        {
        }
};

template<typename T>
class CountIterable : public IIterable<T>
{
    private:
        T mStart;
        T mDelta;

    public:
        CountIterable(T start , T delta) :  mStart(start),
                                            mDelta(delta)
        {
        }

        virtual IIterator<T> * iter() const
        {
            return new CountIterator<T>(mStart, mDelta);
        }

        virtual ~CountIterable()
        {
        }
};

template<typename T>
IIterable<T> * count(T start, T delta)
{
    return new CountIterable<T>(start, delta);
}


template<typename T>
class SkipIterator : public IIterator<T>
{
    private:
        IIterator<T> *mIterator;
        unsigned mSkip;
        unsigned mStep;
        T mNext;
        bool mHasIterationStarted;


    public:
        SkipIterator(unsigned k , IIterator<T> *iterator) : mIterator(iterator),
                                                            mSkip(k),
                                                            mStep(0),
                                                            mNext(),
                                                            mHasIterationStarted(false)
        {
        }

        virtual T next()
        {
            if(!mHasIterationStarted)
            {
                while(mStep <= mSkip)         
                {
                    mNext = mIterator->next();
                    mStep ++;
                }

                mHasIterationStarted = true;
            }
            else
            {
                mNext = mIterator->next();
            }

            return mNext;
        }

        virtual ~SkipIterator()
        {
            delete mIterator;
        }
};

template<typename T>
class SkipIterable : public IIterable<T>
{
    private:
        IIterable<T> *mIterable;
        unsigned mSkip;


    public:
        SkipIterable(unsigned k , IIterable<T> *iterable) : mIterable(iterable),
                                                            mSkip(k)
        {
        }

        virtual IIterator<T> * iter() const
        {
            return new SkipIterator<T>(mSkip, mIterable->iter());
        }

        virtual ~SkipIterable()
        {
            delete mIterable;
        }
};

template<typename T>
IIterable<T> * skip(unsigned k , IIterable<T> *i)
{
    return new SkipIterable<T>(k, i);
}


template<typename T>
class TakeIterator : public IIterator<T>
{
    private:
        IIterator<T> *mIterator;
        unsigned mTake;
        unsigned mStep;
        T mNext;
        bool mHasIterationCompleted;


    public:
        TakeIterator(unsigned k , IIterator<T> *iterator) : mIterator(iterator),
                                                            mTake(k),
                                                            mStep(0),
                                                            mNext(),
                                                            mHasIterationCompleted(false)
        {
        }

        virtual T next()
        {
            if(mStep < mTake)         
            {
                mNext = mIterator->next();
                mStep++;
            } 
            else
            {
                mHasIterationCompleted = true;
            }  

            if(mHasIterationCompleted)   
            {
                throw StopIteration();
            }    

            return mNext;     
        }

        virtual ~TakeIterator()
        {
            delete mIterator;
        }
};

template<typename T>
class TakeIterable : public IIterable<T>
{
    private:
        IIterable<T> *mIterable;
        unsigned mTake;


    public:
        TakeIterable(unsigned k , IIterable<T> *iterable) : mIterable(iterable),
                                                            mTake(k)
        {
        }

        virtual IIterator<T> * iter() const
        {
            return new TakeIterator<T>(mTake, mIterable->iter());
        }

        virtual ~TakeIterable()
        {
            delete mIterable;
        }
};

template<typename T>
IIterable<T> * take(unsigned k , IIterable<T> *i)
{
    return new TakeIterable<T>(k, i);
}


template<typename T>
class CycleIterator : public IIterator<T>
{
    private:
        IIterator<T> *mIterator;
        IIterable<T> *mIterable;
        T mNext;
        bool mHasIterationStarted;


    public:
        CycleIterator(IIterator<T> *iterator, IIterable<T> *iterable) : mIterator(iterator),
                                                                        mIterable(iterable),
                                                                        mNext(),
                                                                        mHasIterationStarted(false)
        {
        }

        virtual T next()
        {
            if (!mHasIterationStarted)
            {
                mNext = mIterator->next();
                mHasIterationStarted = true;
            }
            else
            {
                try
                {
                    mNext = mIterator->next();
                }
                catch (StopIteration &)
                {
                    delete mIterator;
                    mIterator = mIterable->iter();
                    mNext = mIterator->next();
                } 
            }

            return mNext;                    
        }

        virtual ~CycleIterator()
        {
            delete mIterator;
        }
};

template<typename T>
class CycleIterable : public IIterable<T>
{
    private:
        IIterable<T> *mIterable;

    public:
        CycleIterable(IIterable<T> *iterable) : mIterable(iterable)
        {
        }

        virtual IIterator<T> * iter() const
        {
            return new CycleIterator<T>(mIterable->iter(), mIterable);
        }

        virtual ~CycleIterable()
        {
            delete mIterable;
        }
};

template<typename T>
IIterable<T> * cycle(IIterable<T> *i)
{
    return new CycleIterable<T>(i);
}


template<typename T>
class ConcatIterator : public IIterator<T>
{
    private:
        IIterator<T> *mIterator;
        IIterable<T> *mIterable;
        T mNext;
        bool mHasIterationStarted;


    public:
        ConcatIterator(IIterator<T> *iterator, IIterable<T> *iterable) : mIterator(iterator),
                                                                         mIterable(iterable),
                                                                         mNext(),
                                                                         mHasIterationStarted(false)
        {
        }

        virtual T next()
        {
            if (!mHasIterationStarted)
            {
                try
                {
                    mNext = mIterator->next();
                }
                catch (StopIteration &)
                {
                    delete mIterator;
                    mIterator = mIterable->iter();
                    mNext = mIterator->next();
                    mHasIterationStarted = true;
                } 
            }
            else
            {
                try
                {
                    mNext = mIterator->next();
                }
                catch (StopIteration &)
                {
                    throw StopIteration();
                }                
            }

            return mNext;                    
        }

        virtual ~ConcatIterator()
        {
            delete mIterator;
        }
};

template<typename T>
class ConcatIterable : public IIterable<T>
{
    private:
        IIterable<T> *mIterable1;
        IIterable<T> *mIterable2;
       

    public:
        ConcatIterable(IIterable<T> *iterable1, IIterable<T> *iterable2) : mIterable1(iterable1),
                                                                           mIterable2(iterable2)
        {
        }

        virtual IIterator<T> * iter() const
        {
            return new ConcatIterator<T>(mIterable1->iter(), mIterable2);
        }

        virtual ~ConcatIterable()
        {
            delete mIterable1;
            delete mIterable2;
        }
};

template<typename T>
IIterable<T> * concat(IIterable <T> *i, IIterable <T> *j)
{
    return new ConcatIterable<T>(i, j);
}


template<typename T>
class AlternateIterator : public IIterator<T>
{
    private:
        IIterator<T> *mIterator1;
        IIterator<T> *mIterator2;
        T mNext;
        bool mHasIterationSwitch;
        bool mHasIteration1Ended;
        bool mHasIteration2Ended;


    public:
        AlternateIterator(IIterable<T> *iterable1, IIterable<T> *iterable2) : mIterator1(iterable1->iter()),
                                                                              mIterator2(iterable2->iter()),
                                                                              mNext(),
                                                                              mHasIterationSwitch(false),
                                                                              mHasIteration1Ended(false),
                                                                              mHasIteration2Ended(false)

        {
        }

        virtual T next()
        {
            if (!mHasIterationSwitch && !mHasIteration1Ended)
            {
                if (!mHasIteration2Ended)
                {
                    mHasIterationSwitch = true;
                }

                try
                {
                    mNext = mIterator1->next();
                }
                catch (StopIteration &)
                {
                    mHasIteration1Ended = true;
                    if (!mHasIteration2Ended)
                    {
                        mNext = mIterator2->next();
                    }
                } 
            }

            else if (mHasIterationSwitch && !mHasIteration2Ended)
            {
                if (!mHasIteration1Ended)
                {
                    mHasIterationSwitch = false;
                }

                try
                {
                    mNext = mIterator2->next();
                }
                catch (StopIteration &)
                {
                    mHasIteration2Ended = true;
                    if (!mHasIteration1Ended)
                    {
                        mNext = mIterator1->next();
                    }
                }                
            }

            if (mHasIteration1Ended && mHasIteration2Ended)
            {
                throw StopIteration();
            }

            return mNext;                    
        }

        virtual ~AlternateIterator()
        {
            delete mIterator1;
            delete mIterator2;
        }
};

template<typename T>
class AlternateIterable : public IIterable<T>
{
    private:
        IIterable<T> *mIterable1;
        IIterable<T> *mIterable2;
       

    public:
        AlternateIterable(IIterable<T> *iterable1, IIterable<T> *iterable2) : mIterable1(iterable1),
                                                                                 mIterable2(iterable2)
        {
        }

        virtual IIterator<T> * iter() const
        {
            return new AlternateIterator<T>(mIterable1, mIterable2);
        }

        virtual ~AlternateIterable()
        {
            delete mIterable1;
            delete mIterable2;
        }
};

template<typename T>
IIterable<T> * alternate(IIterable <T> *i, IIterable <T> *j)
{
    return new AlternateIterable<T>(i, j);
}



template<typename T>
class FilterIterator : public IIterator<T>
{
    private:
        IIterator<T> *mIterator1;
        IIterator<bool> *mIterator2;
        T mNext;
        bool mBool;
        bool mHasIterationEnded;


    public:
        FilterIterator(IIterable<T> *iterable1, IIterable<bool> *iterable2) : mIterator1(iterable1->iter()),
                                                                              mIterator2(iterable2->iter()),
                                                                              mNext(),
                                                                              mBool(false),
                                                                              mHasIterationEnded(false)

        {
        }

        virtual T next()
        {
            while (!mHasIterationEnded && !mBool)
            {
                try
                {
                    mNext = mIterator1->next();
                }
                catch (StopIteration &)
                {
                    mHasIterationEnded = true;
                } 

                try
                {
                    mBool = mIterator2->next();
                }
                catch (StopIteration &)
                {
                    mHasIterationEnded = true;
                } 
            }

            if (mHasIterationEnded)
            {
                throw StopIteration();
            }

            mBool = false;
            return mNext;                    
        }

        virtual ~FilterIterator()
        {
            delete mIterator1;
            delete mIterator2;
        }
};

template<typename T>
class FilterIterable : public IIterable<T>
{
    private:
        IIterable<T> *mIterable1;
        IIterable<bool> *mIterable2;
       

    public:
        FilterIterable(IIterable<T> *iterable1, IIterable<bool> *iterable2) : mIterable1(iterable1),
                                                                              mIterable2(iterable2)
        {
        }

        virtual IIterator<T> * iter() const
        {
            return new FilterIterator<T>(mIterable1, mIterable2);
        }

        virtual ~FilterIterable()
        {
            delete mIterable1;
            delete mIterable2;
        }
};

template<typename T>
IIterable<T> * filter(IIterable <T> *i, IIterable <bool> *f)
{
    return new FilterIterable<T>(i, f);
}