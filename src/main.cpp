#include <sstream>

#include <seqan3/argument_parser/all.hpp>
#include <seqan3/core/debug_stream.hpp>

#include "filter_matches.hpp"

int main(int argc, char ** argv)
{
    seqan3::argument_parser parser{"Distributed-mapping-consolidator", argc, argv};

    // Declarations for argument parser
    std::filesystem::path sam_file{};
    std::filesystem::path output_file{};
    uint8_t strata_width{0};
    bool verbose = false;

    // Parser
    parser.info.author = "Evelin Aasna"; // give parser some infos
    parser.info.version = "1.0.0";
    parser.add_positional_option(sam_file, "Please provide a sam file.",
                                 seqan3::input_file_validator{{"sam"}}); // Takes a fastq file and validates it
    //output path as option, otherwise output is printed
    parser.add_option(output_file, 'o', "output", "The file for sam output. Default: stdout");
    //this is x in (best+x) strata mapping
    parser.add_option(strata_width, 's', "strata", 
		    "Consider matches within this edit distance compared to the optimal.", 
		    seqan3::option_spec::standard, seqan3::arithmetic_range_validator{0, 10});
    
    parser.add_flag(verbose, 'v', "verbose", "Give more detailed information here."); // example for a flag

    try
    {
         parser.parse();                                                  // trigger command line parsing
    }
    catch (seqan3::argument_parser_error const & ext)                     // catch user errors
    {
        seqan3::debug_stream << "Parsing error. " << ext.what() << "\n"; // give error message
        return -1;
    }

    run_consolidator(sam_file, output_file, strata_width); // Call strata (best+x) based filter

    if (verbose) // if flag is set
        seqan3::debug_stream << "Consolidating read mapping was a success.\n";

    return 0;
}
