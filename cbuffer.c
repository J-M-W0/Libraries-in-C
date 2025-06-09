#include "/opt/localc/cheaders/assertf.h"
#include "/opt/localc/cheaders/cgeneric.h"
#include <stdlib.h>

typedef struct _Buffer Buffer;

struct _Buffer {
    owned char * Body;
    u64 Capacity;
    u64 Size;
};


Buffer * cbuffer_init()
{
    Buffer * buffer = new(Buffer);
    scp(buffer);

    buffer->Body = nil;
    buffer->Capacity = 0;
    buffer->Size = 0;

    return buffer;
}

