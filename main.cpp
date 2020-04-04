#include <iostream>
#include <vector>
#include <filesystem>
#include <fileref.h>
#include <cxxopts.hpp>

namespace fs = std::filesystem;

static const char *ext[] = {
        ".mp3"
};

static const char blacklist[] = "<>:\"/\\|?*";

struct info {
    fs::path file;
    std::string artist;
    std::string album;
};

std::vector<info> files;

bool isMusicFile(const fs::path& path);
bool createDir(const fs::path& path);
std::string sanitize(const std::string& name);

static bool verbose, quiet, force;

int main(int argc, char **argv) {

    cxxopts::Options opts("Declutter", "A program to organize music collections using file metadata.");
    opts.add_options()
            ("v,verbose", "Give more verbose output")
            ("q,quiet", "Hide output (useful for scripts)")
            ("s,source", "The directory in which to search for music", cxxopts::value<std::string>()->default_value("./"))
            ("d,dest", "The destination directory", cxxopts::value<std::string>()->default_value("./declutter"))
            ("f,force", "Allow overwriting of files")
            ("h,help", "Print this message and exit");

    auto args = opts.parse(argc, argv);
    quiet = args["quiet"].as<bool>();
    verbose = args["verbose"].as<bool>();
    force = args["force"].as<bool>();

    if (args.count("help")) {
        std::cout << opts.help() << std::endl;
        return 0;
    }

    // Argument validation
    if (verbose && quiet) {
        std::cerr << R"(Error: "quiet" and "verbose" cannot coexist)" << std::endl;
        return 1;
    }

    if(args["source"].as<std::string>() == args["dest"].as<std::string>()) {
        if (!quiet) {
            std::cout << "Warning: Source and destination are the same; this may cause strange things to happen." << std::endl;
        }
    }


    const fs::path sourceDir{args["source"].as<std::string>()};
    if (verbose) {
        std::cout << "Source directory: " << sourceDir << std::endl;
    }

    const fs::path destDir{args["dest"].as<std::string>()};
    if (verbose) {
        std::cout << "Destination directory: " << destDir << std::endl;
    }

    if (!createDir(destDir)) return 1;

    for (const auto& entry : fs::recursive_directory_iterator(sourceDir)) {
        const fs::path& path = entry.path();

        if (verbose) {
            std::cout << "Examining: " << path << std::endl;
        }

        if (isMusicFile(path)) {
            TagLib::FileRef file(path.c_str());

            TagLib::Tag *tag = file.tag();
            if (tag->isEmpty()) {
                if (verbose) {
                    std::cout << "Skipping untagged file: " << path << std::endl;
                }
                continue;
            }

            if (!quiet) {
                std::cout << "Found music file: " << path << std::endl;
            }

            files.push_back({
                path,
                sanitize(tag->artist().to8Bit(true)),
                sanitize(tag->album().to8Bit(true))
            });

        }

    }

    for (const info& inf : files) {
        const fs::path artistDir = fs::path(destDir).append(inf.artist);
        if (!createDir(artistDir)) return 1;

        const fs::path albumDir = fs::path(artistDir).append(inf.album);
        if (!createDir(albumDir)) return 1;

        const fs::path outFile = fs::path(albumDir).append(inf.file.filename().string());
        if (fs::exists(outFile)) {
            if (!force) {
                if (!quiet) {
                    std::cout << "Skipping " << outFile << ": File already exists" << std::endl;
                }
                continue;
            } else {
                fs::remove(outFile);
            }
        }
        fs::copy(inf.file, outFile);
    }

    return 0;
}

bool isMusicFile(const fs::path& path) {
    return !fs::is_directory(path) && std::find(std::begin(ext), std::end(ext), path.extension()) != std::end(ext);
}

bool createDir(const fs::path& path) {
    if (!fs::exists(path)) {
        fs::create_directory(path);
    } else if (!fs::is_directory(path)) {
        if (!quiet) {
            std::cerr << "Error: " << path << " exists but is not a directory" << std::endl;
        }
        return false;
    }

    return true;
}

std::string sanitize(const std::string& name) {
    std::string clean;

    for (char c : name) {
        if (c < 32 || std::find(std::begin(blacklist), std::end(blacklist), c) != std::end(blacklist)) {
            clean.push_back('_');
        } else {
            clean.push_back(c);
        }
    }

    return clean;
}