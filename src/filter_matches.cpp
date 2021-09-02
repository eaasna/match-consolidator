#include "filter_matches.hpp"

void consolidate_matches_for_read()
{
    /*
    dict = record.tags();
    seqan3::debug_stream << record.id() << '\n';
    auto nm = dict.get<"NM"_tag>();
    seqan3::debug_stream << nm << '\n';
    */
}
void run_consolidator(std::filesystem::path sam_file, std::filesystem::path out, uint8_t x)
{
    using namespace seqan3::literals;
    
    seqan3::sam_file_input fin{sam_file};

    seqan3::sam_tag_dictionary dict{};
    using nm_tag_type = seqan3::sam_tag_type_t<"NM"_tag>;

    using record_type = typename decltype(fin)::record_type;
    std::deque<record_type> records{};
    
    seqan3::sam_file_output fout{out};
    
    bool isFirst = true;
    std::string current_read_id = "";
    for (auto & record : fin)  // this will pass the record by reference (no copy)
    {
	// initialize the read ID and records
	if(isFirst){
	    current_read_id = record.id();
	    records.push_back(record);
	    isFirst = false;
	}
	// for each after first
	else {
	    // if within the same collation of read matches
	    if (current_read_id == record.id()){
	        records.push_back(record);
	    }
	    // if moved to matches for next read
            else {
                if (records.size() == 1){
		    // write out record
		    fout = records;
		}
		else {
		    // TODO:
		    // find optimal read in records
		    // filter out all below best + x
		    // write out filtered records
		}
	    }
	current_read_id = record.id();
	records.clear();
	records.push_back(record);
	}

    }
}    
