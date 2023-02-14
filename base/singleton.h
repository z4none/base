#pragma once

namespace base
{

//
template <typename T>
class Singleton
{
public:
    template<typename... Args>
    static T & GetInstance(Args... args)
    {
        static T s_instance(args ...);
        return s_instance;
    }

protected:
    Singleton()
    {
    }

    virtual ~Singleton()
    {
    }

private:
    Singleton(const Singleton & source) {} // ½ûÖ¹copy¹¹Ôì
    const Singleton & operator=(const Singleton &source) = delete;
};

} // namespace base

