#pragma once

namespace fn
{
    namespace details
    {
        template <typename Function, typename Argument>
        class bind_t
        {
            typedef bind_t<Function, Argument> BindT;

            Function function_;
            Argument argument_;
        public:
            bind_t(Function const&, Argument);
            bind_t(BindT const&);
            bind_t(BindT&&);

            BindT& operator=(BindT);

            template <typename... Arguments>
            auto operator()(Arguments... arguments) -> decltype(function_(argument_, arguments...));

            void swap(BindT&);
        };

        struct placeholder1
        {
        };

        template <typename Function>
        class bind_t<Function, placeholder1>
        {
            typedef bind_t<Function, placeholder1> BindT;

            Function function_;
        public:
            bind_t(Function const& function, placeholder1);
            bind_t(BindT const&);
            bind_t(BindT&&);

            BindT& operator=(BindT);

            template <typename FirstArgument, typename SecondArgument, typename... Arguments>
            auto operator()(FirstArgument first_argument, SecondArgument second_argument, Arguments... arguments) -> decltype(function_(second_argument, first_argument, arguments...));

            void swap(BindT&);
        };

        struct placeholder2
        {
        };

        template <typename Function>
        class bind_t<Function, placeholder2>
        {
            typedef bind_t<Function, placeholder2> BindT;

            Function function_;
        public:
            bind_t(Function const& function, placeholder2);
            bind_t(BindT const&);
            bind_t(BindT&&);

            BindT& operator=(BindT);

            template <typename FirstArgument, typename SecondArgument, typename ThirdArgument, typename... Arguments>
            auto operator()(FirstArgument first_argument, SecondArgument second_argument, ThirdArgument third_argument, Arguments... arguments) -> decltype(function_(third_argument, first_argument, second_argument, arguments...));

            void swap(BindT&);
        };
    } // details

    const static details::placeholder1 _1{};
    const static details::placeholder2 _2{};

    template <typename Function, typename Argument>
    details::bind_t<Function, Argument> bind(Function const& function, Argument argument);

    template <typename Function, typename Argument, typename... Arguments>
    auto bind(Function const& function, Argument argument, Arguments... arguments) -> decltype(bind(details::bind_t<Function, Argument>(function, argument), arguments...));
} // bind & function

template <typename Function, typename Argument>
fn::details::bind_t<Function, Argument>::bind_t(Function const& function, Argument argument)
    : function_(function)
    , argument_(argument)
{
}

template <typename Function, typename Argument>
fn::details::bind_t<Function, Argument>::bind_t(BindT const& other)
    : bind_t(other.function_, other.argument_)
{
}

template <typename Function, typename Argument>
fn::details::bind_t<Function, Argument>::bind_t(BindT&& other)
    : function_(std::move(other.function_))
    , argument_(std::move(other.argument_))
{
}

template <typename Function, typename Argument>
fn::details::bind_t<Function, Argument>& fn::details::bind_t<Function, Argument>::operator=(BindT other)
{
    if (this != &other)
    {
        swap(other);
    }
    return *this;
}

template <typename Function, typename Argument>
template <typename... Arguments>
auto fn::details::bind_t<Function, Argument>::operator()(Arguments... arguments) -> decltype(function_(argument_, arguments...))
{
    return function_(argument_, arguments...);
}

template <typename Function, typename Argument>
void fn::details::bind_t<Function, Argument>::swap(BindT& other)
{
    std::swap(function_, other.function_);
    std::swap(argument_, other.argument_);
}

template <typename Function>
fn::details::bind_t<Function, fn::details::placeholder1>::bind_t(Function const& function, placeholder1)
    : function_(function)
{
}

template <typename Function>
fn::details::bind_t<Function, fn::details::placeholder1>::bind_t(BindT const& other)
    : bind_t(other.function_, _1)
{
}

template <typename Function>
fn::details::bind_t<Function, fn::details::placeholder1>::bind_t(BindT&& other)
    : function_(std::move(other.function_))
{
}

template <typename Function>
fn::details::bind_t<Function, fn::details::placeholder1>& fn::details::bind_t<Function, fn::details::placeholder1>::operator=(BindT other)
{
    if (this != &other)
    {
        swap(other);
    }
    return *this;
}

template <typename Function>
template <typename FirstArgument, typename SecondArgument, typename... Arguments>
auto fn::details::bind_t<Function, fn::details::placeholder1>::operator()(FirstArgument first_argument, SecondArgument second_argument, Arguments... arguments) -> decltype(function_(second_argument, first_argument, arguments...))
{
    return function_(second_argument, first_argument, arguments...);
}

template <typename Function>
void fn::details::bind_t<Function, fn::details::placeholder1>::swap(BindT& other)
{
    std::swap(function_, other.function_);
}

template <typename Function>
fn::details::bind_t<Function, fn::details::placeholder2>::bind_t(Function const& function, placeholder2)
    : function_(function)
{
}

template <typename Function>
fn::details::bind_t<Function, fn::details::placeholder2>::bind_t(BindT const& other)
    : bind_t(other.function_, _2)
{
}

template <typename Function>
fn::details::bind_t<Function, fn::details::placeholder2>::bind_t(BindT&& other)
    : function_(std::move(other.function_))
{
}

template <typename Function>
fn::details::bind_t<Function, fn::details::placeholder2>& fn::details::bind_t<Function, fn::details::placeholder2>::operator=(BindT other)
{
    if (this != &other)
    {
        swap(other);
    }
    return *this;
}

template <typename Function>
template <typename FirstArgument, typename SecondArgument, typename ThirdArgument, typename... Arguments>
auto fn::details::bind_t<Function, fn::details::placeholder2>::operator()(FirstArgument first_argument, SecondArgument second_argument, ThirdArgument third_argument, Arguments... arguments) -> decltype(function_(third_argument, first_argument, second_argument, arguments...))
{
    return function_(third_argument, first_argument, second_argument, arguments...);
}

template <typename Function>
void fn::details::bind_t<Function, fn::details::placeholder2>::swap(BindT& other)
{
    std::swap(function_, other.function_);
}

template <typename Function, typename Argument>
fn::details::bind_t<Function, Argument> fn::bind(Function const& function, Argument argument)
{
    return details::bind_t<Function, Argument>(function, argument);
}

template <typename Function, typename Argument, typename... Arguments>
auto fn::bind(Function const& function, Argument argument, Arguments... arguments) -> decltype(bind(details::bind_t<Function, Argument>(function, argument), arguments...))
{
    details::bind_t<Function, Argument> b(function, argument);
    return bind(b, arguments...);
}