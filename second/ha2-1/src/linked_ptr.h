#pragma once

#include <algorithm>

namespace smart_ptr
{
    namespace details
    {
        class node_t
        {
        public:
            node_t()
                : next_(nullptr)
                , previous_(nullptr)
            {
            }

            ~node_t()
            {
                if (next_ != nullptr)
                {
                    next_->previous_ = previous_;
                }
                if (previous_ != nullptr)
                {
                    previous_->next_ = next_;
                }

                next_ = nullptr;
                previous_ = nullptr;
            }

            node_t(node_t const& node)
                : next_(node.next_)
                , previous_(&node)
            {
                previous_->next_ = this;
                if (next_ != nullptr)
                {
                    next_->previous_ = this;
                }
            }

            void swap(node_t& node)
            {
                bool swap_left = previous_ == &node;
                bool swap_right = next_ == &node;

                std::swap(previous_, node.previous_);
                std::swap(next_, node.next_);

                if (swap_left)
                {
                    next_ = &node;
                    node.previous_ = this;
                }
                else
                if (swap_right)
                {
                    previous_ = &node;
                    node.next_ = this;
                }

                if (previous_ != nullptr)
                {
                    previous_->next_ = this;
                }
                if (next_ != nullptr)
                {
                    next_->previous_ = this;
                }

                if (node.previous_ != nullptr)
                {
                    node.previous_->next_ = &node;
                }
                if (node.next_ != nullptr)
                {
                    node.next_->previous_ = &node;
                }
            }

            bool unique() const
            {
                return next_ == nullptr && previous_ == nullptr;
            }

        private:
            mutable node_t const* next_;
            mutable node_t const* previous_;
        };
    } // linked list node

    template <typename T>
    class linked_ptr
    {
    public:
        explicit linked_ptr();
        ~linked_ptr();

        explicit linked_ptr(T*);
        template <typename U>
        explicit linked_ptr(U*);

        linked_ptr(linked_ptr<T> const&);
        template <typename U>
        linked_ptr(linked_ptr<U> const&);

        linked_ptr<T>& operator=(linked_ptr<T> const&);
        template <typename U>
        linked_ptr<T>& operator=(linked_ptr<U> const&);

        void reset();
        void reset(T*);
        template <typename U>
        void reset(U*);

        T* get() const;
        void swap(linked_ptr<T>&);
        bool unique() const;

        T* operator->() const;
        T& operator*() const;

        template <typename U>
        bool operator==(linked_ptr<U> const&) const;
        template <typename U>
        bool operator<(linked_ptr<U> const&) const;

        explicit operator bool() const;

        details::node_t& node() const;

    private:
        mutable details::node_t node_;
        T* resource_;
    };

    template <typename T>
    linked_ptr<T>::linked_ptr()
        : linked_ptr<T>(nullptr)
    {
    }

    template <typename T>
    linked_ptr<T>::~linked_ptr()
    {
        typedef char type_must_be_complete[sizeof(T)? 1: -1];
        (void) sizeof(type_must_be_complete);

        if (unique())
        {
            delete resource_;
        }
    }

    template <typename T>
    linked_ptr<T>::linked_ptr(T* resource)
        : resource_(resource)
    {
    }

    template <typename T>
    template <typename U>
    linked_ptr<T>::linked_ptr(U* resource)
        : resource_(resource)
    {
    }

    template <typename T>
    linked_ptr<T>::linked_ptr(linked_ptr<T> const& pointer)
        : node_(pointer.node_)
        , resource_(pointer.get())
    {
    }

    template <typename T>
    template <typename U>
    linked_ptr<T>::linked_ptr(linked_ptr<U> const& pointer)
        : node_(pointer.node())
        , resource_(pointer.get())
    {
    }

    template <typename T>
    linked_ptr<T>& linked_ptr<T>::operator=(linked_ptr<T> const& pointer)
    {
        linked_ptr<T> temp(pointer);
        swap(temp);
        return *this;
    }

    template <typename T>
    template <typename U>
    linked_ptr<T>& linked_ptr<T>::operator=(linked_ptr<U> const& pointer)
    {
        linked_ptr<T> temp(pointer);
        swap(temp);
        return *this;
    }

    template <typename T>
    void linked_ptr<T>::reset()
    {
        linked_ptr<T> temp;
        swap(temp);
    }

    template <typename T>
    void linked_ptr<T>::reset(T* resource)
    {
        linked_ptr<T> temp(resource);
        swap(temp);
    }

    template <typename T>
    template <typename U>
    void linked_ptr<T>::reset(U* resource)
    {
        linked_ptr<T> temp(resource);
        swap(temp);
    }

    template <typename T>
    T* linked_ptr<T>::get() const
    {
        return resource_;
    }

    template <typename T>
    void linked_ptr<T>::swap(linked_ptr<T>& pointer)
    {
        if (get() != pointer.get())
        {
            std::swap(resource_, pointer.resource_);
        }
        node_.swap(pointer.node_);
    }

    template <typename T>
    bool linked_ptr<T>::unique() const
    {
        return get() ? node_.unique() : false;
    }

    template <typename T>
    T* linked_ptr<T>::operator->() const
    {
        return get();
    }

    template <typename T>
    T& linked_ptr<T>::operator*() const
    {
        return *resource_;
    }

    template <typename T>
    template <typename U>
    bool linked_ptr<T>::operator==(linked_ptr<U> const& pointer) const
    {
        return get() == pointer.get();
    }

    template <typename T>
    template <typename U>
    bool linked_ptr<T>::operator<(linked_ptr<U> const& pointer) const
    {
        return get() < pointer.get();
    }

    template <typename T>
    linked_ptr<T>::operator bool() const
    {
        return get() != nullptr;
    }

    template <typename T>
    details::node_t& linked_ptr<T>::node() const
    {
        return node_;
    }

    template <typename T, typename U>
    bool operator!=(linked_ptr<T> const& lhs, linked_ptr<U> const& rhs)
    {
        return !(lhs == rhs);
    }

    template <typename T, typename U>
    bool operator<=(linked_ptr<T> const& lhs, linked_ptr<U> const& rhs)
    {
        return !(lhs > rhs);
    }

    template <typename T, typename U>
    bool operator>(linked_ptr<T> const& lhs, linked_ptr<U> const& rhs)
    {
        return rhs < lhs;
    }

    template <typename T, typename U>
    bool operator>=(linked_ptr<T> const& lhs, linked_ptr<U> const& rhs)
    {
        return !(lhs < rhs);
    }
} // smart pointers

