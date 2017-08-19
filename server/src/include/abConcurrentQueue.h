#ifndef _AB_CONCURRENT_QUEUE_H__
#define _AB_CONCURRENT_QUEUE_H__

// ABConcurrentQueue - Proprietary and confidential
// Author Robert J. Gebis
// Copyright (C) Autobon.ai, LLC - All Rights Reserved
// Unauthorized copying of this file via any medium
// is strictly prohibited without the written permission

#include <deque>
#include <cstdint>
#include <mutex>
#include <condition_variable>

struct ABQueueCanceled {};

template<typename T>
class ABConcurrentQueue
{
    public:
        typedef std::deque<T> ABQueue;

        ABConcurrentQueue(void)
            : /*_pThreadGroup(0L),*/ _isCanceled(false) {}

        //ABConcurrentQueue(boost::thread_group& thGroup)
        //    : _pThreadGroup(&thGroup), _isCanceled(false) {}

       ~ABConcurrentQueue(void)
       {
           cancel();

//           if( _pThreadGroup )
//                _pThreadGroup->join_all();
       }

        inline bool isCanceled(void)
        {
            return _isCanceled;
        }

        inline bool push(T const& data)
        {
            //boost::unique_lock<boost::mutex>  lock(_mutex);

            if (_isCanceled)
                return false;

            _queue.push_back(data);

            //lock.unlock();

            //_condVariable.notify_one();

            return true;
        }

        inline void push_front(T const& data)
        {
            //boost::unique_lock<boost::mutex>  lock(_mutex);

            if (_isCanceled)
                throw ABQueueCanceled();

            _queue.push_front(data);

            //lock.unlock();

            //_condVariable.notify_one();
        }

        inline bool    tryPop(T& value)
        {
            //boost::unique_lock<boost::mutex>  lock(_mutex);

            if (_isCanceled)
                throw ABQueueCanceled();

            if(_queue.empty())
                return false;

            value = _queue.front();

            _queue.pop_front();

            return true;
        }

        inline bool    tryPopAll( ABQueue& queue )
        {
            //boost::unique_lock<boost::mutex>  lock(_mutex);

            if (_isCanceled)
                throw ABQueueCanceled();

            if(_queue.empty())
                return false;

            queue.insert(queue.end(), _queue.begin(), _queue.end() );
        
            _queue.clear();

            return true;
        }

        inline bool    pop(T& value)
        {
            //boost::unique_lock<boost::mutex>  lock(_mutex);

            //while(_queue.empty() && !_isCanceled)
            //    _condVariable.wait(lock);

            if (_isCanceled)
                return false;

            value = _queue.front();

            _queue.pop_front();

            return true;
        }

        inline bool    popAll(ABQueue& queue)
        {
            //boost::unique_lock<boost::mutex>  lock(_mutex);

            //while(_queue.empty() && !_isCanceled)
            //    _condVariable.wait(lock);

            if(_isCanceled)
                return false;

            queue.insert(queue.end(), _queue.begin(), _queue.end() );
        
            _queue.clear();

            return true;
        }

        inline int32_t    popAllWithSize(ABQueue& queue )
        {
            //boost::unique_lock<boost::mutex>  lock(_mutex);

            //while(_queue.empty() && !_isCanceled)
            //    _condVariable.wait(lock);

            if(_isCanceled)
                return -1;

            queue.insert(queue.end(), _queue.begin(), _queue.end() );
        
            _queue.clear();

            return static_cast<int32_t>(queue.size());
        }

        inline void    removeAll(ABQueue& queue )
        {
            //boost::unique_lock<boost::mutex>  lock(_mutex);

            if( _queue.size() )
                queue.insert(queue.end(), _queue.begin(), _queue.end() );
        
            _queue.clear();
        }

        inline void    cancel(void)
        {
           //boost::unique_lock<boost::mutex>  lock(_mutex);

           if (_isCanceled)
                return;

           _isCanceled = true;

           //_condVariable.notify_all();
        }

        inline void    cancelAndWait(void)
        {
           //boost::unique_lock<boost::mutex>  lock(_mutex);

           if (_isCanceled)
                return;

           _isCanceled = true;

           //_condVariable.notify_all();

           //while( _queue.empty() == false )
           //    boost::this_thread::sleep(boost::posix_time::milliseconds(100));
        }

        inline bool    empty(void) const throw(ABQueueCanceled)
        {
            //boost::unique_lock<boost::mutex>  lock(_mutex);

            if (_isCanceled)
                throw ABQueueCanceled();

            return _queue.empty();
        }

        inline uint32_t    size(void)
        {
           //boost::unique_lock<boost::mutex>  lock(_mutex);

            return static_cast<int32_t>(_queue.size());
        }

        inline ABQueue& lock(void)
        {
            //_mutex.lock();
            return _queue;
        }

        inline void unlock(void)
        {
            //_mutex.unlock();
        }

        private:
                    ABQueue                    _queue;
            //mutable boost::mutex                _mutex;
            //        boost::condition_variable   _condVariable;
                    std::unique_lock<std::mutex> _condVariable;
            //        boost::thread_group*        _pThreadGroup;
                    bool                        _isCanceled;
};

#endif // _AB_CONCURRENT_QUEUE_H__

