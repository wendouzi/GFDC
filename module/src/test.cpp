
#include "Image.h"
#include "io.h"
#include "Algo.h"
#include <string>
#include <assert.h>
#include "setting.h"
#include "config.h"
#include "AreaCount.h"
// using namespace wendouzi;

int main(int argc, char * argv[])
{
    wendouzi::setting st;
    st.config(argc, argv);
    st.check();
}