#include "filter_matches.hpp"

template <class records_t, class strata_t>
records_t consolidate_matches_for_read(records_t &records, strata_t x)
{
// TODO:
		    // find optimal read in records
		    // filter out all below best + x
    using namespace seqan3::literals;

    seqan3::sam_tag_dictionary dict{};
    using nm_tag_type = seqan3::sam_tag_type_t<"NM"_tag>;
    
    std::deque<int> edit_distances{};
    
    // gather all edit distances for one read
    for (auto & record : records){
	// TODO: don't copy
        dict = record.tags();
        // seqan3::debug_stream << record.id() << '\n';
        auto nm = dict.get<"NM"_tag>();
	edit_distances.push_back(nm);
        // seqan3::debug_stream << nm << '\n';
    }

    std::deque<int>::iterator it = std::min_element(edit_distances.begin(), edit_distances.end());
    seqan3::debug_stream << *it << '\n';

    records_t filtered_records;
    return filtered_records;
}
void run_consolidator(std::filesystem::path sam_file, std::filesystem::path out, uint8_t x)
{
    using namespace seqan3::literals;
    
    seqan3::sam_file_input fin{sam_file};
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
		    consolidate_matches_for_read(records, x);
		    // TODO: write out filtered matches
		}
	        current_read_id = record.id();
	        records.clear();
	        records.push_back(record);
	    }
	}

    }
}    
