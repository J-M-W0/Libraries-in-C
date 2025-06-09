#include "/opt/localc/cheaders/cgeneric.h"
#include "/opt/localc/cheaders/assertf.h"
#include <string.h>

#define TYPEOF(expr)        \
        _Generic((expr),    \
        Return:  "Return",  \
        Option:  "Option",  \
        default: "Unknown"  )

bool ok(Return * res)
{
    scp(res);
    return res->Ok != nil;
}

bool err(Return * res)
{
    scp(res);
    return res->Err != nil;
}

bool some(Option * option)
{
    scp(option);
    return !option->None;
}

bool none(Option * option)
{
    scp(option);
    return option->None;
}

void * Ok(Return * res)
{
    scp(res);
    return res->Ok;
}

void * Err(Return * res)
{
    scp(res);
    return res->Err;
}

void * Some(Option * option)
{
    scp(option);
    return option->Some;
}

void * unwrap(void * p)
{
    scp(p);

    char const * typename = TYPEOF(p);
    if (streq(typename, "Return")) {
        Return * res = (Return *)p;
        if (ok(res)) return res->Ok;
        return res->Err;
    }
    else if (streq(typename, "Option")) {
        Option * option = (Option *)p;
        if (option->None) return nil;
        return option->Some;
    }

    assertf(0, "Fatal Error: Unknown type name while unwrapping!");
    return nil;
}

void set_ok(Return * res, void * value)
{
    scp(res);
    scp(value);
    res->Ok = value;
    res->Err = nil;
}

void set_err(Return * res, void * value)
{
    scp(res);
    scp(value);
    res->Ok = nil;
    res->Err = value;
}

void set_some(Option * option, void * value)
{
    scp(option);
    scp(value);
    option->None = false;
    option->Some = value;
}

void set_none(Option * option)
{
    scp(option);
    option->None = true;
    option->Some = nil;
}
