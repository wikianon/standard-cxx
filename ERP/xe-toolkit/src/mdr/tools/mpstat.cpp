//
// mpstat - display MP values for all CPUs
//

#include <cstdio>
#include <iostream>
#include <exception>
#include <unistd.h>
#include <mdr_mpstat.h>

static inline void header(void)
{
    std::cout << "name       user kernel wait idle" << std::endl;
}

int main(int argc, char *argv[])
{
    try
    {
        int c;
        std::string name;
        int interval = 1;
        int iterations = 1;
        bool do_support_matrix = false;
        const char *remote_host = 0;
        mdr::mmps_node_t n;

        opterr = 0;
        while ((c = getopt(argc, argv, "sh:")) != -1)
        {
            switch (c)
            {
            case 's':
                do_support_matrix = true;
                break;
            case 'h':
                remote_host = optarg;
                break;
            default:
                std::cerr << "Use: " << argv[0] << "[-s] [-h remote_host] "
                    << "[interval [iterations]]" << std::endl;
                return 1;
            }
        }

        mdr::mdr_mpstat_t mp(remote_host);

        if (do_support_matrix)
        {
            mdr::mdr_t::mdr_support_matrix(mp, std::cout);
            return 0;
        }

        if (argv[optind])
        {
            interval = atoi(argv[optind]);
            if (argv[optind + 1])
                iterations = atoi(argv[optind + 1]);
            else
                iterations = 0;
        }

        do
        {
            header();
            for(n=mp.begin(); n != mp.end(); n++)
            {
                mdr::mmps_can_t can = n->second;

                printf("%-10.10s %4d   %4d %4d %4d\n",
                       n->first.c_str(),
                       can.mmps_user_time_percent,
                       can.mmps_kernel_time_percent,
                       can.mmps_wait_time_percent,
                       can.mmps_idle_time_percent);
            }

            if (iterations != 1)
            {
                sleep(interval);
                mp.mdr_refresh();
            }
        }
        while (--iterations);
    }
    catch(std::exception & e)
    {
        std::cout << argv[0] << ": " << e.what() << std::endl;
    }

    return 0;
}
