
#include "includes.h"
#include "model.h"

int main() 
{
    model_s *model = model_create(.a = 1.0, .b = 1.0, .n = 2048);
    model_free(model);
}
