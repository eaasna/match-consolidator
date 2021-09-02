#include "filter_matches.hpp"

void filter_matches(std::filesystem::path sam_file, std::filesystem::path out)
{
    using namespace seqan3::literals;
    seqan3::sam_file_input fin{sam_file};
    seqan3::sam_tag_dictionary dict{};
    using nm_tag_type = seqan3::sam_tag_type_t<"NM"_tag>;

    // for (seqan3::sam_record record : fin) // this will copy the record
    for (auto && record : fin)  // this will pass the record by reference (no copy)
    {
	dict = record.tags();
        seqan3::debug_stream << record.id() << '\n';
	auto nm = dict.get<"NM"_tag>();
        seqan3::debug_stream << nm << '\n';
    }
}
