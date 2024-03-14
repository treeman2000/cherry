#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

namespace cherry{

class noncopyable{
public:
    noncopyable(const noncopyable&) = delete;
    void operator=(const noncopyable&) = delete;

protected:
    noncopyable() = default;
    ~noncopyable() = default;
};

}

#endif  // NONCOPYABLE_H
