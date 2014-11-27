#include <stdexcept>

namespace containers
{
    template <typename T>
    class au_list
    {
    public:
        typedef T value_type;
        typedef value_type& reference;
        typedef value_type const& const_reference;
        typedef value_type* pointer;
        typedef value_type const* const_pointer;
        typedef size_t size_type;

        struct node
        {
            node(const_reference value, node* prev, node* next)
                    : value_(value)
                    , prev_(prev)
                    , next_(next)
            {
            }

            node(const_reference value)
                    : node(value, nullptr, nullptr)
            {}

            node()
                    : node(0)
            {}

            reference getValue() const
            {
                return value_;
            }

            void setValue(const_reference value)
            {
                value_ = value;
            }

            node* getPrev() const
            {
                return prev_;
            }

            node* getNext() const
            {
                return next_;
            }

        private:
            value_type value_;
            node* prev_;
            node* next_;

            friend class au_list;
        };

        au_list()
        {
            begin_ = new node();
            end_ = begin_;

            size_ = 0;
        }

        au_list& operator=(au_list const& list)
        {
            return au_list(list);
        }

        au_list(au_list const& list)
            : au_list()
        {
            node* current = list.begin();

            while (current != list.end())
            {
                insert(end(), current->getValue());
                current = current->getNext();
            }
        }

        ~au_list()
        {
            clear();
        }

        node* insert(node* position, const_reference value)
        {
            node* temp = new node(value, position->getPrev(), position);

            if (position->getPrev() != nullptr)
            {
                position->getPrev()->next_ = temp;
            }

            position->prev_ = temp;
            if (position == begin_)
            {
                begin_ = temp;
            }

            return temp;
        }

        template <typename V>
        node* insert(node* position, V* first, V* last)
        {
            node* result = insert(position, value_type(first));
            ++first;

            while (first != last)
            {
                insert(position, value_type(first));
                ++first;
            }

            return result;
        }

        node* erase(node* position)
        {
            if (empty())
            {
                throw new std::runtime_error("cannot remove element from empty list");
            }

            if (position == end_->getPrev())
            {
                if (position->getPrev() != nullptr)
                {
                    position->getPrev()->next_ = end_;
                }

                end_->prev_ = position->getPrev();
                if (position == begin_)
                {
                    begin_ = position->getNext();
                }

                delete position;

                return end_;
            } else if (position == begin_)
            {
                begin_ = position->getNext();
                begin_->prev_ = nullptr;

                delete position;

                return begin_;
            }
            else
            {
                position->getPrev()->next_ = position->getNext();
                position->getNext()->prev_ = position->getPrev();

                node* result = position->getNext();
                delete position;

                return result;
            }
        }

        node* erase(node* first, node* last)
        {
            while (first != last)
            {
                node* temp = first;
                first = first->getNext();

                erase(temp);
            }
        }

        node* begin() const
        {
            return begin_;
        }

        node* end() const
        {
            return end_;
        }

        bool empty() const
        {
            return begin_ == end_;
        }

        size_type size() const
        {
            return size_;
        }

        void clear()
        {
            node* current = begin();

            while (current != nullptr)
            {
                node* temp = current;
                current = current->getNext();

                delete temp;
            }

            begin_ = new node();
            end_ = begin_;

            size_ = 0;
        }

    private:
        node* begin_;
        node* end_;

        size_type size_;
    };
}