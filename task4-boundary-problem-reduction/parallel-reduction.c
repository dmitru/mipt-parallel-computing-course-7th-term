
#include "includes.h"
#include "model.h"

int main() 
{
    model_s *model = model_create();
    model_free(model);

    model = model_create(.a = 2.0, .b = 10, .n = 2048);
    model_free(model);
}
