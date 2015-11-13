#include "qa_utils.h"

#include <volk_sfmt/volk_sfmt.h>

#include <boost/assign/list_of.hpp>
#include <vector>

// macros for initializing volk_sfmt_test_case_t. Maccros are needed to generate
// function names of the pattern kernel_name_*

// for puppets we need to get all the func_variants for the puppet and just
// keep track of the actual function name to write to results
#define VOLK_INIT_PUPP(func, puppet_master_func, test_params)\
    volk_sfmt_test_case_t(func##_get_func_desc(), (void(*)())func##_manual, std::string(#func),\
    std::string(#puppet_master_func), test_params)

#define VOLK_INIT_TEST(func, test_params)\
    volk_sfmt_test_case_t(func##_get_func_desc(), (void(*)())func##_manual, std::string(#func),\
    test_params)

std::vector<volk_sfmt_test_case_t> init_test_list(volk_sfmt_test_params_t test_params)
{

    // Some kernels need a lower tolerance
    volk_sfmt_test_params_t test_params_inacc = volk_sfmt_test_params_t(1e-3, test_params.scalar(),
            test_params.vlen(), test_params.iter(), test_params.benchmark_mode(), test_params.kernel_regex());
    volk_sfmt_test_params_t test_params_int1 = volk_sfmt_test_params_t(1, test_params.scalar(),
            test_params.vlen(), test_params.iter(), test_params.benchmark_mode(), test_params.kernel_regex());

    std::vector<volk_sfmt_test_case_t> test_cases = boost::assign::list_of

        (VOLK_INIT_PUPP(volk_sfmt_32u_genrandpuppet_32u, volk_sfmt_32u_genrand, test_params))
        (VOLK_INIT_PUPP(volk_sfmt_32u_genrand_arraypuppet_32u, volk_sfmt_32u_genrand_array_32u, test_params))
        ;

    return test_cases;
}
