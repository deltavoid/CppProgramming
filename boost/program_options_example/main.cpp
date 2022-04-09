#include <cstdio>
#include <cstdlib>

// #include <optional>

#include <boost/program_options.hpp>


namespace bpo = boost::program_options;





int main(int argc, char** argv, char** env)
{
    printf("env:\n");
    for (int i = 0; env[i] != NULL; i++)
        printf("%s\n", env[i]);
    printf("\n");

    printf("arg:\n");
    for (int i = 0; i < argc; i++)
        printf("%s\n", argv[i]);


    // using configuration_reader = std::function<void (boost::program_options::variables_map&)>;

    boost::program_options::options_description _opts("options");

    // boost::program_options::options_description _opts_conf_file;
    boost::program_options::positional_options_description _pos_opts;
    // boost::optional<boost::program_options::variables_map> _configuration;
    // configuration_reader _conf_reader;

    _opts.add_options()
                ("help,h", "show help message")
                ;


    bpo::variables_map configuration;


    try {
        
        bpo::store(bpo::command_line_parser(argc, argv)
                    .options(_opts)
                    .positional(_pos_opts)
                    .run()
            , configuration);

        // _conf_reader(configuration);
    } catch (bpo::error& e) {
        // fmt::print("error: {}\n\nTry --help.\n", e.what());
        printf("parse error\n");
        return 2;
    }


    printf("configuration count: %d\n", configuration.count("help"));

    return 0;
}