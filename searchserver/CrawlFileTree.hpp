/*
 * Copyright ©2025 Travis McGaha.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Pennsylvania
 * CIT 5950 for use solely during Spring Semester 2025 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */
 
#ifndef CRAWLFILETREE_HPP_
#define CRAWLFILETREE_HPP_

#include "./WordIndex.hpp"

#include <string>
#include <optional>

namespace searchserver {

// Crawls a directory, indexing ASCII text files.
//
// CrawlFileTree crawls the filesystem subtree rooted at directory "rootdir".
// For each file that it encounters, it scans the file to test whether it
// contains ASCII text data.  If so, it indexes the file into a WordIndex which is returned
//
// Arguments:
// - rootdir: the name of the directory which is the root of the crawl.
//
// Returns:
// - index: an output parameter through which a populated WordIndex is returned.
//
// - Returns nullopt on failure to scan the directory, the WordIndex on success.
std::optional<WordIndex> crawl_filetree(const std::string& root_dir);

}  // namespace searchserver

#endif  // CRAWLFILETREE_HPP_
