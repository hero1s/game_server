
#pragma once

namespace svrlib {

class noncopyable {
protected:
    noncopyable() { }

    virtual ~noncopyable() { }

private:
    noncopyable(const noncopyable&);

    noncopyable& operator=(const noncopyable&);
};//end class noncopyable

}

 
