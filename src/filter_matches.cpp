#include "filter_matches.hpp"

template <class records_t, class strata_t>
records_t consolidate_matches_for_read(records_t &records, strata_t x)
{
    using namespace seqan3::literals;

    using nm_tag_type = seqan3::sam_tag_type_t<"NM"_tag>;
    std::deque<int> edit_distances{};
   
    // gather all edit distances for one read
    for (auto & record : records)
    {
	// TODO: there should be a way to avoid writing the type each time
	seqan3::sam_tag_dictionary & dict = record.tags();  // pass a reference to the vector (no copy)
        auto nm = dict.get<"NM"_tag>();
	edit_distances.push_back(nm);
    }

    // find edit distance for the optimal match
    std::deque<int>::iterator it = std::min_element(edit_distances.begin(), edit_distances.end());
    int optimal_nm = *it;

    /* TODO: should be possible using a view but didn't work
    auto nm_filter = std::views::filter([&] (auto & record)
    {
        return record.tags().get<"NM"_tag>() <= optimal_nm + x;    
    });
    */

    // filter out any matches that are worse than best + x
    records_t filtered_records{};
    for (auto & record : records)
    {
	seqan3::sam_tag_dictionary & dict = record.tags();
	auto nm = dict.get<"NM"_tag>();
	if (nm <= optimal_nm + x)
	{
            filtered_records.push_back(record);
	}
    }

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
	if(isFirst)
	{
	    current_read_id = record.id();
	    records.push_back(record);
	    isFirst = false;
	}
	// for each after first
	else 
	{
	    // if within the same collation of read matches
	    if (current_read_id == record.id())
	    {
	        records.push_back(record);

	    }
	    // if moved to matches for next read
            else 
	    {
                if (records.size() == 1)
		{
		    // write out record
		    fout = records;
		}
		else 
		{
		    auto filtered_records = consolidate_matches_for_read(records, x); // return pointer (no copy)
		    fout = filtered_records;
		}
	        current_read_id = record.id();
	        records.clear();
	        records.push_back(record);
	    }
	}
    }
}    
