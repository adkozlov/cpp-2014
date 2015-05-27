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

            template <typename... Arguments>
            auto operator()(Arguments... arguments) -> decltype(function_(argument_, arguments...));
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

            template <typename FirstArgument, typename... Arguments>
            auto operator()(FirstArgument first_argument, Arguments... arguments) -> decltype(function_(first_argument, arguments...));

            template <typename FirstArgument, typename SecondArgument, typename... Arguments>
            auto operator()(FirstArgument first_argument, SecondArgument second_argument, Arguments... arguments) -> decltype(function_(second_argument, first_argument, arguments...));
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

            template <typename FirstArgument, typename SecondArgument, typename... Arguments>
            auto operator()(FirstArgument first_argument, SecondArgument second_argument, Arguments... arguments) -> decltype(function_(second_argument, first_argument, arguments...));
        };

        template <typename Function>
        class reverse_bind_t
        {
            typedef reverse_bind_t<Function> BindT;

            Function function_;
        public:
            reverse_bind_t(Function const& function);

            template <typename FirstArgument, typename SecondArgument, typename... Arguments>
            auto operator()(FirstArgument first_argument, SecondArgument second_argument, Arguments... arguments) -> decltype(function_(second_argument, first_argument, arguments...));
        };
    } // details

    const static details::placeholder1 _1{};
    const static details::placeholder2 _2{};

    template <typename Function, typename Argument>
    details::bind_t<Function, Argument> bind(Function const function, Argument argument);

    template <typename Function, typename Argument, typename... Arguments>
    auto bind(Function const function, Argument argument, Arguments... arguments) -> decltype(bind(details::bind_t<Function, Argument>(function, argument), arguments...));

    template <typename Function>
    details::reverse_bind_t<Function> bind(Function const function, details::placeholder2 const& first_argument, details::placeholder1 const& second_argument);
} // bind & function

template <typename Function, typename Argument>
fn::details::bind_t<Function, Argument>::bind_t(Function const& function, Argument argument)
    : function_(function)
    , argument_(argument)
{
}

template <typename Function, typename Argument>
template <typename... Arguments>
auto fn::details::bind_t<Function, Argument>::operator()(Arguments... arguments) -> decltype(function_(argument_, arguments...))
{
    return function_(argument_, arguments...);
}

template <typename Function>
fn::details::bind_t<Function, fn::details::placeholder1>::bind_t(Function const& function, placeholder1)
    : function_(function)
{
}

template <typename Function>
template <typename FirstArgument, typename... Arguments>
auto fn::details::bind_t<Function, fn::details::placeholder1>::operator()(FirstArgument first_argument, Arguments... arguments) -> decltype(function_(first_argument, arguments...))
{
    return function_(first_argument, arguments...);
}

template <typename Function>
template <typename FirstArgument, typename SecondArgument, typename... Arguments>
auto fn::details::bind_t<Function, fn::details::placeholder1>::operator()(FirstArgument first_argument, SecondArgument second_argument, Arguments... arguments) -> decltype(function_(second_argument, first_argument, arguments...))
{
    return function_(second_argument, first_argument, arguments...);
}

template <typename Function>
fn::details::bind_t<Function, fn::details::placeholder2>::bind_t(Function const& function, placeholder2)
    : function_(function)
{
}

template <typename Function>
template <typename FirstArgument, typename SecondArgument, typename... Arguments>
auto fn::details::bind_t<Function, fn::details::placeholder2>::operator()(FirstArgument first_argument, SecondArgument second_argument, Arguments... arguments) -> decltype(function_(second_argument, first_argument, arguments...))
{
    return function_(second_argument, first_argument, arguments...);
}

template <typename Function>
fn::details::reverse_bind_t<Function>::reverse_bind_t(Function const& function)
    : function_(function)
{
}

template <typename Function>
template <typename FirstArgument, typename SecondArgument, typename... Arguments>
auto fn::details::reverse_bind_t<Function>::operator()(FirstArgument first_argument, SecondArgument second_argument, Arguments... arguments) -> decltype(function_(second_argument, first_argument, arguments...))
{
    return function_(second_argument, first_argument, arguments...);
}

template <typename Function, typename Argument>
fn::details::bind_t<Function, Argument> fn::bind(Function const function, Argument argument)
{
    return details::bind_t<Function, Argument>(function, argument);
}

template <typename Function, typename Argument, typename... Arguments>
auto fn::bind(Function const function, Argument argument, Arguments... arguments) -> decltype(bind(details::bind_t<Function, Argument>(function, argument), arguments...))
{
    details::bind_t<Function, Argument> b(function, argument);
    return bind(b, arguments...);
}

template <typename Function>
fn::details::reverse_bind_t<Function> fn::bind(Function const function, fn::details::placeholder2 const& first_argument, fn::details::placeholder1 const& second_argument)
{
    return fn::details::reverse_bind_t<Function>(function);
}