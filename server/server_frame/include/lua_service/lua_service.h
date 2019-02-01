//
// Created by toney on 2018/12/15.
//
#pragma  once

#include "svrlib.h"

class lua_service
{
public:
    using lua_state_ptr_t = std::unique_ptr<lua_State, void(*)(lua_State*)>;
    /*
    http://sol2.readthedocs.io/en/latest/safety.html
    http://sol2.readthedocs.io/en/latest/api/protected_function.html
    http://sol2.readthedocs.io/en/latest/errors.html
    */
    using sol_function_t = sol::function;

    lua_service(sol::state* p);

    ~lua_service();

    void set_init(sol_function_t f);

    void set_start(sol_function_t f);

    void set_dispatch(sol_function_t f);

    void set_exit(sol_function_t f);

    void set_destroy(sol_function_t f);

public:
    void start();

    void exit();

    void destroy();

    void dispatch(uint16_t cmd,string msg);

protected:
    void error();

    const char* lua_traceback(lua_State* _state);
private:
    bool error_;
    sol::state* lua_;
    sol_function_t init_;
    sol_function_t start_;
    sol_function_t dispatch_;
    sol_function_t exit_;
    sol_function_t destroy_;

};

