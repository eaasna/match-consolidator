#pragma once
#include <seqan3/alphabet/nucleotide/dna5.hpp>
#include <seqan3/argument_parser/validators.hpp>
#include <seqan3/core/debug_stream.hpp>
#include <seqan3/io/sam_file/all.hpp>
#include <seqan3/std/filesystem>

#include <deque>

void consolidate_matches_for_read();


/* 
 *  \param sam_file input file path to the sam file
 *  \param out output file path for the filtered sam file
 *
 * Function iterates through all records in the sam file and writes a filtered version to out.
 *
 */
void run_consolidator(std::filesystem::path sam_file, std::filesystem::path out, uint8_t x);
